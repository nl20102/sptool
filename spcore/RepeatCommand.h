#pragma once

#include "Command.h"
#include <memory>
#include <boost/container/small_vector.hpp>

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class RepeatCommand : public Command<Output, Error>
    {
    public:
        RepeatCommand(const Context<Output, Error>* outputContext,
            boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> &commands,
            int repetitionsCount, int delay)
            : Command<Output, Error>(outputContext), repetitionsCount(repetitionsCount),
            delay(delay)
        { 
            std::move(commands.begin(), commands.end(), std::back_inserter(this->commands)); 
            commands.clear();
        };
        ~RepeatCommand() {};

        const boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> &Commands() const
        { return commands; }

        const int RepetitionsCount() const { return repetitionsCount; }
        const int Delay() const { return delay; }
        
        void Execute() override {};
        
    private:
        boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> commands;
        const int repetitionsCount;
        const int delay;
    };
}