#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestGet(uint32_t seqId, const UartMessageInterface::RequestGetData *dataArr, size_t arrSize)
{
    // Serial.println(__FUNCTION__);
    // for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    // {
    //     const UartMessageInterface::RequestGetData& data = dataArr[arrIdx];
    //     Serial.print("SeqId:");
    //     Serial.print(seqId);
    //     Serial.print(" Type:");
    //     Serial.print((uint32_t)data.type);
    //     Serial.print(" Name:");
    //     Serial.println(data.name);
    // }
}

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData *dataArr, size_t arrSize)
{
    static bool ledstate = false;
    digitalWrite(LED_BUILTIN, ledstate);
    ledstate = !ledstate;

    Serial.println(__FUNCTION__);
    for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    {
        const UartMessageInterface::ResponseGetData& data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.print(data.name);
        Serial.print(" Value:");
        Serial.println(data.value);
    }
}

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onResponseGet(seqId, dataArr, arrSize);
}


uint8_t readBuffer[256] = {0,};
size_t readBufferIdx = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");
    Serial.flush();

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Example
    // Callback 등록
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestGet);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseGet);
    UartMessageInterface::UartMessageCallbackManagement::registerNotificationCallBack(onNotification);
}

int sequence = 0;

void defaultAction()
{
    if (Serial.available() > 0) serialEventHandler();
}

void loop()
{
    defaultAction();
    const int currentSequence = sequence;
    sequence++;
    sequence = sequence % 10;

    delay(200);

    if (currentSequence == 0)
    {
        static uint32_t seqId = 10000;
        UartMessageInterface::UartMessageSender rspGet(UartMessageInterface::MsgId::RequestGet);
        rspGet.setSeqId(seqId++);
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorPH, "ph", sizeof("ph"));
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorHumidity, "humidity", sizeof("humidityT"));
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorCO2, "co2", sizeof("co2"));
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorTemperature, "WATER", sizeof("WATER"));
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorTemperature, "ROOM", sizeof("ROOM"));
        rspGet.appendRequestGetData(UartMessageInterface::DataType::SensorConductivity, "conducti", sizeof("conducti"));
        rspGet.sendMessage();
        return;
    }

    if (currentSequence == 1)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
        return;
    }

    if (currentSequence == 6)
    {
        digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
        return;
    }
}

void serialEventHandler()
{
    while (Serial.available() > 0)
    {
        readBuffer[readBufferIdx++] = (uint8_t)Serial.read();

        bool isBegin = false;
        if (readBufferIdx >= 7)
        {
            if (memcmp(readBuffer + readBufferIdx - 7, "<BEGIN>", 7) == 0)
            {
                isBegin = true;
            }
        }

        if (isBegin)
        {
            Serial.println("Begin FOUND!!");

            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
            continue;
        }

        bool isEnd = false;
        if (readBufferIdx >= 5)
        {
            if (memcmp(readBuffer + readBufferIdx - 5, "<END>", 5) == 0)
            {
                isEnd = true;
            }
        }

        if (isEnd)
        {
            Serial.println("End FOUND!!");

            readBufferIdx = readBufferIdx - 5;
            UartMessageInterface::UartMessageReceiver rcv(readBuffer, readBufferIdx);
            rcv.processMessage();

            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;

            continue;
        }

        if (readBufferIdx == 256)
        {
            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
        }
    }
}