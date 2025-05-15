#include <vector>
#include "model/control.h"

// app.h
void appSetup();
void appLoop();
extern std::vector<Control*> controls;
#define wifiPin 15