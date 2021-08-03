#include <string>
#include <string.h>
#include <malloc.h>

#include "../../include/zlog.h"
#include "../../include/global.hpp"
#include "../../include/cJSON.h"
#include "../../include/file.h"
#include "../../include/b64.h"
#include "../../include/object.hpp"
#include "../../include/ghttp.h"

zlog_category_t *logger;

cJSON *registerJson;
cJSON *keepaliveJson;
cJSON *unRegisterJson;
cJSON *timeJson;
cJSON *facesJson;
cJSON *motorVehiclesJson;
cJSON *subImageInfoJson;

const char *registerJsonFile = "./config/jsonfile/Register.json";
const char *keepaliveJsonFile = "./config/jsonfile/Keepalive.json";
const char *unRegisterJsonFile = "./config/jsonfile/UnRegister.json";
const char *timeJsonFile = "./config/jsonfile/Time.json";
const char *facesJsonFile = "./config/jsonfile/Faces.json";
const char *motorVehiclesJsonFile = "./config/jsonfile/MotorVehicle.json";
const char *subImageInfoJsonFile = "./config/jsonfile/SubImageInfo.json";

char *images_b64[5];
cJSON *cameraList_json;
int init()
{

    //init logger
    const char *loggerCfgFile = "./config/test_tmp.conf";
    if (zlog_init(loggerCfgFile))
    {
        puts("logger init failedd");
        return -1;
    }
    logger = zlog_get_category("my_log");
    if (!logger)
    {
        printf("get my_log fail\n");
        zlog_fini();
        return -2;
    }
    //init json object for gat1400 message

    /*
    registerJson =  cJSON_Parse(_register_Str);
    if(!registerJson)printf("cJSON_Parse error");
    keepaliveJson = cJSON_Parse(keepaliveStr);
    unRegisterJson = cJSON_Parse(unRegisterStr);
    timeJson = cJSON_Parse(timeStr);
    facesJson = cJSON_Parse(facesStr);
    motorVehiclesJson = cJSON_Parse(motorVehiclesStr);
    subImageInfoJson = cJSON_Parse(subImageInfoStr);
    */

    //init network for http

    //init images to base64 str
    char *image_0_buf = NULL;
    char *image_1_buf = NULL;
    char *image_2_buf = NULL;
    char *image_3_buf = NULL;
    char *image_4_buf = NULL;
    int image_0_len = readBinFileData("./config/image/01.jpg", &image_0_buf);
    int image_1_len = readBinFileData("./config/image/02.jpg", &image_1_buf);
    int image_2_len = readBinFileData("./config/image/08.jpg", &image_2_buf);
    int image_3_len = readBinFileData("./config/image/09.jpg", &image_3_buf);
    int image_4_len = readBinFileData("./config/image/14.jpg", &image_4_buf);

    images_b64[0] = b64_encode((const unsigned char *)image_0_buf, image_0_len);
    images_b64[1] = b64_encode((const unsigned char *)image_1_buf, image_1_len);
    images_b64[2] = b64_encode((const unsigned char *)image_2_buf, image_2_len);
    images_b64[3] = b64_encode((const unsigned char *)image_3_buf, image_3_len);
    images_b64[4] = b64_encode((const unsigned char *)image_4_buf, image_4_len);

    /*init test data*/
    //camera data
    char *testDataFile = "./config/jsonfile/TestDataConfig.json";
    cJSON *testData_json = cJSON_Parse(readFileDate(testDataFile).c_str());
    cameraList_json = cJSON_GetObjectItem(testData_json, "camreaList");
}
