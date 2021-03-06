// WoL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#pragma comment(lib,"Ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <Ws2tcpip.h>


using namespace std;

int main()
{
	struct sockaddr_in si_other;
	WSAData wsaData;
	char mac [] = { 0xFF,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }; // put yours here
	WORD DllVersion = MAKEWORD(2, 0);
	int s, ok;
	//Winsock Startup
	cout << "initialising winsock ..." << endl;
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock n'a pas su demarrer", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	cout << "done ..." << endl;
	//create socket

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		cout << "socket creation failed - error code " << WSAGetLastError() << endl;
		exit(EXIT_FAILURE);
	}
	char broadcast = 1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(3000);
	PCWSTR ipDest = L"255.255.255.255";
	if (InetPton(AF_INET, ipDest, &(si_other.sin_addr))==1)
	{
		cout << "success" << endl;
	}
	//send WoL packet

	char message[6+6*16];
	for (int elem = 0; elem <= 5; elem++)
	{
		message[elem] =0xff;
	}
	for (int i = 0; i < 16; i++)
	{
		for (int elem = 0; elem <= 5; elem++)
		{
			message[elem + i * 6 + 6] = mac[elem];
		}
	}
	cout << "message : " << message <<" size : "<<sizeof(message) << endl;
	int len = 0;
	if ((len=sendto(s, message, sizeof(message), 0, (struct sockaddr *) &si_other, sizeof(si_other))) == SOCKET_ERROR)
	{
		cout << "message not sent\nWSAERROR: "<<WSAGetLastError()<<"\nPress enter to quit" << endl;
		cin >> ok;
		exit(EXIT_FAILURE);
	}
	cout << "done.. Press enter to quit ..." << endl;
	cin >> ok;
	WSACleanup();
    return 0;
}

