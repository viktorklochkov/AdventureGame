# AdventureGame project

[![CMake on a single platform](https://github.com/viktorklochkov/AdventureGame/actions/workflows/cmake-single-platform.yml/badge.svg?branch=main)](https://github.com/viktorklochkov/AdventureGame/actions/workflows/cmake-single-platform.yml)
[![IWYU](https://github.com/viktorklochkov/AdventureGame/actions/workflows/iwyu.yml/badge.svg)](https://github.com/viktorklochkov/AdventureGame/actions/workflows/iwyu.yml)
[![clang-tidy](https://github.com/viktorklochkov/AdventureGame/actions/workflows/clang-tidy.yml/badge.svg)](https://github.com/viktorklochkov/AdventureGame/actions/workflows/clang-tidy.yml)
[![Clang Format Checker](https://github.com/viktorklochkov/AdventureGame/actions/workflows/clang-format.yml/badge.svg)](https://github.com/viktorklochkov/AdventureGame/actions/workflows/clang-format.yml)
[![Doxygen Action](https://github.com/viktorklochkov/AdventureGame/actions/workflows/doxygen.yml/badge.svg)](https://github.com/viktorklochkov/AdventureGame/actions/workflows/doxygen.yml)

## Overview

AdventureGame is a TDD test project

## Features

- **CMake**: Build system support using CMake.
- **IWYU**: Include What You Use analysis.
- **clang-tidy**: Static analysis and linting.
- **Clang Format**: Code formatting checks.
- **Doxygen**: Documentation generation.

## Getting Started

1. **Clone the repository**:
  ```sh
  git clone https://github.com/viktorklochkov/AdventureGame.git
  cd AdventureGame
  ```

2. **Build the project**:
  ```sh
  mkdir build
  cd build
  cmake -DBUILD_TESTS=ON ..
  make -j
  ```

3. **Run tests**:
  ```sh
  ctest
  ```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, feel free to open an issue or contact the repository owner.
