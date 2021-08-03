#ifndef _HTTP__H
#define _HTTP__H
#endif

#include "ghttp.h"
//int http_post(char *url,char *body);
typedef struct http_h
{
    char *addres = NULL;
    char *uri = NULL;
    char *body = NULL;
    char *username = NULL;
    char *password = NULL;
}http_h;  
char* http_post(http_h *c_http,char *device_id);
char* http_post(ghttp_request*request,http_h *c_http,char *device_id);


