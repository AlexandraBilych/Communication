#include "IPv4Address.h"


std::string IPv4Address::intToString(uint32_t address) const
{
    char str[15];
    uint32_t mask = 255;

    sprintf(str,"%u.%u.%u.%u", (mask & (address >> 24)),(mask & (address >> 16)),(mask & (address >> 8)),(mask & address));

    return str;
}


uint32_t IPv4Address::stringToInt(const std::string& address) const
{
    uint32_t int_address = 0;
    std::stringstream ss;
    std::string item;
    auto numberOfBytes = 1;

    ss.str(address);

    for ( uint32_t buffer; getline(ss, item, '.'); )
    {
        buffer = std::stoi(item, nullptr);
        int_address |= (buffer << (4 - numberOfBytes) * 8);

        if ( buffer > 255 || numberOfBytes > 4 ) {
            throw IllegalArgumentException();
        }

        numberOfBytes += 1;
    }

    return int_address;
}


IPv4Address::IPv4Address(const std::string& address)
{
    auto length = address.length();

    if ( length > 15 || length < 7 )
    {
        throw IllegalArgumentException();
    }

    uint32_ipAdress = stringToInt(address);
    string_ipAdress = address;
}


IPv4Address::IPv4Address(long address)
{
    if ( address < 0 || address > 4294967295 )
    {
        throw IllegalArgumentException();
    }

    uint32_ipAdress = address;
    string_ipAdress = intToString(address);
}


bool IPv4Address::lessThan(const IPv4Address& address) const
{
    return uint32_ipAdress < address.uint32_ipAdress;
}


bool IPv4Address::greaterThan(const IPv4Address& address) const
{
    return uint32_ipAdress > address.uint32_ipAdress;
}


bool IPv4Address::equals(const IPv4Address& address) const
{
    return uint32_ipAdress == address.uint32_ipAdress;
}


std::string IPv4Address::toString() const
{
    return string_ipAdress;
}

uint32_t IPv4Address::toInt() const
{
    return uint32_ipAdress;
}


std::ostream& operator<<(std::ostream& out, const IPv4Address& ip) {
    out << ip.toInt();
    return out;
}
