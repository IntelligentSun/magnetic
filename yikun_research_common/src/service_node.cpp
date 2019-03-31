#include "yikun_research_common/computing/service.h"
#include <std_msgs/Int64.h>
#include <std_msgs/Bool.h>
#include <ros/timer.h>
#include <boost/thread.hpp>
#include "yikun_research_common/driver/sonar.h"

#define DISTANCE 400

using namespace yikun_research_common;

void battery_timer(const ros::TimerEvent&);
void sonar_timer(const ros::TimerEvent&);

ros::Publisher  pub_;
ros::Publisher  stop_;
BMS::Ptr        bms_;
NavService      *service_;
boost::recursive_mutex  mutex_,mutex_sonar_;
std::vector<Sonar::Ptr> sonars_;

int main(int argc,char* argv[])
{
  ros::init(argc,argv,"service_node");
  ros::NodeHandle nh;
  ros::NodeHandle private_nh("~");
  ros::MultiThreadedSpinner spinner(5);
  std::string bms_dev;
  std::string sonar1,sonar2,sonar3,sonar4;
  private_nh.param("bms",bms_dev,std::string("/dev/ttyUSB0"));
  private_nh.param("sonar1",sonar1,std::string("DZr"));
  private_nh.param("sonar2",sonar2,std::string("Eg8"));
  private_nh.param("sonar3",sonar3,std::string("Efq"));
  std::vector<std::string> sonars_dev;
  sonars_dev.push_back(sonar1);
  sonars_dev.push_back(sonar2);
  sonars_dev.push_back(sonar3);
//  Sonar s1("192.168.99.30","4223","DZr");
//  Sonar s2("192.168.99.30","4223","Eg8");
//  Sonar s3("192.168.99.30","4223","Efq");
  for(std::string dev : sonars_dev)
  {
    Sonar::Ptr sonar = boost::make_shared<Sonar>("192.168.99.30","4223",dev);
    if(!sonar->connect()) {
      ROS_ERROR("Unable to connect to sonar");
      exit(1);
    }
    sonars_.push_back(sonar);
  }
  pub_ = nh.advertise<std_msgs::Int64>("/yikun/battery",1000);
  stop_ = nh.advertise<std_msgs::Bool>("/e_stop",1);
  ros::Timer battery_superviser = nh.createTimer(ros::Duration(0.2),battery_timer);
  ros::Timer sonar_superviser = nh.createTimer(ros::Duration(0.2),sonar_timer);
  service_ = new NavService();
  service_->initialize();
  bms_ = boost::make_shared<BMS>(bms_dev);
  if(bms_->connect() == -1) {
    ROS_ERROR("Unable to connect to bms");
    exit(1);
  }
  spinner.spin();
  return 0;
}

void battery_timer(const ros::TimerEvent &event)
{
  boost::unique_lock<boost::recursive_mutex> lock(mutex_);
  std_msgs::Int64 battery_capacity;
  float soc = bms_->getSOC();
  if (soc > 0)
  {
    soc = (soc > 100) ? 100 : soc;
    soc = (soc < 0) ? 0 : soc;
    battery_capacity.data = soc;
    //  std::cout<<"battery: "<<battery_capacity<<std::endl;
    if(battery_capacity.data < 15) {//charge prepare
      service_->limitVelocity(0.15,0.15);
    }
    pub_.publish(battery_capacity);
    lock.unlock();
  }

}
void sonar_timer(const ros::TimerEvent &event)
{
  boost::unique_lock<boost::recursive_mutex> threadlock(mutex_sonar_);
  std::vector<uint16_t> distances;
  for(Sonar::Ptr sonar : sonars_)
  {
    uint16_t distance;
    sonar->getDistance(distance);
    distances.push_back(distance);
  }
  int it = 0;
  for(auto d : distances)
  {
    if(d < DISTANCE){
      it++;
    }
  }
  std_msgs::Bool lock;
  if(it == 0) {
    //unlock
    lock.data = false;
  } else {
    //lock
    lock.data = true;
  }
  stop_.publish(lock);
  ros::Duration(0.1).sleep();
  threadlock.unlock();
}
