#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#include <string.h>

#define UM_ASYNSELECT WM_USER+1
#define SOCKET_SIZE 1024

LRESULT CALLBACK WinBack(HWND hwnd, UINT msgID, WPARAM wparam, LPARAM lparap);

struct AllSockets {
    SOCKET sockets[SOCKET_SIZE];
    int count;
}; //存放所有SOCKET的结构体

struct AllSockets all_sockets;
int x = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX WS; //创建窗口结构体
    WS.cbClsExtra = 0; //根据窗口类结构分配的额外字节数。系统将字节初始化为零。一般用不到，填0
    WS.cbSize = sizeof(WNDCLASSEX); //窗口结构体大小
    WS.cbWndExtra = 0; //窗口实例后要分配的额外字节数。系统将字节初始化为零.一般不用，填0。
    WS.hbrBackground = NULL; //窗口颜色，默认为白
    WS.hCursor = NULL; //设置光标形态
    WS.hIcon = NULL; //左上角图标
    WS.hIconSm = NULL; //最小化图标
    WS.hInstance = hInstance; //实例句柄，填参数一 
    WS.lpfnWndProc = WinBack; //填回调函数名字 
    WS.lpszClassName = "SynSelect"; //当前窗口类的名字，随便起一个
    WS.lpszMenuName =  NULL; //菜单
    WS.style = CS_HREDRAW | CS_VREDRAW; //窗口风格
    /*
    CS_HREDRAW 水平刷新  CS_VREDRAW 垂直刷新
    窗口改变时，需要重新绘制，不然窗口就无法正常显示
    */

   //注册窗口
   RegisterClassEx(&WS);

   //创建窗口
   HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "SynSelect", "SynSelect_Test", WS_OVERLAPPEDWINDOW, 200, 200, 600, 400, NULL, NULL, hInstance, NULL);
	if (NULL == hwnd)
	{
		return 0;
	}

    //显示窗口
    ShowWindow(hwnd, 1);

    //重绘（刷新）窗口
    UpdateWindow(hwnd);

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

    all_sockets.count = 0;
    all_sockets.sockets[all_sockets.count] = Socket_Server;
    all_sockets.count++;
    //将服务端SOCKET装入数组

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

    //给SOCEKT绑定消息，并投递给系统
    if(WSAAsyncSelect(Socket_Server, hwnd, UM_ASYNSELECT, FD_ACCEPT) == SOCKET_ERROR){
        closesocket(Socket_Server);
        WSACleanup();
        return 0;
    }

    //消息循环
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for(int n = 0; n<all_sockets.count; ++n){
        closesocket(all_sockets.sockets[n]);
        WSACleanup();
    } //关闭全部SOCKET与清理网络库

    return 0;
}

LRESULT CALLBACK WinBack(HWND hwnd, UINT msgID, WPARAM wparam, LPARAM lparam)
{
    SOCKET socket;
    HDC hdc = GetDC(hwnd); //获取窗口显示区句柄

    switch(msgID){
        case WM_CREATE: //窗口 初始化产生，只产生一次，可以放用于初始化的代码
            break;
        case WM_DESTROY: //这里WM_XX就是消息
            PostQuitMessage(0); //传递退出消息，没有的话窗口关闭不了
            break;
        case UM_ASYNSELECT:
            socket = (SOCKET)wparam; //获取SOCKET
            if(HIWORD(lparam) != 0){ //判断消息是否出错
                if(WSAECONNABORTED == HIWORD(lparam)){ //客户端关闭
                    WSAAsyncSelect(socket, hwnd, UM_ASYNSELECT, 0); //取消SOCKET上的消息
                    closesocket(socket);
                    for(int n =0; n<all_sockets.count; ++n){
                        if(socket == all_sockets.sockets[n]){
                            all_sockets.sockets[n] = all_sockets.sockets[all_sockets.count-1];
                            all_sockets.count--;
                            break;
                        }
                    }
                    TextOut(hdc, 0, x, "Client Close", strlen("Client Close"));
                    x+=15;
                }
                break;
            }

            //具体消息
            switch(LOWORD(lparam)){
                case FD_ACCEPT:
                    TextOut(hdc, 0, x, "Client Connect", strlen("Client Connect")); //输出字符
                    x+=15;
                    SOCKET Socket_Client = accept(socket, NULL, NULL);
                    if(Socket_Client == INVALID_SOCKET){
                        break;
                    }
                    MessageBox(NULL, "Client Connect!", "Prompt", MB_OK); //弹出窗口
                    //将客户端SOCKET绑定消息并投递给消息队列
                    if(WSAAsyncSelect(Socket_Client, hwnd, UM_ASYNSELECT, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR){
                        printf("ACCEPT ERROR! 错误代码: %d\n",WSAGetLastError());
                        closesocket(Socket_Client);
                        break;
                    }
                    all_sockets.sockets[all_sockets.count] = Socket_Client;
                    all_sockets.count++;
                    break;
                case FD_READ:
                {
					char recv_msg[1024] = { 0 };
					if (SOCKET_ERROR == recv(socket, recv_msg, 1023, 0))
					{
						break;
					}
					TextOut(hdc, 30, x, recv_msg, strlen(recv_msg));
					x += 15;
                }
                    break;
                case FD_WRITE:
                    TextOut(hdc, 0, x, "write", strlen("write"));
                    x+=15;
                    break;
                case FD_CLOSE:
                    WSAAsyncSelect(socket, hwnd, UM_ASYNSELECT, 0); //取消SOCKET上的消息
                    closesocket(socket);
                    for(int n =0; n<all_sockets.count; ++n){
                        if(socket == all_sockets.sockets[n]){
                            all_sockets.sockets[n] = all_sockets.sockets[all_sockets.count-1];
                            all_sockets.count--;
                            break;
                        }
                    }
                    TextOut(hdc, 0, x, "Client Close", strlen("Client Close"));
                    x+=15;

                    break;
            }

            break;
    }

    return DefWindowProc(hwnd, msgID, wparam, lparam); //对没有处理的消息默认处理
}