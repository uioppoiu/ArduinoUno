#include "UartMessageReceiver.h"

namespace UartMessageInterface
{
    UartMessageReceiver::UartMessageReceiver(const String &message)
        : _jsonDoc(512)
    {
        if (isUartMessage(message) == false)
        {
            // throw invalid_argument("This message is not Uart message");
            return;
        }

        if (verityCheckSum(message) == false)
        {
            // throw invalid_argument("CheckSum error");
            return;
        }

        deserializeJson(_jsonDoc, message.c_str(), message.length() -1);
    }

    bool UartMessageReceiver::isMessageValid()
    {
        return true; // TODO 내용 검사
    }

    bool UartMessageReceiver::isUartMessage(const String &message)
    {
        return true; // TODO
    }

    void UartMessageReceiver::processMessage()
    {
        JsonObjectConst root = _jsonDoc.to<JsonObject>();

        if(!root.containsKey("MessageType")) return;
        String sMessageType = root["MessageType"];

        if (sMessageType == String("Request"))
        {
            handleRequestMessage(root);
        }
        else if ((sMessageType == String("Response")) ||
                 (sMessageType == String("Notification")))
        {
            handleResponseMessage(root);
        }
    }

    void UartMessageReceiver::handleRequestMessage(JsonObjectConst root)
    {
        // cout << "REQUEST Message arrived" << endl;

        if (!root.containsKey("CommandType")) return;
        String sCommandType = root["CommandType"];

        if (!root.containsKey("Data")) return;
        JsonArrayConst datas = root["Data"];

        if (sCommandType == String("Get"))
        {
            for(JsonArrayConst::iterator iter = datas.begin() ; iter != datas.end() ; ++iter)
            {
                invokeRequestGet(iter->as<JsonObjectConst>());
            }
        }
        else if (sCommandType == String("Subscribe"))
        {
            for(JsonArrayConst::iterator iter = datas.begin() ; iter != datas.end() ; ++iter)
            {
                invokeSubscribe(*iter);
            }
        }
        else if (sCommandType == String("Unsubscribe"))
        {
            for(JsonArrayConst::iterator iter = datas.begin() ; iter != datas.end() ; ++iter)
            {
                invokeUnsubscribe(*iter);
            }
        }
    }

    void UartMessageReceiver::invokeRequestGet(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        String name = data["Name"];
        UartMessageCallbackManagement::invokeRequestGetCallBack(dataType, name);
    }

    void UartMessageReceiver::invokeSubscribe(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        String name = data["Name"];
        unsigned int period = data["Period"];
        UartMessageCallbackManagement::invokeSubscribeCallBack(dataType, name, period);
    }

    void UartMessageReceiver::invokeUnsubscribe(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        String name = data["Name"];
        UartMessageCallbackManagement::invokeUnsubscribeCallBack(dataType, name);
    }

    void UartMessageReceiver::handleResponseMessage(JsonObjectConst root)
    {
        // cout << "RESPONSE/NOTIFICATION Message arrived" << endl;

        if (!root.containsKey("CommandType")) return;
        String sCommandType = root["CommandType"];

        if (!root.containsKey("Data")) return;
        JsonArrayConst datas = root["Data"];

        if (sCommandType == String("Get"))
        {
            for(JsonArrayConst::iterator iter = datas.begin() ; iter != datas.end() ; ++iter)
            {
                invokeResponseGet(*iter);
            }
        }
    }

    void UartMessageReceiver::invokeResponseGet(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        String name = data["Name"];
        String sValueType = data["ValueType"];
        eValueType valueType = (sValueType == String("Float")) ? Float : Integer;

        Value v;
        if(valueType == Float)
        {
            Value v;
            v.type = Float;
            v.value.val_float = data["Value"];
        }
        else
        {
            v.type = Integer;
            v.value.val_int = data["Value"];
        }

        UartMessageCallbackManagement::invokeResponseGetCallBack(dataType, name, v);
    }

}; // namespace UartMessageInterface