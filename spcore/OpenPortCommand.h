#pragma once

#include "Command.h"
#include <string>

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class OpenPortCommand : public Command<Output, Error>
    {
    public:
        OpenPortCommand(const Context<Output, Error>* outputContext, std::string port) :
            Command<Output, Error>(outputContext),
            port(port) {}

        void Execute() override
        {
            this->GetContext()->port()->open(port);
        }
        
        const std::string port;

    };
}