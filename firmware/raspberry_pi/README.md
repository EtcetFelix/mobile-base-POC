# Raspberry Pi (rpi for short) instructions

## Step 1 - Build the micro ros image
1. clone this repo to the raspberry pi (rpi)
2. on the rpi, cd to the directory this readme is in (mobile-base-poc/firmware/raspberry-pi)
3. Build the docker image: `sudo docker build -t micro_ros_agent .`

## Step 2 - How to use the micro_ros image: 
1. Run the docker container with serial ports mounted:  `sudo docker run -it --net=host -v /dev:/dev --privileged micro_ros_agent`
2. Source the ROS 2 installation: `source /opt/ros/foxy/setup.bash`
3. Source the micro_ros installation: `source install/local_setup.bash`
4. Run whatever commands you need to run
