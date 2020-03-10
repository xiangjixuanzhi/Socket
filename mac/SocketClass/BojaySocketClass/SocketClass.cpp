//
//  SocketClass.cpp
//  SocketClass
//
//  Created by Shaojun on 5/3/2020.
//  Copyright © 2020 Shaojun. All rights reserved.
//

#include "SocketClass.hpp"


SocketClass::~SocketClass()
{
    
}


SocketClass::SocketClass()
{
    bAlreadyOepnSocket = false;
}


// 1.创建并打开套接字
string SocketClass::OpenSocket(const char *ip, unsigned int host, float timeout)
{
    // 1.定义sockfd
    SocketAttribute.socket_client = socket(AF_INET,SOCK_STREAM, 0);
    
    // 2.定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    
    // 3.服务器端口和IP
    servaddr.sin_port = htons(host);
    servaddr.sin_addr.s_addr = inet_addr(ip);
    
    // 4.连接服务器，成功返回0，错误返回-1
    if (connect(SocketAttribute.socket_client, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("[OpenSocket]connect fail!");
        return "[OpenSocket]connect fail!";
    }
    
    // 5.设置socket读取超时时间
    string strTime = to_string(float(timeout));
    string sec = strTime.substr(0,strTime.find("."));
    string usec = strTime.substr(strTime.find(".") + 1, (strTime.length()) - strTime.find("."));
    long second = atol(sec.c_str());
    int usecond = atoi(usec.c_str());
    struct timeval recv_timeout = {second,usecond};
    setsockopt(SocketAttribute.socket_client, SOL_SOCKET, SO_RCVTIMEO, (char *)&recv_timeout,sizeof(struct timeval));
    bAlreadyOepnSocket = true;
    return RET_OK;
}


// 2.关闭套接字
string SocketClass::CloseSocket()
{
    if (bAlreadyOepnSocket == true)
    {
        close(SocketAttribute.socket_client);
    }
    return RET_OK;
}


// 3.发送给套接字服务端
string SocketClass::SendSocket(const char *command)
{
    if (bAlreadyOepnSocket != true || command == NULL)
    {
        return "[SendSocket]don't open socket fail!";
    }
    
    size_t len = send(SocketAttribute.socket_client, command, strlen(command),0);
    if (len == strlen(command))
    {
        return RET_OK;
    }
    else
    {
        return "[SendSocket]send fail!";
    }
    
}


// 4.从服务端接收数据
string SocketClass::RecvSocket(int recv_len, char *RecvBuff)
{
    if (bAlreadyOepnSocket != true)
    {
        return "[RecvSocket]don't open socket fail!";
    }
    
    try {
        memset(RecvBuff, 0, recv_len + 1);
        size_t len = recv(SocketAttribute.socket_client, RecvBuff, recv_len, 0);
        if (len == recv_len)
        {
            return RET_OK;
        }
        else
        {
            return "[RecvSocket]recv fail!";
        }
    } catch (...) {
        return "[RecvSocket]exception fail!";
    }

}


// 5.发送并接收
string SocketClass::SendAndRecv(const char *command,const char *confirm,char *RecvBuff,int len,float sleepTime,bool isCheck)
{
    if (bAlreadyOepnSocket != true || command == NULL)
    {
        return "[SendAndRecv]don't open socket fail!";
    }
    
    // 1.send
    size_t send_len = send(SocketAttribute.socket_client, command, strlen(command),0);
    if (send_len != strlen(command))
    {
        return "[SendAndRecv]send fail!";
    }

    // 2.delay
    usleep(sleepTime * 1000 * 1000);
    
    // 3.recv
    memset(RecvBuff, 0, len + 1);
    size_t recv_len = recv(SocketAttribute.socket_client, RecvBuff, len, 0);
    if (recv_len != len)
    {
        return "[SendAndRecv]recv fail!";
    }
    
    // 4.check confirm
    if (isCheck == true)
    {
        if (strstr(RecvBuff, confirm) != NULL)
        {
            return RET_OK;
        }
        else
        {
            return "[SendAndRecv]confirm fail!";
        }
    }
    
    return RET_OK;
}


//hex -> string
int SocketClass::Hex2Ascii(const char *hex_buf, unsigned char *ascii)
{
    if (strlen(hex_buf) % 2 != 0)
    {
        return -1;
    }
    
    unsigned char highByte, lowByte;
    size_t sourceLen = strlen(hex_buf);
    
    for (int i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(hex_buf[i]);
        lowByte  = toupper(hex_buf[i + 1]);
        
        if (highByte > 0x39)    //highByte > '9'
            highByte -= 0x37;   //highByte = highByte - 'A' + 10
        else
            highByte -= 0x30;   //highByte = highByte - '0'
        
        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;
        
        ascii[i / 2] = (highByte << 4) | lowByte;
    }
    return 0;
}

//string -> hex
int SocketClass::Ascii2Hex(const char *ascii_buf, unsigned char *hex)
{
    unsigned char highByte, lowByte;
    size_t sourceLen = strlen(ascii_buf);
    
    for (int i = 0; i < sourceLen; i++)
    {
        highByte = ascii_buf[i] >> 4;
        lowByte = ascii_buf[i] & 0x0f ;
        
        highByte += 0x30;
        
        if (highByte > 0x39)
            hex[i * 2] = highByte + 0x07;
        else
            hex[i * 2] = highByte;
        
        lowByte += 0x30;
        if (lowByte > 0x39)
            hex[i * 2 + 1] = lowByte + 0x07;
        else
            hex[i * 2 + 1] = lowByte;
    }
    return 0;
}
