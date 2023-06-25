<div align="center">
  <img width="500" height="500" src="logo.png">
</div>
<div align="center">
  <img src="https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square">
  <img alt="GitHub Release Date - Published_At" src="https://img.shields.io/github/release-date/thebashpotato/extra-template-library?style=flat-square">
  <img alt="GitHub" src="https://img.shields.io/github/license/thebashpotato/extra-template-library?style=flat-square">
  <img alt="Github" src="https://img.shields.io/badge/C++-17, 20, 23-blue.svg?style=flat-square&logo=c%2B%2B">
  <br>
  <p><b>Extra Templated Library</b> for modern C++ development</p>
</div>

## Table of Contents

- [Etl](#etl)
- [Install](#install)
- [CmakeUsage](#cmakeusage)
- [Usage](#usage)
- [API](#api)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Etl

Etl contains classes that rely on C++ >= 17 to solve common programming problems that the language does not technically have
built in support for. This library has **zero** external dependencies and was born out of my frustration at work and personal projects, as I constantly had to come up with
custom solutions to fix the common problems I ran into, I finally put them all together in this MIT licensed single header templated library.
Hopefully they solve some of your problems, and speed up your development.

### What does this library contain?

Please checkout the unit-tests and example code for use cases and implementations, also don't be afraid to read the source code,
its a single header file, at roughly 500 lines of code. A nice and easy read with descriptive comments.

1. [etl::Result<T, E>](https://github.com/thebashpotato/extra-template-library/blob/main/etl/tests/result_test.cpp)

- Don't like or don't want to use C++'s exception handling paradigm for errors? Cool, then you don't have to, 
  the Result<OkType, ErrType> attempts to be as close to the Rust langauges implementation as possible,
  and may be just what your looking for to ditch those try/catch error handling blocks.

- There is also a [template specialiazation for std::unique_ptr](https://github.com/thebashpotato/extra-template-library/blob/f1dcd42141c26f4826283d84ec39f87d364be621/etl/include/etl.hpp#L457) that can be used as a starting point for you own specialization if
the generic Result<T, E> does not work for your type. This will most likely only be the case for move only types.

2. [etl::EnumerationIterator<IteratorName, IteratorBegin, IteratorEnd>](https://github.com/thebashpotato/extra-template-library/blob/main/etl/tests/enum_iterable_test.cpp)

- Want to use modern C++'s ranged for loops to iterate over an enum safely? There is a templated class for that.

3. [etl::TaggedFundamentalType<Tag, FundamentalType>](https://github.com/thebashpotato/extra-template-library/blob/main/etl/tests/tagged_type_test.cpp)

- Do you have many parameters to a function or constructor of the same type contiguously
  (common when representing geometrical objects such as a Rectangle)? This can result in programmer error by
  accidently passing in values for the wrong parameter, a common solution is to tag your types.
  Etl makes the process generic, quick and easy.

4. [etl::Error](https://github.com/thebashpotato/extra-template-library/blob/f1dcd42141c26f4826283d84ec39f87d364be621/etl/include/etl.hpp#L251)

- A basic error class that supports source code location in your errors, using the function, line, and file macros.
  This makes it much easier to provide usefull runtime error information as it captures the above information through use of a custom
  [SourceCodeLocation](https://github.com/thebashpotato/extra-template-library/blob/f1dcd42141c26f4826283d84ec39f87d364be621/etl/include/etl.hpp#L224) macro which the Error class supports, and can easily be returned in the afore-mentioned `Result<T, E>` object for more Rust like
  behaviour.

- `etl::Error` implements [etl::IError](https://github.com/thebashpotato/extra-template-library/blob/f1dcd42141c26f4826283d84ec39f87d364be621/etl/include/etl.hpp#L234), so if you want to make your own custom errors that play nicely with  `Result<T, E>`, and beable to return a polymorphic error
like so `auto someFunction(std::string const &param) -> etl::Result<std::int32_t, etl::IError>` well you can do that no problem.


## Install

### Add to your project directly (manual way)

[Copy the single header file](extra-template-library/etl/include/etl.hpp) into your project.

Or you can download the `etl.hpp` file from the latest [Releases](https://github.com/thebashpotato/extra-template-library/releases)

### The global manual way

Will install the single header file and Cmake configuration modules

```bash
make install

# Example output
-- Install configuration: "Release"
-- Up-to-date: /usr/local/include
-- Up-to-date: /usr/local/include/etl.hpp
-- Installing: /usr/local/share/pkgconfig/etl.pc
-- Installing: /usr/local/share/cmake/etl/etlConfigVersion.cmake
-- Installing: /usr/local/share/cmake/etl/etlConfig.cmake
```

To remove on linux (relies on xargs)

```bash
make uninstall
```

## CmakeUsage

### CPM (The package manager way)

Please check out how to use [CPM](https://github.com/cpm-cmake/CPM.cmake) it is dead simple.

```cmake
cpmaddpackage(
  NAME
  etl
  GITHUB_REPOSITORY
  thebashpotato/extra-template-library
  VERSION
  0.5.0)

if(etl_ADDED)
  message(STATUS "Extra Template Library added")
  add_library(etl::etl INTERFACE IMPORTED)
  target_include_directories(etl::etl INTERFACE ${etl_SOURCE_DIR}/etl/include)
endif()


# Your Application linking cmake code
target_link_libraries(
  your_awesome_project
  PRIVATE etl::etl)
```

### Find Package

```cmake
find_package(etl 0.5.0 REQUIRED)

# Your Application linking cmake code
target_link_libraries(
  your_awesome_project
  PRIVATE etl::etl)
```

### Package Config

```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(etl REQUIRED etl)

# Your Application linking cmake code
target_link_libraries(your_awesome_project PUBLIC ${etl_LIBRARIES})
target_include_directories(your_awesome_project PUBLIC ${etl_INCLUDE_DIRS})
```

## Usage

[Please see the unit tests](extra-template-library/etl/tests) for examples for each class.
[Please see the example](extra-template-library/etl/examples/blackjack.cpp) for an example blackjack program utilizing the classes to solve real world problems.

```cpp
#include <etl.hpp>
```

## API

TODO

## Maintainers

[@thebashpotato](https://github.com/thebashpotato)

## Contributing

Do you have an idea for something usefull that other C++ programmers would need? Please open an issue, or a Pull Request.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

MIT © 2023 Matt Williams
