#pragma once
#include <string>

class GateId
{
public:
    explicit GateId(std::string value)
        : m_value(std::move(value))
    {
    }

    [[nodiscard]] const std::string& str() const noexcept
    {
        return m_value;
    }

    operator std::string() const
    {
        return m_value;
    }

private:
    std::string m_value;
};