import os

VCPKG_ROOT = os.path.join(".", "vcpkg_installed/arm64-osx")
LIB_PATH = os.path.join(VCPKG_ROOT, "lib")
INC_PATH = os.path.join(VCPKG_ROOT, "include")

LIBS = [
    "glfw3"
]

FRAMEWORKS = [
    'OpenGL',
    'Cocoa',
    'IOKit',
]

source = Glob('./src/*.cpp')

env = Environment(CCFLAGS=['-std=c++17', '-Wall'], CPPDEFINES=['GL_SILENCE_DEPRECATION'])

env.Append(LIBPATH=LIB_PATH)
env.Append(CPPPATH=INC_PATH)
env.Append(LIBS=LIBS)
env.Append(FRAMEWORKS=FRAMEWORKS)

env.Program('./build/CubeDemo', source=source)
