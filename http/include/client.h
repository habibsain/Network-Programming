//client.h

#ifndef __HTTP_CLIENT__
#define __HTTP_CLIENT__

//headers for windows
#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

//headers for Unix based os
#else
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <ifaddrs.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//Berkeley sockets and winsock sockets have some differences
//Which are mitigated by defining macros

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRORNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define GETSOCKETERRORNO() (errno)
#define SOCKET int

#endif

#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

#define TIMEOUT 6.0
#define RESPONSE_SIZE 8192

//Recover hostname, port number and path from url
void parse_url(char* url, char** hostname, char** port, char** path);


//resolve the hostname ip address and establish a tcp connection
//return the corresponding SOCKET file descriptor(Linux)
SOCKET establish_connection(char* hostname, char* port);


//request the resource corresponding to the path from the host
void send_request(SOCKET sock, char* hostname, char* port, char* path);

#endif