# CppTemplate project

[![CMake on a single platform](https://github.com/viktorklochkov/CppTemplate/actions/workflows/cmake-single-platform.yml/badge.svg?branch=main)](https://github.com/viktorklochkov/CppTemplate/actions/workflows/cmake-single-platform.yml)
[![IWYU](https://github.com/viktorklochkov/CppTemplate/actions/workflows/iwyu.yml/badge.svg)](https://github.com/viktorklochkov/CppTemplate/actions/workflows/iwyu.yml)
[![clang-tidy](https://github.com/viktorklochkov/CppTemplate/actions/workflows/clang-tidy.yml/badge.svg)](https://github.com/viktorklochkov/CppTemplate/actions/workflows/clang-tidy.yml)
[![Clang Format Checker](https://github.com/viktorklochkov/CppTemplate/actions/workflows/clang-format.yml/badge.svg)](https://github.com/viktorklochkov/CppTemplate/actions/workflows/clang-format.yml)
[![Doxygen Action](https://github.com/viktorklochkov/CppTemplate/actions/workflows/doxygen.yml/badge.svg)](https://github.com/viktorklochkov/CppTemplate/actions/workflows/doxygen.yml)

## Overview

CppTemplate is a template repository for C++ projects. It includes a set of tools and workflows to help you get started quickly with best practices for C++ development.

## Features

- **CMake**: Build system support using CMake.
- **IWYU**: Include What You Use analysis.
- **clang-tidy**: Static analysis and linting.
- **Clang Format**: Code formatting checks.
- **Doxygen**: Documentation generation.

## Getting Started

1. **Clone the repository**:
  ```sh
  git clone https://github.com/viktorklochkov/CppTemplate.git
  cd CppTemplate
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

## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, feel free to open an issue or contact the repository owner.
