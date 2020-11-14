#include <ArduinoJson.h>

#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestTemp1(UartMessageInterface::eDataType type, const String &name)
{
    Serial.println("onRequestTemp1");
    Serial.println(type);
}

void onRequestTemp2(UartMessageInterface::eDataType type, const String &name)
{
    Serial.println("onRequestTemp2");
    Serial.println(type);
}

void onResponseTemp1(UartMessageInterface::eDataType type, const String &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp1");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_double << endl;
}

void onResponseTemp2(UartMessageInterface::eDataType type, const String &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp2");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_int << endl;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestTemp1);
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestTemp1);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseTemp2);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseTemp2);


}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(2000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(2000);                      // wait for a second

    // if (Serial.available())
    // {
    //     String str = Serial.readString();
    //     Serial.println(str);
    // }

    // Get Response Message 전달
    UartMessageInterface::UartMessageSender rspTemp(UartMessageInterface::Response, UartMessageInterface::Get);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "ROOM", 25.5, UartMessageInterface::Float);
    rspTemp.appendResponse(UartMessageInterface::SensorCO2, "ROOM", 123, UartMessageInterface::Integer);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "WATER", 19.0, UartMessageInterface::Integer);
    rspTemp.sendMessage();
    // Serial.print(String(msgRsp.c_str()));

    // // Get Responses Message 처리
    // UartMessageInterface::UartMessageReceiver rcvRsp(msgRsp);
    // rcvRsp.processMessage();

    // Get Request Message 전달
    UartMessageInterface::UartMessageSender reqTemp(UartMessageInterface::Request, UartMessageInterface::Get);
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "ROOM");
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "WATER");
    reqTemp.appendRequest(UartMessageInterface::SensorCO2, "ROOM");
    reqTemp.sendMessage();
    // Serial.print(String(msgReq.c_str()));

    // // Get Request Message 처리
    // UartMessageInterface::UartMessageReceiver rcvReq(msgReq);
    // rcvReq.processMessage();
}