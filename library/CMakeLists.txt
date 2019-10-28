cmake_minimum_required(VERSION 3.5)
# Project Name Version & main Languages
project(ModernCMake VERSION 1.0.0 LANGUAGES CXX)

# Create an imported target Threads::Threads
find_package(Threads QUIET REQUIRED)

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


# Add subdirectories
add_subdirectory(ModuleA)
add_subdirectory(ModuleB)