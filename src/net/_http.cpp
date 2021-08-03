#include <string.h>
#include "../../include/_http.hpp"
#include "../../include/ghttp.h"
#include "../../include/zlog.h"
extern "C"
{
#include "../../include/digest.h"
}
extern zlog_category_t *logger;

char* _strstr(char *src,char *str)
{
    char frontstr[20] = {0};
    strcpy(frontstr,str);
    strcat(frontstr,"=\"");
    int frontstrlen = strlen(frontstr);
    char *pf = strstr(src,frontstr);
    if(!pf)
    {
        return pf;
    }
    char dst[50] = {0};
    char *pa = strchr(pf+frontstrlen,'\"');
    int dstlen = pa - pf - frontstrlen;
    strncpy(dst,pf+frontstrlen,dstlen);
    return dst;
}

void * set_head_auth(   const char *username,
                        const char *realm,
                        const char *qop,
                        const char *nonce,
                        const char *uri,
                        const char *nc,
                        const char *cnonce,
                        const char *response,
                        char *c_auth)
{
    //char c_auth[256] = {0};
    strcat(c_auth,"Digest ");
    strcat(c_auth,"username=\"");strcat(c_auth,username);
    strcat(c_auth,"\",realm=\"");strcat(c_auth,realm);
    strcat(c_auth,"\",qop=\"");strcat(c_auth,qop);
    strcat(c_auth,"\",nonce=\"");strcat(c_auth,nonce);
    strcat(c_auth,"\",uri=\"");strcat(c_auth,uri);
    strcat(c_auth,"\",nc=");strcat(c_auth,nc);
    strcat(c_auth,",cnonce=\"");strcat(c_auth,cnonce);
    strcat(c_auth,"\",response=\"");strcat(c_auth,response);
    strcat(c_auth,"\"");
    zlog_debug(logger,"response:%s",c_auth);
    
}

char* http_post(http_h *c_http,char *device_id)
{
    ghttp_request *request = NULL;
    ghttp_status status;
    char url[64] = {0};
    char *buf;

    request = ghttp_request_new();
    if(NULL == request)
    {
        return NULL;
    }
    strcat(url,"http://");
    strcat(url,c_http->addres);
    strcat(url,c_http->uri);
    if(ghttp_set_uri(request,url) == -1)
    {
        return NULL;
    }
    if(ghttp_set_type(request,ghttp_type_post) == -1)
    {
        return NULL;
    }
    //ghttp_set_header(request,http_hdr_);
    
    ghttp_set_header(request,"Content-Type","application/VIID+JSON");
    ghttp_set_header(request,"User-Identify",device_id);
    ghttp_set_body(request,c_http->body,strlen((const char*)c_http->body));
    _request:
    ghttp_prepare(request);
    status = ghttp_process(request);
    if(status == ghttp_error)
    {
        return NULL;
    }
    if(ghttp_status_code(request) == 401)

    {
        const char* auth;
        auth = ghttp_get_header(request,"WWW-Authenticate");
        if(auth)
        {
            zlog_debug(logger,"http_s revice auth:%s",auth);
            /*realm="10.30.27.41",nonce="60ee8b26-41a2fb5-d04dbb7c",qop="auth"*/
            char *t_auth = (char *)auth;
            char realm[20] = {0};
            strcpy(realm,_strstr(t_auth,"realm"));
            char nonce[50] = {0};
            strcpy(nonce,_strstr(t_auth,"nonce"));
            char qop[10] = {0};
            strcpy(qop,_strstr(t_auth,"qop"));
            char *cnonce = "60efe685-74408c1a-d04dbb7c";
            char *nc = "00000001";
            char response[33]={0};

            httpauth_t hauth;
            httpauth_set_user_pwd(&hauth,c_http->username,c_http->password);
            httpauth_set_realm_nonce(&hauth,realm,nonce);
            _httpauth_get_response(&hauth,"POST",c_http->uri,nc,cnonce,qop,response);

            char c_auth[256] = {0};
            set_head_auth(c_http->username,realm,qop,nonce,c_http->uri,nc,cnonce,response,c_auth);
            ghttp_set_header(request,"Authorization",c_auth);
            goto _request;
            


        }

    }
    if(ghttp_status_code(request) == 200)
    {
        buf = ghttp_get_body(request);
        ghttp_close(request);
        
        
        zlog_debug(logger,"%s",buf);
        return buf;
    }
        
    ghttp_close(request);
    
    
    return NULL;
}

char* http_post(ghttp_request*request,http_h *c_http,char *device_id)
{
    ghttp_status status;
    char url[64] = {0};
    char *buf;

    strcat(url,"http://");
    strcat(url,c_http->addres);
    strcat(url,c_http->uri);
    if(ghttp_set_uri(request,url) == -1)
    {
        return NULL;
    }
    if(ghttp_set_type(request,ghttp_type_post) == -1)
    {
        return NULL;
    }
    //ghttp_set_header(request,http_hdr_);
    
    ghttp_set_header(request,"Content-Type","application/VIID+JSON");
    ghttp_set_header(request,"User-Identify",device_id);
    ghttp_set_body(request,c_http->body,strlen((const char*)c_http->body));
    _request:
    ghttp_prepare(request);
    status = ghttp_process(request);
    if(status == ghttp_error)
    {
        return NULL;
    }
    if(ghttp_status_code(request) == 401)

    {
        const char* auth;
        auth = ghttp_get_header(request,"WWW-Authenticate");
        if(auth)
        {
            zlog_debug(logger,"http_s revice auth:%s",auth);
            /*realm="10.30.27.41",nonce="60ee8b26-41a2fb5-d04dbb7c",qop="auth"*/
            char *t_auth = (char *)auth;
            char realm[20] = {0};
            strcpy(realm,_strstr(t_auth,"realm"));
            char nonce[50] = {0};
            strcpy(nonce,_strstr(t_auth,"nonce"));
            char qop[10] = {0};
            strcpy(qop,_strstr(t_auth,"qop"));
            char *cnonce = "60efe685-74408c1a-d04dbb7c";
            char *nc = "00000001";
            char response[33]={0};

            httpauth_t hauth;
            httpauth_set_user_pwd(&hauth,c_http->username,c_http->password);
            httpauth_set_realm_nonce(&hauth,realm,nonce);
            _httpauth_get_response(&hauth,"POST",c_http->uri,nc,cnonce,qop,response);

            char c_auth[256] = {0};
            set_head_auth(c_http->username,realm,qop,nonce,c_http->uri,nc,cnonce,response,c_auth);
            ghttp_set_header(request,"Authorization",c_auth);
            goto _request;
            


        }

    }
    if(ghttp_status_code(request) == 200)
    {
        buf = ghttp_get_body(request);
        ghttp_close(request);
        
        
        zlog_debug(logger,"%s",buf);
        return buf;
    }
        
    ghttp_close(request);
    
    
    return NULL;
}
