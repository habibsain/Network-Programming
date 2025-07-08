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

    //Create socket
    SOCKET host_socket;
    host_socket = create_socket(host_addr);
    if (!ISVALIDSOCKET(host_socket))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }


    //binding host(listening) socket to host(local) address
    printf("Binding-----\n");
    
    if (bind(host_socket, host_addr->ai_addr, host_addr->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }

    //Listen on the socket in host server
    printf("Listening-----\n");
    int max_num = 20;
    if (listen(host_socket, max_num) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }

    fd_set master;
    FD_ZERO(&master);
    FD_SET(host_socket, &master);
    SOCKET max_socket = host_socket;

    printf("Waiting for connections---\n");

    while(1)
    {
        //Create a copy of the socket collection for reading data
        fd_set reads;
        reads = master;

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_microsec = 10000;

        if (select(max_socket + 1, &reads, 0, 0, &timeout) < 0)
        {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRORNO());
            return 1;
        }

        //check readiness of sockets
        SOCKET i;
        for(i = 1; i <= max_socket; i++)
        {
            if (FD_ISSET(i, &reads))
            {
                if (i == host_socket)
                {
                    
                }
            }
        }
    }

    


    return 0;
}