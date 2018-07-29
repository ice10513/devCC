#pragma once

#include <string>
#include "ILogHandler.hpp"
#include "ShbUtils.hpp"
#include "IDataRepository.hpp"

namespace Shb
{
struct StateTransitioMatchResult
{
    UeContextId ueContextId;
    std::string fsmName;
    std::string fromState;
    std::string toState;
};

class StateTransitionLogHandler : public ILogHandler
{
public:
    StateTransitionLogHandler(IDataRepository& p_dataRepositoy);
    bool handle(const std::string&) override;

private:
    optional<StateTransitioMatchResult> getStateTransitionMatchResult(const std::string&) const;
    void insertToDataRepository(const StateTransitioMatchResult&);

    IDataRepository& m_dataRepositoy;
};
} // namespace Shb