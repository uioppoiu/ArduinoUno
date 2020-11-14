#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include <Arduino.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    class UartMessageCallbackManagement
    {
    public:
        typedef void(*CallBackRequestGet)(eDataType, const std::string &);
        typedef void(*CallBackResponseGet)(eDataType, const std::string &, const Value &);
        typedef void(*CallBackSubscribe)(eDataType, const std::string &, unsigned int);
        typedef void(*CallBackUnsubscribe)(eDataType, const std::string &);

        static void registerRequestGetCallBack(const CallBackRequestGet func);
        static void invokeRequestGetCallBack(eDataType type, const std::string &name);
        static void registerResponseGetCallBack(const CallBackResponseGet func);
        static void invokeResponseGetCallBack(eDataType type, const std::string &name, const Value &value);
        static void registerSubscribeCallBack(const CallBackSubscribe func);
        static void invokeSubscribeCallBack(eDataType type, const std::string &name, unsigned int period);
        static void registerUnsubscribeCallBack(const CallBackUnsubscribe func);
        static void invokeUnsubscribeCallBack(eDataType type, const std::string &name);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();

        CallBackRequestGet _callBackRequestGet;
        CallBackResponseGet _callBackResponseGet;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
    };


}; // namespace UartMessageInterface

#endif
