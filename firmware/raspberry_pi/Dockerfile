FROM ros:jazzy
SHELL ["/bin/bash", "-c"] 

WORKDIR microros_ws
# Install micro-ros build system
RUN source /opt/ros/jazzy/setup.bash \
    && git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup \ 
    && sudo apt update && rosdep update \
    && rosdep install --from-paths src --ignore-src -y \
    && sudo apt-get install -y python3-pip \
    # Build micro-ROS tools
    && colcon build 

# Install Micro-ros agent
RUN source /opt/ros/jazzy/setup.bash \
    && source install/local_setup.bash \
    && ros2 run micro_ros_setup create_agent_ws.sh