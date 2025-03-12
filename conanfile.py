from conan import ConanFile
from conan.errors import ConanInvalidConfiguration


class BasicConanfile(ConanFile):
    name = "C_script_runner"
    version = "1.0"
    description = "c script runner recipe"
    license = "Apache-2.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("rapidcsv/8.84")
        self.requires("implot/0.16")

    def build_requirements(self):
        pass

    def generate(self):
        pass

    def build(self):
        pass

    def package(self):
        pass
