#include <ArduinoJson.h>

#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequest(UartMessageInterface::eDataType type, const String &name)
{
    Serial.println("onRequest");
    Serial.println(enum2Str(type));
    Serial.println(name);
}

void onResponse(UartMessageInterface::eDataType type, const String &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponse");
    Serial.println(enum2Str(type));
    Serial.println(name);
    Serial.println(value.type);

    if (value.type == UartMessageInterface::Float)
    {
        Serial.println(value.value.val_float);
    }
    else
    {
        Serial.println(value.value.val_int);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    readBuffer = "";

    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequest);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponse);
}


// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(2000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(2000);                     // wait for a second

    // if (Serial.available())
    // {
    //     String str = Serial.readString();
    //     Serial.println(str);
    // }
    {
        // Get Response Message 전달
        UartMessageInterface::UartMessageSender rspTemp(UartMessageInterface::Response, UartMessageInterface::Get);
        rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "ROOM", UartMessageInterface::Float, 25.5);
        rspTemp.appendResponse(UartMessageInterface::SensorCO2, "ROOM", UartMessageInterface::Integer, 234);
        rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "WATER", UartMessageInterface::Integer, 456);
        String msgRsp = rspTemp.sendMessage();
        Serial.println(msgRsp);

        // Get Responses Message 처리
        UartMessageInterface::UartMessageReceiver rcvRsp(msgRsp);
        rcvRsp.processMessage();
    }

    {
        // Get Request Message 전달
        UartMessageInterface::UartMessageSender reqTemp(UartMessageInterface::Request, UartMessageInterface::Get);
        reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "ROOM");
        reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "WATER");
        reqTemp.appendRequest(UartMessageInterface::SensorCO2, "ROOM");
        String msgReq = reqTemp.sendMessage();
        Serial.println(String(msgReq.c_str()));

        // Get Request Message 처리
        UartMessageInterface::UartMessageReceiver rcvReq(msgReq);
        rcvReq.processMessage();
    }
}
