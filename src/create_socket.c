#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "create_socket.h"

int create_socket(int PORT){

    int server_fd;
    struct sockaddr_in address;

    //Creazione della socket del server 
    if((server_fd = socket(AF_INET, SOCK_STREAM,0))< 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //permette di riutilizzare una porta usata poco fa
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))<0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //configurazione dell'inidirzzo dell'host
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // accetta da tutte le interfacce
    address.sin_port = htons(PORT);// trasformazione big endian

    //binding della socket con l'address
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //in ascolto per connessioni in entrata con queue di 10
    if(listen(server_fd, 10)<0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}