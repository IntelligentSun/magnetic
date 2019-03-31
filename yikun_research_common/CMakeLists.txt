cmake_minimum_required(VERSION 2.8.3)
project(yikun_research_common)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  yikun_research_msgs
  tf
  serial
)
find_package(Boost REQUIRED
  COMPONENTS
  filesystem
  program_options
  signals
  system
  thread
)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES
  CATKIN_DEPENDS roscpp serial
#  DEPENDS system_lib
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${TINKERFORGE_INCLUDE_DIRS}
)
#########mysql##########
#add_library(db_helper
#    src/mysql/db_helper.cpp)
#add_dependencies(db_helper ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} yikun_client_common_gencpp)
#target_link_libraries(db_helper
#    ${MYSQL_LIBRARY_DIRS}
#    ${catkin_LIBRARIES})

############slave manager##########
#add_library(system_helper
#    src/cluster_manager/system_helper.cpp)
#add_dependencies(system_helper ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} yikun_client_common_gencpp)
#target_link_libraries(system_helper
#    ${catkin_LIBRARIES})
############################
#add_executable(robot_info_sync
#  src/cluster_manager/robot_info_sync.cpp
#  src/cluster_manager/robot_info.cpp
#  src/cluster_manager/robot_thread.cpp)
#target_link_libraries(robot_info_sync
#    system_helper
#    db_helper
#    ${catkin_LIBRARIES})
##
#add_executable(robot_pose
#  src/robot_manager/robot_pose.cpp)
#target_link_libraries(robot_pose
#    db_helper
#    ${catkin_LIBRARIES})
add_executable(test
  src/driver/bms.cpp
  src/driver/sonar.cpp
  src/driver/test.cpp
  src/driver/magnetic.cpp)
target_link_libraries(test
    ${catkin_LIBRARIES}
    tinkerforge.so
    pthread)

#
add_executable(service_node
  src/driver/bms.cpp
  src/driver/magnetic.cpp
  src/driver/sonar.cpp
  src/computing/pid.cpp
  src/computing/service.cpp
  src/service_node.cpp)
target_link_libraries(service_node
    ${catkin_LIBRARIES}
    tinkerforge.so
    pthread)
#
add_executable(subcriber_demo
  src/subcriber.cpp)
target_link_libraries(subcriber_demo
    ${catkin_LIBRARIES})
