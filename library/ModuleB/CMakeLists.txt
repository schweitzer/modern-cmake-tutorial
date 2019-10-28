# Creation of B library (don't GLOB)
add_library(B
    include/B.hpp
    src/B.cpp)

# Use Generators expression to deals with folders (includes may not be at the same place between build and install).
target_include_directories(
    B
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:headers>
)

# Linking A with Thread::Thread library.
target_link_libraries(B PUBLIC A)

install(TARGETS B
  EXPORT ${MC_TARGETS_EXPORT_NAME}
  RUNTIME DESTINATION ${RUNTIME_DESTINATION}
  LIBRARY DESTINATION ${LIBRARY_DESTINATION}
  ARCHIVE DESTINATION ${ARCHIVE_DESTINATION}
)