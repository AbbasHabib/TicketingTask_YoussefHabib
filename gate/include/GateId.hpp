#pragma once
#include <string>

class GateId
{
public:
    explicit GateId(std::string value)
        : value_(std::move(value))
    {
    }


    [[nodiscard]] const std::string& str() const noexcept
    {
        return value_;
    }

    operator std::string() const
    {
        return value_;
    }

private:
    std::string value_;
};