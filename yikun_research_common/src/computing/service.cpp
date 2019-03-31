#include "yikun_research_common/computing/service.h"
#define F_KP  0.46  //PSD translation controller
#define F_KD  0.06
#define F_KI  0.0
#define R_KP  1.5  //PSD rotation controller
#define R_KD  0.05
#define R_KI  0.0
namespace yikun_research_common {

NavService::NavService()
{
  private_nh_ = new ros::NodeHandle("~");
  private_nh_->param("front_magnetic",front_magnetic_dev_,std::string("/dev/front_magnetic"));
  private_nh_->param("rear_magnetic",rear_magnetic_dev_,std::string("/dev/rear_magnetic"));
  private_nh_->param("baudrate",baudrate_,115200);
  private_nh_->param("timeout",timeout_,1000);
  private_nh_->param("max_linear",max_linear_,0.15);
  private_nh_->param("max_angular",max_angular_,0.15);
  private_nh_->param("vel_topic",vel_topic_,std::string("/ir100_velocity_controller/cmd_vel"));
  private_nh_->param("stop_topic",stop_topic_,std::string("/e_stop"));
}
NavService::~NavService()
{}

bool NavService::initialize()
{
  front_magnetic_ = boost::make_shared<Magnetic>(front_magnetic_dev_,baudrate_,timeout_);
  rear_magnetic_ = boost::make_shared<Magnetic>(rear_magnetic_dev_,baudrate_,timeout_);

  forward_ = private_nh_->advertiseService("/yikun/forward",&NavService::forwardCallback,this);
  backward_ = private_nh_->advertiseService("/yikun/backward",&NavService::backwardCallback,this);
  interrupt_ = private_nh_->advertiseService("/yikun/interrupt",&NavService::interruptCallback,this);

  cmd_vel_ = private_nh_->advertise<geometry_msgs::Twist>(vel_topic_.c_str(),1000);

  if(!front_magnetic_->connect()) {
    ROS_ERROR("Unable to connect to %s",front_magnetic_dev_.data());
    return false;
  }
  if(!rear_magnetic_->connect()) {
    ROS_ERROR("Unable to connect to %s",front_magnetic_dev_.data());
    return false;
  }
  return true;
}

void NavService::limitVelocity(const double linear, const double angular)
{
  max_linear_ = linear;
  max_angular_ = angular;
}

void NavService::interrupt()
{
  status_ = NavStatus::Stop;
}

bool NavService::forwardCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
{
  status_ = NavStatus::Forward;

  PID linear_pid,angular_pid;
  double sum,target;
  int iterations = 0;
  linear_pid.init(F_KP,F_KI,F_KD);
  angular_pid.init(R_KP,R_KI,R_KD);
  Pos start;
  front_magnetic_->prepare();
  while(private_nh_->ok()) {
    if(status_ != NavStatus::Forward){
      ROS_WARN_STREAM("forward interrupted");
      break;
    }

    int left,right;
    double value;
    geometry_msgs::Twist twist;
    front_magnetic_->getInfo(left,right);
    if(left < 0 && right < 0) {
      value = left*0.01;
    } else if(left > 0 && right > 0) {
      value = right*0.01;
    } else {
      value = right*0.01;
    }
    double current_time = ros::Time::now().toSec();
    if(iterations == 0) {
      start.timestamp = current_time;
      start.value = value;
      target = 0;
    }
    iterations++;
    //angular
    twist.angular.z = Min(max_angular_,linear_pid.calculate(current_time,value-start.value,
                                           linear_pid.getValue().value-start.value,target,sum));
    //linear
    twist.linear.x = max_linear_*((max_angular_-fabs(twist.angular.z))/max_angular_)*1.5;
    ROS_INFO("MGT %d,%d(%f); z: %f; x: %f",left,right,value,twist.angular.z,twist.linear.x);
    linear_pid.saveValue(value);
    cmd_vel_.publish(twist);
    ros::Duration(0.1).sleep();
  }
  return true;
}

bool NavService::backwardCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
{
  status_ = NavStatus::Backward;
  int iterations = 0;
  PID linear_pid,angular_pid;
  double sum,target;
  linear_pid.init(F_KP,F_KI,F_KD);
  angular_pid.init(R_KP,R_KI,R_KD);
  Pos start;
  rear_magnetic_->prepare();
  while(private_nh_->ok()) {
    if(status_ != NavStatus::Backward){
      ROS_WARN_STREAM("forward interrupted");
      break;
    }
    int left,right;
    double value;
    geometry_msgs::Twist twist;
    rear_magnetic_->getInfo(left,right);
    if(left < 0 && right < 0) {
      value = left*0.01;
    } else if(left > 0 && right > 0) {
      value = right*0.01;
    } else {
      value = right*0.01;
    }
    double current_time = ros::Time::now().toSec();
    if(iterations == 0) {
      start.timestamp = current_time;
      start.value = value;
      target = 0;
    }
    iterations++;
    //angular
    twist.angular.z = Min(max_angular_,linear_pid.calculate(current_time,value-start.value,
                                                             linear_pid.getValue().value-start.value,target,sum));
    //linear
    twist.linear.x = -max_linear_*((max_angular_-fabs(twist.angular.z))/max_angular_)*1.5;
    ROS_INFO("MGT %d,%d(%f); z: %f; x: %f",left,right,value,twist.angular.z,twist.linear.x);
    linear_pid.saveValue(value);
    cmd_vel_.publish(twist);
    ros::Duration(0.1).sleep();
  }
  return true;
}

bool NavService::interruptCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
{
  status_ = NavStatus::Stop;
  return true;
}

}//namespace
