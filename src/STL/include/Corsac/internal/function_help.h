//
// Created by Falldot on 16.11.2021.
//

#ifndef CORSAC_FUNCTION_HELP_H
#define CORSAC_FUNCTION_HELP_H

#include "Corsac/internal/config.h"
#include "Corsac/type_traits.h"

namespace corsac
{
    namespace internal
    {
        // is_null
        template <typename T>
        bool is_null(const T&)
        {
            return false;
        }

        template <typename Result, typename... Arguments>
        bool is_null(Result (*const& function_pointer)(Arguments...))
        {
            return function_pointer == nullptr;
        }

        template <typename Result, typename Class, typename... Arguments>
        bool is_null(Result (Class::*const& function_pointer)(Arguments...))
        {
            return function_pointer == nullptr;
        }

        template <typename Result, typename Class, typename... Arguments>
        bool is_null(Result (Class::*const& function_pointer)(Arguments...) const)
        {
            return function_pointer == nullptr;
        }
    }
}

#endif //CORSAC_FUNCTION_HELP_H
