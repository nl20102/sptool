#pragma once

#include <gmock/gmock.h>
#include "SendCommand.h"
#include "Context.h"
#include "MockOutput.h"

namespace test_sptool
{
    class MockSerialPort;
    
    class SendCommandTest : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<MockSerialPort, MockOutput> context {};
        sptcore::SendCommand<MockOutput> Command { &context, "some data" };
        const MockSerialPort& port() const { return *dynamic_cast<const MockSerialPort*>(context.port()); }
    };
}