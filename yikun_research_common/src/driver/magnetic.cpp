#include "yikun_research_common/driver/magnetic.h"

namespace yikun_research_common {

Magnetic::Magnetic(const std::string port,const unsigned int baudrate,const unsigned int timeout)
  : port_(port),baudrate_(baudrate),timeout_(timeout)
{}

Magnetic::~Magnetic()
{}

bool Magnetic::connect()
{
  try
  {
    serial_.setPort(port_);
    serial_.setBaudrate(baudrate_);//115200
    serial::Timeout timeout = serial::Timeout::simpleTimeout(timeout_);//1000
    serial_.setTimeout(timeout);
    serial_.open();
  }
  catch (serial::IOException& e)
  {
    ROS_ERROR("Unable to open magnetic port. %s",e.what());
    return false;
  }
  //check again
  if (serial_.isOpen()) {
    ROS_INFO("Magnetic serial port '%s' initialized.",port_.data());
    return true;
  } else {
    return false;
  }
}

bool Magnetic::prepare()
{
//  serial_.write("# C\r\n");
//  serial_.write("?MGT\r\n");
//  serial_.write("# 100\r\n");
  serial_.write("!R\r\n");
  serial_.write("# C\r\n");
  serial_.write("?MGT\r\n");
  serial_.write("# 100\r\n");
  return true;
}

bool Magnetic::getInfo(int &left, int &right)
{
//  serial_.write("?MGT\r\n");//send cmd
  if(serial_.available()) {
    std::string info = serial_.readline(serial_.available());
//    std::cout<<"recv: "<<info.data()<<std::endl;
    if(info.find('=') == std::string::npos)
      return false;
    std::vector<std::string> data;
    boost::split(data,info,boost::is_any_of("="));
    if(data[1].find(':') == std::string::npos)
      return false;
    std::vector<std::string> distance;
    boost::split(distance,data[1],boost::is_any_of(":"));
    left = atoi(distance[0].c_str());
    right = atoi(distance[1].c_str());
    return true;
  } else {
    return false;
  }
}

}//namespace
