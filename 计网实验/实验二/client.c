#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
#define  PORT 37808
typedef struct sockaddr_in sockaddr_in;

int main(int argc, char* argv[])
{   //初始化网络环境
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("WSAStartup failed\n");
		return -1;
	}
	//建立一个UDP的socket
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockClient == INVALID_SOCKET) {
		printf("create socket failed\n");
		return -1;
	}
	int nNetTimeout=50;//50ms  recv timeout
	setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char*)&nNetTimeout,sizeof(int));
	// 申明一个网络地址信息的结构体，保存服务器的地址信息
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT); // 37808
	addr.sin_addr.S_un.S_addr = inet_addr("112.74.89.58");
    int n = 500;
    int cnt = 0;
	char buf[] = "client test!";
	for(int i=1; i<=n; i++) {//发送数据
        Sleep(50);
        int dwSent = sendto(sockClient, buf, strlen(buf), 0, (SOCKADDR *)&addr, sizeof(SOCKADDR));
        if (dwSent <= 0)
        {
            printf("send failed\n");
            cnt++;
        } // else printf("Send msg: %s\n", buf);
	}
	printf("send %d msg.\n", n-cnt);
	char recvBuf[512];
	memset(recvBuf, 0, 512);
	sockaddr_in addrSever = { 0 };
	int nServerAddrLen=sizeof(sockaddr_in);
	int cnt2 = 0;
	for(int i=1;i<=n-cnt;i++){// 接收数据
        int dwRecv = recvfrom(sockClient, recvBuf, 512, 0, (SOCKADDR *)&addrSever,&nServerAddrLen);
        if(dwRecv<=0) {
            printf("timeout:\"%s\" %d\n", inet_ntoa(addrSever.sin_addr),i);
            cnt2++;
        } // else printf("Recv msg from server [%s] : %s\n", inet_ntoa(addrSever.sin_addr), recvBuf);
	}
    printf("Recv %d msg.\n", n-cnt-cnt2);
	closesocket(sockClient);//关闭SOCKET连接
	WSACleanup();//清理网络环境
	return 0;
}
