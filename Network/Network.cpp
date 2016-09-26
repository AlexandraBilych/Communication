#include "Network.h"

#define MAX_MASK 0xFFFFFFFF
#define MAX_BYTE 255

#define NET10_0_0_0 0xA000000
#define NET10_255_255_255 0xAFFFFFF

#define NET172_16_0_0 0xAC100000
#define NET172_31_255_255 0xAC1FFFFF

#define NET192_168_0_0 0xC0A80000
#define NET192_168_255_255 0xC0A8FFFF


std::string Network::maskToString() const
{
    char str[15];
    uint32_t mask = MAX_MASK;

    mask <<= (32 - maskLength);
    sprintf(str,"%u.%u.%u.%u", (MAX_BYTE & mask>>24),(MAX_BYTE & (mask>>16)),(MAX_BYTE & (mask>>8)),(MAX_BYTE & mask));

    return str;
}


Network::Network(const IPv4Address& address, int maskLength)
{
    if ( maskLength < 0 || maskLength > 32 )
    {
        throw InvalidMaskLengthException();
    }

    this->address = address;
    this->maskLength = maskLength;
    string_mask = maskToString();
    string_network = address.toString() + "/" + std::to_string(maskLength);
}


bool Network::contains(const IPv4Address& address) const
{
    return address.greaterThan(this->address) && address.lessThan(getBroadcastAddress());
}


IPv4Address Network::getAddress() const
{
    return address;
}


IPv4Address Network::getBroadcastAddress() const
{
    uint32_t LastHost = (1 << (32 - maskLength)) - 1;
    IPv4Address temp = IPv4Address(address.toInt() + LastHost);

    return temp;
}


IPv4Address Network::getFirstUsableAddress() const
{
    IPv4Address temp = IPv4Address(address.toInt() + 1);
    return temp;
}


IPv4Address Network::getLastUsableAddress() const
{
    uint32_t LastHost = (1 << (32 - maskLength)) - 2;
    IPv4Address temp = IPv4Address(address.toInt() + LastHost);

    return temp;
}


long Network::getMask() const
{
    return maskLength;
}


std::string Network::getMaskString() const
{
    return string_mask;
}


int Network::getMaskLength() const
{
    return maskLength;
}


std::vector< Network > Network::getSubnets() const // produce two half-sized subnets
{
    uint32_t ipAddress = address.toInt();

    return {Network( IPv4Address(ipAddress), maskLength + 1),
            Network( IPv4Address(ipAddress + ( 1 << ( 31 - maskLength ) )), maskLength + 1)};
}


long Network::getTotalHosts() const // excluding network and broadcast
{
    return (1 << (32 - maskLength)) - 2;
}


bool Network::isPublic() const
{
    uint32_t ipAddress = address.toInt();


    if ( ipAddress >= NET10_0_0_0 && ipAddress <= NET10_255_255_255 )
    {
        return false;
    }

    if ( ipAddress >= NET172_16_0_0 && ipAddress <= NET172_31_255_255 )
    {
        return false;
    }

    if ( ipAddress >= NET192_168_0_0 && ipAddress <= NET192_168_255_255 )
    {
        return false;
    }

    return true;
}

bool Network::operator==(const Network& network) {
    return address == network.getAddress() && maskLength == network.getMaskLength();
}


std::string Network::toString() const
{
    return string_network;
}
