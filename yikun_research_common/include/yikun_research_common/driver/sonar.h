#ifndef SONAR_H
#define SONAR_H

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <netinet/in.h>
#include <tinkerforge/ip_connection.h>
#include <tinkerforge/bricklet_distance_us.h>

namespace yikun_research_common {

class Sonar
{
public:
  typedef boost::shared_ptr<yikun_research_common::Sonar> Ptr;
  Sonar(const std::string host,const std::string port,const std::string uid);
  ~Sonar();

  bool connect();
  void disconnect();
  bool getDistance(uint16_t &distance);

private:
  ros::NodeHandle* private_nh_;
  std::string host_,port_;
  std::string uid_;
  IPConnection ipcon_;
  DistanceUS dus_;
};

}//namespace


#endif // SONAR_H
