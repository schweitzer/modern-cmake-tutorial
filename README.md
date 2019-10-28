# modern_cmake

Tutorial/Example to deal with modern cmake.
This tutorial assume that you already know how to write a CMakeLists.txt

## Introduction

This repository contains samples to deals with modern cmake (imported targets, etc...).
The repository is splitted into two differents parts:

* A library called `ModernCMake`
* An executable called `sampleExec`

## ModernCMake library

### General

The library contains some dummy C++ code (that's not the point) to show how to make a library in a modern-cmake-way.
Please read comments in each CMakeLists.txt for detailled informations about each instruction.

Here's the basic structure of the project:

```
|library
  | CMakeLists.txt
  | ModuleA
    | CMakeLists.txt
    | src/A.cpp
    | include/A.hpp
  | ModuleB
    | CMakeLists.txt
    | src/B.cpp
    | include/B.cpp
  |cmake_modules
    | config.cmake.in
```

The concept here is to build two libraries (libA & libB), libA depends on std::thread "library" [(an imported target provided by CMake)](https://cmake.org/cmake/help/v3.16/module/FindThreads.html), and libB depends on libA. The example shows you how to made thoses dependencies transitives.

After that you can build `sampleExec` to link against ModernCMake library in a modern-way.

### How to build it

To build `ModernCMake` libraries

`$ cmake path/to/modern_cmake/src/library -G Ninja -DCMAKE_BUILD_TYPE=[Release or Debug] -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/path/to/install/dir`

To install it please run:

`$ ninja install`

### Modern CMake concepts

I'm pretty bad in preambles so...

Credits from Pablo Arias from the article ["It's time to do CMake Right"](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/).

> Enough preambles. Does this look familiar to you?

```cmake
find_package(Boost 1.55 COMPONENTS asio)
list(APPEND INCLUDE_DIRS ${BOOST_INCLUDE_DIRS})
list(APPEND LIBRARIES ${BOOST_LIBRARIES})

include_directories(${INCLUDE_DIRS})
link_libraries(${LIBRARIES})
```

>Don’t. Just don’t. This is wrong in so many dimensions. You are just blindly throwing stuff into a pot of include directories and compiler flags. There is no structure. There is no transparency. Not to mention that functions like include_directories work at the directory level and apply to all entities defined in scope.

>And this isn’t even the real problem, what do you do with transitive dependencies? What about the order of linking? Yes, you need to take care about that yourself. The moment you need to deal with the dependencies of your dependencies is the moment your life needs to be reevaluated.

I try here to explain a few basic concepts that you should keep in mind when creating a library with CMake using the ModernCMake library as an example.

Let's start with the project

```cmake
cmake_minimum_required(VERSION 3.5)
# Project Name Version & main Languages
project(ModernCMake VERSION 1.0.0 LANGUAGES CXX)
```

Nothing fancy here, the project is defined by a NAME a VERSION and the main LANGUAGES.

Note that CMake generates `${PROJECT_NAME}` , `${PROJECT_VERSION}`.

#### Imported Targets

Find your required external libraries, here I will use `std::thread` and CMake made a nice [imported target](https://cmake.org/cmake/help/v3.16/module/FindThreads.html) to use it.

```cmake
# Create an imported target Threads::Threads
find_package(Threads QUIET REQUIRED)
```

Let's create the first library **A** (a really inspired name)

```cmake
# Creation of A library (don't GLOB)
add_library(A
    include/A.hpp
    src/A.cpp
    )

```

Please avoid globbing expression, it is much more readable if all necessary files are written in the `add_library`.

Let's define some property on **A** target, why not start with includes ?

#### Use generator-expressions

```cmake
# Use Generators expression to deals with folders (includes may not be at the same place between build and install).
target_include_directories(
    A
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:headers>
)
```

Here I use what cmake called [generator-expressions](https://cmake.org/cmake/help/v3.16/manual/cmake-generator-expressions.7.html). Includes directory may be different between build and install, so with generators I can tell to the target where to find headers. You can also provide more complex mechanism with PRIVATE headers, etc...

#### Please export your Targets

Last but not the least is the export of our target. We want to do this in a nice way so that users will only to call somehting like this:

```cmake
findPackage(ModernCMake QUIET COMPONENTS ModernCMake::A REQUIRED)
```

 and to link the desired target to their library/application.

```cmake
target_link_libraries( TARGET PUBLIC ModernCMake::A)
```

**AND THAT'S IT**, we don't want to link with additionnal library or includes 1000 directories, because exported targets are transitives (they forwards theirs needed requirements) and also exports the public headers.

If you need **A** library, you only **want** to link against **A**. The last think the user want to do is to dig into your library to find which library is missing...

In CMake, installed targets are registered to exports using the **EXPORT** argument. Exports are therefore just a set of targets that can be exported and installed. Here we just told CMake to install our library and to register the target in the export file.

_in Module/A/CMakeLists.txt_

```cmake
...
# Install A library, the EXPORT is required to export your library as a COMPONENTS (ModernCMake::A)
install(TARGETS A
  EXPORT ${MC_TARGETS_EXPORT_NAME}
  RUNTIME DESTINATION ${RUNTIME_DESTINATION}
  LIBRARY DESTINATION ${LIBRARY_DESTINATION}
  ARCHIVE DESTINATION ${ARCHIVE_DESTINATION}
```

If you look into the main CMakeLists.txt you will see

```cmake
# Set standard installation directories
set(RUNTIME_DESTINATION ${CMAKE_INSTALL_PREFIX}/bin) #DLL
set(LIBRARY_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib) # Libraries (.so, .lib, .dylib)
set(ARCHIVE_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib) # Archives files (if needed)
set(INCLUDES_DESTINATION ${CMAKE_INSTALL_PREFIX}/headers) # Install headers in a headers directory
set(INCLUDES_INSTALL_DIR ${INCLUDES_DESTINATION}/ModernCMake) #Add a ModernCmake directory after headers/

# Generate cmake configuration scripts (MC = ModernCMake)
# Where to generate cmake files (build dir)
set(MC_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
# Name of cmake config files
set(MC_VERSION_CONFIG "${MC_GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(MC_PROJECT_CONFIG "${MC_GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
# Name of CMake Targets file (ModernCMakeTargets.cmake)
set(MC_TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
# Where to install cmake "Targets" files
set(MC_CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")
# Set a namespace before targets (each target will be prefixed by ModernCMake::)
set(MC_NAMESPACE "${PROJECT_NAME}::")
# Version (redondant here we can use ${PROJECT_VERSION})
set(MC_VERSION 1.0.0)

# Generate basic config version files
include(CMakePackageConfigHelpers)
# This is a cmake function that create basic ProjectconfigVersion.cmake files.
write_basic_package_version_file(
    "${MC_VERSION_CONFIG}" VERSION ${MC_VERSION} COMPATIBILITY SameMajorVersion
)
# Configure the config.cmake.in
configure_file("${ModernCMake_SOURCE_DIR}/cmake_modules/Config.cmake.in" "${MC_PROJECT_CONFIG}" @ONLY)

# Install cmake config files
install(
    FILES "${MC_PROJECT_CONFIG}" "${MC_VERSION_CONFIG}"
    DESTINATION "${MC_CONFIG_INSTALL_DIR}")

# Install cmake targets files
install(
    EXPORT "${MC_TARGETS_EXPORT_NAME}"
    NAMESPACE "${MC_NAMESPACE}"
    DESTINATION "${MC_CONFIG_INSTALL_DIR}")
```

While the two first blocks are only setting some filenames or paths, the interesting part is the end. CMake provide a function to generate [a basic configVersion file](https://cmake.org/cmake/help/v3.16/module/CMakePackageConfigHelpers.html#command:write_basic_package_version_file). 



The interesting part are the generated cmake files. When the library is installed you will see some additionnal files installed by CMAKE in `your/install/dir/lib/cmake/ModernCMake`.

This files are realy the basis of doing cmake right, while when writting `find_package(myPackage)`CMake will first looking for `myPackageConfig.cmake` ! So you don't have any excuses to not provide Config Files.

Let's take a look in the install folder of ModernCMake, you can see that config files are installed in `path/to/the/install/dir/lib/cmake/ModernCMake/` , you will see 4 files:

* `ModernCMakeConfig.cmake`: the basic config file (here provided by us)
* `ModernCMakeConfigVersion.cmake`: the generated config file by cmake, it contains some code to check if version are compatible (required version vs provided version).
* `ModernCMakeTargets.cmake`: our exported targets see below.
* `ModernCMakeTargets-_[release/debug]_.cmake`: additionnal informations of our targets for specific build (debug, release, ...).

The most important part are in the `ModernCMakeTargets.cmake`

```cmake
# Create imported target ModernCMake::A
add_library(ModernCMake::A SHARED IMPORTED)

set_target_properties(ModernCMake::A PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/headers"
  INTERFACE_LINK_LIBRARIES "Threads::Threads"
)

# Create imported target ModernCMake::B
add_library(ModernCMake::B SHARED IMPORTED)

set_target_properties(ModernCMake::B PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/headers"
  INTERFACE_LINK_LIBRARIES "ModernCMake::A"
)
```

You can see that each of our targets exports their own includes dirs and link libraries. As a result if you write an application or a library that use for example `ModernCMake::B`, it will automaticaly forward the link to `ModernCMake::A` and therefore the link to `Threads::Threads` !

Let's verify this.

## A sample to rules them all

Let's build an application that uses ModernCMake library.

the executable folder contain a CMakeLists.txt and a main.cpp

To build it launch (from a build directory):

`$ cmake ../../../src/modern_cmake/executable -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/Path/to/a/install/directory/`

CMake will ask you to set the `ModernCMake_DIR`. You will need to set it to the `install/dir/of/ModernCMake/lib/cmake/ModernCMake`, so that cmake find the config files, remember ?

after that run `ninja`, and magic of cmake should happend.

A quick look to the `./sampleExec` shared libraries (ldd or otools), and you will see that both ModernCMake::A & B are linked. 

Yeah no suprise, but if we look at the CMakeLists.txt of sampleExec you will that

```cmake
# Project Name Version & main Languages.
project(sampleExec VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the B Component of ModernCMake.
find_package(ModernCMake QUIET COMPONENTS ModernCMake::B)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ModernCMake::B)
```

We only ask cmake to find `ModernCMake::B` and to link `ModernCMake::B` to `sampleExec`. But since B needs A and thanks to our nice exported targets cmake find everyting, that's **transitivity baby** !

You can also launch `sampleExec` to see that we use both libraries:

```shell
$ ./sampleExec 
[sampleExec]: Hello world !
[ModernCMake::B]: Call ModernCMake::A
[ModernCMake::A]: Lanch process...
Value: 10
```

## More Ressources

* https://cliutils.gitlab.io/modern-cmake/
* https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
* https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
* https://rix0r.nl/blog/2015/08/13/cmake-guide/
* https://www.youtube.com/watch?v=y7ndUhdQuU8&feature=youtu.be