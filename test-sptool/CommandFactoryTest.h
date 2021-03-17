#pragma once

#include <gmock/gmock.h>
#include "CommandFactory.h"
#include "Context.h"
#include <memory>
#include "MockOutput.h"
#include "MockSerialPort.h"

namespace test_sptool
{
    class CommandFactoryTest : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<MockSerialPort, MockOutput> context {};
        sptcore::CommandFactory<MockOutput, MockOutput, MockSerialPort> CommandFactory {&context};
    };
}