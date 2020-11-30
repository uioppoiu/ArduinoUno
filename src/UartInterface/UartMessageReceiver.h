#ifndef _UART_MESSAGE_RECEIVER_H_
#define _UART_MESSAGE_RECEIVER_H_

#include <Arduino.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageReceiver
    {
    public:
        UartMessageReceiver(const char* msg, size_t msgSize);
        ~UartMessageReceiver();
        
        bool isMessageValid();
        void processMessage();

    private:
        char _message[128];
        UartMessageReceiver();
    };

}; // namespace UartMessageInterface

#endif
