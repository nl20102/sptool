#include <gmock/gmock.h>
#include <stdexcept>
#include <chrono>
#include <boost/asio.hpp>
#include "AnswerCommand.h"
#include "AnswerCommandTest.h"
#include "ClosePortCommand.h"
#include "CommandFactoryTest.h"
#include "CommandParser.h"
#include "CommandParserTest.h"
#include "Context.h"
#include "DelayCommand.h"
#include "DelayCommandTest.h"
#include "Integration_ReceivingDataTest.h"
#include "Integration_SendingData.h"
#include "MockSerialPort.h"
#include "OpenPortCommand.h"
#include "RepeatCommand.h"
#include "SendCommand.h"
#include "SendCommandTest.h"
#include "SPDelayedWrite.h"
#include "test-defintions.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define COUNT_ARGC(argv) sizeof((argv))/sizeof((argv)[0])

using ::testing::AtLeast;
using ::testing::InSequence;

namespace test_sptool
{
    using MockSendCommand = sptcore::SendCommand<MockOutput>;
    using MockOpenCommand = sptcore::OpenPortCommand<MockOutput>;
    using MockCloseCommand = sptcore::ClosePortCommand<MockOutput>;

    TEST_F(CommandFactoryTest, CreateSendCommand)
    {
        auto cmd = CommandFactory.CreateSendCommand("some text");
        auto sendCommand { dynamic_cast<MockSendCommand*>(cmd.release())};
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text");
    }

    TEST_F(CommandParserTest, ParseHelpCommand)
    {
        const char* argv[] = {"path", "--help"};
        auto* context = Parser.GetContext();
        ::testing::DefaultValue<const MockOutput&>::Set(context->output);
        EXPECT_CALL(context->output, OutputOperator).Times(2);
        auto commands = Parser.parse(COUNT_ARGC(argv), argv);
    }

    TEST_F(CommandParserTest, ParseOnlyHelpMustBeSpecified)
    {
        const char* argv[] = {"path", "--help", "--port", "COM5"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseDataRequiresPort)
    {
        const char* argv[] = {"path", "--data", "--delay"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseDataCommand)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 3);
        auto openCommand = dynamic_cast<MockOpenCommand*>(commands[0].get());
        EXPECT_STREQ(openCommand->port.c_str(), "COM5");
        auto sendCommand = dynamic_cast<MockSendCommand*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
        auto closeCommand = dynamic_cast<MockCloseCommand*>(commands[2].get());
    }

    TEST_F(CommandParserTest, ParseDataCommandSeveralPortions)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--data", "another text to send"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 4);
        auto openCommand = dynamic_cast<MockOpenCommand*>(commands[0].get());
        EXPECT_STREQ(openCommand->port.c_str(), "COM5");
        auto sendCommand = dynamic_cast<MockSendCommand*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
        auto sendCommand2 = dynamic_cast<MockSendCommand*>(commands[2].get());
        EXPECT_STREQ(sendCommand2->Content().c_str(), "another text to send");
        auto closeCommand = dynamic_cast<MockCloseCommand*>(commands[3].get());
    }

    TEST_F(CommandParserTest, ParseMultiplePortSpecifing)
    {
        const char* argv[] = {"path", "--port", "COM1", "--port", "COM12"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseDataWithAnswer)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--answer", "some answer", "--data", "some text again"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 5);
        auto openCommand = dynamic_cast<MockOpenCommand*>(commands[0].get());
        EXPECT_STREQ(openCommand->port.c_str(), "COM5");
        auto sendCommand = dynamic_cast<MockSendCommand*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
        auto answerCommand = dynamic_cast<sptcore::AnswerCommand<MockOutput>*>(commands[2].get());
        EXPECT_STREQ(answerCommand->Content().c_str(), "some answer");
        auto sendCommand2 = dynamic_cast<MockSendCommand*>(commands[3].get());
        EXPECT_STREQ(sendCommand2->Content().c_str(), "some text again");
        auto closeCommand = dynamic_cast<MockCloseCommand*>(commands[4].get());
   }

    TEST_F(CommandParserTest, ParseSendingWithDelay)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--data", "another text to send",
            "--delay", "500"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 5);
        auto openCommand = dynamic_cast<MockOpenCommand*>(commands[0].get());
        EXPECT_STREQ(openCommand->port.c_str(), "COM5");
        auto sendCommand = dynamic_cast<MockSendCommand*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
        auto delayCommand = dynamic_cast<sptcore::DelayCommand<MockOutput>*>(commands[2].get());
        EXPECT_EQ(delayCommand->Delay(), 500);
        auto closeCommand = dynamic_cast<MockCloseCommand*>(commands[4].get());
    }

    TEST_F(CommandParserTest, ParseSendingAndAswerGettingWithDelay)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--answer", "some answer",
            "--data", "another text to send",
            "--delay", "500"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 6);
        auto openCommand = dynamic_cast<MockOpenCommand*>(commands[0].get());
        EXPECT_STREQ(openCommand->port.c_str(), "COM5");
        auto sendCommand = dynamic_cast<MockSendCommand*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
        auto answerCommand = dynamic_cast<sptcore::AnswerCommand<MockOutput>*>(commands[2].get());
        EXPECT_STREQ(answerCommand->Content().c_str(), "some answer");
        auto delayCommand = dynamic_cast<sptcore::DelayCommand<MockOutput>*>(commands[3].get());
        EXPECT_EQ(delayCommand->Delay(), 500);
    }

    TEST_F(CommandParserTest, ParseSendingAndAswerGettingWithRepeat)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--answer", "some answer",
            "--data", "another text to send",
            "--repeat", "5"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 3);
        auto repeatCommand = dynamic_cast<sptcore::RepeatCommand<MockOutput>*>(commands[1].get());
        EXPECT_EQ(repeatCommand->Commands().size(), 3);
        EXPECT_EQ(repeatCommand->RepetitionsCount(), 5);
        EXPECT_EQ(repeatCommand->Delay(), 0);
    }

    TEST_F(CommandParserTest, ParseSendingAndAswerGettingWithRepeatWithDelay)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--answer", "some answer",
            "--data", "another text to send",
            "--repeat", "5",
            "--delay", "600"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 3);
        auto repeatCommand = dynamic_cast<sptcore::RepeatCommand<MockOutput>*>(commands[1].get());
        EXPECT_EQ(repeatCommand->Commands().size(), 4);
        EXPECT_EQ(repeatCommand->RepetitionsCount(), 5);
        EXPECT_EQ(repeatCommand->Delay(), 600);
    }

    TEST_F(CommandParserTest, ParseOnlyPortSpecified)
    {
        const char* argv[] = {"path", "--port", "COM1"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseOnlyDelaySpecified)
    {
        const char* argv[] = {"path", "--delay", "500"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseOnlyRepeatSpecified)
    {
        const char* argv[] = {"path", "--repeat", "5"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseOnlyAnswerSpecified)
    {
        const char* argv[] = {"path", "--answer"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(CommandParserTest, ParseOnlyAnswerAndPortSpecified)
    {
        const char* argv[] = {"path", "--answer", "--port", "COM2"};
        ASSERT_THROW(Parser.parse(COUNT_ARGC(argv), argv), std::logic_error);
    }

    TEST_F(SendCommandTest, Execution)
    {
        ::testing::DefaultValue<const MockOutput&>::Set(Command.GetContext()->output);
        {
            InSequence seq;
            EXPECT_CALL(port(), write).Times(1);
            EXPECT_CALL(context.output, OutputOperator("Sent successfully!")).Times(1);
            EXPECT_CALL(context.output, OutputOperator("\n")).Times(1);
        }
        Command.Execute();
    }

    TEST_F(AnswerCommandTest, Execution)
    {
        ::testing::DefaultValue<const MockOutput&>::Set(Command.GetContext()->output);
        {
            InSequence seq;
            
            EXPECT_CALL(context.output, OutputOperator("Waiting for response: "));
            EXPECT_CALL(context.output, OutputOperator("some data"));
            EXPECT_CALL(port(), read).Times(1).WillOnce(::testing::Return("some data\n"));
            EXPECT_CALL(context.output, OutputOperator(". OK.")).Times(1);
            EXPECT_CALL(context.output, OutputOperator("\n")).Times(1);
        }
        Command.Execute();
    }

    TEST_F(DelayCommandTest, Execution)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        Command.Execute();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        auto realDelay = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        EXPECT_GE(realDelay, 300);
        EXPECT_LE(realDelay, 320);
    }

    TEST_F(Integration_SendingData, SendSimpleText)
    {
        ::testing::DefaultValue<const MockOutput&>::Set(Command.GetContext()->output);
        boost::asio::io_service service;
        boost::asio::serial_port port {service, COM_PORT_TO_READ};
        boost::array<char, 128> buffer;
        Command.GetContext()->port()->open(COM_PORT_TO_WRITE);
        Command.Execute();
        Command.GetContext()->port()->close();
        auto dataSize = port.read_some(boost::asio::buffer(buffer));
        EXPECT_STREQ(std::string(buffer.data(), dataSize).c_str(), "some data\n");
    }

    TEST_F(Integration_ReceivingDataTest, ReceiveSimpleText)
    {
        auto* context = Command.GetContext();
        ::testing::DefaultValue<const MockOutput&>::Set(context->output);
        EXPECT_CALL(context->output, OutputOperator).Times(4);

        Command.GetContext()->port()->open(COM_PORT_TO_READ);

        SPDelayedWrite dw { COM_PORT_TO_WRITE, 10 };
        dw();

        Command.Execute();
        Command.GetContext()->port()->close();
    }

    
    TEST_F(CommandParserTest, ParseSendingAndReceiving)
    {
        const char* argv[] = {"path", "--port", "COM5", "--data", "some text to send",
            "--answer", "some answer",
            "--data", "another text to send",
            "--answer", "another some answer",
            "--delay", "600"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 7);
    }

    TEST_F(CommandParserTest, ParseCommandWithAbbreviatedArgument)
    {
        const char* argv[] = {"path", "--port", "COM5", "-d", "some text to send"};
        auto commands = Parser.parse(COUNT_ARGC(argv), (const char**)&argv);
        EXPECT_EQ(commands.size(), 3);
        auto sendCommand = dynamic_cast<sptcore::SendCommand<MockOutput>*>(commands[1].get());
        EXPECT_STREQ(sendCommand->Content().c_str(), "some text to send");
    }

    TEST(Integration_MemoryLeaks, HasMemoryLeaks)
    {
   		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

		_CrtMemState s1;
		_CrtMemCheckpoint(&s1);

        {
            SPDelayedWrite dw { COM_PORT_TO_READ, 50 };
            dw();

            const char* argv[] = {"path", "--port", COM_PORT_TO_WRITE, "--data", "some text to send",
                "--answer", "some answer",
                "--data", "another text to send",
                "--delay", "600"};

            sptcore::CommandParser<sptcore::CMDOutput> parser;
            auto commands = parser.parse(COUNT_ARGC(argv), argv);
            for_each(commands.begin(), commands.end(), [](auto& commandIter) { commandIter->Execute(); });
        }

   		_CrtMemState s2;
		_CrtMemCheckpoint(&s2);
		_CrtMemState s3;
		auto _hasMemoryLeaks = _CrtMemDifference(&s3, &s1, &s2);
		if (_hasMemoryLeaks)
		{
			_CrtMemDumpStatistics(&s3);
			FAIL() << "!!! Memory leaks detected !!!";
		}
    }

    TEST(Integration_MemoryLeaks, HasMemoryLeaksWithRepeat)
    {
   		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

		_CrtMemState s1;
		_CrtMemCheckpoint(&s1);

        {
            const char* argv[] = {"path", "--port", COM_PORT_TO_WRITE, "--data", "some text to send",
                "--data", "another text to send",
                "--delay", "600",
                "--repeat", "3"};

            sptcore::CommandParser<sptcore::CMDOutput> parser;
            auto commands = parser.parse(COUNT_ARGC(argv), argv);
            for_each(commands.begin(), commands.end(), [](auto& commandIter) { commandIter->Execute(); });
        }

   		_CrtMemState s2;
		_CrtMemCheckpoint(&s2);
		_CrtMemState s3;
		auto _hasMemoryLeaks = _CrtMemDifference(&s3, &s1, &s2);
		if (_hasMemoryLeaks)
		{
			_CrtMemDumpStatistics(&s3);
			FAIL() << "!!! Memory leaks detected !!!";
		}

    }
}