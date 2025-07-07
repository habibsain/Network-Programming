//tcp_server.c

#include "include/socket_header.h"

int main()
{
    #if defined(_WIN32)
    WSDATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize socket");
        returrn -1;
    }
    #endif

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    
    struct addrinfo *host_addr;
    getaddrinfo(0, "8080", &hints, &host_addr);

    //bind


    SOCKET host_socket;

    return 0;
}