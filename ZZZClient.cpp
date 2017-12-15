/*
	ZZZClient.cpp

	Copyright 2017 ZZZ Ltd. - Bulgaria

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "ZZZClient.h"


int ZZZClient::InitSocket()
{
#ifdef WIN32
	WSADATA wsaData;
	if (
		WSAStartup(MAKEWORD(2, 2), &wsaData) &&
		WSAStartup(MAKEWORD(2, 1), &wsaData) &&
		WSAStartup(MAKEWORD(1, 1), &wsaData)
		)
	{
		return 0;
	}
#endif
	return 1;
}

void ZZZClient::UninitSocket()
{
#ifdef WIN32
	WSACleanup();
#endif
}

ZZZClient::ZZZClient()
{
	this->sock = -1;

	InitSocket();
}

ZZZClient::~ZZZClient()
{
	Close();

	UninitSocket();
}

/**
 * Connect to a host on a certain port number
 */
BOOL ZZZClient::Connect(string address , int port)
{
	// create socket if it is not already created
	if(sock == -1)
	{
		//Create socket
		sock = (int)socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
			// Could not create socket
		}
		// Socket created
	}
	else {   /* OK , nothing */  }

	// setup address structure
	if((int)inet_addr(address.c_str()) == -1)
	{
		struct hostent *he;
		struct in_addr **addr_list;

		// resolve the hostname, its not an ip address
		if((he = gethostbyname(address.c_str())) == NULL)
		{
			// Failed to resolve hostname

			return FALSE;
		}
		// Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
		addr_list = (struct in_addr **) he->h_addr_list;

		server.sin_addr = *addr_list[0];
	}
	else // plain ip address
	{
		server.sin_addr.s_addr = inet_addr( address.c_str() );
	}

	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	// Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		// Error: Connect failed.
		return 1;
	}

#ifndef _WIN32
	{
		int sockFlags;
		sockFlags = fcntl(sock, F_GETFL, 0);
		sockFlags |= O_NONBLOCK;
		fcntl(sock, F_SETFL, sockFlags);
	}
#endif

	// Connected
	return TRUE;
}

/**
 * Send data to the connected host
*/
BOOL ZZZClient::Send(string data)
{
	// Send some data
	if( send(sock , data.c_str() , (int)strlen(data.c_str())+1 , 0) < 0)
	{
		// Send failed
		return FALSE;
	}
	// Data "data" send;

	return TRUE;
}

/**
 * Receive data from the connected host
 */
string ZZZClient::Receive()
{
	char buffer[512];
	string reply = "";

	// Receive a reply from the server
	int len = 0;
	while((len = (int)recv(sock , buffer , sizeof(buffer) , 0)) > 0)
	{
		buffer[len] = '\0';
		reply += buffer;
	}
	return reply;
}

void ZZZClient::Close()
{
	if(this->sock >= 0)
	{
		closesocket(this->sock);
		this->sock = -1;
	}
}

string ZZZClient::ZZZProgram(string host, int port, string data)
{
	string result;

	if(Connect(host, port))
	{
		Send(data);
		result = Receive();
		Close();
	}
	return result;
}
