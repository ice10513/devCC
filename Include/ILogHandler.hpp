#pragma once
#include <string>

namespace Shb
{
class ILogHandler
{
public:
    virtual ~ILogHandler() = default;
    virtual bool handle(const std::string&) = 0;
};
} // namespace Shb
