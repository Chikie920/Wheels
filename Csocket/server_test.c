#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
//引用头文件与网络库

typedef struct event_socket_set {
    SOCKET socket_all[64];
    WSAEVENT event_all[64];
    unsigned int count;
} event_socket_set;

fd_set AllSockets;
//创建存放SOCKET与事件对象的容器，因为我们需要对每个SOCKET绑定相应的事件
event_socket_set es_set = {{0}, {0}, 0};


BOOL WINAPI Console_Shutdown(DWORD dwCtrlType)
{
    switch(dwCtrlType){
        case CTRL_CLOSE_EVENT: 
            for(int n = 0; n < es_set.count; ++n){
            closesocket(es_set.socket_all[n]);
            WSACloseEvent(es_set.event_all[n]);
        }
        WSACleanup();
        break;
    }

    return TRUE;
}

int main(void)
{

    SetConsoleCtrlHandler(Console_Shutdown, TRUE);

    WORD WSVersion = (2, 2);
	WSADATA wsadata;
    int wsas_return = WSAStartup(WSVersion, &wsadata);
    if(wsas_return != 0){
        printf("初始化网络库失败! 请检查网络库，或者重启电脑\n");
    }
    //初始化网络库
    
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_socket == INVALID_SOCKET){
        printf("创建socket失败! 错误代码:");
        int error = WSAGetLastError();
        printf("%d\n",error);
        WSACleanup();
        return 1;
    }
    //创建本地socket
    
    struct sockaddr_in bind_info;
    bind_info.sin_family = AF_INET;
    bind_info.sin_port = htons(2333);
    bind_info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    int bind_return = bind(server_socket, (const struct sockaddr *)&bind_info, sizeof(bind_info));
    if(bind_return != 0){
        printf("BIND ERROR! 错误代码: %d\n",WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    //绑定本地ip与端口
    
    int listen_return = listen(server_socket, SOMAXCONN);
    if(listen_return != 0){
        printf("LISETEN ERROR! 错误代码: %d\n",WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Listening...\n");
    //监听客户端链接


    //创建事件
    WSAEVENT EventServer = WSACreateEvent();
    //返回值处理
    if(EventServer == WSA_INVALID_EVENT){
        printf("CREATE EVENT ERROR! 错误代码: %d\n",WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }

    //事件绑定与投递
    int EventSelect_return = WSAEventSelect(server_socket, EventServer, FD_ACCEPT | FD_WRITE | FD_READ);
    if(EventSelect_return == SOCKET_ERROR){
        printf("ERROR! 错误代码: %d\n",WSAGetLastError());
        WSACloseEvent(EventServer);
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }

    es_set.socket_all[es_set.count] = server_socket;
    es_set.event_all[es_set.count] = EventServer;
    es_set.count++;
    //添加到容器中

    while(1){
        DWORD Wait_return = WSAWaitForMultipleEvents(es_set.count, es_set.event_all, FALSE, 200, FALSE);
        //函数错误
        if(Wait_return == WSA_WAIT_FAILED){
            printf("WAIT ERROR! 错误代码: %d\n",WSAGetLastError());
            break;
        }
        //超时
        if(Wait_return == WSA_WAIT_TIMEOUT){
            continue;
        }
        DWORD event_index = Wait_return - WSA_WAIT_EVENT_0;

        //得到下标对应事件
        WSANETWORKEVENTS NetworkEvents;
        int n_t_return = WSAEnumNetworkEvents(es_set.socket_all[event_index], es_set.event_all[event_index], &NetworkEvents);
        if(n_t_return == SOCKET_ERROR){
            printf("GETINDEX ERROR! 错误代码: %d\n",WSAGetLastError());
            break;
        }

        if(NetworkEvents.lNetworkEvents & FD_ACCEPT){
            if(NetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0){ //等于0时为无错误码情况，正常处理
                SOCKET ClientSocket = accept(es_set.socket_all[event_index], NULL, NULL);
                if(ClientSocket == INVALID_SOCKET){
                    continue;
                }
                //给客户端SOCKET绑定事件
                WSAEVENT ClientEvent = WSACreateEvent();
                if(ClientEvent == WSA_INVALID_EVENT){
                    closesocket(ClientSocket);
                    continue;
                }
                //投递给系统监视
                if( WSAEventSelect(ClientSocket, ClientEvent, FD_READ | FD_CLOSE | FD_WRITE) == SOCKET_ERROR){
                    closesocket(ClientSocket);
                    WSACloseEvent(ClientEvent);
                    continue;
                }

                //将链接的客户端SOCKET装入数组集合进行后续监视
                es_set.socket_all[es_set.count] = ClientSocket;
                es_set.event_all[es_set.count] = ClientEvent;
                es_set.count++;
                printf("Client Connect Success\n");
            } else {
                continue;
            }
        }
        
        //根据客户端信号进行分类处理
        if(NetworkEvents.lNetworkEvents & FD_WRITE){
            if(NetworkEvents.iErrorCode[FD_WRITE_BIT] == 0){
                if(send(es_set.socket_all[event_index], "CONNECT SUCCESS", strlen("CONNECT SUCCESS"), 0) == SOCKET_ERROR){
                    printf("SEND ERROR! 错误代码: %d\n",WSAGetLastError());
                }
            } else {
                printf("CLIENT SOCKET ERROR! 错误代码: %d\n",NetworkEvents.iErrorCode[FD_WRITE_BIT]);
            }
            printf("Write Event\n");
        }

        if(NetworkEvents.lNetworkEvents & FD_READ){
            if(NetworkEvents.iErrorCode[FD_READ_BIT] == 0){
                char rec_msg[1500] = {0};
                if(recv(es_set.socket_all[event_index], rec_msg, 1499, 0) == SOCKET_ERROR){
                    printf("RECV ERROR! 错误代码: %d\n",WSAGetLastError());
                    continue;
                }
                printf("%s\n", rec_msg);
            } else {
                printf("CLIENT SOCKET ERROR! 错误代码: %d\n",NetworkEvents.iErrorCode[FD_READ_BIT]);
                continue;
            }
        }

        if(NetworkEvents.lNetworkEvents & FD_CLOSE){
            printf("Client ShutDown\n");
            //清理下线客户端SOCKET与事件
            closesocket(es_set.socket_all[event_index]);
            es_set.socket_all[event_index] = es_set.socket_all[es_set.count-1];
            //将已关闭SOCKET的位置用最后一个有效SOCKET替补，并将有效个数减一，避免移动后面所有SOCKET的情况，下同
            WSACloseEvent(es_set.event_all[event_index]);
            es_set.event_all[event_index] = es_set.event_all[es_set.count-1];
            es_set.count--;
        }

    }

	for(int n = 0; n < es_set.count; ++n){
		closesocket(es_set.socket_all[n]);
		WSACloseEvent(es_set.event_all[n]);
	}
    WSACleanup();
	//清理SOCKET、事件与网络库
    
    return 0;
}