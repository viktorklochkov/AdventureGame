# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Build with tests (manual)
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make -j

# Build using CMake presets (preferred on macOS)
cmake --preset clang        # Apple clang
cmake --preset clang-llvm   # Homebrew LLVM (required for clang-tidy/IWYU)
cmake --build build/clang-llvm -j

# Run all tests
ctest

# Run a single test by name (lib tests)
./build/clang/lib/GameLogicTests --gtest_filter=GameTest.moveToValidRoomChangesPlayerRoom
```

## Code Quality Tools

The project uses `just` for common tasks (requires [just](https://github.com/casey/just)):

```bash
just build-export    # Generate compile_commands.json (uses Homebrew LLVM)
just clang-tidy      # Run static analysis
just clang-tidy-fix  # Auto-fix clang-tidy issues
just iwyu            # Run Include What You Use
just format          # Format code with clang-format
```

> **Note**: `just build-export` uses `/opt/homebrew/opt/llvm/bin/clang++`. Apple's system clang cannot find C++ standard headers for IWYU/clang-tidy. Always use the Homebrew LLVM preset for these tools.

## CI Checks

All PRs must pass: build (Ubuntu/gcc), clang-tidy, IWYU, clang-format. Warnings are treated as errors.

## Architecture

The game logic lives in `lib/` as a shared library (`GameLogic`), with `main.cpp` as a thin entry point that wires up concrete implementations.

**Interfaces** (namespace `adv_sk`) — the dependency-inversion layer:
- `IInputHandler` — input/output abstraction; `Action` enum defines all user actions
- `IMap` — map query abstraction (`next_room`, `get_room`, `get_welcome_message`)
- `IPlayer` — player state abstraction (room, inventory)

**Concrete classes**:
- `Game` — owns `IMap`, `IPlayer`, `IInputHandler` via `unique_ptr`; drives the game loop
- `Map` / `Room` — concrete map with connected rooms; built via `create_map()`
- `Player` — tracks current room and inventory (`InventoryItem`)
- `ConsoleInputHandler` — terminal I/O implementation
- `Direction` — enum (North/South/East/West) with `ALL_DIRECTIONS` constant

**Test doubles** (in `lib/`, namespace `adv_sk::test`):
- `MockInputHandler`, `MockMap`, `MockPlayer` — GMock mocks for London-style unit tests
- Tests inject mocks via `Game(unique_ptr<IMap>, unique_ptr<IPlayer>, unique_ptr<IInputHandler>)`

## Naming Conventions

Enforced by clang-tidy:
- Classes: `CamelCase`
- Functions/variables: `snake_case`
- Private members: `_prefixed`
- Struct initialization: use designated initializers — `InventoryItem{.name = "sword"}` (not positional)

## Known NOLINT Patterns

GMock `MOCK_METHOD` macros generate public member variables, requiring suppression on the surrounding class:

```cpp
// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
class MockFoo : public IFoo { ... };
// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
```

GoogleTest `ASSERT_TRUE` does not satisfy `bugprone-unchecked-optional-access` — suppress on the next line accessing the optional:

```cpp
ASSERT_TRUE(opt.has_value());
// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
use(*opt);
```
