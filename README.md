<div align="center">
  <img width="500" height="500" src="logo.png">
</div>
<div align="center">
  <img alt="GitHub Workflow Status (with event)" src="https://img.shields.io/github/actions/workflow/status/thebashpotato/etl/cmake.yml?style=flat-square&logo=ubuntu&label=Build%20Status">
  <img src="https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square">
  <img alt="GitHub Release Date - Published_At" src="https://img.shields.io/github/release-date/thebashpotato/etl?style=flat-square">
  <img alt="GitHub" src="https://img.shields.io/github/license/thebashpotato/etl?style=flat-square">
  <img alt="Github" src="https://img.shields.io/badge/C++-17, 20, 23-blue.svg?style=flat-square&logo=c%2B%2B">
  <br>
  <p><b>Extra Templated Library</b> for modern C++ development</p>
</div>

## Table of Contents

- [Etl](#etl)
- [Integration](#integration)
- [Usage](#usage)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Etl

Etl contains classes that rely on C++ >= 17 to solve common programming problems that the language does not technically have
built in support for. This library has **zero** external dependencies and was born out of my frustration at work and personal projects, as I constantly had to come up with
custom solutions to fix the common problems I ran into, I finally put them all together in this `BSD-Clause-3` licensed single header templated library.
Hopefully they solve some of your problems, and speed up your development.

### What does this library contain?

Please checkout the unit-tests and example code for use cases and implementations, also don't be afraid to read the source code,
its a single header file, at roughly 798 lines of code. A nice and easy read with descriptive comments.

1. [etl::Result<T, E>](https://github.com/thebashpotato/etl/blob/2510c16bfcf22b4730ace5f7337c6d51adbc53a0/etl/include/etl.hpp#L586)

- Don't like or don't want to use C++'s exception handling paradigm for errors? Cool, then you don't have to, 
  the `etl::Result<T, E>` attempts to be as close to the Rust langauges implementation as possible,
  and may be just what your looking for to ditch those try/catch error handling blocks.

- If you are thinking, how do I handle returning a `Result<T, E>` from my `void` routines, it's easy. etl supplies a stub type called `etl::Void`
  (it is just an empty class).

```cpp
// Old code which throws an exception
void DoTheThing(const bool thing)
{
    if (thing)
    {
        std::cout << "Doing the thing" << '\n';
        // more code
        return;
    }

    throw std::runtime_error("Couldn't do the thing");
}

using namespace etl;

// New code ported to use etl::Result<T, E>
[[nodiscard]] auto DoingTheThingBetter(const bool thing) noexcept -> Result<Void, BaseError>
{
    if (thing)
    {
        std::cout << "Doing the thing" << '\n';
        // more code
        return Result<Void, BaseError>(Void());
    }

    return Result<Void, BaseError>({"Still couldn't do the thing", RUNTIME_INFO});
}

// Let's compare the code using both functions
auto main() -> int
{
    try
    {
        DoTheThing(false);
        // Hey we can continue..
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
    }

    if (auto result = DoingTheThingBetter(false); result.is_ok())
    {
        // Hey we can continue..
        
        // You could get the underlying object, but the point is that it's void
        [[maybe_unused]] Void void_type = result.ok().value();
    } 
    else 
    {
        std::cerr << result.err().value().info() << '\n'
    }


}
```

- One current catch with the `etl::Result<T, E>` type is if you have a **move only type** you will need to "hi-jack" the etl namespace
  and create a template specialization for `Result<YourMoveOnlyType, E>`, but don't worry it's easy. 
  I have provided an example [here](https://github.com/thebashpotato/etl/blob/2510c16bfcf22b4730ace5f7337c6d51adbc53a0/etl/examples/moveonly/moveonly.hpp#L12) which you can copy and paste, (Just replace the name of the the namespaced class with your own).

2. [etl::EnumerationIterator<IteratorName, IteratorBegin, IteratorEnd>](https://github.com/thebashpotato/etl/blob/main/etl/tests/enum_iterable_test.cpp)

- Want to use modern C++'s ranged for loops to iterate over an enum safely? There is a templated class for that.

- Be aware that your enum **MUST** be contiguous otherwise`EnumerationIterator` won't work.

3. [etl::TaggedFundamentalType<Tag, FundamentalType>](https://github.com/thebashpotato/etl/blob/main/etl/tests/tagged_type_test.cpp)

- Do you have many parameters to a function or constructor of the same type contiguously
  (common when representing geometrical objects such as a Rectangle)? This can result in programmer error by
  accidently passing in values for the wrong parameter, a common solution is to tag your types.
  etl makes the process generic, quick and easy.

- The TaggedFundamentalType supports all arithmetic, boolean and bitwise operator overloads.

4. [etl::BaseError](https://github.com/thebashpotato/etl/blob/2510c16bfcf22b4730ace5f7337c6d51adbc53a0/etl/include/etl.hpp#L512)

- An Abstract error class that supports source code location in yours errors, using the function, line and file macros.
  You can easily override all the methods, but will likely be uneeded. Just define two constructors and you're on your way.

- Example of using the BaseError class in your code.

  ```cpp
  #include <iostream>
  #include <etl.hpp> 

  namespace YourApp {

  class Error : public etl::BaseError
  {
    public:
      explicit Error(const std::string_view &msg) noexcept : etl::BaseError(msg)
      {
      }

      Error(const std::string_view &msg, const etl::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
      {
      }
  };
  } // namespace YourApp

  auto main() -> int
  {
      // Create a basic error
      YourApp::Error first_error("My custom message");
      std::cout << first_error.msg() << '\n';

      // Create an error with the correct source code location (file, function name, line, supplied by the convenience macro RUNTIME_INFO) 
      // of where the error was constructed.
      YourApp::Error second_error("Another error with information", etl::RUNTIME_INFO);
      std::cout << second_error.info() << '\n';

      return 0;
  }
  ```


- A basic error class that supports source code location in your errors, using the function, line, and file macros.
  This makes it much easier to provide usefull runtime error information as it captures the above information through use of a custom source code location macro called
  [etl::RUNTIME_INFO](https://github.com/thebashpotato/etl/blob/2510c16bfcf22b4730ace5f7337c6d51adbc53a0/etl/include/etl.hpp#L502) macro which the BaseError class supports.

- `etl::DynError` a type definition which is just a `std::shared_ptr<BaseError>` to support returning polymorphic errors up a function call chain with less typing.
   ```cpp
   [[nodiscard]] auto SomeClass::SomeMethod() noexcept -> etl::Result<std::string, etl::DynError>;
   ```


## Integration


[Copy the single header file](etl/include/etl.hpp) into your project.

Or you can download the `etl.hpp` file from the latest [Releases](https://github.com/thebashpotato/etl/releases)

### Install globally on your system (assuming you're on Linux or a BSD)

Will install the single header file and Cmake configuration modules, which `find_package` can use.

```bash
make install

# Example output
-- Install configuration: "Release"
-- Up-to-date: /usr/local/include
-- Up-to-date: /usr/local/include/etl.hpp
-- Installing: /usr/local/share/pkgconfig/etl.pc
-- Installing: /usr/local/share/cmake/etl/etlConfigVersion.cmake
-- Installing: /usr/local/share/cmake/etl/etlConfig.cmake
-- Installing: /usr/local/share/cmake/etl/etlTargets.cmake
```

To remove on linux (relies on xargs)

```bash
make uninstall
```

### Cmake

### Find Package (assuming it has been installed globally on your system)

```cmake
find_package(etl 0.8.0 REQUIRED)

# Your Application linking cmake code
target_link_libraries(
  your_awesome_project
  PRIVATE etl::etl)
```

#### CPM (The package manager way)

Please check out how to use [CPM](https://github.com/cpm-cmake/CPM.cmake) it is dead simple.

```cmake
cpmaddpackage(
  NAME
  etl
  GITHUB_REPOSITORY
  thebashpotato/etl
  VERSION
  0.8.0)

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

### Package Config

#### Cmake

```cmake
find_package(PkgConfig REQUIRED IMPORTED_TARGET GLOBAL)
pkg_check_modules(Etl REQUIRED etl)

# Your Application linking cmake code
target_link_libraries(your_awesome_project PUBLIC PkgConfig::Etl)
target_include_directories(your_awesome_project PUBLIC ${Etl_INCLUDE_DIRS})
```

#### Bare Makefile

If you are using bare Makefiles, you can use `pkg-config` to generate the include flags that point to where the library is installed.

```bash
pkg-config etl --cflags
```

## Usage

1. [Please see the unit tests](etl/tests) for bite size examples for each class.

2. [Please see](etl/examples/blackjack) for an example blackjack program utilizing etl to solve real world problems.

3. [Please see](etl/examples/moveonly) for an example for a Result<T, E> move only class template specialization.

```cpp
#include <etl.hpp>
```

## Maintainers

[@thebashpotato](https://github.com/thebashpotato)

## Contributing

Do you have an idea for something usefull that other C++ programmers would need? Please open an issue, or a Pull Request.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

BSD-Clause-3 © 2024 Matt Williams
