#include <iostream>
#include "CommandParser.h"

using namespace sptcore;

int main(int argc, char const* argv[])
{
    std::cout << "sptool 1.0" << std::endl;
    CommandParser<CMDOutput> parser;
    auto commands = parser.parse(argc, argv);
    for_each(commands.begin(), commands.end(), [](auto& commandIter) { commandIter->Execute(); });
    std::cout << "==" << std::endl;
}