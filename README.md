<div align="center">
  <img width="500" height="500" src="logo.png">
</div>
<div align="center">
  <img src="https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square">
  <br>
  <p><b>Extra Templated Library</b> for modern C++ development</p>
</div>


## Table of Contents

- [Install](#install)
- [Usage](#usage)
- [API](#api)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Install

### Add to your project directly (manual way)

[Copy the single header file](extra-template-library/etl/include/etl.hpp) into your project.

### The global manual way

Will install the single header file and Cmake configuration modules

``` bash
sudo make install

# Example output
-- Install configuration: "Release"
-- Up-to-date: /usr/local/include
-- Up-to-date: /usr/local/include/etl.hpp
-- Installing: /usr/local/lib/Etl/cmake/EtlConfig.cmake
-- Installing: /usr/local/lib/Etl/cmake/EtlConfigVersion.cmake
```

To remove on linux (relies on xargs)
``` bash
sudo make uninstall
```

### CPM (The package manager way)

Please check out how to use [CPM](https://github.com/cpm-cmake/CPM.cmake) it is dead simple.

```cmake
CPMAddPackage("gh:thebashpotato/extra-template-library@0.2.0")
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

PRs accepted.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

MIT © 2023 Matt Williams
