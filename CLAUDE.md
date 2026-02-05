# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure and build with tests
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make -j

# Run all tests
ctest

# Run a single test by name
./lib/GameLogicTests --gtest_filter=Game.moveNorth
```

## Code Quality Tools

The project uses `just` for common tasks (requires [just](https://github.com/casey/just)):

```bash
just build-export    # Build with compile_commands.json
just clang-tidy      # Run static analysis
just clang-tidy-fix  # Auto-fix clang-tidy issues
just iwyu            # Run Include What You Use
just format          # Format code with clang-format
```

Manual equivalents:
```bash
# clang-tidy (after build-export)
clang-tidy -p build-export *.cpp */*.cpp

# IWYU
iwyu_tool.py -p build-export main*.cpp */*.cpp -- -Xiwyu --error -Xiwyu --cxx17ns

# Format
clang-format -style=file -i main*.cpp */*.*pp
```

## CI Checks

All PRs must pass: build, clang-tidy, IWYU, clang-format. Warnings are treated as errors.

## Architecture

The game logic lives in `lib/` as a shared library (`GameLogic`), with `main.cpp` as a thin entry point.

**Core classes** (namespace `adv_sk`):
- `Game` - Main game controller with game loop, owns Map, Player, and IInputHandler
- `IInputHandler` - Interface for input/output abstraction (enables testing via mock)
- `Map` - Collection of connected rooms, created via `create_map()`
- `Room` - Location with connections and inventory
- `Player` - Tracks current room and inventory
- `Direction` - Enum (North/South/East/West) with `ALL_DIRECTIONS` constant

**Key patterns**:
- Dependency injection: Game accepts `IInputHandler` for testability
- Tests use `TestInputHandler` mock to script game behavior without console I/O

## Naming Conventions

Enforced by clang-tidy:
- Classes: `CamelCase`
- Functions/variables: `lower_case`
- Private members: `_prefixed`
