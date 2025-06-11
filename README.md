# meSP2

![meSP2](./assets/logo.png)

**meSP2** is an open-source C++20 Quake 3 modding library for OSP2.
This is highly experimental, but free to use library.

Basic usage implies usage of LLVM's infrastructure with `LLVM IR -> q3asm` transpilers, so you can use llvm-compatible compiler, e.g. `clang`, to compile code. _**Wait for more information to be added.**_

**Note**: _meSP2_ significantly relies on llvm's `libc++`. For more information, visit official llvm's [libc++ site](https://libcxx.llvm.org/).

It has several submodules:

- **Math submodule**: responsive for most linear and basic algebra operations.
- **Parsing submodule**: provides parsing functionality for _CPMA superhud configurations_ and _CSS meSP2's AHUD_.
- **UI submodule**: implements new UI approach, specific for meSP2 - _DOM-like interface_, inspired by web-browsers.

Find out more in the docs.

## Docs

Docs are available [here](./docs/index.md).
