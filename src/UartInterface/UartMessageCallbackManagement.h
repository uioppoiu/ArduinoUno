#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include <String>
#include <ctime>
#include <functional>
#include <map>
#include "tinyxml2/tinyxml2.h"
#include "UartMessageInterface.h"

using namespace tinyxml2;

namespace UartMessageInterface
{

    class UartMessageCallbackManagement
    {
    public:
        typedef std::function<void(eDataType, const String &)> CallBackRequestGet;
        typedef std::function<void(eDataType, const String &, const Value &)> CallBackResponseGet;
        typedef std::function<void(eDataType, const String &, uint32_t)> CallBackSubscribe;
        typedef std::function<void(eDataType, const String &)> CallBackUnsubscribe;

        static void registerRequestGetCallBack(eDataType type, const String &name, const CallBackRequestGet &func);
        static void invokeRequestGetCallBack(eDataType type, const String &name);
        static void registerResponseGetCallBack(eDataType type, const String &name, const CallBackResponseGet &func);
        static void invokeResponseGetCallBack(eDataType type, const String &name, const Value &value);
        static void registerSubscribeCallBack(CallBackSubscribe &func);
        static void invokeSubscribeCallBack(eDataType type, const String &name, uint32_t period);
        static void registerUnsubscribeCallBack(CallBackUnsubscribe &func);
        static void invokeUnsubscribeCallBack(eDataType type, const String &name);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();

        typedef std::map<std::tuple<eDataType, String>, CallBackRequestGet> CallBackListRequestGet;
        CallBackListRequestGet _callBackListRequestGet;
        typedef std::map<std::tuple<eDataType, String>, CallBackResponseGet> CallBackListResponseGet;
        CallBackListResponseGet _callBackListResponseGet;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
    };


}; // namespace UartMessageInterface

#endif
