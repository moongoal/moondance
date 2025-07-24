# Moondance Testing Framework

Moondance is a C testing framework.

## Coding Standards

### Versioning

This project uses [Semantic Versioning 2.0](https://semver.org/spec/v2.0.0.html).

## Development

Requirements:

* Python 3.12+
* Python package "pipenv"
* LLVM 20+

### Development Environment Configuration

In this section we'll be assuming all conan profiles are named "default".

Install the required Python packages

```
pipenv sync
```

Install/update the required C dependencies

```
conan install -d full_deploy -pr:a default --build missing -u "" .
```

### Building the Application

In this section we'll assume a debug build type on Windows.

```
scripts/build.bat
```
