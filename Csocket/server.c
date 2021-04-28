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

    struct sockaddr_in bind_info;
    bind_info.sin_family = AF_INET;
    bind_info.sin_port = htons(2333);
    bind_info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //配置协议端口与IP地址

    int bind_return = bind(Socket_Server,(const struct sockaddr *)&bind_info,sizeof(bind_info));
    if(bind_return == SOCKET_ERROR){
        printf("绑定地址与端口失败!");
        closesocket(Socket_Server);  //关闭SOCKET
        WSACleanup();
        return 0;
    }

    int listen_return = listen(Socket_Server,10);
    //开始监听客户端连接
    if(listen_return == SOCKET_ERROR){
        printf("监听失败!");
        closesocket(Socket_Server);
        WSACleanup();
        return 0;
    } else {
        printf("Listening...\n");
    }

    fd_set ClientSockets;
    FD_ZERO(&ClientSockets);
    FD_SET(Socket_Server,&ClientSockets);
    struct timeval secondtime;
    secondtime.tv_sec = 3;
    secondtime.tv_usec = 0;

    while(1){
        fd_set TempSockets = ClientSockets;
        int select_return = select(0,&TempSockets,NULL,NULL,&secondtime);

        if(select_return == 0){
            continue;
        } else if(select_return > 0){

            for(int i = 0; i < TempSockets.fd_count; ++i){

                if(TempSockets.fd_array[i] == Socket_Server){
                    SOCKET Socket_Client = accept(Socket_Server,NULL,NULL);
                    if(Socket_Client == INVALID_SOCKET){
                        continue;
                    }
                    printf("客户端链接成功!\n");
                    FD_SET(Socket_Client,&ClientSockets);
                } else {
                    char msg[1500] = {0};
                    int recv_return = recv(TempSockets.fd_array[i],msg,1024,0);
                    if(recv_return == 0){
                        printf("客户端下线!\n");
                        SOCKET socketTemp = TempSockets.fd_array[i];
                        FD_CLR(TempSockets.fd_array[i],&ClientSockets);
                        closesocket(socketTemp);
                        //关闭下线客户端SOCKET
                    } else if(recv_return > 0){
                        printf("%s\n",msg);
                    } else {
                        //WSAGetLastError();
                        printf("ERROR 客户端强制下线\n");
                        SOCKET socketTemp = TempSockets.fd_array[i];
                        FD_CLR(TempSockets.fd_array[i],&ClientSockets);
                        closesocket(socketTemp);
                    }
                }
            }

        } else {
            printf("发生错误!\n");
        }

    }

    closesocket(Socket_Server);
    //closesocket(Socket_Client);  //关闭SOCKET
    WSACleanup();
    return 0;
}