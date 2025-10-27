# Moondance Testing Framework

Moondance is a C89 testing framework.

## Coding Standards

### Versioning

This project uses [Semantic Versioning 2.0](https://semver.org/spec/v2.0.0.html).

## Development

Requirements:

* CMake
* Ninja
* LLVM

### Development Environment Configuration

Configure CMake using one of the available presets, for example

```
cmake --preset Debug
```

### Building the Application

Build the application

```
cmake --build --preset Debug
```

You can check the files that will be installed by running

```
cmake --install build/Debug
```

### Packaging the Application

CMake is configured to generate the application packages when executing

```
cpack --preset Debug
```

This will generate the configured packages into the build folder.

## Sample Test

```
#include <moondance/test.h>

MD_CASE(my_test) { md_assert(1 == 1); }

int main(int argc, char **argv) {
  md_suite suite = md_suite_create();

  md_add(&suite, my_test);

  return md_run(argc, argv, &suite);
}
```
