#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "glfw" for configuration "Release"
set_property(TARGET glfw APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(glfw PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "C:/Users/jordan/Documents/HCI_557_2015_JKW/Final Project/SDK/lib/glfw.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS glfw )
list(APPEND _IMPORT_CHECK_FILES_FOR_glfw "C:/Users/jordan/Documents/HCI_557_2015_JKW/Final Project/SDK/lib/glfw.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
