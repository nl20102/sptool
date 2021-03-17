#pragma once

#include "SerialPort.h"
#include <string>
#include <gmock/gmock.h>

namespace test_sptool
{
    class MockSerialPort : public sptcore::BasicSerialPort
    {
    public:
        MOCK_METHOD(void, open, (const std::string &portName), (const, override));
        MOCK_METHOD(void, write, (const std::string_view data), (const, override));
        MOCK_METHOD(std::string, read, (), (const, override));
        MOCK_METHOD(void, close, (), (const, override));
    };
}