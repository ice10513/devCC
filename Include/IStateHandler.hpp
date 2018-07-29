#pragma once
#include <string>

namespace Shb
{
class IStateHandler
{
public:
    virtual ~IStateHandler() = default;
    virtual bool handle(const std::string&) = 0;
};
} // namespace Shb