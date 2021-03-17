#pragma once

#include <gmock/gmock.h>
#include "Context.h"
#include "DelayCommand.h"
#include "MockOutput.h"

namespace test_sptool
{
    class DelayCommandTest : public ::testing::Test
    {
    protected:
        sptcore::SPTContext<MockSerialPort, MockOutput> context {};
        sptcore::DelayCommand<MockOutput> Command { &context, 300 };
    };
}