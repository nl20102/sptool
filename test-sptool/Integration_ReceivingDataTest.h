#pragma once

#include <gmock/gmock.h>
#include "Context.h"
#include "AnswerCommand.h"
#include "MockOutput.h"
#include "SerialPort.h"

namespace test_sptool
{
    class Integration_ReceivingDataTest : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<sptcore::SerialPort, MockOutput> context {};
        sptcore::AnswerCommand<MockOutput> Command { &context, "some data" };
        const sptcore::SerialPort& port() const { return *dynamic_cast<const sptcore::SerialPort*>(context.port()); }
    };
}