#include <iostream>
#include <fstream>
#include <memory>
#include "DataRepository.hpp"
#include "StateTransitionLogHandler.hpp"
#include "RespMessageLogHandler.hpp"
#include "PlainMessageLogHandler.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }

    Shb::DataRepository l_dataRepository;

    std::vector<std::shared_ptr<Shb::ILogHandler>> l_logHandlers = {
        std::make_shared<Shb::StateTransitionLogHandler>(l_dataRepository),
        std::make_shared<Shb::RespMessageLogHandler>(l_dataRepository),
        std::make_shared<Shb::PlainMessageLogHandler>(l_dataRepository)
    };

    std::ifstream l_in(argv[1]);
    std::string l_line;

    int l_sum = 96;
    while (std::getline(l_in, l_line) && l_sum--)
    {
        // std::cout << l_line << "\n";
        for (auto& l_logHandler : l_logHandlers)
        {
            if (l_logHandler->handle(l_line))
            {
                std::swap(l_logHandler, l_logHandlers.front());
                break;
            }
        }
    }

    for (auto& l_logHandler : l_logHandlers)
    {
        l_logHandler->handle("");
    }

    // l_dataRepository.dump();
    l_dataRepository.dumpToJson();

    return 0;
}