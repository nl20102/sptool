#pragma once

namespace sptcore
{
    template <class Output, class Error>
    struct Context;

    template <class Output, class Error = Output>
    class Command
    {
    public:
        virtual void Execute() = 0;
        const Context<Output, Error>* GetContext() { return outputContext; }
    protected:
        Command(const Context<Output, Error>* outputContext) : outputContext(outputContext){};

    private:
        const Context<Output, Error>* outputContext;
    };
}