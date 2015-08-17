#ifndef h_ITDConfiguration
#define h_ITDConfiguration

#include <memory>

namespace rtd
{

class ITDConfigurationProvider
{
public:
    /*!
     * Retrieve a configuration parameter of string type by given key.
     *
     * \param [in] key - name of the configuration key.
     * \param [in] valueData - pointer to string that receives the value data.
     * \param [in, out] valueLength - specifies maximum length of the output string with trailing zero included (raw size)
     * \return returns true if key is found, false if given key does not exist also writes copied number of characters to valueLength, trailing zero not included.
     */
    virtual bool getStringParameter(const char* key, char* valueData, size_t& valueLength) = 0;

protected:
    virtual ~ITDConfigurationProvider() = default;
};

using ITDConfigurationProviderPtr = std::shared_ptr<ITDConfigurationProvider>;

}

#endif
