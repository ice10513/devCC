#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ShbUtils.hpp"

namespace Shb
{
struct MessageResult
{
    int status{};
    int cause{};
    int specificCause{};
};

struct Message
{
    std::string name;
    optional<MessageResult> result;
};

struct UeContext
{
    std::vector<std::string> fsmNames;
    std::vector<std::string> states;
    std::map<std::string, std::vector<Message>> messages;
};

struct CrntiContext
{
    std::map<std::string, std::vector<Message>> messages;
};

template <typename UnderlyingType, typename Tag>
class StrongType
{
    template <typename T1, typename T2>
    friend std::ostream& operator << (std::ostream&, const StrongType<T1, T2>&);
public:
    StrongType() = default;

    explicit StrongType(const UnderlyingType& p_value) :
        m_value(p_value)
    {}
    
    bool operator == (const StrongType& p_other) const
    {
        return m_value == p_other.m_value;
    }

    std::size_t hash() const
    {
        return std::hash<UnderlyingType>{}(m_value);
    }
private:
    UnderlyingType m_value = {};
};

template <typename UnderlyingType, typename Tag>
inline std::ostream& operator << (std::ostream& p_out, const StrongType<UnderlyingType, Tag>& p_value)
{
    p_out << p_value.m_value;
}

class HashStrongType
{
public:
    template <typename UnderlyingType, typename Tag>
    std::size_t operator () (const StrongType<UnderlyingType, Tag>& p_value) const
    {
        return p_value.hash();
    }
};

using UeContextId = StrongType<std::size_t, struct UeContextIdTag>;
using Crnti = StrongType<std::size_t, struct CrntiTag>;
} // namespace Shb