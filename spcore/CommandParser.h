#pragma once

#include "CommandFactory.h"
#include "Context.h"
#include "Command.h"
#include "SerialPort.h"
#include <boost/container/small_vector.hpp>
#include <stdexcept>
#include <memory>
#include <boost/program_options.hpp>
#include <string>
#include <boost/iterator/filter_iterator.hpp>

namespace opt = boost::program_options;

namespace sptcore
{
    struct is_eq_options {
        bool operator()(boost::program_options::option option) 
        {
            return option.string_key == "data" || option.string_key == "answer";
        }
    };
    
    template <class Output, class Error = Output, class Port = SerialPort>
    class CommandParser
    {
    public:
        CommandParser()
            : context(),
              factory(&context),
              desc("All options")
        {
            desc.add_options()
                ("help,h", "produce help message")
                ("data,d", opt::value<std::vector<std::string>>(),
                    "sends to port specified data block. Each sent block is supplemented with a \\n")
                ("answer,a", opt::value<std::vector<std::string>>(),
                    "waits for a response from the port and compares it with the specified argument, outputs the result of the comparsion. '*' in data argument means any data to waiting for.")
                ("port,p", opt::value<std::string>(),
                    "the port to which data blocks are sended")
                ("delay,w", opt::value<int>(),
                    "delay between sended data blocks in miliseconds")
                ("repeat,r", opt::value<int>(),
                    "forces the specified commands to be repeated the specified number of times");
        };

        virtual ~CommandParser(){};

        boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> parse(
            int argc, const char *argv[])
        {
            boost::container::small_vector<std::unique_ptr<Command<Output, Error>>, 10> result;

            opt::variables_map vm;
            auto parsedOptions = opt::parse_command_line(argc, argv, desc);
            opt::store(parsedOptions, vm);
            option_conflict(vm, "help", "port");
            option_conflict(vm, "help", "data");
            option_conflict(vm, "help", "answer");
            option_conflict(vm, "help", "delay");
            option_conflict(vm, "help", "repeat");

            option_dependency(vm, "data", "port");
            option_dependency(vm, "answer", "port");

            option_dependency(vm, "port", "data");
            option_dependency(vm, "delay", "data");
            option_dependency(vm, "repeat", "data");

            opt::notify(vm);

            if (vm.count("help"))
            {
                context.output << desc << std::endl;
                return result;
            }

            int delay = vm.count("delay") ? vm["delay"].as<int>() : 0;
            bool repetadly = vm.count("repeat");


            if (vm.count("data") || vm.count("answer"))
            {
                auto port = vm["port"].as<std::string>();
                if (!repetadly) result.push_back(factory.CreateOpenPortCommand(port));

                auto beginIterator = boost::make_filter_iterator<is_eq_options>(
                    parsedOptions.options.begin(),
                    parsedOptions.options.end());
                
                auto endIterator = boost::make_filter_iterator<is_eq_options>(
                    parsedOptions.options.end(),
                    parsedOptions.options.end());
                    
                bool isFirstCommand = true;
                for (; beginIterator != endIterator; ++beginIterator)
                {
                    auto dataString = *(beginIterator->value.begin());
                    bool isAnswer = beginIterator->string_key == "answer";
                    if (delay && !isFirstCommand && !isAnswer)
                        result.push_back(factory.CreateDelayCommand(delay));
                    auto command = isAnswer ? factory.CreateAnswerCommand(dataString) : factory.CreateSendCommand(dataString);
                    result.push_back(std::unique_ptr<Command<Output, Error>>(std::move(command)));
                    isFirstCommand = false;
                }

                if (repetadly)
                {
                    auto repeatCommand = factory.CreateRepeatCommand(result,
                        vm["repeat"].as<int>(), delay);
                    result.push_back(factory.CreateOpenPortCommand(port));
                    result.push_back(std::move(repeatCommand));
                }

                result.push_back(factory.CreateClosePortCommand());

            }


            return result;
        };

        void option_dependency(const opt::variables_map & vm,
            const std::string & for_what, const std::string & required_option)
        {
            if (vm.count(for_what) && !vm[for_what].defaulted())
                if (vm.count(required_option) == 0 || vm[required_option].defaulted())
                    throw std::logic_error(std::string("Option '") + for_what 
                        + "' requires option '" + required_option + "'.");
        }

        void option_conflict(const opt::variables_map & vm,
            const std::string & for_what, const std::string & conflicted_option)
        {
            if (vm.count(for_what) && !vm[for_what].defaulted())
                if (vm.count(conflicted_option) && !vm[conflicted_option].defaulted())
                    throw std::logic_error(std::string("Option '") + conflicted_option 
                        + "' conflicts with the option '" + for_what + "'.");
        }

        const Context<Output, Error> *GetContext() const { return &context; };

    private:
        const SPTContext<Port, Output, Error> context;
        CommandFactory<Output, Error, Port> factory;
        opt::options_description desc;
    };
} // namespace sptcore