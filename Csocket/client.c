#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
//引用头文件与网络库

int main(void)
{
    WORD WSversion = MAKEWORD(2,2);  //版本号
    WSADATA ServerSocket;
    int return_WASA = WSAStartup(WSversion,&ServerSocket);
    if(return_WASA!=0){
        printf("初始化网络库失败!");
        WSACleanup();  //清理网络库
        return 0;
    }
    //启动网络库

    SOCKET Socket_Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //创建服务端SOCKET Socket_Server
    if(Socket_Server == INVALID_SOCKET){
        //INVALID_SOCKET为socket函数返回失败值
        printf("创建SOCKET失败!");
        WSACleanup();
        return 0;
    }
    //创建SOCKDET


    struct sockaddr_in connect_info;
    connect_info.sin_family = AF_INET;
    connect_info.sin_port = htons(2333);
    connect_info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //配置协议端口与IP地址
    int connect_return = connect(Socket_Server,(const struct sockaddr *)&connect_info,sizeof(connect_info));
    if(connect_return == SOCKET_ERROR){
        printf("与服务器连接失败!\n");
        return 0;
    }  else {
            printf("与服务器连接成功!\n");
            char buf[1500] = {0};
            while(scanf("%s",buf) != 0){
                send(Socket_Server,buf,sizeof(buf),0);
            }
    }
    closesocket(Socket_Server);  //关闭SOCKET
    WSACleanup();
    return 0;
}