/**
 * corsac::STL
 *
 * string.h
 *
 * Created by Falldot on 23.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STRING_H
#define CORSAC_STRING_H

#pragma once
/**
 *
 *
 */

#include "internal/config.h"
#include "allocator.h"
#include "iterator"
#include "algorithm"
#include "initializer_list.h"

#include <stddef.h>             // size_t, ptrdiff_t, etc.
#include <stdarg.h>             // vararg functionality.

#include <stdlib.h>             // malloc, free.
#include <stdio.h>              // snprintf, etc.
#include <ctype.h>              // toupper, etc.
#include <wchar.h>              // wchar, etc.
#include <string.h>             // strlen, etc.

#if CORSAC_EXCEPTIONS_ENABLED
    #include <stdexcept> // std::out_of_range, std::length_error.
#endif

#include "internal/char_traits.h"
#include "string_view.h"

namespace corsac
{

}

#endif //CORSAC_STRING_H
