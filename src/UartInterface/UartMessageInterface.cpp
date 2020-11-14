#include <time.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    eDataType str2EnumDataType(const char *input)
    {
        String str(input);
        if (str == String("SensorAll"))
            return SensorAll;
        if (str == String("SensorTemperature"))
            return SensorTemperature;
        if (str == String("SensorCO2"))
            return SensorCO2;
        if (str == String("SensorHumidity"))
            return SensorHumidity;
        if (str == String("SensorConductivity"))
            return SensorConductivity;
        if (str == String("ControlAll"))
            return ControlAll;
        if (str == String("Control0"))
            return Control0;
        if (str == String("Control1"))
            return Control1;
        if (str == String("Control2"))
            return Control2;
        if (str == String("DateTime"))
            return DateTime;
        return Invalid;
    }

    String enum2Str(eDataType input)
    {
        switch (input)
        {
        case SensorAll:
            return "SensorAll";
        case SensorTemperature:
            return "SensorTemperature";
        case SensorCO2:
            return "SensorCO2";
        case SensorHumidity:
            return "SensorHumidity";
        case SensorConductivity:
            return "SensorConductivity";
        case ControlAll:
            return "ControlAll";
        case Control0:
            return "Control0";
        case Control1:
            return "Control1";
        case Control2:
            return "Control2";
        case DateTime:
            return "DateTime";
        case Invalid:
        default:
            return "-";
        }
    }

    String getCurrentTime()
    {
        char sResult[32] = {
            0,
        };
        time_t result = time(nullptr);
        strftime(sResult, sizeof(sResult), "%FT%T", localtime(&result));

        // cout << sResult << endl;
        return sResult;
    }

    uint8_t getCheckSum(const String &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        uint8_t checkSum = (sum ^ 0xFF) + 1;

        // cout << "CheckSum : " << (int)checkSum << endl;
        return checkSum;
    }

    void appendCheckSum(String &message)
    {
        message += ((char)getCheckSum(message));
    }

    bool verityCheckSum(const String &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        // cout << "Verity : " << (sum == 0) << endl;
        return (sum == 0);
    }

}; // namespace UartMessageInterface