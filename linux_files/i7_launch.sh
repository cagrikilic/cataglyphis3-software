#!/bin/bash
source /opt/ros/indigo/setup.bash
source ~/cataglyphis_ws/devel/setup.bash

gnome-terminal --tab -e "bash cataglyphis_ws/src/linux_files/script_launch/roscore.sh" --tab -e "bash -c 'sleep 5 && bash cataglyphis_ws/src/linux_files/script_launch/i7_nb1_udp_receiver.sh'" --tab -e "bash -c 'sleep 6 && bash cataglyphis_ws/src/linux_files/script_launch/i7_nb2_udp_receiver.sh'" --tab -e "bash -c 'sleep 7 && bash cataglyphis_ws/src/linux_files/script_launch/i7_nb3_udp_receiver.sh'" --tab -e "bash -c 'sleep 8 && bash cataglyphis_ws/src/linux_files/script_launch/i7_nb1_udp_sender.sh'" --tab -e "bash -c 'sleep 10 && bash cataglyphis_ws/src/linux_files/script_launch/left_roboteq.sh'" --tab -e "bash -c 'sleep 12 && bash cataglyphis_ws/src/linux_files/script_launch/right_roboteq.sh'" --tab -e "bash -c 'sleep 14 && bash cataglyphis_ws/src/linux_files/script_launch/grabber_roboteq.sh'" --tab -e "bash -c 'sleep 16 && bash cataglyphis_ws/src/linux_files/script_launch/velodyne_launch.sh'"

gnome-terminal -e --tab -e "bash -c 'sleep 18 && bash cataglyphis_ws/src/linux_files/script_launch/nav_filter_node.sh'" --tab -e "bash -c 'sleep 20 && bash cataglyphis_ws/src/linux_files/script_launch/safe_pathing_node.sh'" --tab -e "bash -c 'sleep 22 && bash cataglyphis_ws/src/linux_files/script_launch/lidar_filtering_node.sh'" --tab -e "bash -c 'sleep 24 && bash cataglyphis_ws/src/linux_files/script_launch/collision_detection_node.sh'" --tab -e "bash -c 'sleep 26 && bash cataglyphis_ws/src/linux_files/script_launch/exec_node.sh'" --tab -e "bash -c 'sleep 28 && bash cataglyphis_ws/src/linux_files/script_launch/map_manager_node.sh'" --tab -e "bash -c 'sleep 30 && bash cataglyphis_ws/src/linux_files/script_launch/mission_planning_node.sh'"
