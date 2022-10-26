#!/usr/bin/env bash
set -eou pipefail

cd "$(realpath "$(dirname "$0")")"

# Show version
clang --version

# Compile C++
clang++ -std=c++17 -I../dotl main.cpp -o test && ./test