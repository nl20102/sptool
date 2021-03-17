#pragma once

#include <gmock/gmock.h>
#include "Context.h"
#include "MockOutput.h"
#include "SerialPort.h"
#include "test-defintions.h"
#include "SendCommand.h"

namespace test_sptool
{
    class Integration_SendingData : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<sptcore::SerialPort, MockOutput> context {};
        sptcore::SendCommand<MockOutput> Command { &context, "some data" };

    };
}