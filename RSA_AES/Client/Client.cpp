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

char output[LEN] = { 0 };
unsigned char temp[LEN] = { 0 };
char iv[] = { 103,35,148,239,76,213,47,118,255,222,123,176,106,134,98,92 };
unsigned char key[] = { 143,194,34,208,145,203,230,143,177,246,97,206,145,92,255,84 };
//string s_iv = "103,35,148,239,76,213,47,118,255,222,123,176,106,134,98,92";
//string s_key = "143,194,34,208,145,203,230,143,177,246,97,206,145,92,255,84";

int main()
{
	// read RSA keys from txt
	BigInt cn, ce, cd, sn, se;
	ifstream openc("../ClientRsa.txt");
	ifstream opens("../ServerPk.txt");
	openc >> cn;
	openc >> ce;
	openc >> cd;
	opens >> sn;
	opens >> se;
	openc.close();
	opens.close();
	Rsa ServerRsa(sn, se);	
	Rsa ClientRsa(cn, ce, cd);	

	//create client socket
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Failed to create socket..." << endl;
		WSACleanup();
		return 0;
	}

	// connet server
	SOCKADDR_IN ServerAddr;
	ServerAddr.sin_family = AF_INET;
	USHORT uPort = 8888;    // 服务器监听端口  
	ServerAddr.sin_port = htons(uPort);   // 绑定端口号  
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	cout << "Successfully connect to Server!" << endl;

	// buffer
	char buffer_send[257];
	char buffer_receive[257];

	int nNetTimeout = 100000;
	setsockopt(ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));    // 设置recv()阻塞时长


	// init AES
	 

	AES c_aes;
	c_aes.setKey(key);
	string s_key = (char*)key;
	string s_iv = (char*)iv;


	// message interaction
	while (1)
	{
		// send message
		cout << endl;




		while (1)
		{
			cout << "input message less than 1024 char [KEY: send AESKEY; IV: send AESIV; END: turn to receive; EXIT: end program]" << endl;
			// reset buffer
			memset(buffer_send, '\0', sizeof(buffer_send));
			string message;
			cin >> message;

			if (message == "EXIT")
			{
				// send "EXIT"
				strcpy(buffer_send, message.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);

				// exit
				cout << "The program is about to exit automatically..." << endl;
				Sleep(3000);
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
			else if (message == "KEY")
			{
				cout << "Send AESKEY..." << endl;
			
				strcpy(buffer_send, message.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);
				Sleep(1000);
				
				BigInt m = StringToBigInt(s_key);	// message to BigInt
				BigInt c = RSAEncode(m, ClientRsa);	// encode
				string text = BigIntToHex(c);    // text to send
				strcpy(buffer_send, text.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);    // send text
			}
			else if (message == "IV")
			{
				cout << "Send AESIV..." << endl;
				
				strcpy(buffer_send, message.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);
				Sleep(1000);

				BigInt m = StringToBigInt(s_iv);	// message to BigInt
				BigInt c = RSAEncode(m, ClientRsa);	// encode
				string text = BigIntToHex(c);    // text to send
				strcpy(buffer_send, text.c_str());
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);    // send text
			}

			else
			{
				char* temp = c_aes.Padding((char*)message.c_str());
				c_aes.Encrypt(temp, strlen(temp), buffer_send,iv);
				send(ClientSocket, buffer_send, sizeof(buffer_send), 0);    // send text

			}
		}

		// receive message
		cout << endl;
		while (1)
		{
			// reset buffer
			memset(buffer_receive, '\0', sizeof(buffer_receive));
			recv(ClientSocket, buffer_receive, sizeof(buffer_receive), MSG_WAITALL);    // receive text
			string text = buffer_receive;
			if (text == "EXIT")
			{
				// exit program
				cout << "The program is about to exit automatically..." << endl;
				Sleep(3000);
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}
			else if (text == "END")
			{
				cout << "message receive over..." << endl;
				break;
			}

			else
			{
				
				c_aes.Decrypt((char*)text.c_str(), strlen(text.c_str()), output, iv);

				cout << "--->Receive from Server: " << endl;
				cout << output << endl;
			}
		}
	}
}