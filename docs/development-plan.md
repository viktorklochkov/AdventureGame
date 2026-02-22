# Development Plan

A phased roadmap for extending AdventureGame. Each phase follows the established patterns:
dependency injection through pure-virtual interfaces, London-style TDD with GMock mocks,
C++20/Google style, and clang-tidy/IWYU compliance.

All new code lives in `lib/` (namespace `adv_sk`), tests in `adv_sk::test`.
Each phase ships as its own PR so CI gates validate incrementally.

---

## Phase 1 — Fix Existing Gaps

### 1.1 Fix `ConsoleInputHandler` missing commands

**File:** `lib/ConsoleInputHandler.cpp`

`"drop"` → `Action::DropItem` and `"inventory"` → `Action::DisplayInventory` are absent from
`get_action()`. Add both to the existing if/else chain. No interface changes.

### 1.2 Give `rusty sword` a `use_message`

**File:** `lib/Map.cpp` (`create_map()`)

The `rusty sword` `InventoryItem` has an empty `use_message`. Assign e.g.:
`"You swing the rusty sword. The blade holds, but barely."`

### 1.3 Add a "look" action

Re-displays the current room description and available exits without moving.

**Files:**
- `lib/IInputHandler.hpp` — add `Look` to the `Action` enum
- `lib/ConsoleInputHandler.cpp` — add `"look"` to `get_action()` parser
- `lib/Game.hpp` / `lib/Game.cpp` — add private `handle_look()`, dispatch from `handle_user_action()`

**Testing:** Follow the `investigate` test pattern — inject `MockMap`, `MockPlayer`,
`MockInputHandler`; set expectations that the room description and available directions are
sent via `MockInputHandler::provide_message()`.

> **clang-tidy:** Every `switch(action)` in `Game.cpp` must gain a `case Action::Look:` branch.
> Missing cases are `-Wswitch` errors.

---

## Phase 2 — Expand the World

### 2.1 Map layout

Extend `create_map()` in `lib/Map.cpp` from 2 rooms to 7. Bidirectional connections are
created automatically via `opposite_direction()`.

```
           [Armoury] — East — [Barracks]
               ↕
[Library] ← [Grand Hall] → [Throne Room]
               ↕
           [Kitchen]
               ↕
           [Cellar]
```

### 2.2 Item placement

All items start with `is_visible = false` and are revealed by `investigate`.

| Room        | Item            | `use_message`                                              |
|-------------|-----------------|-------------------------------------------------------------|
| Grand Hall  | golden chalice  | `"The chalice gleams. It belongs on the throne."`           |
| Armoury     | rusty sword     | (Phase 1.2)                                                 |
| Library     | old tome        | `"The tome describes a hidden cellar beneath the kitchen."` |
| Kitchen     | iron key        | `"The key is cold and heavy in your hand."`                 |
| Cellar      | silver pendant  | `"The pendant pulses with faint warmth."`                   |

**Narrative through-line:** Find the chalice → explore to gather clues → unlock the Cellar →
return the chalice to the Throne Room.

No structural changes required — only data additions to `create_map()`.

---

## Phase 3 — Win Condition

### 3.1 `GameOutcome` enum

Add to `lib/Game.hpp` (or a new `lib/GameOutcome.hpp`):

```cpp
enum class GameOutcome : std::uint8_t { Ongoing, Win, Lose };
```

### 3.2 Victory trigger

**Victory:** Using the `golden chalice` while standing in the Throne Room.

- `Game` holds `GameOutcome _outcome = GameOutcome::Ongoing`
- `Game::start()` returns `GameOutcome` instead of `void`; loop exits when `_outcome != Ongoing`
- `handle_use_item()` checks item name + current room; on match, sets `_outcome = Win` and
  sends a victory message via `IInputHandler::provide_message()`

### 3.3 Testing

Script `MockInputHandler::get_action()` → `Action::UseItem`. Configure
`MockPlayer::get_current_room()` to return the Throne Room or another room. Verify `_outcome`
is `Win` only when both conditions are met.

---

## Phase 4 — Locked Doors / Item Keys

Lock state is map state, so lock logic belongs in `IMap`.

### 4.1 `IMap` extension

**File:** `lib/IMap.hpp`

```cpp
virtual std::optional<std::string> get_passage_key(
    const RoomName& from, const RoomName& to) const = 0;
virtual void unlock_passage(const RoomName& from, const RoomName& to) = 0;
```

`get_passage_key` returns `nullopt` for open passages and the required key name for locked ones.
Returning the key name in a single call avoids a two-call round-trip.

**File:** `lib/Map.hpp` / `lib/Map.cpp`

`Map` holds `std::map<std::pair<RoomName, RoomName>, std::string> _locked_passages`.
`create_map()` locks the Kitchen → Cellar passage, requiring `"iron key"`.

### 4.2 `Game::handle_move()` change

Before calling `IMap::next_room()`:
1. Call `get_passage_key(current_room, target_room)`
2. If a key is required and the player has it → call `unlock_passage()` then proceed
3. If a key is required and the player lacks it → send `"The way is blocked.\n"` and return

### 4.3 `MockMap` update

Add `MOCK_METHOD` declarations for both new methods in `lib/MockMap.hpp`.
Wrap in `NOLINTBEGIN/END` per the established pattern.

> **IWYU:** `<optional>` must be a direct include in every file that uses `std::optional`.
> Annotate any `ASSERT_TRUE` on an optional before dereferencing with
> `// NOLINTNEXTLINE(bugprone-unchecked-optional-access)`.

---

## Phase 5 — NPC System

### 5.1 `INPC` interface

**New file:** `lib/INPC.hpp` — follows the `IPlayer` pattern.

```cpp
namespace adv_sk {
  class INPC {
   public:
    virtual ~INPC() = default;
    virtual std::string get_name() const = 0;
    virtual std::string get_next_dialogue() = 0;
  };
}
```

### 5.2 Concrete `NPC`

**New files:** `lib/NPC.hpp`, `lib/NPC.cpp`

Holds `std::string _name`, `std::vector<std::string> _dialogue_lines`, and
`std::size_t _dialogue_index = 0`. `get_next_dialogue()` returns the current line and
advances the index with wrap-around.

### 5.3 `MockNPC`

**New file:** `lib/MockNPC.hpp` — follows `MockPlayer` / `MockInputHandler` pattern.

```cpp
// NOLINTBEGIN(misc-non-private-member-variables-in-classes,
//             cppcoreguidelines-non-private-member-variables-in-classes)
class MockNPC : public INPC {
 public:
  MOCK_METHOD(std::string, get_name, (), (const, override));
  MOCK_METHOD(std::string, get_next_dialogue, (), (override));
};
// NOLINTEND(misc-non-private-member-variables-in-classes,
//           cppcoreguidelines-non-private-member-variables-in-classes)
```

### 5.4 NPC ownership

`IMap` owns NPCs (keyed by room name) to avoid making `Room` non-copyable with `unique_ptr`
members:

```cpp
virtual INPC* get_npc(const RoomName& room) const = 0;  // nullptr = no NPC present
```

`Map` holds `std::map<RoomName, std::unique_ptr<INPC>> _npcs`.

### 5.5 `IInputHandler` and `Action` additions

- Add `Talk` to `Action` enum in `lib/IInputHandler.hpp`
- Add `virtual std::string get_npc_name() = 0;` to `IInputHandler` (needed for future
  multi-NPC rooms; also keeps `ConsoleInputHandler` and all mocks consistent)
- Add `"talk"` to `ConsoleInputHandler::get_action()` parser
- Add private `handle_talk()` to `Game`, dispatched from `handle_user_action()`

### 5.6 NPC placement in `create_map()`

| Room     | NPC name        | Dialogue lines                                                              |
|----------|-----------------|-----------------------------------------------------------------------------|
| Library  | Old Librarian   | `"The chalice belongs to the throne."`, `"I haven't left in years."`        |
| Barracks | Sleeping Guard  | `"Hmm... what? Move along."`, `"Don't tell anyone I was sleeping."`         |

### 5.7 Testing

- No NPC in room → `handle_talk()` sends `"There is no one here to talk to.\n"`
- NPC present → `MockNPC::get_next_dialogue()` is called; return value sent via
  `MockInputHandler::provide_message()`
- `MockMap` gains a `MOCK_METHOD` for `get_npc`

---

## Phase 6 — Save / Load

### 6.1 `GameSnapshot`

**New file:** `lib/GameSnapshot.hpp` — plain struct, no logic.

```cpp
struct GameSnapshot {
  RoomName player_room;
  std::vector<std::string> player_inventory;
  std::map<RoomName, std::vector<std::string>> room_visible_items;
  std::map<RoomName, std::vector<std::string>> room_taken_items;
  std::vector<std::pair<std::pair<RoomName, RoomName>, std::string>> locked_passages;
  std::map<std::string, std::size_t> npc_dialogue_indices;
};
```

### 6.2 `ISaveManager` interface

**New file:** `lib/ISaveManager.hpp`

```cpp
class ISaveManager {
 public:
  virtual ~ISaveManager() = default;
  virtual void save(const GameSnapshot& snapshot) = 0;
  virtual std::optional<GameSnapshot> load() = 0;
};
```

Injected into `Game` as a fourth `unique_ptr` parameter alongside `IMap`, `IPlayer`,
`IInputHandler`.

### 6.3 Concrete `FileSaveManager`

**New files:** `lib/FileSaveManager.hpp`, `lib/FileSaveManager.cpp`

Writes a simple line-based text format to `savegame.txt` in the working directory.
No external dependencies (no JSON library). `load()` returns `std::nullopt` if the file
is absent or malformed.

### 6.4 Interface additions for snapshot extraction

Extracting and restoring state requires new query/mutator methods on `IMap` and `IPlayer`:

**`IMap` additions:**
```cpp
virtual std::vector<RoomName> get_room_names() const = 0;
virtual std::vector<InventoryItem> get_room_inventory(const RoomName& room) const = 0;
virtual std::vector<std::pair<std::pair<RoomName, RoomName>, std::string>>
    get_locked_passages() const = 0;
virtual std::map<std::string, std::size_t> get_npc_dialogue_state() const = 0;
// restore counterparts omitted for brevity — mirror the above as setters
```

All additions propagate to `MockMap`, `MockPlayer`, and `MockSaveManager`.

### 6.5 `Action` additions

Add `Save` and `Load` to the `Action` enum. Add `"save"` and `"load"` to
`ConsoleInputHandler::get_action()`.

### 6.6 Testing

- `handle_save()` → verify `MockSaveManager::save()` is called with a snapshot containing
  the expected room and inventory from `MockPlayer`
- `handle_load()` with a valid snapshot → verify mutators are called on `MockMap` and
  `MockPlayer` with the correct values
- `handle_load()` with `std::nullopt` → verify a "no save file found" message and no
  mutators are called

---

## Implementation Order

| Phase | Depends on |
|-------|-----------|
| 1 — Fix gaps | — |
| 2 — Expand world | — |
| 3 — Win condition | Phase 2 (Throne Room + chalice) |
| 4 — Locked doors | Phase 2 (Cellar passage + iron key) |
| 5 — NPCs | — (independent) |
| 6 — Save / load | Phases 1–5 (needs final interface shapes) |

Phases 3, 4, and 5 can be developed in parallel after Phase 2 merges.

---

## Cross-Cutting Rules

These apply to every phase and are enforced by CI:

| Concern | Rule |
|---------|------|
| `Action` enum additions | Every `switch(action)` in `Game.cpp` must gain a new `case`; missing cases are clang-tidy errors |
| IWYU | Every new file includes only what it directly uses: `<string>`, `<vector>`, `<memory>`, `<optional>` etc. as needed |
| GMock `NOLINT` | Wrap every mock class in `NOLINTBEGIN/END(misc-non-private-member-variables-in-classes, cppcoreguidelines-non-private-member-variables-in-classes)` |
| Optional access | Annotate the line after `ASSERT_TRUE(opt)` with `// NOLINTNEXTLINE(bugprone-unchecked-optional-access)` |
| Designated initializers | New struct construction sites use `InventoryItem{.name = "x", .use_message = "y"}` syntax |
| Naming | Classes: `CamelCase`, functions/variables: `snake_case`, private members: `_prefixed` |
