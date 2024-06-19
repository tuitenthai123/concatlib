#ifndef catIOT_h
#define catIOT_h

#include "Arduino.h"

class catIOT {
private:
    String id_project;
    static const int MAX_DATA_POINTS = 10;
    float arrayData[MAX_DATA_POINTS];

public:
    catIOT(String id_project);

    float sendToMongoDB(String payload, String type);
    float sendToMongoDB(String payload, String type, String Pin);
    float readOne(String Pin);
    void WriteArr(String Pin, float data);
};

#endif