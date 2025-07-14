//UDP server

#include "../include/socket_header.h"
#include <ctype.h>

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

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    
    printf("Waiting for connection----\n");

    while(1)
    {
        fd_set readfds; 
        readfds = master;
        if (select(max_socket + 1, &readfds, 0, 0, 0) < 0)
        {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRORNO());
            return 1;   
        }

        if (FD_ISSET(socket_listen, &readfds))
        {
            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);

            char read[1024];
            int bytes_received = recvfrom(socket_listen, read, 1024, 0, (struct sockaddr*) &client_address, &client_len);
            if (bytes_received < 1)
            {
                fprintf(stderr, "recvfrom() failed. (%d)\n", GETSOCKETERRORNO());
                return 1;
            }

            //printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
            
            for (int j = 0; j < bytes_received; j++)
            {
                read[j] = toupper(read[j]);
            }

            sendto(socket_listen, read, bytes_received, 0, (struct sockaddr*) &client_address, &client_len);
        }
        
    }
    printf("Closing Listening socket----\n");
    CLOSESOCKET(socket_listen);

    #if defined(_WIN32)
        WSACleanup();
    #endif
    
    printf("Finished");

    return 0;

}