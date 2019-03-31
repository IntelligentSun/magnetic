#ifndef PID_H
#define PID_H

#include <ros/ros.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

namespace yikun_research_common {

typedef struct pos {
//  pos(const double& timestamp,const double& value)
//  {
//    this->timestamp = timestamp;
//    this->value = value;
//  }
  double timestamp;
  double value;
}Pos;

template<typename T>
static T Min(const T a,const T b)
{
  if(b>0)
    return fabs(a)>fabs(b)?b:a;
  else
    return fabs(a)>fabs(b)?b:-a;
}

class PID
{
public:
  PID();
  void init(double kp, double ki, double kd);
  double calculate(double current_time, double actualValue, double lastValue,
                   double reference,double& sum);

  void saveValue(const double value);//last
  Pos getValue();//last

private:
  double Kp_;
  double Ki_;
  double Kd_;
  Pos lastpos_;
};

}//namespace


#endif // PID_H
