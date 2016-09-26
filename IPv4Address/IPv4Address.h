#ifndef IPV4_H
#define IPV4_H

#include <string>
#include <stdint.h>
#include <sstream>

class IllegalArgumentException {};

class IPv4Address {
    private:
        uint32_t uint32_ipAdress;
        std::string string_ipAdress;

        std::string intToString(uint32_t ipAdress) const;
        uint32_t stringToInt(const std::string& address) const;

    public:
        IPv4Address() {};
        IPv4Address(const std::string& address);
        IPv4Address(long address);

        bool lessThan(const IPv4Address& address) const;
        bool greaterThan(const IPv4Address& address) const;
        bool equals(const IPv4Address& address) const;
        bool operator==(const IPv4Address& address);

        std::string toString() const;
        uint32_t toInt() const;
};

std::ostream& operator<<(std::ostream& out, const IPv4Address& ip);

#endif
