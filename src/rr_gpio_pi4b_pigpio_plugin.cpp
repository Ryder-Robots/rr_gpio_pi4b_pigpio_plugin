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

  unsigned rev = gpioHardwareRevision();
  if (rev == 0)
  {
    RCLCPP_ERROR(node->get_logger(), "pigpio has an unknown revision");
    return CallbackReturn::FAILURE;
  }

  unsigned model = (rev >> 4) & 0xFF;
  std::ostringstream oss;
  oss << "0x" << std::hex << rev << std::dec;
  hw_rpt["HARDWARE_REVISION"] = oss.str();
  hw_rpt["MODEL_NUMBER"] = static_cast<int>(model);

  return CallbackReturn::SUCCESS;
}

// Calls initialise, future versions should depreicate initialise from common interface.
CallbackReturn RrGpioPi4BPigpioPlugin::on_activate(const rclcpp_lifecycle::State& state)
{
  (void)state;

  // TODO: For safety all pins should be pulled low, this will ensure that they
  // are not left floating.
  int pigpio_ver = 0;
  if ((pigpio_ver = gpioInitialise()) == PI_INIT_FAILED)
  {
    return CallbackReturn::FAILURE;
  }

  hw_rpt["PIGPIO_VERSION"] = pigpio_ver;

  return CallbackReturn::SUCCESS;
}

CallbackReturn RrGpioPi4BPigpioPlugin::on_deactivate(const rclcpp_lifecycle::State& state)
{
  (void)state;

  // TODO set all pins low.
  gpioTerminate();

  // remove version, this will mean this will stop interactions with backend library.
  hw_rpt.erase("PIGPIO_VERSION");
  return CallbackReturn::SUCCESS;
}

CallbackReturn RrGpioPi4BPigpioPlugin::on_cleanup(const rclcpp_lifecycle::State& state)
{
  (void)state;
  hw_rpt.clear();
  return CallbackReturn::SUCCESS;
}

std::map<std::string, ValueType> RrGpioPi4BPigpioPlugin::hardware_report() const
{
  if (hw_rpt.size() == 0)
  {
    // TODO: WARN THAT CLASS WAS NOT CONFIGURED
  }
  return hw_rpt;
}

// TODO: Depreciate
int RrGpioPi4BPigpioPlugin::initialise()
{
  // spit out that activate should be used instead of initialise.
  return 0;
}

// TODO: Deprecate
int RrGpioPi4BPigpioPlugin::terminate()
{
  // spit out that deactivate should be used instead of terminate.
  return 0;
}

int RrGpioPi4BPigpioPlugin::set_pin_mode(unsigned pin, int mode)
{
    if (hw_rpt.find("PIGPIO_VERSION") == hw_rpt.end())
    {
        // TODO spit out that configure and activate must be ran before using this method.
        return -1;
    }
    return gpioSetMode(pin, mode);
}

}  // namespace rr_gpio_pi4b_pigpio_plugin
