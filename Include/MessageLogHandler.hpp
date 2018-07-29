#pragma once
#include <regex>
#include <functional>
#include "ILogHandler.hpp"
#include "DataRepStructure.hpp"
#include "IDataRepository.hpp"

namespace Shb
{
class IStateHandler;
class IDataRepository;

class MessageLogHandler : public ILogHandler
{
public:
    MessageLogHandler(IDataRepository& p_dataRepository, IStateHandler& p_initialState);

protected:
    void stateTransition(IStateHandler& p_stateHandler);
    std::smatch getRegexMatchResult(const std::string&, const std::string&) const;

    optional<UeContextId> getUeContexIdMatchResult(const std::string&) const;
    optional<Crnti> getCrntiMatchResult(const std::string&) const;
    optional<std::string> getAsnMsgIdMatchResult(const std::string&) const;

    std::string getMessageNamePrefix(const Message& p_msg) const;

    template <typename MatchResult>
    void insertToDataRepository(optional<MatchResult>& p_matchResult, const std::function<Message()>& p_createDataRepMessage)
    {
        if (!p_matchResult->uecContextId && !p_matchResult->crnti)
        {
            SHB_ERR << "Invalid info with ueContextId and crnti both is empty"
                << " message name " << p_matchResult->messageName << std::endl;
            return;
        }

        if (p_matchResult->uecContextId && p_matchResult->crnti)
        {
            SHB_ERR << "Invalid info with ueContextId and crnti both exist"
                << " message name " << p_matchResult->messageName << std::endl;
            return;
        }

        auto l_message = p_createDataRepMessage();
        auto l_msgNamePrefix = getMessageNamePrefix(l_message);
        if (p_matchResult->uecContextId)
        {
            m_dataRepository.insertMessage(*p_matchResult->uecContextId, std::move(l_msgNamePrefix), std::move(l_message));
        }
        else
        {
            m_dataRepository.insertMessage(*p_matchResult->crnti, std::move(l_msgNamePrefix), std::move(l_message));
        }
    }

    std::reference_wrapper<IStateHandler> m_currentStateHandler;
    IDataRepository& m_dataRepository;
};
} // namespace Shb
