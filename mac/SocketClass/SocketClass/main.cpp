//
//  main.cpp
//  SocketClass
//
//  Created by Shaojun on 5/3/2020.
//  Copyright © 2020 Shaojun. All rights reserved.
//

#include "SocketClass.hpp"

int main(int argc, const char * argv[]) {
    SocketClass *pInstance = new SocketClass();
    string err = pInstance->OpenSocket("127.0.0.1", 7000, 0.2);
    cout << err << endl;
    for(int i = 0; i < 10; i++)
    {
        string sed = "hello";
        char recv[1024] = {0};
        // write and read
        char confirm[1024] = {"hello"};
        printf("发送给服务端的数据是：%s\n", sed.c_str());
        string wr = pInstance->SendAndRecv(sed.c_str(), confirm, recv, (int)sed.length(), 0.05);
        cout << "从服务端的返回值是：" << wr << endl;
        printf("从服务端读取的数据是: %s\n", recv);
        sleep(1);
    }
    pInstance->CloseSocket();

    return 0;
}
