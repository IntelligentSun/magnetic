#ifndef BMS_H
#define BMS_H

#include <ros/ros.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>
#include <sstream>
#include <unistd.h>

namespace yikun_research_common {

class BMS {
public:
  typedef boost::shared_ptr<yikun_research_common::BMS> Ptr;
  BMS(const std::string dev);
  ~BMS();

  int connect();
  float getSOC();

  private:
    int handle;
    std::string port;

  protected:
    void initPort();
};

}//namespace

#endif // BMS_H
