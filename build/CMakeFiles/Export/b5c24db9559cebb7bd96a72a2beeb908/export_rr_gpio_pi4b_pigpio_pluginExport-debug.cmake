#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin" for configuration "Debug"
set_property(TARGET rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/librr_gpio_pi4b_pigpio_plugin.so"
  IMPORTED_SONAME_DEBUG "librr_gpio_pi4b_pigpio_plugin.so"
  )

list(APPEND _cmake_import_check_targets rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin )
list(APPEND _cmake_import_check_files_for_rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin "${_IMPORT_PREFIX}/lib/librr_gpio_pi4b_pigpio_plugin.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
