#pragma once
#include <functional>
#include "IStateHandler.hpp"

namespace Shb
{
class StateHandler : public IStateHandler
{
public:
    using ForwardHandleFunc = std::function<bool(const std::string&)>;

    StateHandler(const ForwardHandleFunc& p_forwardHandlerFunc);
    bool handle(const std::string&) override;

private:
    ForwardHandleFunc m_forwardHandlerFunc;
};
} // namespace Shb