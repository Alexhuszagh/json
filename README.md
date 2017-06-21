Json++
======

Json++ is a memory-efficient, dependency-free, lazy C++11 JSON reader and writer.

[![Build Status](https://travis-ci.org/Alexhuszagh/json.svg?branch=master)](https://travis-ci.org/Alexhuszagh/json)
[![Build Status](https://tea-ci.org/api/badges/Alexhuszagh/json/status.svg)](https://tea-ci.org/Alexhuszagh/json)
[![Build status](https://ci.appveyor.com/api/projects/status/mds5o52wguleb54b?svg=true)](https://ci.appveyor.com/project/Alexhuszagh/json)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/12985/badge.svg)](https://scan.coverity.com/projects/alexhuszagh-json)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/99332f82456648e08d34ffd688d98e9a)](https://www.codacy.com/app/Alexhuszagh/json?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Alexhuszagh/json&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Alexhuszagh/json/blob/master/LICENSE.md)

**Table of Contents**

- [Motivation](#motivation)
- [Getting Started](#getting-started)
- [Design](#design)
- [Building](#building)
- [Testing](#testing)
- [Portability](#portability)
- [Documentation](#documentation)
- [Planned Features](#planned-features)
- [Acknowledgments](#acknowledgements)
- [Contributor Guidelines](#contributor-guidelines)
- [License](#license)

## Motivation

**DOM sucks.** DOM APIs read the entire document into memory, making DOM useless for large workloads.

**SAX sucks.** SAX APIs map specific events to specific handlers, requiring conditional branches for each tree in the document, producing tedious amounts of boilerplate.

**Iteration rules.** Inspired by the TextReader and TextWriter APIs from .NET, Json++ provides iterative access to each node as the document is parsed, simplifying tree-specific parsing with minimal overhead. 

Json++ features:

- Low memory overhead
- Tree-like logic
- STL-like iterators, auto-ranges, and STL container adapters

## Getting Started

```cpp
#include <json/reader.hpp>
#include <json/writer.hpp>
#include <iostream>


int main(void)
{
    // read document into map
    json::StringTextReader reader(" {\"1\":2}  \n");
    std::unordered_map<int, int> map(reader.object());

    // re-create JSON document from map 
    json::StringTextWriter writer;
    writer.write(map);

    // write {"1":2} to stdout
    std::cout << writer.str() << std::endl;

    return 0;
}
```

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/json.git
cd json
git submodule update --init --recursive
cd build
cmake .. -_DBUILD_TESTS=ON      # "-DBUILD_FUZZ=ON" for fuzzing with Clang
make -j 5                       # "msbuild json.sln" for MSVC
```

## Testing

Json++ has been tested with a variety of JSON files, for compliance and accurate parsing, fully passing the [conformance tests](https://github.com/miloyip/nativejson-benchmark/tree/master/data/jsonchecker) and [roundtrip tests](https://github.com/miloyip/nativejson-benchmark/tree/master/data/roundtrip) from Milo Yip's JSON benchmarks.

The core parser has also been extensively fuzzed (see [Fuzz Tests](/test/fuzz)), since bugs in JSON parsing can be used as attack vectors against web services.

## Portability

Json++ is continually built with the following compiler and compiler versions:

- Clang 3.8+
- GCC 5.3+
- MinGW 5.3.0 (MXE, MinGW, and MSYS2) 

## Documentation

Coming soon, for now, see the the [examples](/example) for how to use Json++.

## Planned Features

- DOM-style API using the TextReader internally.
- Pretty Printing
- Fast string formatting and extraction (replacing std::to_string and std::stod).

## Acknowledgments

Json++ also uses some code and test files from Niels Lohmann and Milo Yip, from their [Json](https://github.com/nlohmann/json) and [RapidJson](https://github.com/miloyip/rapidjson/) projects. All files in the [jsonchecker](/test/data/jsonchecker) directory are from Milo Yip's [nativejson-benchmark](https://github.com/miloyip/nativejson-benchmark/tree/master/data/roundtrip) and the remaining sample files come from Niel Lohmann's JSON tests.

## Contributor Guidelines

All useful pull requests will be merged, provided that they do not add external dependencies and follow these guidelines.

1. Preprocessor macros should be used sparingly.
2. Code syntax should not depend on the preprocessor.
3. Your code must be readable.

## License

MIT, see [license](LICENSE.md).
