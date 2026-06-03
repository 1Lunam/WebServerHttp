#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "create_socket.h"
#include "connection_manager.h"
#include "logger.h"

#define PORT 8080

int main(){

    int server_fd;
    server_fd = create_socket(PORT);

    logger_thread();

    printf("==================================\n"
           "HTTP/1.0 WEB SERVER\n"
           "Porta: 8080\n"
           "Select(): attivo\n"
           "Logger: attivo\n"
           "==================================\n");

    connection_manager(server_fd);

    close(server_fd);
    return 0;
    }




