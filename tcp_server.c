//tcp_server.c

#include "include/socket_header.h"
#include<ctype.h>

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


    //Configure host addr
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET6;
    hints.ai_flags = AI_PASSIVE;
    
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

    //Make it support dual-stack sockets
    int  option = 0;
    if(setsockopt(host_socket, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option)))
    {
        fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRORNO());
        return  1;
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

        if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
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
                    struct  sockaddr_storage client_addr;
                    socklen_t  client_len = sizeof(client_addr);
                    SOCKET client_socket = accept(host_socket, (struct sockaddr*) &client_addr, &client_len);
                    if (!ISVALIDSOCKET(client_socket))
                    {
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRORNO());
                        return 1;
                    }
                    
                    //Add the client socket to collection of sockets
                    FD_SET(client_socket, &master);
                    if (client_socket > max_socket)
                    {
                        max_socket = client_socket;
                    }

                    //print the client addr info
                    char address_buffer[100];
                    getnameinfo((struct sockaddr*) &client_addr, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);

                }
                else
                {
                    //read input from socket
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    
                    //If the client socket is disconnected
                    if (bytes_received < 1)
                    {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }

                    //Capitilising the received message from socket
                    for(int j = 0; j < bytes_received; j++)
                    {
                        read[j] = toupper(read[j]);
                    }

                    //sending the Capitalised data
                    send(i, read, bytes_received, 0);

                }//endif i == host_socket 
            }//endif FD_ISSET 
        }//endfor (loop in socket set)
    }//end while (listen and recv)

    printf("Closing listening socket....\n");
    CLOSESOCKET(host_socket);

    #if defined(_WIN32)
        WSACleanup();
    #endif

    printf("Finished.\n");
    return 0;
}