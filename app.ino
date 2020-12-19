#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestGet(uint32_t seqId, const UartMessageInterface::RequestGetData *dataArr, size_t arrSize)
{
    // Serial.println(__FUNCTION__);
    // for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    // {
    //     const UartMessageInterface::RequestGetData &data = dataArr[arrIdx];
    //     Serial.print("SeqId:");
    //     Serial.print(seqId);
    //     Serial.print(" Type:");
    //     Serial.println((uint32_t)data.type);
    // }

    static int v = 0;
    v++;
    UartMessageInterface::UartMessageSender rspGet(UartMessageInterface::MsgId::ResponseGet);
    rspGet.setSeqId(seqId++);
    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::RequestGetData &msg = dataArr[arrIdx];
        switch (msg.type)
        {
        case UartMessageInterface::DataType::SensorRoomTemperature:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorRoomTemperature, 2000 + v);
            break;
        case UartMessageInterface::DataType::SensorWaterTemperature:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorRoomTemperature, 2000 + v);
            break;
        case UartMessageInterface::DataType::SensorCO2:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorCO2, 3000 + v);
            break;
        case UartMessageInterface::DataType::SensorHumidity:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorHumidity, 4000 + v);
            break;
        case UartMessageInterface::DataType::SensorConductivity:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorConductivity, 5000 + v);
            break;
        case UartMessageInterface::DataType::SensorPH:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorPH, 6000 + v);
            break;
        default:
            break;
        }
    }

    delay(100);
    rspGet.sendMessage();
}

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData *dataArr, size_t arrSize)
{
    // static bool ledstate = false;
    // digitalWrite(LED_BUILTIN, ledstate);
    // ledstate = !ledstate;

    // Serial.println(__FUNCTION__);
    // for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    // {
    //     const UartMessageInterface::ResponseGetData& data = dataArr[arrIdx];
    //     Serial.print("SeqId:");
    //     Serial.print(seqId);
    //     Serial.print(" Type:");
    //     Serial.print((uint32_t)data.type);
    //     Serial.print(" Value:");
    //     Serial.println(data.value);
    // }
}

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onResponseGet(seqId, dataArr, arrSize);
}

void onRequestSet(uint32_t seqId, const UartMessageInterface::RequestSetData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::RequestSetData &data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Value:");
        Serial.println(data.value);
    }
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
    UartMessageInterface::UartMessageCallbackManagement::registerRequestSetCallBack(onRequestSet);
}

void sendTestMessage()
{
    static int v = 0;
    UartMessageInterface::UartMessageSender notiMsg(UartMessageInterface::MsgId::Notification);
    notiMsg.setSeqId(v++);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorWaterTemperature, 1 + v);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorRoomTemperature, 2 + v);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorCO2, 3 + v);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorHumidity, 4 + v);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorConductivity, 5 + v);
    notiMsg.appendNotificationData(UartMessageInterface::DataType::SensorPH, 6 + v);
    notiMsg.sendMessage();
}

void defaultAction()
{
    if (Serial.available() > 0) serialEventHandler();
}

int sequence = 0;
void loop()
{
    defaultAction();
    const int currentSequence = sequence;
    sequence++;
    sequence = sequence % 3000;

    delay(1);

    if (currentSequence == 1)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
        return;
    }

    if (currentSequence == 401)
    {
        digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
        return;
    }

    if(currentSequence == 999)
    {
        sendTestMessage();
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
            // Serial.println("Begin FOUND!!");

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
            // Serial.println("End FOUND!!");

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