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
    
    char url_1[]  = "http//:example.com";
    parse_url(url_1, &hostname, &port, &path);
    assert(strcmp( hostname, "example.com") == 0 && "parse_url()" );


    char url_1[]  = "http//:80/example.com/";
    parse_url(url_1, &hostname, &port, &path);
    assert(strcmp( hostname, "example.com") == 0 && strcmp( port, "80") == 0 && "parse_url()" );
    
}

int main()
{
    //mainvrr
    test_parser();
    return 0;
}