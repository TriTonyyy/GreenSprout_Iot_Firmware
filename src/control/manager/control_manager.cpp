#include "control_manager.h"
#include "../service/server_api.h"


void receiveControlsData(String deviceID, std::vector<Control*>& controls){
    StaticJsonDocument<512> doc = getData(getDevicePath+deviceID);
    for (size_t i = 0; i < controls.size(); i++)
    {
        controls[i]->updateFromJson(doc["controls"][i]);
    }
}
void updateControlsBehave(std::vector<Control*>& controls,std::vector<Sensor*> sensors){
    controls[0]->update(sensors[2]->getValue());
    controls[1]->update(sensors[3]->getValue());
    controls[2]->update(sensors[4]->getValue());
}
