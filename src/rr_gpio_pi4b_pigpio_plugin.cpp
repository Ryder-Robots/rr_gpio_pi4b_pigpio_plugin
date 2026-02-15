// Copyright (c) 2026 Ryder Robots
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "rr_gpio_pi4b_pigpio_plugin/rr_gpio_pi4b_pigpio_plugin.hpp"

namespace rr_gpio_pi4b_pigpio_plugin
{
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
using ValueType = std::variant<int, std::string>;
using gpio_isr_func_ex_t = void (*)(int gpio, int level, uint32_t tick, void* userdata);

// Generate hardware report. This can be used for later debugging if needed.
CallbackReturn RrGpioPi4BPigpioPlugin::configure(const rclcpp_lifecycle::State& state,
                                                 rclcpp_lifecycle::LifecycleNode::SharedPtr node)
{
  (void)state;
  (void)node;

  unsigned rev = gpioHardwareRevision();
  if (rev == 0)
  {
    RCLCPP_ERROR(rclcpp::get_logger("GPIO_PI4"), "pigpio has an unknown revision!!!");
    return CallbackReturn::FAILURE;
  }

  RCLCPP_INFO(rclcpp::get_logger("GPIO_PI4"), "configuring GPIO");
  unsigned model = (rev >> 4) & 0xFF;
  std::ostringstream oss;
  oss << "0x" << std::hex << rev << std::dec;
  hw_rpt_["HARDWARE_REVISION"] = oss.str();
  hw_rpt_["RAW_HW_REVISION"] = static_cast<int>(rev);
  hw_rpt_["MODEL_NUMBER"] = static_cast<int>(model);

  return CallbackReturn::SUCCESS;
}

// Calls initialise, future versions should depreicate initialise from common interface.
CallbackReturn RrGpioPi4BPigpioPlugin::on_activate(const rclcpp_lifecycle::State& state)
{
  (void)state;
  int pigpio_ver = 0;
  if ((pigpio_ver = gpioInitialise()) == PI_INIT_FAILED)
  {
    RCLCPP_ERROR(rclcpp::get_logger("GPIO_PI4"), "monitor initilization failed!!!");
    return CallbackReturn::FAILURE;
  }

  hw_rpt_["PIGPIO_VERSION"] = pigpio_ver;

  return CallbackReturn::SUCCESS;
}

CallbackReturn RrGpioPi4BPigpioPlugin::on_deactivate(const rclcpp_lifecycle::State& state)
{
  (void)state;

  // TODO set all pins low.
  gpioTerminate();

  // remove version, this will mean this will stop interactions with backend library.
  hw_rpt_.erase("PIGPIO_VERSION");
  return CallbackReturn::SUCCESS;
}

CallbackReturn RrGpioPi4BPigpioPlugin::on_cleanup(const rclcpp_lifecycle::State& state)
{
  (void)state;
  hw_rpt_.clear();
  return CallbackReturn::SUCCESS;
}

std::map<std::string, ValueType> RrGpioPi4BPigpioPlugin::hardware_report() const
{
  if (hw_rpt_.size() == 0)
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "interface does not appear to be configured");
  }
  return hw_rpt_;
}

// TODO: Depreciate
int RrGpioPi4BPigpioPlugin::initialise()
{
  // spit out that activate should be used instead of initialise.
  return gpioInitialise();
}

// TODO: Deprecate
int RrGpioPi4BPigpioPlugin::terminate()
{
  // spit out that deactivate should be used instead of terminate.
  gpioTerminate();
  return 0;
}

int RrGpioPi4BPigpioPlugin::set_pin_mode(unsigned pin, int mode)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "set_pin_mode - interface is not activated");
    return -1;
  }
  return gpioSetMode(pin, mode);
}

int RrGpioPi4BPigpioPlugin::set_pull_up_down(unsigned pin, unsigned pud)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "set_pull_up_down - interface is not activated");
    return -1;
  }
  return gpioSetPullUpDown(pin, pud);
}

int RrGpioPi4BPigpioPlugin::set_isr_func_ex(unsigned gpio, unsigned edge, int timeout, gpio_isr_func_ex_t func,
                                            void* userdata)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "set_isr_func_ex - interface is not activated");
    return -1;
  }
  int rv = -1;
  if ((rv = gpioSetISRFuncEx(gpio, edge, timeout, func, userdata)) >= 0)
  {
    gpio_cb_[gpio] = func;
  }
  return rv;
}

int RrGpioPi4BPigpioPlugin::clear_isr_func(unsigned gpio)
{
  gpio_cb_[gpio] = nullptr;
  return 0;
}

int RrGpioPi4BPigpioPlugin::digital_write(unsigned gpio, unsigned level)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "digital_write - interface is not activated");
    return -1;
  }
  return gpioWrite(gpio, level);
}

int RrGpioPi4BPigpioPlugin::digital_read(unsigned gpio)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "digital_read - interface is not activated");
    return -1;
  }
  return gpioRead(gpio);
}

// TODO this needs to be renamed to analog_write() in order to match common names.
int RrGpioPi4BPigpioPlugin::gpio_hardware_pwm(unsigned pin, unsigned pwm_freq, unsigned pwm_duty_cycle)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "gpio_hardware_pwm - interface is not activated");
    return -1;
  }
  return gpioHardwarePWM(pin, pwm_freq, pwm_duty_cycle);
}

// TODO this needs to have its name changed to analog_read() to match common reference in next version.
// Note this only returns what the interface has written to the pin, not what is externally recieved.
// Perhaps more work needs to be done here, to include a callback.
int RrGpioPi4BPigpioPlugin::gpio_hardware_get_pwm(unsigned pin)
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "gpio_hardware_get_pwm - interface is not activated");
    return -1;
  }
  return gpioGetPWMdutycycle(pin);
}

std::list<unsigned> RrGpioPi4BPigpioPlugin::get_pwm_pins() const
{
  std::list<unsigned> pwm_pins = {};
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "get_pwm_pins - interface is not activated");
    return pwm_pins;
  }

  auto it = hw_rpt_.find("RAW_HW_REVISION");
  if (it == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "hardware revision not available");
    return pwm_pins;
  }

  unsigned rev = static_cast<unsigned>(std::get<int>(hw_rpt_.at("RAW_HW_REVISION")));
  // BCM2835 (and BCM2711 on Pi 4)
  if ((rev & 0x800000) || (rev > 0 && rev < 0x0016))
  {
    // New-style revision code
    pwm_pins = { 12, 13, 18, 19 };
  }
  else
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "unknown board");
  }

  return pwm_pins;
}

uint32_t RrGpioPi4BPigpioPlugin::tick()
{
  if (hw_rpt_.find("PIGPIO_VERSION") == hw_rpt_.end())
  {
    RCLCPP_WARN(rclcpp::get_logger("GPIO_PI4"), "tick - interface is not activated");
    return 0;
  }
  return gpioTick();
}

}  // namespace rr_gpio_pi4b_pigpio_plugin
PLUGINLIB_EXPORT_CLASS(rr_gpio_pi4b_pigpio_plugin::RrGpioPi4BPigpioPlugin, rrobots::interfaces::RRGPIOInterface)