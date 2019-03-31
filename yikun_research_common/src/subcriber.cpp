#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ros/ros.h>
#include <std_msgs/String.h>

#ifndef UDP_PORT
#define UDP_PORT 20000
#endif
#ifndef UDP_ADDR
#define UDP_ADDR "127.0.0.1"
#endif

int main(int argc,char* argv[])
{
  ros::init(argc,argv,"subcriber_java");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::String>("recv",1000);
  constexpr int listen_port = UDP_PORT;
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock == -1)
  {
    std::perror("socket");
    exit(1);
  }
  sockaddr_in client;
  socklen_t len = sizeof(client);
  sockaddr_in addr;

  std::memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(listen_port);
  addr.sin_addr.s_addr = inet_addr(UDP_ADDR);
  int ret = bind(sock, (sockaddr *)&addr, sizeof(addr));
  if (ret == -1)
  {
    std::perror("bind");
    return 0;
  }
  char tmp[255];
  while (nh.ok())
  {
    // get connect
    std::cout << "Waiting udp access..." << std::endl;
    int *client_sock = new int();
    *client_sock = recvfrom(sock, tmp, sizeof(tmp), 0,reinterpret_cast<sockaddr *>(&client), &len);
    if (*client_sock == -1 && errno == EAGAIN)
    {
      std::perror("recv");
      continue;
    }
    //response
    std_msgs::String str;
    str.data = tmp;
//    std::cout<<str.data<<std::endl;
    pub.publish(str);
  }
  return 0;
}
