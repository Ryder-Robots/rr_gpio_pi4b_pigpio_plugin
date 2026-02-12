#ifndef RR_GPIO_PI4B_PIGPIO_PLUGIN__VISIBILITY_CONTROL_H_
#define RR_GPIO_PI4B_PIGPIO_PLUGIN__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_EXPORT __attribute__ ((dllexport))
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_IMPORT __attribute__ ((dllimport))
  #else
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_EXPORT __declspec(dllexport)
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_IMPORT __declspec(dllimport)
  #endif
  #ifdef RR_GPIO_PI4B_PIGPIO_PLUGIN_BUILDING_LIBRARY
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC RR_GPIO_PI4B_PIGPIO_PLUGIN_EXPORT
  #else
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC RR_GPIO_PI4B_PIGPIO_PLUGIN_IMPORT
  #endif
  #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC_TYPE RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC
  #define RR_GPIO_PI4B_PIGPIO_PLUGIN_LOCAL
#else
  #define RR_GPIO_PI4B_PIGPIO_PLUGIN_EXPORT __attribute__ ((visibility("default")))
  #define RR_GPIO_PI4B_PIGPIO_PLUGIN_IMPORT
  #if __GNUC__ >= 4
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC __attribute__ ((visibility("default")))
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC
    #define RR_GPIO_PI4B_PIGPIO_PLUGIN_LOCAL
  #endif
  #define RR_GPIO_PI4B_PIGPIO_PLUGIN_PUBLIC_TYPE
#endif

#endif  // RR_GPIO_PI4B_PIGPIO_PLUGIN__VISIBILITY_CONTROL_H_
