#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    UartMessageCallbackManagement *UartMessageCallbackManagement::_Instance = NULL;

    UartMessageCallbackManagement &UartMessageCallbackManagement::getInstance()
    {
        if (_Instance == NULL)
        {
            _Instance = new UartMessageCallbackManagement;
        }
        return *_Instance;
    }

    void UartMessageCallbackManagement::registerRequestGetCallBack(eDataType dataType, const std::string &, const CallBackRequestGet func)
    {
        getInstance()._callBackListRequestGet.insert(make_pair(dataType, func));
    }

    void UartMessageCallbackManagement::invokeRequestGetCallBack(eDataType dataType, const std::string &name)
    {
        CallBackListRequestGet::iterator iter =
            getInstance()._callBackListRequestGet.find(dataType);

        if (iter == getInstance()._callBackListRequestGet.end())
            return;

        iter->second(dataType, name);
    }

    void UartMessageCallbackManagement::registerResponseGetCallBack(eDataType dataType, const std::string &, const CallBackResponseGet func)
    {
        getInstance()._callBackListResponseGet.insert(make_pair(dataType, func));
    }

    void UartMessageCallbackManagement::invokeResponseGetCallBack(eDataType dataType, const std::string &name, const Value &value)
    {
        CallBackListResponseGet::iterator iter =
            getInstance()._callBackListResponseGet.find(dataType);

        if (iter == getInstance()._callBackListResponseGet.end())
            return;

        iter->second(dataType, name, value);
    }

    void UartMessageCallbackManagement::registerSubscribeCallBack(CallBackSubscribe &func)
    {
        getInstance()._callBackSubscribe = func;
    }

    void UartMessageCallbackManagement::invokeSubscribeCallBack(eDataType dataType, const std::string &name, unsigned int period)
    {
        getInstance()._callBackSubscribe(dataType, name, period);
    }

    void UartMessageCallbackManagement::registerUnsubscribeCallBack(CallBackUnsubscribe func)
    {
        getInstance()._callBackUnsubscribe = func;
    }

    void UartMessageCallbackManagement::invokeUnsubscribeCallBack(eDataType dataType, const std::string &name)
    {
        getInstance()._callBackUnsubscribe(dataType, name);
    }

}; // namespace UartMessageInterface