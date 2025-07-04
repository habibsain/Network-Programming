//TCP client

#include  "include/socket_header.h"

int main(int argc, char* argv[])
{
    //Winsock Initialization
    #if defined(_WIN32)
    WSADATA d;

    if(WSAStartup(MAKEWORD(2, 2),  &d))
    {
        fprintf(stderr,"Failed to initialize socket.\n");
        return 1;
    }
    #endif

    //Check for  CL input
    if (argc < 3)
    {
        fprintf(stderr, "Usage: tcp_client hostname port\n");
        return 1;
    }

    //Configure remote addr
    printf("Configuring remote address-----\n");
    struct addrinfo hints;
    memsett(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* peer_address;

    if (getaddrinfo(argv[1], argv[2], &hints, &peer_address))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d) \n", GETSOCKETERRORNO());
        return 1;
    }

    //Display remote address
    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];

    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer, sizeof(address_buffer), service_buffer, sizeof(service_buffer), NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    //Create socket
    printf("Creating socket----\n");
    SOCKET socket_peer;
    socket_peer = create_socket(peer_address);

    if (!ISVALIDSOCKET(socket_peer))
    {
        printf(stderr, "socket() failed. (%d)\n", GETSOCKETERRORNO());
        return 1;
    }

    //Establish connection with remote server
}