//  TwilioIPMessagingConfigurationProvider.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef TwilioIPMessagingConfigurationProvider_cpp
#define TwilioIPMessagingConfigurationProvider_cpp

#include <Common/ITDConfiguration.h>
#include <map>
#include <string>

using namespace rtd;

class TwilioIPMessagingConfigurationProvider : public ITDConfigurationProvider
{
public:
   TwilioIPMessagingConfigurationProvider(std::map<std::string, std::string> configMap);
   virtual bool getStringParameter(const char* key, char* valueData, size_t& valueLength) override;

private:
    std::map<std::string, std::string> _configMap; //<key, value>
    void fillParameter(char* valueData, size_t& valueLength, const char* source);
};
#endif /* TwilioIPMessagingConfigurationProvider_cpp */
