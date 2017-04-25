#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
//#include <algorithm> 

using namespace std;

typedef int int32_t;
// typedef signed char int8_t;
// typedef unsigned char int8_t;

//#define ETH_NAME    "ens33"  //如果要获取其他网卡的地址，将这个换为其他网卡名称，比如eth0

string ip = "";
string mac_addr = "";

char * ETH_NAME = NULL;
string get_ip();
void get_mac(char * mac_a);

int main(int argc, char* argv[])
{
    //ip
    ETH_NAME = argv[1];
    cout << "ethname: " << ETH_NAME << endl;
    ip = get_ip();
    if(ip != ""){
        //const char * ip_addr = ip.c_str();
         //printf("ip: %s\n", ip.c_str());
        cout << "ip: " << ip << endl;
    }else{
        printf("ip: %s\n", "is NULL");
    }

    // mac address
    char * this_mac = new char[6];
    get_mac(this_mac);
    char * mac = new char[20];
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", this_mac[0]&0xff, this_mac[1]&0xff, this_mac[2]&0xff, this_mac[3]&0xff, this_mac[4]&0xff, this_mac[5]&0xff);
    mac_addr = mac;
    cout << "mac: " << mac_addr << endl;
    //transform(mac_addr.begin(), mac_addr.end(), mac_addr.begin(), ::toupper);
    //cout << "mac: " << mac_addr << endl;

    int sockfd2;
    struct sockaddr_in servaddr;
    sockfd2 = socket(PF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(20015);
    servaddr.sin_addr.s_addr = inet_addr("172.16.1.1");
  
    char *data;
    char *p;
    int nIP[4];
    int len = ip.length();
    data = (char *)malloc((len+1)*sizeof(char));
    ip.copy(data,len,0);

    p=strtok(data,".");
    nIP[0] = atoi(p);
    cout << nIP[0]  << endl;

    p=strtok(NULL,".");
    nIP[1] = atoi(p);
    cout << nIP[1] << endl;

    p=strtok(NULL,".");
    nIP[2] = atoi(p);
    cout << nIP[2] << endl;

    p=strtok(NULL,".");
    nIP[3] = atoi(p);
    cout << nIP[3] << endl;

    int ispKey=0x4e67c6a7;
        int ECX = 0;
        int ESI = 0;
        int EBX = 0;
        int EAX = 0;
        unsigned char localInfo[]={0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                                  0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                                  0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                                  0x02,0x00};
    int nInfo=sizeof(localInfo);
    int nMac=mac_addr.size();
    int ispNum = (int) argv[2][0] - 48;
    localInfo[nInfo-2]=(unsigned char)ispNum;
    cout << "ispNum: " << ispNum << ' ' << (unsigned char)ispNum <<endl;
    localInfo[0]='1';

    for (int i=0;i<4;i++)
    {
        localInfo[i+30]=(unsigned char)nIP[i];
    }

    for (int i=0;i<nMac;i++)
    {
        localInfo[i+34]=(unsigned char)mac_addr[i];
        //cout << (unsigned char)mac_addr[i] << endl;
    }

    /****************Calculating Key************/
    ECX=ispKey;
    for (int i=0;i<nInfo;i++)
    {
         ESI=ECX;
         ESI=(int)(ECX<<5);
        if (ECX>0)
         {
             EBX=ECX;
             EBX=EBX>>2;
         }
         else
         {
             EBX=ECX;
             EBX=EBX>>2;
             EBX=EBX|(0xC0000000);
         }
         ESI=ESI+(int)localInfo[i];
         EBX=(int)(EBX+ESI);
         ECX=ECX^EBX;
    }
    ECX=ECX&0x7FFFFFFF;

    unsigned char sendData[60];
    for(int i=0; i < nInfo; i++){
        sendData[i] = localInfo[i];
    }


    for (int i=0; i < 4; i++)
    {
        unsigned char keypart;
        keypart=(unsigned char)(ECX>>(i*8))&0x000000FF;
        sendData[nInfo + i] = keypart;
    }
    sendto(sockfd2, sendData, sizeof(sendData), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    close(sockfd2);

    for(int i = 0; i < sizeof(sendData); i++){
        printf("%02x ", sendData[i]);
        if((i+1)%16 == 0){
            cout << endl;
        }
    }
    cout << endl;

    return 0;
}

string get_ip()
{
    int                 sockfd;
    struct sockaddr_in  sin;
    struct ifreq        ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
          //Interface name
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) == 0) {    //SIOCGIFADDR 获取interface address
        //cout << "here" << endl;
        memcpy(&sin, &ifr.ifr_addr, sizeof(ifr.ifr_addr));
        //cout << "here1" << endl;
        close(sockfd);
        return inet_ntoa(sin.sin_addr);
    }
    //cout << "here2" << endl;
    close(sockfd);
    return "";
}

void get_mac(char * mac_a)
{
    int                 sockfd;
    struct ifreq        ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        //close(sockfd);
        perror("socket error");
        exit(1);
    }
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);      //Interface name

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) {  //SIOCGIFHWADDR 获取hardware address
        memcpy(mac_a, ifr.ifr_hwaddr.sa_data, 6);
        //close(sockfd);
    }
}
