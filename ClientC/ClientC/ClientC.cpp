#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#pragma warning(disable : 4996)

using namespace std;

int main(int args, char* argv[])
{
	WSADATA wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	//Try instantiate a WinSock
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}

	//Set settings
	SOCKADDR_IN sockaddr;
	int sizeofSockaddr = sizeof(sockaddr);
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(1035);
	sockaddr.sin_family = AF_INET;

	//Try to connect to the server
	SOCKET myConnection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(myConnection, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) != 0)
	{
		cout << "Error: failed connection to server\n";
		return 1;
	}
	cout << "You connected to server\n";

	char socketMassage[256];
	string massage;
	int num_hesh[256], j = 0;

	while (true) 
	{
		cin >> socketMassage;
		string if_not_exit = socketMassage;
		send(myConnection, socketMassage, sizeof(socketMassage), NULL);
		cout << "=================================================================" << endl;
		recv(myConnection, socketMassage, sizeof(socketMassage), NULL);

		char delims[] = "#";
		char* result = NULL;
		result = strtok(socketMassage, delims);
		while (result != NULL) 
		{
			printf("\%s\n", result);
			result = strtok(NULL, delims);
		}

		cout << "=================================================================" << endl;

		if (if_not_exit == "Exit" || if_not_exit == "exit")
		{
			break;
		}
	}

	return 0;
}