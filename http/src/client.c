//client.c

#include "client.h"

void parse_url(char* url, char** hostname, char** port, char** path)
{
    printf("url: %s", url);

    //check if the url has correct protocol
    char* ptr;
    ptr = strstr(url, "//:");

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
    *path = ptr;

    while (*ptr && *ptr != '/' && *ptr != '#' && *ptr != '?' )
        ptr++;
    *ptr = 0;
    ptr++;    




}