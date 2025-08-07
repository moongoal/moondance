import os.path as path

from conan import ConanFile
from conan.tools.files import load, copy
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


required_conan_version = ">=2.10"


class Moondance(ConanFile):
    name = "moondance"
    description = "Moondance C testing framework."
    license = "UNLICENSED"
    author = "Alfredo Mungo"
    topics = "testing",
    url = "https://github.com/moongoal/moondance"
    package_type = "static-library"
    version = "1.0.2"

    build_requires = (
        "cmake/[>=4.0]",
        "ninja/[>=1.12.1]"
    )

    exports = (
        "README.md",
        "Pipfile",
        "Pipfile.lock"
    )

    exports_sources = (
        "include/*",
        "src/*",
        "test/*",
        "CMakeLists.txt",
        "scripts/*.cmake"
    )

    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        deps = CMakeDeps(self)

        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.cache_variables["MD_VERSION"] = str(self.version)

        tc.generate()
        deps.generate()

    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        copy(self, "**.h", path.join(self.source_folder, "include"), path.join(self.package_folder, "include"))
        copy(self, "**.h", path.join(self.build_folder, "include"), path.join(self.package_folder, "include"))
        copy(self, self._libname("**"), self.build_folder, path.join(self.package_folder, "lib"))
        copy(self, "**.pdb", self.build_folder, path.join(self.package_folder, "lib"))

    def package_info(self):
        self.cpp_info.libs = ["moondance"]

    def _libname(self, basename: str) -> str:
        if self.settings.os == "Windows":
            return f'{basename}.lib'
        else:
            return f'lib{basename}.a'
