import os.path

from conan import ConanFile, conan_version
from conan.errors import ConanException
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run


if conan_version.major < 2:
    raise ConanException("This recipe requires at least conan 2.")


class TestPackageRecipe(ConanFile):
    name = "moondance-test-package"
    generators = "CMakeToolchain", "CMakeDeps"
    settings = "os", "compiler", "build_type", "arch"
    tool_requires = "cmake/[>=4.0.3]",

    def requirements(self):
        self.requires(self.tested_reference_str)
    
    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "test_exe.exe")
            self.run(cmd, env="conanrun")
