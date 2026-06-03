#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "http_parser.h"

void parse_request(char *buffer,Httprequest *request){

    // prendo tutti i caratteri consecutivi in buffer e metto il primo in metho il secondo in path e il terzo in version
    sscanf(buffer,"%15s %255s %15s", request->method, request->path, request->version);

}