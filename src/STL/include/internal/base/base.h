//
// internal/base/base.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//

#ifndef CORSAC_BASE_H
#define CORSAC_BASE_H

#include "compiler.h"
#include "compiler_traits.h"
#include "platform.h"

/**
* По умолчанию GCC определяет NULL как ((void) 0), что является определением C.
* Это вызывает всевозможные проблемы для кода C++,
* поэтому можно обойтись путем отмены определения NULL.
*/
#if defined(NULL)
    #undef NULL
#endif

/**
* Определите указатель NULL.
* Обычно это определяется в <stddef.h>, но мы не хотим вызывать глобальную зависимость
* от этого заголовка, поэтому определение здесь дублируется.
*/
#if defined(__cplusplus)
    #define NULL 0
#else
    #define NULL ((void*)0)
#endif

#endif //CORSAC_BASE_H
