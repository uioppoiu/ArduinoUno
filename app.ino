#include <ArduinoJson.h>

#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestTemp(UartMessageInterface::eDataType type, const String &name)
{
    Serial.println("onRequestTemp");
    Serial.println(type);

    // // Get Response Message 전달
    // UartMessageInterface::UartMessageSender rspTemp(UartMessageInterface::Response, UartMessageInterface::Get);
    // rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "ROOM", UartMessageInterface::Float, 25.5);
    // rspTemp.appendResponse(UartMessageInterface::SensorCO2, "ROOM", UartMessageInterface::Integer, 234);
    // rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "WATER", UartMessageInterface::Integer, 456);
    // String msgRsp = rspTemp.sendMessage();
    // Serial.println(msgRsp);
}

void onResponseTemp(UartMessageInterface::eDataType type, const String &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_double << endl;
}

String readBuffer;

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    readBuffer = "";

    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestTemp);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseTemp);
}


// the loop function runs over and over again forever
void loop()
{
    // digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    // delay(2000);                      // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    // delay(2000);                      // wait for a second

    int size = Serial.available();
    if (size > 0)
    {
        char buf[64] = {0,};
        Serial.readBytes(buf, 
        String buf = Serial.readString();
        if(buf.startsWith("<BEGIN>"))
        {
            readBuffer = "";
            readBuffer += buf.substring(7);
            Serial.print("BEGIN:");
            Serial.println(readBuffer);
        }
        else if(buf.endsWith("<END>"));
        {
            .
            Serial.print("READ:");
            Serial.println(readBuffer);
        }
        else
        {
            readBuffer += buf;
        }

        // // Get Request Message 처리
        // UartMessageInterface::UartMessageReceiver rcvReq(str);
        // rcvReq.processMessage();
    }
}

// void serialEvent()
// {
//     Serial.println(Serial.readString());
// }