/**
 * corsac::STL
 *
 * internal/integer_sequence.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_PIECEWISE_CONSTRUCT_T_H
#define CORSAC_PIECEWISE_CONSTRUCT_T_H

#pragma once

namespace corsac
{
    struct piecewise_construct_t
    {
        explicit piecewise_construct_t() = default;
    };

    // Тип тега, используемый для устранения неоднозначности между перегрузками функций, которые принимают два аргумента кортежа.
    constexpr piecewise_construct_t piecewise_construct = corsac::piecewise_construct_t();
}

#endif //CORSAC_PIECEWISE_CONSTRUCT_T_H
