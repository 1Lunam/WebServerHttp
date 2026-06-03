#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client_handler.h"

void connection_manager(int server_fd){
    fd_set master_set;
    FD_ZERO(&master_set);
    FD_SET(server_fd,&master_set);
    int max_fd = server_fd;

    while(1){
        fd_set read_set = master_set;
        //aspetta finchè almeno una socket è pronta e mette la socket in read_set
        int ready = select(max_fd+1, &read_set, NULL, NULL, NULL);

        if(ready<0){
            perror("select");
            continue;
        }




        for(int i = 0; i<=max_fd; i++){
            if(FD_ISSET(i, &read_set)){
                if(i == server_fd){
                    // richiesta al server di una nuova connessione
                    int client_fd = accept(server_fd, NULL, NULL);

                    if(client_fd<0){
                        perror("accept");
                        continue;
                    }

                    printf("\n");

                    //aggiungo client_fd al master_set
                    FD_SET(client_fd, &master_set);

                    //aggiorno max
                    if(client_fd>max_fd){
                        max_fd = client_fd;
                    }
                }else{
                    // richiesta di un file e dopo in handle http viene chiuso fd e poi rimosso da master_set
                    handle_http_request(i);
                    FD_CLR(i,&master_set);
                }
            }
        }
    }
}