#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "http_parser.h"
#include "logger.h"

//funzione per prendere il content type e stamparlo nell'header
char *get_content_type(char *filepath){
    //ricerca del valore nel path
    if(strstr(filepath, ".html"))
        return "text/html";
    if(strstr(filepath, ".css"))
        return "text/css";
    if(strstr(filepath, ".js"))
        return "application/javascript";
    if(strstr(filepath, ".png"))
        return "image/png";
    if(strstr(filepath, ".jpg"))
        return "image/jpeg";

    return "text/none";
}

void handle_file_request(int client_fd,Httprequest *request){
    
    char filepath[512];
    if(strcmp(request->method,"GET")==0){

        //controllo se il path è solo uno / e ritorno index in tal caso
        if(strcmp(request->path,"/")==0){
            strcpy(filepath,"./index.html");
        }else{
            sprintf(filepath, ".%s", request->path);
        }

        // punto ad un file aperto con path (filepath) in sola lettura ("r") 
        FILE *file = fopen(filepath, "r");

        //controllo se il file esiste e do 404(se non esiste) o 200(se esiste e invio il file) 
        if(file == NULL){
            // mando 404 se il file non esiste
            char response[256];
            sprintf(
                response,
                "HTTP/1.0 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"
                "<h1>404 File Not Found</h1>\r\n"
            );

            send(client_fd, response, strlen(response),0);
            
            // gestione log invia il messaggio a add log che si trova all'interno di logger
            char log_msg[256];
            sprintf(log_msg, "[404] %s %s", request->method, request->path);
            add_log(log_msg);
        }else{
            //mando header 200 se il file esiste e successivamente il file 
            char header[256];
            sprintf(
                header,
                "HTTP/1.0 200 OK\r\n"
                "Content-Type: %s\r\n"
                "\r\n",
                get_content_type(filepath)
            );
            send(client_fd, header,strlen(header),0);

            // gestione log invia il messaggio a add log che si trova all'interno di logger
            char log_msg[256];
            sprintf(log_msg, "[200] %s %s", request->method, request->path);
            add_log(log_msg);
            

            // prendo i pezzi dei file che metto in buffer tramite fgets e li sendo al client
            char buffer[1024];
            while(fgets(buffer, sizeof(buffer),file)!=NULL){
                send(client_fd, buffer, strlen(buffer),0);
            }
        
            //chiusura del file
            fclose(file);    
        }
    

    }else{
        // se il metodo non è GET do 405 method not found e lo sendo
        char response[256];            
        sprintf(
            response,
            "HTTP/1.0 405 Method Not Allowed\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
        );    

        send(client_fd,response, strlen(response),0);

        char log_msg[256];
        sprintf(log_msg, "[405] %s %s", request->method, request->path);
        add_log(log_msg);
    }
}