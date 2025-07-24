//client.c

#include "client.h"

//Recover hostname, port number and path from url
void parse_url(char* url, char** hostname, char** port, char** path)
{
    printf("url: %s\n", url);

    //check if the url has correct protocol
    char* ptr;
    ptr = strstr(url, "://");

    char* protocol = 0;

    if (ptr)//protocol provided in url
    {
        protocol = url;
        *ptr = 0;
        ptr += 3;//points to hostname onwards
    }
    else
        ptr = url;

    if (protocol)//Not '\0'
    {
        if( strcmp(protocol, "http"))//returns 0 if same
        {
            fprintf(stderr, "Protocol %s is unknown. Only 'http' is supported", protocol);
            exit(1);
        }
    }

    //Get hostname
    *hostname  = ptr;
    while (*ptr && *ptr != '/' && *ptr != ':' && *ptr != '#')
        ptr++;
        
    
    //Check for port numbber
    if (*ptr == ':')
    {
        *ptr = 0;
        ptr++;
        *port = ptr;

    }
    else
    {
        *port = "80";
    }
    while (*ptr && *ptr != '/' && *ptr != '#' && *ptr != '?' )
        ptr++;

    //check for document path
    if(*ptr)
        *path = ptr;

    if (*ptr == '/')
    {
        *ptr = 0;
        *path = ptr + 1;//We can skip the first "/"
    }

    while (*ptr && *ptr != '#' )
        ptr++;
    
    if (*ptr == '#')
        *ptr = 0;
   
}


void send_request(SOCKET sock, char* hostname, char* port, char* path)
{
    char buffer[2048];

    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-agent: localpc http_get_client\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    send(sock, buffer, strlen(buffer), 0);
    printf("sentHeaders:\n%s", buffer);

}

SOCKET establish_connection(char* hostname, char* port)
{
    //Configure remote addr
    printf("Configuring remote address-----\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* peer_address;

    if (getaddrinfo(hostname, port, &hints, &peer_address))
    {
        fprintf(stderr, "getaddrinfo() failed. (%d) \n", GETSOCKETERRORNO());
        exit(1);
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
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);

    if (!ISVALIDSOCKET(socket_peer))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRORNO());
        exit(1);
    }

    //Establish connection with remote server
    printf("Connecting----\n");
    if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen))
    {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRORNO());
        exit(1);
    }
    freeaddrinfo(peer_address);
    
    printf("Connected.\n");
    
    return socket_peer;
}