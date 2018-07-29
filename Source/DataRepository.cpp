#include "DataRepository.hpp"
#include <iostream>
#include <fstream>

namespace Shb
{
void DataRepository::insertFsmName(UeContextId p_ueContextId, std::string p_name)
{
    m_ueContextMapper[p_ueContextId].fsmNames.emplace_back(std::move(p_name));
}

void DataRepository::insertStateName(UeContextId p_ueContextId, std::string p_state)
{
    if (p_state != "Idle")
    {
        m_ueContextMapper[p_ueContextId].states.emplace_back(std::move(p_state));
    }
}

void DataRepository::insertMessage(UeContextId p_ueContextId, std::string p_prefix, Message p_message)
{
    m_ueContextMapper[p_ueContextId].messages[p_prefix].emplace_back(std::move(p_message));
}

void DataRepository::insertMessage(Crnti p_crnti, std::string p_prefix, Message p_message)
{
    m_crnriMapper[p_crnti].messages[p_prefix].emplace_back(std::move(p_message));
}

void DataRepository::dumpToJson() const
{
    std::ofstream l_jsonFile("output.json");
    l_jsonFile << "{" << std::endl;
    l_jsonFile << "\"\tUeList\": {" << std::endl;
    l_jsonFile << "\t\t\"UeContextIdList\": [" << std::endl;

    for (auto& l_ueContextPair : m_ueContextMapper)
    {
        l_jsonFile << "\"UeContextId\": " << l_ueContextPair.first << "," << "\n";
    }

    l_jsonFile << "\t\t]," << std::endl;

    //---

    l_jsonFile << "\t\t\"CrntiList\": [" << std::endl;

    l_jsonFile << "\t\t]" << std::endl;
    l_jsonFile << "\t}" << std::endl;
    l_jsonFile << "}" << std::endl;
    l_jsonFile.close();
}

void DataRepository::dump()
{
    std::cout << "UeContextId\n";
    for (auto& l_ueContextPair : m_ueContextMapper)
    {
        std::cout << "UeContextId: " << l_ueContextPair.first << "\n";
        std::cout << "Fsm Names\n";
        for (auto& l_fsmName : l_ueContextPair.second.fsmNames)
        {
            std::cout << "  " << l_fsmName << "\n";
        }

        std::cout << "states\n";
        for (auto& l_state : l_ueContextPair.second.states)
        {
            std::cout << "  " << l_state << "\n";
        }

        std::cout << "message\n";
        for (auto& l_msgPair : l_ueContextPair.second.messages)
        {
            std::cout << l_msgPair.first << "\n";
            for (auto& l_msg : l_msgPair.second)
            {
                std::cout << l_msg.name << "\n";
                if (l_msg.result)
                {
                    std::cout << "status: " << l_msg.result->status
                        << "cause: " << l_msg.result->cause
                        << "specificCause: " << l_msg.result->specificCause << std::endl;
                }
            }
        }
    }

    std::cout << "------------------------------------\n";
    std::cout << "CRNTI\n";
    for (auto& l_crntiPair : m_crnriMapper)
    {
        std::cout << "CRNTI: " << l_crntiPair.first << "\n";

        std::cout << "messages\n";
        for (auto& l_msgPair : l_crntiPair.second.messages)
        {
            std::cout << l_msgPair.first << "\n";
            for (auto& l_msg : l_msgPair.second)
            {
                std::cout << l_msg.name << "\n";
                if (l_msg.result)
                {
                    std::cout << "status: " << l_msg.result->status
                        << "cause: " << l_msg.result->cause
                        << "specificCause: " << l_msg.result->specificCause << std::endl;
                }
            }
        }
    }
}

} // namespace Shb
