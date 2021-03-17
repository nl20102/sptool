#pragma once

#include "Command.h"
#include <boost\algorithm\string\predicate.hpp>

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class AnswerCommand : public Command<Output, Error>
    {
    public:
        AnswerCommand(const Context<Output, Error>* outputContext, std::string content)
            : Command<Output, Error>(outputContext),
            content(content)
            {};
        ~AnswerCommand() {};
        
        const std::string Content() const { return content; }

        void Execute() override
        {
            this->GetContext()->output << "Waiting for response: " << content;
            std::string result;
            while (!boost::algorithm::ends_with(result, "\n"))
                result += this->GetContext()->port()->read();
            result.pop_back(); // remove \n in the end of string
            if (content == "*" || content == result)
                this->GetContext()->output << ". OK." << std::endl;
            else
                this->GetContext()->output << ". ERROR, received: " << result << std::endl;
        };
        
    private:
        const std::string content;
    };
}