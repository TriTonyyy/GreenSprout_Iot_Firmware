#ifndef SERVER_API_H
#define SERVER_API_H

#include "config.h"

void sendSensorData(String type, float value, bool status, String apiPath, bool isPost);

#endif
