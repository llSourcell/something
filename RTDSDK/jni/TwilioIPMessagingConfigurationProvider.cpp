//
//  TwilioIPMessagingConfigurationProvider.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include "TwilioIPMessagingConfigurationProvider.h"
//#include <iostream>

using namespace rtd;

TwilioIPMessagingConfigurationProvider::TwilioIPMessagingConfigurationProvider(std::map<std::string, std::string> configMap) :
    _configMap(configMap)
{

}


bool TwilioIPMessagingConfigurationProvider::getStringParameter(const char* key, char* valueData, size_t& valueLength) {
    std::map<std::string,std::string>::const_iterator param = _configMap.find(std::string(key));
    if (param == _configMap.end()) {
        return false;
    }

    fillParameter(valueData, valueLength, param->second.c_str());
    return true;
}

void TwilioIPMessagingConfigurationProvider::fillParameter(char* valueData, size_t& valueLength, const char* source) {
    strncpy(valueData, source, valueLength - 1);
    valueData[valueLength - 1] = 0;
    valueLength = strlen(valueData);
}
