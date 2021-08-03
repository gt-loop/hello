#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

#include "include/file.h"
#include "include/cJSON.h"
#include "include/zlog.h"

using namespace std;


int main()
{
    //vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    
    int rc;
    zlog_category_t *c;

    rc = zlog_init("./config/test_tmp.conf");
    if (rc) {
        printf("init failed\n");
        return -1;
    }

    c = zlog_get_category("my_cat");
    if (!c) {
        printf("get cat fail\n");
        zlog_fini();
        return -2;
    }

    zlog_info(c, "hello, zlog");

    zlog_fini();

    char *strJson = NULL;
    cJSON *json = NULL;
    cJSON *_jsonObject_name = NULL;
    cJSON *_jsonObject_resolutions = NULL;
    strJson = "{ \"name\": \"Awesome 4K\", "
                    "\"resolutions\":" 
                    "[" 
                       "{ \"width\": 1280, \"height\": 720 }," 
                        "{ \"width\": 1920, \"height\": 1080 }, "
                        "{ \"width\": 3840, \"height\": 2160 } "
                    "]" 
                
                "}";
    puts(strJson);
    json = cJSON_Parse(strJson);
    if(NULL == json)
    {
        puts("parse json fail");
        return -1;
    }
    _jsonObject_name = cJSON_GetObjectItem(json,"name");
    _jsonObject_resolutions = cJSON_GetObjectItem(json,"resolutions");
    printf("_jsonObject_resolutions size is %d \n",cJSON_GetArraySize(_jsonObject_resolutions));
    puts(_jsonObject_name->valuestring);
    if(_jsonObject_resolutions == NULL)
    {
        puts("parse _jsonObject_resolutions fail");
        return -1;
    }
    cJSON *tmp =  cJSON_GetArrayItem(_jsonObject_resolutions,1);

    printf("width is %d\n",cJSON_GetObjectItem(tmp,"width")->valueint);

    
    cJSON_ReplaceItemInObject(tmp,"width",cJSON_CreateNumber(666));

    printf("width is %d\n",cJSON_GetObjectItem(tmp,"width")->valueint);

    puts(cJSON_PrintUnformatted(json));

   
    return 0;
}