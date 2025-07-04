#if defined(_WIN32)//windows

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else//Linux
#define _GNU_SOURCE
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<ifaddrs.h>
#endif

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>


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

typedef struct Timeval  
{
    long tv_sec;
    long tv_microsec;
} timeval;

SOCKET create_socket(struct addrinfo* address_ptr)
{
    printf("Creating Socket----\n");

    SOCKET new_socket;
    new_socket = socket(address_ptr->ai_family, address_ptr->ai_socktype, address_ptr->ai_protocol);

    return new_socket;
}
