#pragma once

#include "Command.h"
#include "Context.h"

namespace sptcore
{
    template <class Output, class Error = Output>
    class ClosePortCommand : public Command<Output, Error>
    {
    public:
        ClosePortCommand(const Context<Output, Error>* outputContext) :
            Command<Output, Error>(outputContext) {}

        void Execute() override
        {
            this->GetContext()->port()->close();
        }

    };
}