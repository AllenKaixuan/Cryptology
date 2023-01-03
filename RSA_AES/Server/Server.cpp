#pragma once
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<winsock.h>
#include<iostream>
#include<string>
#include<time.h>
#include <sstream>
#include <iomanip>
#include <chrono>
#include"BigInt.h"
#include"des.h"
#include"rsa.h"
#include"aes.h"
#include<fstream>
#define LEN 128

using namespace std;
 char* iv = new char[16];
 unsigned char* key = new  unsigned char[16];
 bool aes_keyinit = 0;
 bool aes_ivinit = 0;
 char output[LEN] = { 0 };
 unsigned char temp[LEN] = { 0 };
int main()
{
	// read RSA keys from txt
	BigInt sn, se, sd, cn, ce;
	ifstream openc("../ClientPk.txt");
	ifstream opens("../ServerRsa.txt");
	openc >> cn;
	openc >> ce;
	opens >> sn;
	opens >> se;
	opens >> sd;
	openc.close();
	opens.close();
	Rsa ServerRsa(sn, se, sd);	// for encode
	Rsa ClientRsa(cn, ce);		// for decode

	//create server socket
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "Failed to create socket..." << endl;
		WSACleanup();
		return 0;
	}

	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_family = AF_INET;
	USHORT uPort = 8888;    // 服务器监听端口  
	ServerAddr.sin_port = htons(uPort);    // 绑定端口号 
	ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "Failed to bind...";
		closesocket(ServerSocket);
		return 0;
	}

	//创建连接
	listen(ServerSocket, 1);    //开始监听，等待队列最长为1	
	cout << "Waiting for connection..." << endl;
	SOCKET ClientSocket;
	SOCKADDR_IN ClientAddr;
	int ClientAddrlen = sizeof(ClientAddr);
	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrlen);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Failed to receive client request..." << endl << WSAGetLastError();
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}
	else
		cout << "Successfully connected to client！" << endl;

	// buffer
	char buffer_send[257];
	char buffer_receive[257];

	int nNetTimeout = 100000;
	setsockopt(ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
	
	AES s_aes;
	

	// message interaction
	while (1)
	{
		// receive message
		cout << endl;
		while (1)
		{
			// reset buffer
			memset(buffer_receive, '\0', sizeof(buffer_receive));
			recv(ClientSocket, buffer_receive, sizeof(buffer_receive), MSG_WAITALL);
			string text = buffer_receive;

			if (text == "EXIT")
			{
				// exit program
				cout << "The program is about to exit automatically..." << endl;
				Sleep(3000);
				closesocket(ServerSocket);
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}
			else if (text == "END")
			{
				cout << "message receive over..." << endl;
				break;
			}
			else if (text == "KEY")
			{
				
				cout << "Receieve AESKEY..." << endl;
				memset(buffer_receive, '\0', sizeof(buffer_receive));
				recv(ClientSocket, buffer_receive, sizeof(buffer_receive), MSG_WAITALL);
				string text = buffer_receive;

				BigInt c = HexToBigInt(text);
				BigInt m = RSADecode(c, ClientRsa);
				string message = BigIntToString(m);
				strcpy((char*)key,message.c_str());
				aes_keyinit = 1;

				
			}
			else if (text == "IV")
			{
				cout << "Receieve IV..." << endl;
				memset(buffer_receive, '\0', sizeof(buffer_receive));
				recv(ClientSocket, buffer_receive, sizeof(buffer_receive), MSG_WAITALL);
				string text = buffer_receive;

				BigInt c = HexToBigInt(text);
				BigInt m = RSADecode(c,ClientRsa);
				string message = BigIntToString(m);
				
				strcpy(iv, message.c_str());
				aes_ivinit = 1;
				
			}
			else
			{
				if (aes_ivinit && aes_keyinit)
				{
					// init AES
					

					s_aes.setKey(key);
					aes_ivinit = 0;
					aes_keyinit = 0;
				}
				
				s_aes.Decrypt((char*)text.c_str(), strlen(text.c_str()), output, iv);
				
				cout << "--->Receive from Client: " << endl;
				cout << output << endl;
			}
		}

		// send message
		cout << endl;
		while (1)
		{
			cout << "input message less than 1024 char [END: turn to receive; EXIT: end program]" << endl;
			// reset buffer
			memset(buffer_send, '\0', sizeof(buffer_send));
			string message;
			//cout << " ->send FROM client:" << endl;
			cin >> message;

			if (message == "EXIT")
			{
				// send "EXIT"
				strcpy(buffer_send, message.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);

				// exit
				cout << "The program is about to exit automatically..." << endl;
				Sleep(3000);
				closesocket(ServerSocket);
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}
			else if (message == "END")
			{
				// send "END"
				strcpy(buffer_send, message.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);
				break;    // turn to recevie
			}
			else
			{
				

				char* temp = s_aes.Padding((char*)message.c_str());
				s_aes.Encrypt(temp, strlen(temp), buffer_send, iv);
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);    // send text
			}
		}
	}
}