#pragma once
#include <regex>
#include <functional>
#include "MessageLogHandler.hpp"
#include "StateHandler.hpp"
#include "DataRepStructure.hpp"

namespace Shb
{
class IDataRepository;

struct RespMessageMatchReult
{
    std::string messageName;
    MessageResult messageResult{};
    optional<UeContextId> uecContextId;
    optional<Crnti> crnti;
    optional<std::string> asnMsgId;
};

class RespMessageLogHandler : public MessageLogHandler
{
public:
    RespMessageLogHandler(IDataRepository& p_dataRepository);

    bool handle(const std::string&) override;

private:
    void resetJob();
    void resetAbnormalJob(const std::string&, const std::string&);

    optional<std::string> getRespMsgNameMatchResult(const std::string&) const;
    bool handleDefaultState(const std::string&);

    optional<int> getResultStatusMatchResult(const std::string&) const;
    bool handleResultStatusState(const std::string&);

    optional<int> getResultCauseMatchResult(const std::string&) const;
    bool handleResultCauseState(const std::string&);

    optional<int> getResultSpecificCauseMatchResult(const std::string&) const;
    bool handleResultSpecificCauseState(const std::string&);

    bool handleUnorderedState(const std::string&);
    Message createDataRepMessage() const;

    StateHandler m_defaultState;
    StateHandler m_resultStatusState;
    StateHandler m_resultCauseState;
    StateHandler m_resultSpecificCauseState;
    StateHandler m_unorderedState;
    optional<RespMessageMatchReult> m_respMessageResult;
};
} // namespace Shb