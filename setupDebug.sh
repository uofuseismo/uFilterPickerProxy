#!/usr/bin/bash
conan install . --build=missing -s build_type=Debug -pr:a=Linux-x86_64-clang-21 -s:a compiler.cppstd=23 --output-folder ./
cmake --preset conan-debug -DWITH_CONAN=ON -DUSE_CLANG_TIDY=ON -DCLANG_TIDY_EXECUTABLE=clang-tidy-21
#cmake --build --preset conan-debug
#ctest --preset conan-debug
#cmake --install build/Debug


