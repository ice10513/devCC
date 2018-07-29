#pragma once
#include <unordered_map>
#include "IDataRepository.hpp"

namespace Shb
{
class DataRepository : public IDataRepository
{
public:
    void insertFsmName(UeContextId, std::string p_name) override;
    void insertStateName(UeContextId, std::string p_state) override;
    void insertMessage(UeContextId, std::string p_prefix, Message p_message) override;
    void insertMessage(Crnti, std::string p_prefix, Message p_message) override;
    void dumpToJson() const override;

    void dump();

private:
    std::unordered_map<UeContextId, UeContext, HashStrongType> m_ueContextMapper;
    std::unordered_map<Crnti, CrntiContext, HashStrongType> m_crnriMapper;
};
} // namespace Shb
