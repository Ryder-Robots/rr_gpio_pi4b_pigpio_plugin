cmake_minimum_required(VERSION 3.20)
project(rr_gpio_pi4b_pigpio_plugin)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(ros-kilted-rr-common-base REQUIRED)
find_package(ros-kilted-rr-interfaces REQUIRED)
find_package(ros-kilted-rr-motor-controller REQUIRED)

add_library(rr_gpio_pi4b_pigpio_plugin src/rr_gpio_pi4b_pigpio_plugin.cpp)
add_library(rr_gpio_pi4b_pigpio_plugin::rr_gpio_pi4b_pigpio_plugin ALIAS rr_gpio_pi4b_pigpio_plugin)
target_compile_features(rr_gpio_pi4b_pigpio_plugin PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_include_directories(rr_gpio_pi4b_pigpio_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_link_libraries(
  rr_gpio_pi4b_pigpio_plugin PUBLIC
  ${ros-kilted-rr-common-base_TARGETS}
  ${ros-kilted-rr-interfaces_TARGETS}
  ${ros-kilted-rr-motor-controller_TARGETS}
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(rr_gpio_pi4b_pigpio_plugin PRIVATE "RR_GPIO_PI4B_PIGPIO_PLUGIN_BUILDING_LIBRARY")

install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)
install(
  TARGETS rr_gpio_pi4b_pigpio_plugin
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(
  "include/${PROJECT_NAME}"
)
ament_export_libraries(
  rr_gpio_pi4b_pigpio_plugin
)
ament_export_targets(
  export_${PROJECT_NAME}
)

ament_package()
