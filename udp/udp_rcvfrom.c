//udp server(recv)

#include "../include/socket_header.h"

int main()
{
    #if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize socket");
        return 1;
    }
    #endif

    printf("Configuring local address----\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET6;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_addr;
    getaddrinfo(0, "8080", &hints, &bind_addr);

    printf("Creating socket----\n");
    SOCKET socket_listen;
    socket_listen = create_socket(bind_addr);
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }

    //Make it support dual-stack sockets
    int  option = 0;
    if(setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option)))
    {
        fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRORNO());
        return  1;
    }

    printf("Binding-----\n");
    if (bind(socket_listen, bind_addr->ai_addr, bind_addr->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }
    freeaddrinfo(bind_addr);



}