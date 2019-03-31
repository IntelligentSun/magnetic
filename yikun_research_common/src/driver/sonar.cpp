#include "yikun_research_common/driver/sonar.h"

namespace yikun_research_common {

Sonar::Sonar(const std::string host,const std::string port,const std::string uid)
  : host_(host),port_(port),uid_(uid)
{
  private_nh_ = new ros::NodeHandle("~");
}

Sonar::~Sonar()
{
  distance_us_destroy(&dus_);
  ipcon_destroy(&ipcon_);
}

bool Sonar::connect()
{
  ipcon_create(&ipcon_);
  // Create device object
  distance_us_create(&dus_,uid_.c_str(), &ipcon_);
  // Connect to brickd
  if(ipcon_connect(&ipcon_, host_.c_str(), 4223) < 0) {
    ROS_ERROR("Could not connect to %s",uid_.c_str());
    return false;
  }
  return true;
}

void Sonar::disconnect()
{
  distance_us_destroy(&dus_);
  ipcon_destroy(&ipcon_);
}

bool Sonar::getDistance(uint16_t &distance)
{
  if(distance_us_get_distance_value(&dus_, &distance) < 0) {
    ROS_ERROR("Could not get distance value, probably timeout");
    return false;
  }
  return true;
}

}//namespace
