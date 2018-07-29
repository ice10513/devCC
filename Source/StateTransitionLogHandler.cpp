#include <regex>
#include "StateTransitionLogHandler.hpp"

namespace Shb
{
StateTransitionLogHandler::StateTransitionLogHandler(IDataRepository& p_dataRepositoy) :
    m_dataRepositoy(p_dataRepositoy)
{}

optional<StateTransitioMatchResult> StateTransitionLogHandler::getStateTransitionMatchResult(
    const std::string& p_singleLineData) const
{
    std::regex l_stateTransitionRegex(
        SHB_LOG_LINE_REGEX_PREFIX
        R"(state transition Cell Id = \d+, Ue Context Id = (\d+), Fsm Name = (\w+), (\w+) -> (\w+))");

    std::smatch l_matchResult;
    if (std::regex_match(p_singleLineData, l_matchResult, l_stateTransitionRegex))
    {
        return StateTransitioMatchResult{UeContextId(std::stoi(l_matchResult[1])), l_matchResult[2], l_matchResult[3], l_matchResult[4]};
    }
    return {};
}

void StateTransitionLogHandler::insertToDataRepository(const StateTransitioMatchResult& p_matchResult)
{
    m_dataRepositoy.insertFsmName(p_matchResult.ueContextId, p_matchResult.fsmName);
    m_dataRepositoy.insertStateName(p_matchResult.ueContextId, p_matchResult.fromState);
    m_dataRepositoy.insertStateName(p_matchResult.ueContextId, p_matchResult.toState);
}

bool StateTransitionLogHandler::handle(const std::string& p_singleLineData)
{
    auto l_stateTransitioMatchResult = getStateTransitionMatchResult(p_singleLineData);
    if (l_stateTransitioMatchResult)
    {
        insertToDataRepository(*l_stateTransitioMatchResult);
        return true;
    }

    return false;
}
} // namespace Shb