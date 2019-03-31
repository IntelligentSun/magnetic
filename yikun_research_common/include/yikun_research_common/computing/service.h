#ifndef SERVICE_H
#define SERVICE_H

#include "yikun_research_common/driver/bms.h"
#include "yikun_research_common/driver/magnetic.h"
#include "yikun_research_common/computing/pid.h"
#include <std_srvs/Empty.h>
#include <geometry_msgs/Twist.h>

namespace yikun_research_common {

enum NavStatus {
  Forward   = 1,
  Backward  = 2,
  Stop      = 3
};

#ifndef Status
#define Status int
#endif

class NavService
{
public:
  NavService();
  ~NavService();

  bool initialize();
  void limitVelocity(const double linear,const double angular);
  void interrupt();

private:
  bool forwardCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res);
  bool backwardCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res);
  bool interruptCallback(std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res);

  ros::NodeHandle *private_nh_;
  ros::Publisher cmd_vel_;
  ros::ServiceServer forward_,backward_,interrupt_;
  Status status_;
  std::string front_magnetic_dev_,rear_magnetic_dev_;
  std::string vel_topic_,stop_topic_;
  Magnetic::Ptr front_magnetic_,rear_magnetic_;
  int baudrate_,timeout_;
  double max_linear_,max_angular_;

};

}//namespace

#endif // SERVICE_H
