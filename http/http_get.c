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
    char *p = response;
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

            char* q;//Used to point to beginning of searched string 
            
            //Look for blank line after header to determine start oof body
            if (!body && (body = strstr(response, "\r\n\r\n")))
            {
                //strstr returns the pointer to the beginning of the matched string found first
                *body  = 0;
                body += 4;
                printf("Received Header:\n%s\n", response);

                q = strstr(response, "\nContent-Length: ");

                if (q)
                {
                    encoding = length;
                    q = strchr(q, ' ');
                    q++;
                    remaining = strtol(q, 0, 10);
                }
                else
                {
                    q = strstr(response, "\nTransfer-Encoding: chunked");
                    if (q)
                    {
                        encoding = chunked;
                        remaining = 0;
                    }
                    else{
                        encoding = connection;
                    }
                }
                printf("\nReceived body:\n");

            }

            if (body)
            {
                if (encoding == length)
                {
                    if (p - body >= remaining)//wait until receiving the remaining bytes
                    {
                        printf("%.*s", remaining, body);
                        break;
                    }
                }
                else if (encoding == chunked)
                {
                    //Want to loop over all the chunks and reassign remaining to zero after printing a chunk
                    do
                    {
                        if (remaining == 0)
                        {
                            if ((q = strstr(body, "\r\n")))
                            {
                                remaining = strtol(body, 0, 16);//chunk length is given in hexadecimal format
                                if (!remaining)//Since the chunked message ends with zero-length chunk we use goto
                                {
                                    goto finish;//break both loop by jumping to finish label
                                }
                                body = q + 2;//body points to the remaining length chunk, since there is '\r\n' (newline) after the hexadecimal number
                            }
                            else
                            {
                                break;
                            }
                                
                        }
                        if (remaining && p - body >= remaining)//Again wait for remaining chunk before printing
                        {
                            printf("%.*s", remaining, body);
                            body += remaining + 2;//Now body points to the next hexadecimal
                            remaining = 0;

                        }
                    }
                    while(!remaining);
                }
            }
        }
                                                 
    }
    finish://target for goto

    printf("\nClosing socket-----\n");
    CLOSESOCKET(server);

    #if defined(_WIN32)
        WSACleanup()
    #endif

    printf("Finished.\n");

    return 0;
}