#include "StateHandler.hpp"

namespace Shb
{
StateHandler::StateHandler(const ForwardHandleFunc& p_forwardHandlerFunc) :
    m_forwardHandlerFunc(p_forwardHandlerFunc)
{}

bool StateHandler::handle(const std::string& p_data)
{
    return m_forwardHandlerFunc(p_data);
}
} // namespace Shb