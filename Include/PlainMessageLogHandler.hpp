#pragma once
#include <functional>
#include "MessageLogHandler.hpp"
#include "StateHandler.hpp"

namespace Shb
{
class IDataRepository;

struct PlainMsgMatchResult
{
    std::string messageName;
    optional<UeContextId> uecContextId;
    optional<Crnti> crnti;
    optional<std::string> asnMsgId;
};

class PlainMessageLogHandler : public MessageLogHandler
{
public:
    PlainMessageLogHandler(IDataRepository& p_dataRepository);
    bool handle(const std::string&) override;

private:
    void resetJob();
    optional<std::string> getRecvMessageName(const std::string& p_singleLineData) const;
    optional<std::string> getMessageName(const std::string&) const;
    bool handleDefaultState(const std::string&);
    bool handleUnorderedState(const std::string&);
    Message createDataRepMessage() const;

    StateHandler m_defaultState;
    StateHandler m_unorderedState;
    optional<PlainMsgMatchResult> m_plainMsgMatchResult;
};
} // namespace Shb