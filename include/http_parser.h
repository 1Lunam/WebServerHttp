#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

//struttura per il parsing creata in server e riempita in http_parser.c
typedef struct{
    char method[16];
    char path[256];
    char version[16];
}Httprequest;

void parse_request(char *buffer, Httprequest *request);

#endif