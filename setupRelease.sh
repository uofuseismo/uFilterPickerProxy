#!/usr/bin/bash
conan install . --build=missing -s build_type=Release -pr:a=Linux-x86_64-clang-21 -s:a compiler.cppstd=23 --output-folder ./
cmake --preset conan-release -DWITH_CONAN=ON -DUSE_CLANG_TIDY=ON -DCLANG_TIDY_EXECUTABLE=clang-tidy-21
#cmake --build --preset conan-release
#ctest --preset conan-release
#cmake --install ./build/Release

