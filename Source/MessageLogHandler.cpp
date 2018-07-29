#include "MessageLogHandler.hpp"
#include "ShbUtils.hpp"
#include "MessageLogHandlerRegex.hpp"

namespace Shb
{
MessageLogHandler::MessageLogHandler(IDataRepository& p_dataRepository, IStateHandler& p_initialState) :
    m_dataRepository(p_dataRepository),
    m_currentStateHandler(p_initialState)
{}

void MessageLogHandler::stateTransition(IStateHandler& p_stateHandler)
{
    m_currentStateHandler = p_stateHandler;
}

std::smatch MessageLogHandler::getRegexMatchResult(const std::string& p_regex, const std::string& p_singleLineData) const
{
    std::smatch l_matchResult;
    std::regex_match(p_singleLineData, l_matchResult, std::regex(SHB_LOG_LINE_REGEX_PREFIX + p_regex));

    return l_matchResult;
}

optional<UeContextId> MessageLogHandler::getUeContexIdMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_ueContextIdRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "UeContextId: " << l_matchResult[1] << std::endl;
        return UeContextId(std::stoi(l_matchResult[1]));
    }

    return {};
}

optional<Crnti> MessageLogHandler::getCrntiMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_crntiRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "Crnti: " << l_matchResult[1] << std::endl;
        return Crnti(std::stoi(l_matchResult[1]));
    }

    return {};
}

optional<std::string> MessageLogHandler::getAsnMsgIdMatchResult(const std::string& p_singleLineData) const
{
    auto l_matchResult = getRegexMatchResult(g_asnMsgIdRegex, p_singleLineData);
    if (!l_matchResult.empty())
    {
        SHB_LOG << "asnMsgId: " << l_matchResult[1] << std::endl;
        return l_matchResult[1];
    }

    return {};
}

std::string MessageLogHandler::getMessageNamePrefix(const Message& p_msg) const
{
    auto l_pos = p_msg.name.find('_');
    if (l_pos == std::string::npos)
    {
        SHB_ERR << "Invalid message name " << p_msg.name << " without _" << std::endl;
    }

    return p_msg.name.substr(0, l_pos);
}
} // namespace Shb