#include "PlainMessageLogHandler.hpp"
#include "IDataRepository.hpp"
#include "MessageLogHandlerRegex.hpp"

namespace Shb
{
namespace
{
bool isEndsWith(const std::string& p_str, const std::string& p_subStr)
{
    return p_str.compare(p_str.size() - p_subStr.size(), p_subStr.size(), p_subStr) == 0;
}
} // namespace

using namespace std::placeholders;

PlainMessageLogHandler::PlainMessageLogHandler(IDataRepository& p_dataRepository) :
    MessageLogHandler(p_dataRepository, m_defaultState),
    m_defaultState(std::bind(&PlainMessageLogHandler::handleDefaultState, this, _1)),
    m_unorderedState(std::bind(&PlainMessageLogHandler::handleUnorderedState, this, _1))
{}

bool PlainMessageLogHandler::handle(const std::string& p_singleLineData)
{
    return m_currentStateHandler.get().handle(p_singleLineData);
}

void PlainMessageLogHandler::resetJob()
{
    m_plainMsgMatchResult = {};
    stateTransition(m_defaultState);
}

optional<std::string> PlainMessageLogHandler::getRecvMessageName(const std::string& p_singleLineData) const
{
    auto l_recvMatchResult = getRegexMatchResult(g_msgRecvRegex, p_singleLineData);
    if (l_recvMatchResult.empty())
    {
        return {};
    }

    if (isEndsWith(l_recvMatchResult[1], "Resp")) //due to c++11 not support (?<!)
    {
        return {};
    }

    SHB_LOG << "msg recv: " << l_recvMatchResult[1] << std::endl;
    return l_recvMatchResult[1];
}

optional<std::string> PlainMessageLogHandler::getMessageName(const std::string& p_singleLineData) const
{
    auto l_sentMatchResult = getRegexMatchResult(g_msgSentRegex, p_singleLineData);
    if (!l_sentMatchResult.empty())
    {
        SHB_LOG << "msg sent: " << l_sentMatchResult[1] << std::endl;
        return l_sentMatchResult[1];
    }

    return getRecvMessageName(p_singleLineData);
}
bool PlainMessageLogHandler::handleDefaultState(const std::string& p_singleLineData)
{
    auto l_msgName = getMessageName(p_singleLineData);
    if (l_msgName)
    {
        m_plainMsgMatchResult = PlainMsgMatchResult{*l_msgName};
        stateTransition(m_unorderedState);
        return true;
    }

    return false;
}

Message PlainMessageLogHandler::createDataRepMessage() const
{
    if (m_plainMsgMatchResult->asnMsgId)
    {
        return {*m_plainMsgMatchResult->asnMsgId};
    }

    return {m_plainMsgMatchResult->messageName};
}

bool PlainMessageLogHandler::handleUnorderedState(const std::string& p_singleLineData)
{
    if (getRegexMatchResult(g_unorderedCommonRegex, p_singleLineData).empty())
    {
        insertToDataRepository(m_plainMsgMatchResult, std::bind(&PlainMessageLogHandler::createDataRepMessage, this));
        resetJob();
        return false;
    }

    auto l_ueContextId = getUeContexIdMatchResult(p_singleLineData);
    if (l_ueContextId)
    {
        m_plainMsgMatchResult->uecContextId = l_ueContextId;
        return true;
    }

    auto l_crnti = getCrntiMatchResult(p_singleLineData);
    if (l_crnti)
    {
        m_plainMsgMatchResult->crnti = l_crnti;
        return true;
    }

    auto l_asnMsgId = getAsnMsgIdMatchResult(p_singleLineData);
    if (l_asnMsgId)
    {
        m_plainMsgMatchResult->asnMsgId = l_asnMsgId;
        return true;
    }

    return true;
}

}// namespace Shb