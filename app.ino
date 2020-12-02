#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"


void onRequestGet(uint32_t seqId, const UartMessageInterface::RequestGetData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    {
        const UartMessageInterface::RequestGetData& data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.println(data.name);
    }
}

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData* dataArr, size_t arrSize)
{
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

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onNotification(seqId, dataArr, arrSize);
}

void onSubscribe(uint32_t seqId, const UartMessageInterface::SubscribeData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onUnsubscribe(uint32_t seqId, const UartMessageInterface::UnsubscribeData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onRequestSet(uint32_t seqId, const UartMessageInterface::RequestSetData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onResponseGet(seqId, dataArr, arrSize);
}

void onAcknowledge(uint32_t seqId, unsigned char msgId)
{
    Serial.println(__FUNCTION__);
    Serial.print("SeqId:");
    Serial.print(seqId);
    Serial.print("MsgId:");
    Serial.println((uint32_t)msgId);
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial OK...");
    Serial.flush();

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // Example
    // Callback 등록
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestGet);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseGet);
    UartMessageInterface::UartMessageCallbackManagement::registerRequestSetCallBack(onRequestSet);
    UartMessageInterface::UartMessageCallbackManagement::registerNotificationCallBack(onNotification);
    UartMessageInterface::UartMessageCallbackManagement::registerSubscribeCallBack(onSubscribe);
    UartMessageInterface::UartMessageCallbackManagement::registerUnsubscribeCallBack(onUnsubscribe);
    UartMessageInterface::UartMessageCallbackManagement::registerAcknowledgeCallBack(onAcknowledge);
}

char readBuffer[1024] = {0,};
size_t readBufferIdx = 0;

// the loop function runs over and over again forever
void loop()
{
    // digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    // delay(500);                     // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    // delay(500);                     // wait for a second

    while (Serial.available() > 0)
    {
        int c = Serial.read();
        readBuffer[readBufferIdx] = c;

        // Serial.print("Char:");
        // Serial.println(readBuffer[readBufferIdx]);
        // Serial.print("Raw:");
        // Serial.println((int)readBuffer[readBufferIdx]);
        Serial.println((unsigned int)readBuffer[readBufferIdx], 10);
        // Serial.print(" ");
        // Serial.println((char)readBuffer[readBufferIdx]);
        readBufferIdx++;

        // bool isBegin = false;
        // if(readBufferIdx >= 7)
        // {
        //     if (memcmp(
        //             readBuffer + readBufferIdx - 7,
        //             "<BEGIN>",
        //             7) == 0)
        //     {
        //         isBegin = true;
        //     }
        // }

        // if (isBegin)
        // {
        //     Serial.println("BEGIN FOUND!!");
        //     memset(readBuffer, 0x00, sizeof(readBuffer));
        //     readBufferIdx = 0;

        //     continue;
        // }

        // bool isEnd = false;
        // if(readBufferIdx >= (sizeof("<END>") - 1))
        // {
        //     if (memcmp(
        //             (readBuffer + readBufferIdx) - (sizeof("<END>") - 1),
        //             "<END>",
        //             (sizeof("<END>") - 1)) == 0)
        //     {
        //         isEnd = true;
        //     }
        // }

        // if(isEnd)
        // {
        //     readBufferIdx = readBufferIdx - (sizeof("<END>") - 1);
        //     UartMessageInterface::UartMessageReceiver rcv(readBuffer, readBufferIdx);
        //     rcv.processMessage();

        //     memset(readBuffer, 0x00, sizeof(readBuffer));
        //     readBufferIdx = 0;

        //     continue;
        // }

        if(readBufferIdx == 1024)
        {
            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
        }
    }

    Serial.flush();
}