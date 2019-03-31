#include "yikun_research_common/driver/bms.h"

namespace yikun_research_common {

BMS::BMS(std::string dev)
{
  handle = -1;
  port.assign(dev);
}

BMS::~BMS()
{
  if(handle != -1)
    close(handle);

  handle = -1;
}

int BMS::connect()
{
  if(handle != -1)
    return 0;

  handle = open(port.c_str(), O_RDWR |O_NOCTTY | O_NDELAY);
  if(handle == -1)
  {
    ROS_ERROR("Error opening BMS port");
    return -1;
  }

  fcntl (handle, F_SETFL, O_APPEND | O_NONBLOCK & ~FNDELAY);
  initPort();

  std::cout<<"Opening BMS: '"<<port<<"'..."<<"succeeded."<<std::endl;
  return 0;
}


void BMS::initPort()
{
  if(handle == -1)
    return;

  int BAUDRATE = B57600;
  struct termios newtio;
  tcgetattr (handle, &newtio);

  cfsetospeed (&newtio, (speed_t)BAUDRATE);
  cfsetispeed (&newtio, (speed_t)BAUDRATE);

  newtio.c_iflag = IGNBRK;
  newtio.c_lflag = 0;
  newtio.c_oflag = 0;
  newtio.c_cflag |= (CLOCAL | CREAD);
  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
  newtio.c_cflag &= ~CSIZE;
  newtio.c_cflag |= CS8;
  newtio.c_cflag &= ~PARENB;
  newtio.c_cflag &= ~PARODD;
  newtio.c_cflag &= ~CSTOPB;
  newtio.c_cc[VMIN] = 0;
  newtio.c_cc[VTIME] = 1;

  tcflush (handle, TCIFLUSH);
  tcsetattr (handle, TCSANOW, &newtio);
}

float BMS::getSOC()
{
  if(handle == -1)
    return -1;

  std::string str("\x5A\x03\xA1\x02\xF0\x10");
  int countSent = write(handle, str.c_str(), str.length());
  if(countSent < 0)
  {
    return -1;
  }

  int countRcv;
  char buf[512] = "";
  std::string dataI = "";
  while((countRcv = read(handle, buf, 512)) > 0)
  {
    dataI.append(buf, countRcv);
    if(countRcv < 512)
      break;
  }
  if(countRcv < 0)
  {
    return -1;
  }

  //cout<<"data: "<< dataI <<endl;
  int soc = 0;
  if ((int)dataI[0] == 90)
  {
    //cout<<"SOC: "<< (int)dataI[16] <<endl;
    return (int)dataI[16];
  }
  else
  {
    return -1;
  }
}
}//namespace
