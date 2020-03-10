//
//  SocketThread.h
//  SocketServer
//
//  Created by Shaojun on 11/3/2020.
//  Copyright © 2020 Shaojun. All rights reserved.
//

#ifndef SocketThread_h
#define SocketThread_h
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;
class SocketThread{
private:
    int sock;
public:
    SocketThread(int sock){
        this->sock=sock;
    }
    ~SocketThread(){
        cout<<"release"<<endl;
    }
    void run(){
        int n=0;
        cout<<"new thread for socket "<<this->sock<<endl;
        char buff[1024];
        for(;; ) {
            n = recv(this->sock,buff,1024,0);
            if(n<=0) {
                //如果客户端断开了，这里就跳出循环
                break;
            }
            int send_len = send(this->sock, buff, strlen(buff), 0);
            if (send_len != strlen(buff))
            {
                printf("[thread]send error!\n");
                break;
            }
            buff[n] = '\0';
            printf("%d=>%s\n",n,buff);
        }
        close(this->sock);
        cout<<this->sock<<" closed"<<endl;
    }
};
#endif /* SocketThread_h */
