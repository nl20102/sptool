#pragma once

#include <gmock/gmock.h>
#include "AnswerCommand.h"
#include "Context.h"
#include "MockOutput.h"
#include "MockSerialPort.h"

namespace test_sptool
{
    class AnswerCommandTest : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<MockSerialPort, MockOutput> context {};
        sptcore::AnswerCommand<MockOutput> Command { &context, "some data" };
        const MockSerialPort& port() const { return *dynamic_cast<const MockSerialPort*>(context.port()); }
    };
}