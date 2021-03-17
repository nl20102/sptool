#pragma once

#include "Command.h"
#include <chrono>
#include <thread>

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class DelayCommand : public Command<Output, Error>
    {
    public:
        DelayCommand(const Context<Output, Error>* outputContext, int delay)
            : Command<Output, Error>(outputContext), delay(delay) {};
        ~DelayCommand() {};
        
        const int Delay() const { return delay; }

        void Execute() override
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        };
        
    private:
        const int delay;

    };
}