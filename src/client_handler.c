#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "http_parser.h"
#include "file_handler.h"


void handle_http_request(int client_fd){
    char buffer[4096] = {0};

    //riceve le richieste da parte del client per fare il parsing
    int bytes = recv(client_fd,buffer,sizeof(buffer)-1,0);
    if(bytes<=0){
        close(client_fd);
        return;
    }
    buffer[bytes]= '\0';

    if(bytes > 0){
        // parsing di estrapolazione del metodo path e versione
        Httprequest request;
        parse_request(buffer, &request);

        printf("Metodo: %s\n", request.method);
        printf("Path: %s\n", request.path);
        printf("Versione: %s\n", request.version);

        handle_file_request(client_fd, &request);

    }
    close(client_fd);
}
        