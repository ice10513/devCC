#include <iostream>
#include "RespMessageLogHandler.hpp"
#include "ShbUtils.hpp"
#include "IDataRepository.hpp"
#include "MessageLogHandlerRegex.hpp"

namespace Shb
{

using namespace std::placeholders;

RespMessageLogHandler::RespMessageLogHandler(IDataRepository& p_dataRepository) :
    MessageLogHandler(p_dataRepository, m_defaultState),
    m_defaultState(std::bind(&RespMessageLogHandler::handleDefaultState, this, _1)),
    m_resultStatusState(std::bind(&RespMessageLogHandler::handleResultStatusState, this, _1)),
    m_resultCauseState(std::bind(&RespMessageLogHandler::handleResultCauseState, this, _1)),
    m_resultSpecificCauseState(std::bind(&RespMessageLogHandler::handleResultSpecificCauseState, this, _1)),
    m_unorderedState(std::bind(&RespMessageLogHandler::handleUnorderedState, this, _1))
{}

bool RespMessageLogHandler::handle(const std::string& p_singleLineData)
{
    return m_currentStateHandler.get().handle(p_singleLineData);
}

void RespMessageLogHandler::resetJob()
{
    m_respMessageResult = {};
    stateTransition(m_defaultState);
}

void RespMessageLogHandler::resetAbnormalJob(const std::string& p_stateName, const std::string& p_singleLineData)
{
    SHB_ERR << "Abnormal resp message with state: " << p_stateName << " when handle: " << p_singleLineData << std::endl;
    resetJob();
}

optional<std::string> RespMessageLogHandler::getRespMsgNameMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_respMsgNameRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "respMsgName: " << l_matchResult[1] << std::endl;
        return l_matchResult[1];
    }

    return {};
}

bool RespMessageLogHandler::handleDefaultState(const std::string& p_singleLineData)
{
    auto l_messageName = getRespMsgNameMatchResult(p_singleLineData);
    if (l_messageName)
    {
        m_respMessageResult = RespMessageMatchReult{std::move(*l_messageName)};
        stateTransition(m_resultStatusState);
        return true;
    }

    return false;
}

optional<int> RespMessageLogHandler::getResultStatusMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_resultStatusRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "resultStatus: " << l_matchResult[1] << std::endl;
        return std::stoi(l_matchResult[1]);
    }

    return {};
}

bool RespMessageLogHandler::handleResultStatusState(const std::string& p_singleLineData)
{
    auto l_resultStatus = getResultStatusMatchResult(p_singleLineData);
    if (!l_resultStatus)
    {
        resetAbnormalJob("ResultStatusState", p_singleLineData);
        return false;
    }

    m_respMessageResult->messageResult.status = *l_resultStatus;
    stateTransition(m_resultCauseState);
    return true;
}

optional<int> RespMessageLogHandler::getResultCauseMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_resultCauseRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "resultCause: " << l_matchResult[1] << std::endl;
        return std::stoi(l_matchResult[1]);
    }

    return {};
}

bool RespMessageLogHandler::handleResultCauseState(const std::string& p_singleLineData)
{
    auto l_resultCause = getResultCauseMatchResult(p_singleLineData);
    if (!l_resultCause)
    {
        resetAbnormalJob("ResultCauseState", p_singleLineData);
        return false;
    }

    m_respMessageResult->messageResult.cause = *l_resultCause;
    stateTransition(m_resultSpecificCauseState);
    return true;
}

optional<int> RespMessageLogHandler::getResultSpecificCauseMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_resultSpecificCauseRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "resultSpecificCause: " << l_matchResult[1] << std::endl;
        return std::stoi(l_matchResult[1]);
    }

    return {};
}

bool RespMessageLogHandler::handleResultSpecificCauseState(const std::string& p_singleLineData)
{
    auto l_resultSpecificCause = getResultSpecificCauseMatchResult(p_singleLineData);
    if (!l_resultSpecificCause)
    {
        resetAbnormalJob("ResultSpecificCauseState", p_singleLineData);
        return false;
    }

    m_respMessageResult->messageResult.specificCause = *l_resultSpecificCause;
    stateTransition(m_unorderedState);
    return true;
}

Message RespMessageLogHandler::createDataRepMessage() const
{
    if (m_respMessageResult->asnMsgId)
    {
        return {*m_respMessageResult->asnMsgId, m_respMessageResult->messageResult};
    }

    return {m_respMessageResult->messageName, m_respMessageResult->messageResult};
}

bool RespMessageLogHandler::handleUnorderedState(const std::string& p_singleLineData)
{
    if (getRegexMatchResult(g_unorderedCommonRegex, p_singleLineData).empty())
    {
        insertToDataRepository(m_respMessageResult, std::bind(&RespMessageLogHandler::createDataRepMessage, this));
        resetJob();
        return false;
    }

    auto l_ueContextId = getUeContexIdMatchResult(p_singleLineData);
    if (l_ueContextId)
    {
        m_respMessageResult->uecContextId = l_ueContextId;
        return true;
    }

    auto l_crnti = getCrntiMatchResult(p_singleLineData);
    if (l_crnti)
    {
        m_respMessageResult->crnti = l_crnti;
        return true;
    }

    auto l_asnMsgId = getAsnMsgIdMatchResult(p_singleLineData);
    if (l_asnMsgId)
    {
        m_respMessageResult->asnMsgId = l_asnMsgId;
        return true;
    }

    return true;
}
} // namespace Shb
