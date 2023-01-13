//vs code ,MinGW-w64 g++
//windows 10
//class:2019211305 Name:RenChenxu ID:2019211279

#include<iostream>
#include<WinSock2.h>
using namespace std;
#define CLEARBUFF memset(sendBuffer,0,8192);memset(recvBuffer,0,8192);

#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));


    char recvBuffer[8192] = {0};
    char sendBuffer[8192]= {0};
  /************************************************************************************/  
    while(1){
        recv(sock, recvBuffer, 8192,0);
        //cout<<"recvBuffer size:"<<strlen(recvBuffer)<<endl;
        cout<<recvBuffer<<endl;
        if(recvBuffer[0]=='L'&&recvBuffer[1]=='o'&&recvBuffer[2]=='g'
            &&recvBuffer[4]=='o'&&recvBuffer[5]=='u'&&recvBuffer[6]=='t'){
            break;
        }
        CLEARBUFF
        string str;
        getline(cin,str);
        str.copy(sendBuffer,str.length(),0);
        send(sock, sendBuffer,strlen(sendBuffer)+sizeof(char),0);    
    }
  /************************************************************************************/  

    system("pause");

    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    return 0;
}