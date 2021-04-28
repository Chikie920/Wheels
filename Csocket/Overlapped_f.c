#include <stdio.h>
#include <winsock2.h>
#include <mswsock.h>

#define MAXSIZE_SOCKET 1024
#define MAXSIZE_BUF 1024

SOCKET allsocket[MAXSIZE_SOCKET]; //创建SOCKET数组
OVERLAPPED alloverlapp[MAXSIZE_SOCKET]; //创建重叠结构体数组
int count;
char str[MAXSIZE_BUF] = {0}; // 接收缓冲区
HANDLE hport;

DWORD WINAPI ThreadProc(LPVOID lpParameter);
int PostAccept(void);
int PostRecv(int);
int PostSend(int);
void Clear(void); //清理函数

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

    SOCKET Socket_Server = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
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

    allsocket[count] = Socket_Server;
    alloverlapp[count].hEvent = WSACreateEvent(); //给重叠结构的事件成员赋值一个合法的事件
    count++;

    hport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0); //创建完成端口
    if(hport == 0){
        int error = GetLastError();
        printf("ERROR CODE: %d\n", error);
        closesocket(Socket_Server);
        //WSACloseEvent(alloverlapp[0].hEvent);
        WSACleanup();
        return 0;
    }

    HANDLE s_hport = CreateIoCompletionPort((HANDLE)Socket_Server, hport, 0, 0); //绑定socket
    if(s_hport != hport){
        int error = GetLastError();
        printf("ERROR CODE: %d\n", error);
        closesocket(Socket_Server);
        CloseHandle(hport);
        WSACleanup();
        return 0;
    }

    int listen_return = listen(Socket_Server,10);
    //开始监听客户端连接
    if(listen_return == SOCKET_ERROR){
        printf("监听失败!");
        closesocket(Socket_Server);
        CloseHandle(hport);
        WSACleanup();
        return 0;
    } else {
        printf("Listening...\n");
    }

    if (0 != PostAccept())
	{
		Clear();
		//清理网络库
		WSACleanup();
		return 0;
	}

    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    int nProcessorsCount = SystemInfo.dwNumberOfProcessors; //获取CPU核数

    HANDLE *pthread = (HANDLE *) malloc(sizeof(HANDLE)*nProcessorsCount); //创建线程数组

    for(int n = 0; n < nProcessorsCount; ++n){
        pthread[n] = CreateThread(NULL, 0, ThreadProc, hport, 0, NULL);
        if(pthread[n] == NULL){
            int error = GetLastError();
            printf("ERROR CODE: %d\n", error);
            closesocket(Socket_Server);
            CloseHandle(hport);
            WSACleanup();
        }
    }

    while(1){ //主线程阻塞，防止程序(线程)结束
        Sleep(1000); //休眠1s，线程挂起
    }

    for(int n = 0;n < nProcessorsCount;++n){
        CloseHandle(pthread[n]);
    } //清理线程
    free(pthread); //释放线程数组
    CloseHandle(hport);
    Clear();
    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter) //线程函数
{
    HANDLE port = (HANDLE)lpParameter;
    DWORD NumberOfBytes;
    LONG_PTR index;
    LPOVERLAPPED lpOverlapped;
    while(1){
        BOOL bflag = GetQueuedCompletionStatus(port, &NumberOfBytes, &index, &lpOverlapped, INFINITE);
        if(bflag == FALSE){
            if(GetLastError() == 64){
                printf("Client Close\n");
                continue;
            }
            printf("ERROR CODE: %d\n", GetLastError());
            continue;
        }
        //处理accept
        if(index == 0){
            printf("Client Connect\n");
            //绑定到完成端口
            HANDLE hport1 = CreateIoCompletionPort((HANDLE)allsocket[count], hport, count, 0);
            if(hport1 != hport){
                int error = GetLastError();
                printf("ERROR CODE: %d\n", error);
                closesocket(allsocket[count]);
                continue;
            }
            //PostSend(index);
            //新客户端投递recv
            PostRecv(count);
            count++;
            PostAccept();
        } else {
            if(NumberOfBytes == 0){
                //客户端正常下线
                printf("Client Close\n");
                closesocket(allsocket[index]);
                WSACloseEvent(alloverlapp[index].hEvent);
                allsocket[index] = 0;
                alloverlapp[index].hEvent = NULL;
            } else {
                if(str[0] != 0){
                    //recv
                    printf("%s\n", str);
                    memset(str, 0, MAXSIZE_BUF);
                    PostRecv(index);
                } else {
                    //send
                }
            }
        }
    }


    return 0;
}

int PostAccept(void)
{
    allsocket[count] =  WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); //使用socket函数也可以，但该函数更好
    alloverlapp[count].hEvent = WSACreateEvent();
    char str[1024] = {0};
    BOOL Acp_return = AcceptEx(allsocket[0], allsocket[count], str, 0, sizeof(struct sockaddr_in)+16, sizeof(struct sockaddr_in)+16, NULL, &alloverlapp[0]); //基本地址长度为sizeof(struct sockaddr_in)
    if(Acp_return == FALSE){ //错误处理
    int error = WSAGetLastError();
        if(error == ERROR_IO_PENDING){
            return 0; //延时处理
        } else {
            return error; //出错
        }
    } else {
        //立即完成
        HANDLE hport1 = CreateIoCompletionPort((HANDLE)allsocket[count], hport, count, 0);
        if(hport1 != hport){
            int error = GetLastError();
            printf("ERROR CODE: %d\n", error);
            closesocket(allsocket[count]);
        }
        PostRecv(count); //投递recv
        count++;
        PostAccept(); //立即完成就递归
    }

    return 0;
}

int PostRecv(int index)
{
    WSABUF wsabuf;
    wsabuf.buf = str;
    wsabuf.len = MAXSIZE_BUF;
    DWORD buffcount;
    DWORD dwflag = 0;
    int recv_return = WSARecv(allsocket[index], &wsabuf, 1, &buffcount, &dwflag, &alloverlapp[index], NULL);
    if(recv_return == 0){
        //立即完成
        if(str[0] != 0){
            printf("Client: %s", str);
            memset(str, 0, MAXSIZE_BUF); //打印信息与清空缓冲区
        }
        PostRecv(index); //递归，继续投递recv
        return 0;
    } else {
        int error = WSAGetLastError();
        if(error == WSA_IO_PENDING){
            return 0; //延迟处理
        }
        return error;
    }

}

int PostSend(int index)
{
    WSABUF wsabuf;
    wsabuf.buf = "Hello";
    wsabuf.len = MAXSIZE_BUF;
    DWORD dwSendCount;
	DWORD dwFlag = 0;
	int nRes = WSASend(allsocket[index], &wsabuf, 1, &dwSendCount, dwFlag, &alloverlapp[index], NULL);

	if (0 == nRes)
	{
		//立即完成的
		//打印信息
		printf("send成功\n");

		return 0;
	}
	else
	{
		int a = WSAGetLastError();
		if (ERROR_IO_PENDING == a)
		{
			//延迟处理
			return 0;
		}
		else
		{
			return a;
		}
	}
}

void Clear(void)
{
    for(int n = 0;n < count; ++n){
        if(allsocket[n] == 0){
            continue;
        }
        closesocket(allsocket[n]);
        WSACloseEvent(alloverlapp[n].hEvent);
    }
    WSACleanup();
}
