#pragma once
#include "DataRepStructure.hpp"

namespace Shb
{
class IDataRepository
{
public:
    virtual ~IDataRepository() = default;
    virtual void insertFsmName(UeContextId, std::string p_name) = 0;
    virtual void insertStateName(UeContextId, std::string p_state) = 0;
    virtual void insertMessage(UeContextId, std::string p_prefix, Message p_message) = 0;
    virtual void insertMessage(Crnti, std::string p_prefix, Message p_message) = 0;
    virtual void dumpToJson() const = 0;
};
} // namespace Shb
