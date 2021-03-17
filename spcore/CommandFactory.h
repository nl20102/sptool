#pragma once

#include <memory>
#include "Context.h"
#include <string>
#include "SendCommand.h"
#include "AnswerCommand.h"
#include "DelayCommand.h"
#include "RepeatCommand.h"
#include "OpenPortCommand.h"
#include "ClosePortCommand.h"
#include "SerialPort.h"
#include <boost/container/small_vector.hpp>
#include <memory>

namespace sptcore
{
    class SerialPort;

    template <class Output, class Error = Output, class Port = SerialPort>
    class CommandFactory
    {
    public:
        CommandFactory(const Context<Output, Error>* context) : context(context) {};

        std::unique_ptr<sptcore::Command<Output, Error>> CreateOpenPortCommand(std::string port)
            { return std::make_unique<sptcore::OpenPortCommand<Output, Error>>(context, port); };

        std::unique_ptr<sptcore::Command<Output, Error>> CreateClosePortCommand()
            { return std::make_unique<sptcore::ClosePortCommand<Output, Error>>(context); };

        std::unique_ptr<sptcore::Command<Output, Error>> CreateSendCommand(std::string content)
            { return std::make_unique<sptcore::SendCommand<Output, Error>>(context, content); };

        std::unique_ptr<sptcore::Command<Output, Error>> CreateAnswerCommand(std::string content)
            { return std::make_unique<sptcore::AnswerCommand<Output, Error>>(context, content); };
            
        std::unique_ptr<sptcore::Command<Output, Error>> CreateDelayCommand(int delay)
            { return std::make_unique<sptcore::DelayCommand<Output, Error>>(context, delay); };

        std::unique_ptr<sptcore::Command<Output, Error>> CreateRepeatCommand(
            boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> &commands,
            int repetitionsCount, int delay)
            { return std::make_unique<sptcore::RepeatCommand<Output, Error>>(
                context, commands, repetitionsCount, delay); };

    private:
        const Context<Output, Error>* context;
    };
}