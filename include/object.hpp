#ifndef object__h
#define object__h
#endif
#include "cJSON.h"

/* 1400 http uri define*/
#define URI_REGISTER_1400       "/VIID/System/Register"
#define URI_UNREGISTER_1400     "/VIID/System/UnRegister"
#define URI_KEEPALIVE_1400      "/VIID/System/Keepalive"
#define URI_MOTORVEHICLES_1400  "/VIID/MotorVehicles"
#define URI_FACES_1400          "/VIID/Faces"



typedef struct camera
{
    char device_id[21];
    char device_name[21];
    char tollgate_id[21];
    char tollgate_name[21];
    int lane_no;
    char direction[3];

    char username[20];
    char password[20];
    char server_addres[20];
    int register_rate;
    int keepalive_rate;
    int motorvm_rate;
    int faces_rate;

}camera;

class Gat1400 
{
//private:
public:
    /* data */
    cJSON *Register;
    cJSON *UnRegister;
    cJSON *Keepalive;
    cJSON *Time;
    cJSON *Faces;
    cJSON *MotorVehicles;
    cJSON *SubImageInfo;

    /*message json*/
    
    cJSON *mvoljs;
    cJSON *foljs;

public:
    Gat1400();
    Gat1400(cJSON *rgtJs,cJSON *uRgtJs,cJSON *kpaJs,cJSON *tmJs,cJSON *fcJs,cJSON *mvJs,cJSON *siijs);
    Gat1400(    const char *rgtStr,
                const char *uRgtStr,
                const char *kpaStr,
                const char *tmStr,
                const char *fcStr,
                const char *mvStr,
                const char *siiStr);
    ~Gat1400();
    //create Register UnRegister and Keepalive
    int createRgUrgKp(const char *deviceId);
    int createTime();
    int createFaces();
    int createMotorVehicles(   char *PlateNo,
                                char *TollgateID,
                                int LaneNo,
                                char *PassTime,
                                char *Direction,
                                char *VehicleClass,
                                int Speed,
                                char *DeviceID);
    int refreshMotorVehicles();

    int puRun(  float registerInterval,
                float unRegisterInterval,
                float keepaliveInterval,
                float timeInterval,
                float facesInterval,
                float motervehicleInterval);
    int setSubImageList(cJSON *mvjs,char *img[],char *device_id,char *shot_time);

};

camera setCameraInfo(cJSON *camera_json);