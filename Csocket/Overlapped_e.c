#include <stdio.h>
#include <winsock2.h>
#include <mswsock.h>

#define MAXSIZE_SOCKET 1024
#define MAXSIZE_BUF 1024

SOCKET allsocket[MAXSIZE_SOCKET]; //创建SOCKET数组
OVERLAPPED alloverlapp[MAXSIZE_SOCKET]; //创建重叠结构体数组
int count;
char str[MAXSIZE_BUF] = {0}; // 接收缓冲区

int PostAccept();
int PostRecv(int);
int PostSend(int);
void Clear(); //清理函数

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

    allsocket[count] = Socket_Server;
    alloverlapp[count].hEvent = WSACreateEvent(); //给重叠结构的事件成员赋值一个合法的事件
    count++;

    if(PostAccept() != 0){
        Clear();
        return 0;
    } //函数的调用逻辑比较复杂，我们封装成一个函数

    while(1){ //循环等待事件
        for(int n = 0;n < count;++n){
            int wait_return = WSAWaitForMultipleEvents(1, &alloverlapp[n].hEvent, FALSE, 0, FALSE);
            if(wait_return == WSA_WAIT_TIMEOUT || wait_return == WSA_WAIT_FAILED){ //超时或错误
                continue;
            }
            DWORD dwstate;
            DWORD dwflag;
            BOOL get_return = WSAGetOverlappedResult(allsocket[n], &alloverlapp[n], &dwstate, TRUE, &dwflag);

            WSAResetEvent(alloverlapp[n].hEvent); //信号制空

            if(get_return == FALSE){
                if(GetLastError() == 10054){
                    printf("Client Close\n");
                    closesocket(allsocket[n]);
                    WSACloseEvent(alloverlapp[n].hEvent);
                    allsocket[n] = allsocket[count-1];
                    alloverlapp[n] = alloverlapp[count-1];
                    n--; //循环控制变量减一
                    count--; //个数减一
                }
                continue;
            }
            if(n == 0){ //接受链接完成
                printf("Client Connect\n");
                PostRecv(count);
                count++;
                PostAccept();
                continue;
            }
            if(dwstate == 0){ //客户端下线
                printf("Client Close\n");
                closesocket(allsocket[n]);
                WSACloseEvent(alloverlapp[n].hEvent);
                allsocket[n] = allsocket[count-1];
                alloverlapp[n] = alloverlapp[count-1];
                n--; //循环控制变量减一
                count--; //个数减一
                continue;
            }
            if(dwstate != 0){ //发送或者接收成功
                if(str[0] != 0){ //接收成功
                    printf("Client: %s\n", str);
                    memset(str, 0, MAXSIZE_BUF); //打印信息与清空缓冲区
                    PostRecv(n); //递归，继续投递recv
                } else { //发送成功
                    printf("Send Success\n");
                }
            }
        }
    }

    Clear();
    return 0;
}

int PostAccept()
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

void Clear()
{
    for(int n = 0;n < count; ++n){
        closesocket(allsocket[n]);
        WSACloseEvent(alloverlapp[n].hEvent);
    }
    WSACleanup();
}