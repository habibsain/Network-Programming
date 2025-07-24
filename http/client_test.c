//main.c

#include "client.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


SOCKET numb = 1;

void test_parser()
{
    char* hostname;
    char* port;
    char* path;
    
    char url_1[]  = "http://www.example.com";
    parse_url(url_1, &hostname, &port, &path);
    // printf("%s\n", hostname);
    // printf("%s\n", port);
    assert(strcmp( hostname, "www.example.com") == 0 && strcmp( port, "80") == 0 && "parse_url()" );

    char url_2[]  = "http://www.example.com:40";
    parse_url(url_2, &hostname, &port, &path);
    // printf("hostname: %s\n", hostname);
    // printf("port: %s\n", port);
    assert(strcmp( hostname, "www.example.com") == 0 && strcmp( port, "40") == 0 && "parse_url()" );
    
    char url_3[]  = "http://www.example.com:35/mysite/todo.html";
    parse_url(url_3, &hostname, &port, &path);
    // printf("hostname: %s\n", hostname);
    // printf("port: %s\n", port);
    // printf("path: %s\n", path);
    assert(strcmp( hostname, "www.example.com") == 0 && strcmp( port, "35") == 0 && strcmp( path, "mysite/todo.html") == 0 && "parse_url()" );

}

int main()
{
    //mainvrr
    test_parser();
    return 0;
}