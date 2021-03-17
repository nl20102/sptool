#pragma once

#include <string>
#include "Command.h"
#include "SerialPort.h"

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class SendCommand : public Command<Output, Error>
    {
    public:
    
        SendCommand(const Context<Output, Error>* outputContext, std::string content)
            : Command<Output, Error>(outputContext), content(content) {};
        ~SendCommand() {};

        const std::string Content() const { return content; }

        void Execute() override
        {
            this->GetContext()->port()->write(content + "\n");
            this->GetContext()->output << "Sent successfully!" << std::endl;
        };
        
    private:
        const std::string content;
    };
}