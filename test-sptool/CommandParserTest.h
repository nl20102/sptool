#pragma once

#include <gmock/gmock.h>
#include "CommandParser.h"
#include "Context.h"
#include "MockOutput.h"
#include "MockSerialPort.h"
#include <memory>

namespace test_sptool
{
    class CommandParserTest : public ::testing::Test
    {
    protected:
        sptcore::CommandParser<MockOutput, MockOutput, MockSerialPort> Parser {};
    };
}