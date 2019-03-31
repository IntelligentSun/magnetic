cmake_minimum_required(VERSION 2.8.3)
project(yikun_research_msgs)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib
  message_generation
  roscpp
  rospy
  nav_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Application.msg
  AppStatus.msg
  AppUpdate.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  #apps
  Ismapping.srv
  Localization.srv
  SaveMap.srv
  ListTasks.srv
  StartTask.srv
  StatusUpdate.srv
  StopTask.srv
  SetPose.srv
  SetState.srv
  GetState.srv
)

## Generate actions in the 'action' folder
add_action_files(
  FILES
  #movebase
  MoveBase.action
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  nav_msgs
  actionlib_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES yikun_msgs
#  CATKIN_DEPENDS message_generation roscpp rospy
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
