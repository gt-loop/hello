#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/global.hpp"
#include "../include/object.hpp"
#include "../include/zlog.h"
#include "../include/file.h"
#include "../include/_http.hpp"
#include "../include/ghttp.h"

using namespace std;
extern zlog_category_t *logger;
extern cJSON *registerJson;
extern cJSON *keepaliveJson;
extern cJSON *unRegisterJson;
extern cJSON *timeJson;
extern cJSON *facesJson;
extern cJSON *motorVehiclesJson;
extern cJSON *subImageInfoJson;
extern cJSON *cameraList_json;

extern const char *registerJsonFile;
extern const char *keepaliveJsonFile;
extern const char *unRegisterJsonFile;
extern const char *timeJsonFile;
extern const char *facesJsonFile;
extern const char *motorVehiclesJsonFile;
extern const char *subImageInfoJsonFile;

void *camera_start(void *c_camera)
{

    camera *tc_camera = (camera *)c_camera;
    zlog_debug(logger, "camera is %s pthread %d is start", tc_camera->device_id, pthread_self());
    //Gat1400 pu_1400(registerJson,unRegisterJson,keepaliveJson,timeJson,facesJson,motorVehiclesJson,subImageInfoJson);
    Gat1400 pu_1400(readFileDate(registerJsonFile).c_str(),
                    readFileDate(unRegisterJsonFile).c_str(),
                    readFileDate(keepaliveJsonFile).c_str(),
                    readFileDate(timeJsonFile).c_str(),
                    readFileDate(facesJsonFile).c_str(),
                    readFileDate(motorVehiclesJsonFile).c_str(),
                    readFileDate(subImageInfoJsonFile).c_str());
    pu_1400.createRgUrgKp(tc_camera->device_id);
    pu_1400.createMotorVehicles("晋A00001", tc_camera->tollgate_id, tc_camera->lane_no, "202107281410111", tc_camera->direction, "vc", 80, tc_camera->device_id);

    http_h *c_http = (http_h *)malloc(sizeof(http_h));
    if (!c_http)
        return NULL;
    c_http->addres = tc_camera->server_addres;
    c_http->username = tc_camera->username;
    c_http->password = tc_camera->password;

    ghttp_request *request = ghttp_request_new();
    if (NULL == request)
    {
        zlog_error(logger, "ghttp_request_new failed");
        return NULL;
    }
    int i = 0;
    int isRgister = 0;
    while (1)
    {
        if ((!(i % tc_camera->register_rate)) && tc_camera->register_rate || (!isRgister))
        {
            c_http->uri = (char *)URI_REGISTER_1400;
            c_http->body = cJSON_Print(pu_1400.Register);
            if (http_post(request, c_http, tc_camera->device_id))
            {
                isRgister = 1;
                zlog_info(logger,"camera %s register succeed",tc_camera->device_id);
            }
            else
            {
                isRgister = 0;
                zlog_error(logger,"camera %s register failed",tc_camera->device_id);
            };
        }
        if ((!(i % tc_camera->keepalive_rate)) && tc_camera->keepalive_rate && isRgister)
        {
            c_http->uri = (char *)URI_KEEPALIVE_1400;
            c_http->body = cJSON_Print(pu_1400.Keepalive);
            http_post(request, c_http, tc_camera->device_id);
        }
        if ((!(i % tc_camera->motorvm_rate)) && tc_camera->motorvm_rate && isRgister)
        {
            pu_1400.refreshMotorVehicles();
            c_http->uri = (char *)URI_MOTORVEHICLES_1400;
            c_http->body = cJSON_Print(pu_1400.mvoljs);
            if(http_post(request, c_http, tc_camera->device_id))
            {
                zlog_debug(logger,"camera %s send motorVC succeed",tc_camera->device_id);
            }
        }
        if ((!(i % tc_camera->faces_rate)) && tc_camera->faces_rate && isRgister)
        {
        }

        i++;
        sleep(1);
        zlog_debug(logger, "camera is %s pthread %d is runing", tc_camera->device_id, pthread_self());
    }
    //return (void *)"ok";
}

int main()
{
    int r = 0;
    r = init();
    if (!r)
    {
        puts("init failed\n");
        return -1;
    }
    zlog_info(logger, "init done");

    int cameraNum = cJSON_GetArraySize(cameraList_json);
    pthread_t camera_pthread[cameraNum];
    int pthc_tmp;
    camera c_camera_camera[cameraNum];
    cJSON *c_camera_json, *c_tmp;
    for (r = 0; r < cameraNum; r++)
    {
        c_camera_json = cJSON_GetArrayItem(cameraList_json, r);
        c_camera_camera[r] = setCameraInfo(c_camera_json);
        //camera_start(&c_camera_camera);
        pthc_tmp = pthread_create(&camera_pthread[r], NULL, &camera_start, (void *)&c_camera_camera[r]);
        if (!pthc_tmp)
        {
            zlog_debug(logger, "camera %s pthread:%d is create ok", c_camera_camera[r].device_id,camera_pthread[r]);
        }
        else
        {
            zlog_debug(logger, "camera %s pthread:%d is create failed",c_camera_camera[r].device_id,camera_pthread[r]);
        }
    }

    while (1)
    {
        sleep(1);
    }
}
