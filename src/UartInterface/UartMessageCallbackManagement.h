#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include "ArduinoSTL.h"
#include "system_configuration.h"
#include "unwind-cxx.h"
#include "functional"
#include "map"

#include "tinyxml2/tinyxml2.h"
#include "UartMessageInterface.h"

using namespace tinyxml2;

namespace UartMessageInterface
{

    class UartMessageCallbackManagement
    {
    public:
        typedef void(*CallBackRequestGet)(eDataType, const std::string &);
        typedef void(*CallBackResponseGet)(eDataType, const std::string &, const Value &);
        typedef void(*CallBackSubscribe)(eDataType, const std::string &, unsigned int);
        typedef void(*CallBackUnsubscribe)(eDataType, const std::string &);

        static void registerRequestGetCallBack(eDataType type, const std::string &name, const CallBackRequestGet func);
        static void invokeRequestGetCallBack(eDataType type, const std::string &name);
        static void registerResponseGetCallBack(eDataType type, const std::string &name, const CallBackResponseGet func);
        static void invokeResponseGetCallBack(eDataType type, const std::string &name, const Value &value);
        static void registerSubscribeCallBack(CallBackSubscribe &func);
        static void invokeSubscribeCallBack(eDataType type, const std::string &name, unsigned int period);
        static void registerUnsubscribeCallBack(CallBackUnsubscribe func);
        static void invokeUnsubscribeCallBack(eDataType type, const std::string &name);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();

        typedef std::map<eDataType, CallBackRequestGet> CallBackListRequestGet;
        CallBackListRequestGet _callBackListRequestGet;
        typedef std::map<eDataType, CallBackResponseGet> CallBackListResponseGet;
        CallBackListResponseGet _callBackListResponseGet;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
    };


}; // namespace UartMessageInterface

#endif
