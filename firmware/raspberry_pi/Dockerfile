FROM ros:jazzy
SHELL ["/bin/bash", "-c"] 

# Install micro-ros build system
RUN source /opt/ros/jazzy/setup.bash
WORKDIR microros_ws
RUN git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup
RUN sudo apt update && rosdep update
RUN rosdep install --from-paths src --ignore-src -y
RUN sudo apt-get install -y python3-pip
# Build micro-ROS tools and source them
ENV AMENT_PREFIX_PATH=/opt/ros/jazzy
ENV PYTHONPATH=/opt/ros/jazzy/lib/python3.12/site-packages
RUN env
RUN colcon build
RUN source install/local_setup.bash

# Install Micro-ros agent