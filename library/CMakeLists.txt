cmake_minimum_required(VERSION 3.5)
# Project Name Version & main Languages
project(ModernCMake VERSION 1.0.0 LANGUAGES CXX)

# Create an imported target Threads::Threads
find_package(Threads QUIET REQUIRED)

# Set standard installation directories
set(RUNTIME_DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
set(LIBRARY_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
set(ARCHIVE_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
set(INCLUDES_DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
set(INCLUDES_INSTALL_DIR ${INCLUDES_DESTINATION}/ModernCMake)

# Generate cmake configuration scripts (MC = ModernCMake)
set(MC_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(MC_VERSION_CONFIG "${MC_GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(MC_PROJECT_CONFIG "${MC_GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
set(MC_TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(MC_CONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}")
set(MC_NAMESPACE "${PROJECT_NAME}::")
set(MC_VERSION 1.0.0)

# Generate basic config version files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${MC_VERSION_CONFIG}" VERSION ${MC_VERSION} COMPATIBILITY SameMajorVersion
)

configure_file("${ModernCMake_SOURCE_DIR}/cmake_modules/Config.cmake.in" "${MC_PROJECT_CONFIG}" @ONLY)

install(
    FILES "${MC_PROJECT_CONFIG}" "${MC_VERSION_CONFIG}"
    DESTINATION "${MC_CONFIG_INSTALL_DIR}")

install(
    EXPORT "${MC_TARGETS_EXPORT_NAME}"
    NAMESPACE "${MC_NAMESPACE}"
    DESTINATION "${MC_CONFIG_INSTALL_DIR}")


# Add subdirectories
add_subdirectory(ModuleA)
add_subdirectory(ModuleB)