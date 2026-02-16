# rr_gpio_pi4b_pigpio_plugin

Note this library needs to run as root user

```bash
sudo su -
export ROS_LOCALHOST_ONLY=1
export ROS_DISTRO=kilted
source /opt/ros/${ROS_DISTRO}/setup.bash
cd ~[YOUR_USERNAME]/ros2_ws/
source install/setup.bash
ros2 launch rr_mousebot_bringup rr_mousebot.launch.py &
```
