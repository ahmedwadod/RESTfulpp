#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Sockpp::sockpp-static" for configuration ""
set_property(TARGET Sockpp::sockpp-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Sockpp::sockpp-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libsockpp.a"
  )

list(APPEND _cmake_import_check_targets Sockpp::sockpp-static )
list(APPEND _cmake_import_check_files_for_Sockpp::sockpp-static "${_IMPORT_PREFIX}/lib/libsockpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
