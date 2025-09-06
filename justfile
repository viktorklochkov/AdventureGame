default:
  just --list

build-export:
  cmake -B build-export -DBUILD_TESTS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

clang-tidy: build-export
  clang-tidy -p build-export *.cpp */*.cpp

clang-tidy-fix: build-export
  clang-tidy -fix -p build-export *.cpp */*.cpp

iwyu: build-export
  iwyu_tool.py -p build-export main*.cpp */*.cpp -- -Xiwyu --error -Xiwyu --cxx17ns

format:
   clang-format -style=file -i main*.cpp */*.*pp