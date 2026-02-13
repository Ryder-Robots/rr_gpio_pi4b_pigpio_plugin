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

/*
 * This interface can be used by GPIO plugins, note that while this header attempts to be generic. The implementations
 * are not. For example the GPIO implementation for Pi 4B MUST not be used on a Jetson, or Pi5. This will
 * risk damage to hardware. For development purposes non working implementations will be supplied these can
 * act as interfaces for Gazebo simulation and testing on unsupported hardware, or test generic logic
 * that is not hardware specific.
 *
 * This header assumes that direct headers to the SBCs GPIO interface is required.
 */


#pragma once

#include <map>
#include <string>
#include <pigpio.h>
#include "rr_common_base/rr_gpio_plugin_iface.hpp"
#include "rr_gpio_pi4b_pigpio_plugin/visibility_control.h"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

namespace rr_gpio_pi4b_pigpio_plugin
{

class RrGpioPi4BPigpioPlugin
{
public:
  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;
  using ValueType = std::variant<int, std::string>;
  using gpio_isr_func_ex_t = void (*)(int gpio, int level, uint32_t tick, void* userdata);

  RrGpioPi4BPigpioPlugin() = default;

  virtual ~RrGpioPi4BPigpioPlugin() = default;
CallbackReturn configure(const rclcpp_lifecycle::State& state,
                                   rclcpp_lifecycle::LifecycleNode::SharedPtr node) override;


    CallbackReturn on_activate(const rclcpp_lifecycle::State& state) override;

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State& state) override;

CallbackReturn on_cleanup(const rclcpp_lifecycle::State& state) override;


std::map<std::string, ValueType> hardware_report() const override;

int initialise() override;

int terminate() override;

int set_pin_mode(int pin, int mode) override;

int set_pull_up_down(int pin, int pud) override;

int set_isr_func_ex(int pin, int edge, gpio_isr_func_ex_t func, void* userdata) override;

int clear_isr_func(unsigned gpio) override;

int digital_write(unsigned gpio, unsigned level) override;

int digital_read(unsigned gpio) override;

int gpio_hardware_pwm(unsigned pin, unsigned pwm_freq, unsigned pwm_duty_cycle) override;

int gpio_hardware_get_pwm(unsigned pin) override;

std::list <unsigned> get_pwm_pins() const override;
};

}  // namespace rr_gpio_pi4b_pigpio_plugin

