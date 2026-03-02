// Copyright (C) 2026 Peter Wild
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <stdexcept>


namespace rx
{
    class regex_error : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class pattern_error : public regex_error
    {
    public:
        using regex_error::regex_error;
    };

    class parser_error : public regex_error
    {
    public:
        using regex_error::regex_error;
    };

    class tree_error : public regex_error
    {
    public:
        using regex_error::regex_error;
    };

    class tnfa_error : public regex_error
    {
    public:
        using regex_error::regex_error;
    };
}