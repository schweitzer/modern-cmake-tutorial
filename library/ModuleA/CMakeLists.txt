# Creation of A library (don't GLOB)
add_library(A
    include/A.hpp
    src/A.cpp
    )

# Use Generators expression to deals with folders (includes may not be at the same place between build and install).
target_include_directories(
    A
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:headers>
)

# Linking A with Thread::Thread library. Will populate the INTERFACE_LINK_LIBRARIES
target_link_libraries(A PUBLIC Threads::Threads)

# Install A library, the EXPORT is required to export your library as a COMPONENTS (ModernCMake::A)
install(TARGETS A
  EXPORT ${MC_TARGETS_EXPORT_NAME}
  RUNTIME DESTINATION ${RUNTIME_DESTINATION}
  LIBRARY DESTINATION ${LIBRARY_DESTINATION}
  ARCHIVE DESTINATION ${ARCHIVE_DESTINATION}
)

install(FILES include/A.hpp DESTINATION ${INCLUDES_INSTALL_DIR})