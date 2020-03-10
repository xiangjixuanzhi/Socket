//
//  SocketClass.hpp
//  SocketClass
//
//  Created by Shaojun on 5/3/2020.
//  Copyright © 2020 Shaojun. All rights reserved.
//

#ifndef SocketClass_hpp
#define SocketClass_hpp

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
using namespace std;

#define DefaultHost 8000
#define buffer_size 1024
#define RET_OK      "ok"

// typedef声明一个结构体，需要先声明才可以使用
typedef struct _SocketAttribute
{
    int socket_client;
    fd_set rfds;
    struct timeval Timeout;//select
    int retval, maxfd;
}mySocketAttribute;


class SocketClass
{
    
private:
    // 声明mySocketAttribute变量
    mySocketAttribute SocketAttribute;
    
    bool bAlreadyOepnSocket;
    
public:
    // 构造函数和析构函数
    SocketClass();
    ~SocketClass();
    
    string OpenSocket(const char *ip, unsigned int host = DefaultHost, float timeout = 5);
    string CloseSocket();
    string SendSocket(const char *command);
    string RecvSocket(int recv_len, char *RecvBuff);
    string SendAndRecv(const char *command,const char *confirm,char *readBuffer,int len,float sleepTime,bool isCheck=true);
    
    int Ascii2Hex(const char *ascii_buf, unsigned char *hex);
    int Hex2Ascii(const char *hex_buf, unsigned char *ascii);
    
};

#endif /* SocketClass_hpp */
