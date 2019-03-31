#include "yikun_research_common/driver/bms.h"
#include "yikun_research_common/driver/magnetic.h"
#include "yikun_research_common/driver/sonar.h"

using namespace yikun_research_common;
int main(int argc,char* argv[])
{
  ros::init(argc,argv,"test");
  ros::NodeHandle nh;

  Sonar s1("192.168.99.30","4223","DZr");
  Sonar s2("192.168.99.30","4223","Eg8");
  Sonar s3("192.168.99.30","4223","Efq");
  s1.connect();
  s2.connect();
  s3.connect();
  uint16_t distance;
  while(nh.ok()) {
    s1.getDistance(distance);
    std::cout<<distance<<std::endl;
    s2.getDistance(distance);
    std::cout<<distance<<std::endl;
    s3.getDistance(distance);
    std::cout<<distance<<std::endl;
    ros::Duration(1.0).sleep();
  }
  /*
  std::string port = "/dev/ttyUSB2";
  BMS::Ptr bms = boost::make_shared<BMS>(port);
  if(bms->connect() == -1) {
    exit(1);
  }

  while(nh.ok()) {
    int battery_capacity;
    float soc = bms->getSOC();
    if (soc > 0)
    {
      soc = (soc > 100) ? 100 : soc;
      soc = (soc < 0) ? 0 : soc;
      battery_capacity = soc;
    }
    std::cout<<"battery: "<<battery_capacity<<std::endl;
    ros::Duration(1.0).sleep();
  }
  */
  /*
  std::string front_port = "/dev/front_magnetic";
  std::string rear_port = "/dev/rear_magnetic";
  Magnetic::Ptr front_magnetic = boost::make_shared<Magnetic>(front_port,115200,1000);
  Magnetic::Ptr rear_magnetic = boost::make_shared<Magnetic>(rear_port,115200,1000);

  if(front_magnetic->connect() && rear_magnetic->connect()) {
    front_magnetic->prepare();
    rear_magnetic->prepare();
    while(ros::ok()) {
      int left,right;
      if(front_magnetic->getInfo(left,right)){
        ROS_INFO("front %d,%d",left,right);
      }
      if(rear_magnetic->getInfo(left,right)){
        ROS_INFO("rear %d,%d",left,right);
      }
      ros::Duration(0.5).sleep();
    }
  }

  if(rear_magnetic->connect()) {
    rear_magnetic->prepare();
    while(ros::ok()) {
      int left,right;
      if(rear_magnetic->getInfo(left,right)){
        ROS_INFO("rear %d,%d",left,right);
        ros::Duration(0.5).sleep();
      }
      sleep(1);
//      std::cout<<"----"<<std::endl;
    }
  }
  */
  return 0;
}
