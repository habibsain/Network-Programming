//http_get.c
/*
    Implement HTTP GET request
    Parse the response from server
*/

#include "client.h"

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

    //Check for  Command Line input
    if (argc < 2)
    {
        fprintf(stderr, "Usage: main url\n");
        return 1;
    }   

    char* url = argv[1];
    char *hostname, *port, *path;

    //Main client task:
    parse_url(url, &hostname, &port, &path);

    SOCKET server = establish_connection(hostname, port);

    send_request(server, hostname, port, path);

    //Check time taken for response to the request
    //Timeout the request if response takes more than specified time

    const clock_t start_time = clock();

    char response[RESPONSE_SIZE + 1];
    //p ponter keeps track of how much of response is filled
    char *p = response, *q;
    //end pointer ponts to end of response
    char* end = response + RESPONSE_SIZE;
    char* body = 0;

    while (1)
    {
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT)
        {
            fprintf(stderr, "Timeout after %.2f seconds", TIMEOUT);
            return 1;
        }

        if (p == end)
        {
            fprintf(stderr, "Buffer overflow");
        }

        //Initiate the collection of file descriptors to check IO readiness
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 200000;

        //Set flags to determine how response body length is realised
        enum {
            length,//Directly given by the web server
            chunked,//encoded into the response body
            connection//realised wheen the server closes the connection
        };

        int encoding = 0;
        int remaining = 0;


        if (select(server + 1, &readfds, 0, 0, &timeout) < 0)
        {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRORNO());
            return 1;
        }

        if (FD_ISSET(server, &readfds))
        {
            int bytes_received = recv(server, p, end - p, 0);//end - p is response buf size
            if (bytes_received < 0)//contection was closed by web server
            {
                if (encoding == connection && body)//body stores beginning of response body if one is received
                {
                    printf("Received: %.*s", (int) (end - body), body);
                }
                
                printf("\nConnection closed by peer.\n");
                break;
            }

            //make the received data null terminated
            //and mark the level upto which buffer is filled
            p += bytes_received;
            *p = 0;

            //Note:
            //Null terminated string enables us to use strstr()
            
            //Look for blank line after header to determine start oof body
            if (!body && (body = strstr(response, "\r\n\r\n")))
            {
                //strstr returns the pointer to the beginning of the matched string found first
                *body  = 0;
                body += 4;
                printf("Received Header:\n%s\n", response);
            }
        }
                                                 
    }

    return 0;
}