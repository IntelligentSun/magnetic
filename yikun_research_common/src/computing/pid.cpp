#include "yikun_research_common/computing/pid.h"

namespace yikun_research_common {
PID::PID()
{}

void PID::init(double kp, double ki, double kd)
{
  Kp_ = kp;
  Ki_ = ki;
  Kd_ = kd;
  lastpos_.timestamp = ros::Time::now().toSec();
  lastpos_.value = 0;
}

double PID::calculate(double current_time, double actualValue, double lastValue,
                 double reference,double& sum)
{
  double speed = 0;
  double previousError = reference - lastValue;
  double dt = current_time - lastpos_.timestamp;
  //kP
  double error = reference - actualValue;
  //kD
  double derivative = (error - previousError)/dt;
  //kS
  sum = sum + error*dt;
  //PSD
  speed = Kp_*error + Kd_*derivative + Ki_*sum;

  return speed;
}

void PID::saveValue(const double value)
{
  lastpos_.timestamp = ros::Time::now().toSec();
  lastpos_.value = value;
}
Pos PID::getValue()
{
  return lastpos_;
}

}//namespace


