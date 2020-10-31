#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <ctime>

#pragma warning(disable : 4996)

using namespace std;

int main(int args, char* argv[]) 
{
	WSADATA wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	//Try instantiate a WinSock
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Error: winsock not be initialized" << endl;
		exit(1);
	}

	//Set settings
	SOCKADDR_IN sockaddr;
	int sizeofSockaddr = sizeof(sockaddr);
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(1035);
	sockaddr.sin_family = AF_INET;

	//Instantiate a listen socket
	SOCKET socketListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(socketListen, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	listen(socketListen, SOMAXCONN);

	//Accepts a new connection
	SOCKET newConnect;
	newConnect = accept(socketListen, (SOCKADDR*)&sockaddr, &sizeofSockaddr);

	ofstream logFile;
	logFile.open("C:\\Users\\dinaf\\OneDrive\\Рабочий стол\\Lab1\\ServerS\\DATA\\LOG.txt", ios::trunc);
	time_t sec = time(NULL);

	if (newConnect == 0)
	{
		cout << "ERROR\n";
	}
	else
	{
		cout << "Client connected to the server\n";
		logFile << "Client connected to the server\n" << time(NULL) - sec 
			<< "\n=========================================" << endl;
		char socketMassage[256];										   
		string massage;

		while (true) 
		{
			cout << "Server is waiting\n";
			logFile << "Server is waiting\n" << time(NULL) - sec << 
				"\n=========================================" << endl;
			recv(newConnect, socketMassage, sizeof(socketMassage), NULL);

			massage = socketMassage;
			if (massage == "Who" || massage == "who") 
			{
				cout << "Get the command 'Who'\n";
				logFile << "Server got the command 'Who'\n" << time(NULL) - sec 
						<< "\n=========================================" << endl;
				massage = "Formakidova Dina from K-26#";
				strcpy(socketMassage, massage.c_str());
				send(newConnect, socketMassage, sizeof(socketMassage), NULL);
				cout << "Sent the massage 'Who:'\n";
				logFile << "Server the sent message 'Who:#Formakidova DinaK-26 \n"<< time(NULL) - sec 
					<< "\n=========================================" << endl;
				continue;
			}
			else if (massage == "Exit" || massage == "exit")
			{
				cout << "Get the command 'Exit'\n";
				logFile << "Server got the command 'Exit'\n" << time(NULL) - sec 
					<< "\n=========================================" << endl;
				massage = "Good bye#";
				strcpy(socketMassage, massage.c_str());
				send(newConnect, socketMassage, sizeof(socketMassage), NULL);
				cout << "Sent the massage 'The End!!!'\n";
				logFile << "Server sent the message 'The End!!!#'\n" << time(NULL) - sec;
				break;
			}
			else if (massage == "Find" || massage == "find")
			{
				cout << "Get the command 'Find'\n";
				massage = "Give me your word!#";
				logFile << "Server got the command 'Find'\n" << time(NULL) - sec 
					<< "\n=========================================" << endl;
				strcpy(socketMassage, massage.c_str());

				//Asks client's word 
				send(newConnect, socketMassage, sizeof(socketMassage), NULL);
				cout << "Sent the massage 'Give me your word!'\n";
				logFile << "Server is waiting the word\n" << time(NULL) - sec 
					<< "\n=========================================" << endl;

				//Get client's word
				recv(newConnect, socketMassage, sizeof(socketMassage), NULL);
				massage = socketMassage;
				cout << "Server got the word '" << massage << "'\n"; //===========================
				logFile << "Server got the word\n" << time(NULL) - sec 
					<< "\n=========================================" << endl;

				//Try to find need word
				string strToParse = "";
				string clientWord = massage;
				ifstream inputFile("C:\\Users\\dinaf\\OneDrive\\Рабочий стол\\Lab1\\ServerS\\DATA\\DATA.txt");
				massage = "";
				while (getline(inputFile, strToParse))
				{
					if (strToParse.find(clientWord) != std::string::npos)
					{
						massage += strToParse + "#";
					}
				}
				if (massage == "") 
				{
					massage = "Server can not find strings with your word#'" + clientWord + "'";
					cout << "Server can not find the word '" << clientWord << "'\n";
					logFile << "Server can not find the word '" << clientWord << "'\n" << time(NULL) - sec
						<< "\n=========================================" << endl;
					strcpy(socketMassage, massage.c_str());
					send(newConnect, socketMassage, sizeof(socketMassage), NULL);
				}
				else 
				{
					logFile << "Server found:\n" << massage << endl << time(NULL) - sec 
						<< "\n=========================================" << endl;
					strcpy(socketMassage, massage.c_str());
					send(newConnect, socketMassage, sizeof(socketMassage), NULL);
				}
				inputFile.close();
			}
			else
			{
				massage = "<<< ERROR: the command is not exist >>>#";
				logFile << "Server got the wrong command\n" << time(NULL) - sec 
					<< "\n=========================================" << endl;
				strcpy(socketMassage, massage.c_str());
				send(newConnect, socketMassage, sizeof(socketMassage), NULL);
			}
		}
	}

	logFile.close();
	return 0;
}
