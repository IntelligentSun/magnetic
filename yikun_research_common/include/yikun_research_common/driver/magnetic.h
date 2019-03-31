#ifndef MAGNETIC_H
#define MAGNETIC_H

#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace yikun_research_common {

class Magnetic
{
public:
  typedef boost::shared_ptr<yikun_research_common::Magnetic> Ptr;
  Magnetic(const std::string port,const unsigned int baudrate,const unsigned int timeout);
  ~Magnetic();

  bool connect();
  bool prepare();
  bool getInfo(int& left,int& right);
private:
  std::string port_;
  serial::Serial serial_;
  unsigned int baudrate_;
  unsigned int timeout_;
};

}//namespace

#endif // MAGNETIC_H
