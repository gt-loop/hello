#include <string>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "../../../include/cJSON.h"
#include "../../../include/zlog.h"
#include "../../../include/global.hpp"
#include "../../../include/object.hpp"

extern zlog_category_t *logger;
extern char *images_b64[5];

Gat1400::Gat1400(/* args */)
{
}

Gat1400::Gat1400(cJSON *rgtJs, cJSON *uRgtJs, cJSON *kpaJs, cJSON *tmJs, cJSON *fcJs, cJSON *mvJs, cJSON *siijs)
{

    this->Register = rgtJs;
    this->UnRegister = uRgtJs;
    this->Keepalive = kpaJs;
    this->Time = tmJs;
    this->Faces = fcJs;
    this->MotorVehicles = mvJs;
    this->SubImageInfo = siijs;
    if (NULL == this->Register || NULL == this->UnRegister || NULL == this->Keepalive || NULL == this->Time || NULL == this->Faces || NULL == this->MotorVehicles)
    {
        zlog_error(logger, "jsonstr parse json failed");
        //return;
    }
}

Gat1400::Gat1400(const char *rgtStr,
                 const char *uRgtStr,
                 const char *kpaStr,
                 const char *tmStr,
                 const char *fcStr,
                 const char *mvStr,
                 const char *siiStr)
{

    zlog_debug(logger, "%s");
    this->Register = cJSON_Parse(rgtStr);
    this->UnRegister = cJSON_Parse(uRgtStr);
    this->Keepalive = cJSON_Parse(kpaStr);
    this->Time = cJSON_Parse(tmStr);
    this->Faces = cJSON_Parse(fcStr);
    this->MotorVehicles = cJSON_Parse(mvStr);
    this->SubImageInfo = cJSON_Parse(siiStr);
    if (NULL == this->Register || NULL == this->UnRegister || NULL == this->Keepalive || NULL == this->Time || NULL == this->Faces || NULL == this->MotorVehicles)
    {
        zlog_error(logger, "jsonstr parse json failed");
        //return;
    }
}

Gat1400::~Gat1400()
{
}

int Gat1400::createRgUrgKp(const char *deviceId)
{
    /* RegisterObject json
    { 
        "RegisterObject":
            {
                "DeviceID": "31000051511191251215"
            } 
    }*/
    cJSON *RegisterObject = NULL;
    RegisterObject = cJSON_GetObjectItem(this->Register, "RegisterObject");
    if (!RegisterObject)
    {
        zlog_error(logger, "RegisterObject get failed");
        return -1;
    }
    cJSON_bool r = false;
    r = cJSON_ReplaceItemInObject(RegisterObject, "DeviceID", cJSON_CreateString(deviceId));
    if (!r)
    {
        zlog_error(logger, "deviceid set failed");
        return -1;
    };

    cJSON *UnRegisterObject = NULL;
    UnRegisterObject = cJSON_GetObjectItem(this->UnRegister, "UnRegisterObject");
    if (!UnRegisterObject)
    {
        zlog_error(logger, "UnRegisterObject get failed");
        return -1;
    }

    r = cJSON_ReplaceItemInObject(UnRegisterObject, "DeviceID", cJSON_CreateString(deviceId));
    if (!r)
    {
        zlog_error(logger, "deviceid set failed");
        return -1;
    };

    cJSON *KeepaliveObject = NULL;
    KeepaliveObject = cJSON_GetObjectItem(this->Keepalive, "KeepaliveObject");
    if (!KeepaliveObject)
    {
        zlog_error(logger, "KeepaliveObject get failed");
        return -1;
    }

    r = cJSON_ReplaceItemInObject(KeepaliveObject, "DeviceID", cJSON_CreateString(deviceId));
    if (!r)
    {
        zlog_error(logger, "deviceid set failed");
        return -1;
    };
}

int Gat1400::createFaces()
{
}

int Gat1400::createMotorVehicles(char *PlateNo,
                                 char *TollgateID,
                                 int LaneNo,
                                 char *PassTime,
                                 char *Direction,
                                 char *VehicleClass,
                                 int Speed,
                                 char *DeviceID)

{
    /*{
    "MotorVehicleObjectList":
    {
        "MotorVehicleObject":[
            MotorVehicle
        ]
    }
    }*/
    cJSON_ReplaceItemInObject(this->MotorVehicles, "PlateNo", cJSON_CreateString(PlateNo));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "TollgateID", cJSON_CreateString(TollgateID));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "LaneNo", cJSON_CreateNumber(LaneNo));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "PassTime", cJSON_CreateString(PassTime));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "Direction", cJSON_CreateString(Direction));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "VehicleClass", cJSON_CreateString(VehicleClass));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "Speed", cJSON_CreateNumber(Speed));
    cJSON_ReplaceItemInObject(this->MotorVehicles, "DeviceID", cJSON_CreateString(DeviceID));
    cJSON *mvol = cJSON_CreateObject();
    this->mvoljs = cJSON_CreateObject();
    cJSON *mvo = cJSON_CreateArray();
    cJSON_AddItemToArray(mvo, this->MotorVehicles);
    cJSON_AddItemToObject(mvol, "MotorVehicleObject", mvo);
    cJSON_AddItemToObject(this->mvoljs, "MotorVehicleListObject", mvol);

    //char *img[5] = {"1","2","3","4","5"};
    setSubImageList(this->MotorVehicles, images_b64, DeviceID, PassTime);
    //char *mvstr = cJSON_Print(mvoljs);
    //zlog_debug(logger,"post body ==%s",mvstr);
    return 0;
}
int Gat1400::refreshMotorVehicles()
{
    time_t nowTime;
    struct tm *timeinfo;
    time(&nowTime);
    timeinfo = localtime(&nowTime);
    char timeStr[80];
    strftime(timeStr, 80, "%Y%m%d%H%M%S010", timeinfo);
    cJSON_ReplaceItemInObject(this->MotorVehicles, "PassTime", cJSON_CreateString(timeStr));

    char *plateNo_1_20[] = {"京", "沪", "晋", "湘", "津", "新", "吉", "桂", "辽", "黑", "苏", "鲁", "豫", "鄂", "陕", "青", "宁", "粤", "警", "川"};
    char *plateNo_STR_17[] = {"A", "B", "C", "D", "E", "F", "G", "H", "G", "K", "M", "N", "P", "Q", "R", "S", "T"};
    char *plateNo_INT_10[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char plateNo[20] = {0};
    int i_r = rand() % 10;
    strcat(plateNo, plateNo_1_20[i_r + 10]);
    strcat(plateNo, plateNo_STR_17[i_r + 7]);
    strcat(plateNo, plateNo_INT_10[i_r]);
    if (i_r & 1)
    {
        strcat(plateNo, plateNo_INT_10[rand() % 10]);
        strcat(plateNo, plateNo_INT_10[rand() % 10]);
    }
    else
    {
        i_r = rand() % 10;
        strcat(plateNo, plateNo_INT_10[rand() % 10]);
        strcat(plateNo, plateNo_STR_17[rand() % 10 + 7]);
    };
    if (i_r & 1)
    {
        strcat(plateNo, plateNo_STR_17[rand() % 10 + 7]);
        strcat(plateNo, plateNo_INT_10[i_r]);
    }
    else
    {
        strcat(plateNo, plateNo_INT_10[i_r]);
        strcat(plateNo, plateNo_STR_17[i_r + 7]);
    };
    cJSON_ReplaceItemInObject(this->MotorVehicles, "PlateNo", cJSON_CreateString(plateNo));

    int speed_int_10[] = {40, 50, 60, 70, 80, 90, 100, 110, 115, 120};
    int speed = speed_int_10[rand() % 10];
    cJSON_ReplaceItemInObject(this->MotorVehicles, "Speed", cJSON_CreateNumber(speed));

    char *VC_STR_12[] = {"K", "H", "Q", "Z", "D", "M", "N", "T", "T", "G", "B", "X"};
    char vehicleClass[5] = {0};
    strcat(vehicleClass, VC_STR_12[rand() % 12]);
    strcat(vehicleClass, plateNo_INT_10[rand() % 10]);
    strcat(vehicleClass, plateNo_INT_10[rand() % 10]);
    cJSON_ReplaceItemInObject(this->MotorVehicles, "VehicleClass", cJSON_CreateString(vehicleClass));
}

int Gat1400::puRun(float registerInterval,
                   float unRegisterInterval,
                   float keepaliveInterval,
                   float timeInterval,
                   float facesInterval,
                   float motervehicleInterval)
{

    return 0;
}

int Gat1400::setSubImageList(cJSON *mvjs, char *img[], char *device_id, char *shot_time)
{
    int i = 0;
    int imgnum = 5;
    char *type[5] = {"01", "02", "08", "09", "14"};
    cJSON *siitmp;
    cJSON *silo = cJSON_CreateObject();
    cJSON *sil = cJSON_CreateArray();
    for (i; i < imgnum; i++)
    {

        //zlog_debug(logger,"sil==%s",cJSON_Print(sil));

        cJSON_AddItemToArray(sil, siitmp = cJSON_CreateObject());
        cJSON_AddStringToObject(siitmp, "Data", img[i]);
        cJSON_AddStringToObject(siitmp, "DeviceID", device_id);
        cJSON_AddStringToObject(siitmp, "FileFormat", "Jpeg");
        cJSON_AddStringToObject(siitmp, "ImageID", device_id);
        cJSON_AddStringToObject(siitmp, "ShotTime", shot_time);
        cJSON_AddStringToObject(siitmp, "StoragePath", "1");
        cJSON_AddStringToObject(siitmp, "Type", type[i]);
        cJSON_AddNumberToObject(siitmp, "EventSort", 12);
        cJSON_AddNumberToObject(siitmp, "Height", 12);
        cJSON_AddNumberToObject(siitmp, "Width", 12);
    }
    //cJSON_AddItemToArray(sil,this->SubImageInfo);
    //zlog_debug(logger,"SubImageInfo==%s",cJSON_Print((cJSON *)this->SubImageInfo));
    //zlog_debug(logger,"sil==%s",cJSON_Print(sil));
    cJSON_AddItemToObject(silo, "SubImageInfoObject", sil);
    //zlog_debug(logger,"silo==%s",cJSON_Print(silo));
    cJSON_ReplaceItemInObject(mvjs, "SubImageList", silo);
}

/*
in: camera info,type is JSON
out:camera info,type is typedef struct camera
*/
camera setCameraInfo(cJSON *camera_json)
{
    cJSON *c_tmp;
    camera c_camera;
    c_tmp = cJSON_GetObjectItem(camera_json, "device_id");
    strcpy(c_camera.device_id, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "tollgate_id");
    strcpy(c_camera.tollgate_id, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "username");
    strcpy(c_camera.username, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "password");
    strcpy(c_camera.password, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "server_addres");
    strcpy(c_camera.server_addres, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "direction");
    strcpy(c_camera.direction, cJSON_GetStringValue(c_tmp));
    c_tmp = cJSON_GetObjectItem(camera_json, "lane_no");
    c_camera.lane_no = cJSON_GetNumberValue(c_tmp);

    c_tmp = cJSON_GetObjectItem(camera_json, "register_rate");
    c_camera.register_rate = cJSON_GetNumberValue(c_tmp);
    c_tmp = cJSON_GetObjectItem(camera_json, "keepalive_rate");
    c_camera.keepalive_rate = cJSON_GetNumberValue(c_tmp);
    c_tmp = cJSON_GetObjectItem(camera_json, "motorvm_rate");
    c_camera.motorvm_rate = cJSON_GetNumberValue(c_tmp);
    c_tmp = cJSON_GetObjectItem(camera_json, "faces_rate");
    c_camera.faces_rate = cJSON_GetNumberValue(c_tmp);

    zlog_debug(logger, "camera device_id=%s", c_camera.device_id);

    return c_camera;
}