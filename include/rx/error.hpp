#pragma once

#include <stdexcept>

namespace rx
{
    class pattern_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class parser_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class tree_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class tnfa_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };
}