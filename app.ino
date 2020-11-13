#include "ArduinoSTL.h"
#include "system_configuration.h"
#include "unwind-cxx.h"
#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestTemp1(UartMessageInterface::eDataType type, const std::string &name)
{
    Serial.println("onRequestTemp1");
    Serial.println(type);
}

void onRequestTemp2(UartMessageInterface::eDataType type, const std::string &name)
{
    Serial.println("onRequestTemp2");
    Serial.println(type);
}

void onResponseTemp1(UartMessageInterface::eDataType type, const std::string &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp1");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_double << endl;
}

void onResponseTemp2(UartMessageInterface::eDataType type, const std::string &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp2");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_int << endl;
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial OK...");

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);



    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(UartMessageInterface::SensorTemperature, "ROOM", onRequestTemp1);
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(UartMessageInterface::SensorTemperature, "WATER", onRequestTemp1);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(UartMessageInterface::SensorTemperature, "ROOM", onResponseTemp2);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(UartMessageInterface::SensorTemperature, "WATER", onResponseTemp2);

    // Get Request Message 전달
    UartMessageInterface::UartMessageSender reqTemp(UartMessageInterface::Request, UartMessageInterface::Get);
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "ROOM");
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "WATER");
    std::string msgReq = reqTemp.sendMessage();
    Serial.print(String(msgReq.c_str()));

    // Get Request Message 처리
    UartMessageInterface::UartMessageReceiver rcvReq(msgReq);
    rcvReq.processMessage();

    // Get Response Message 전달
    UartMessageInterface::UartMessageSender rspTemp(UartMessageInterface::Response, UartMessageInterface::Get);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "ROOM", 25.5, UartMessageInterface::Double);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "WATER", 19.0, UartMessageInterface::Integer);
    std::string msgRsp = rspTemp.sendMessage();
    Serial.print(String(msgRsp.c_str()));

    // Get Responses Message 처리
    UartMessageInterface::UartMessageReceiver rcvRsp(msgReq);
    rcvRsp.processMessage();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(200);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(200);                      // wait for a second

    if (Serial.available())
    {
        String str = Serial.readString();
        Serial.println(str);
    }
}
