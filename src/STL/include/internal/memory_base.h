/**
 * corsac::STL
 *
 * internal/memory_base.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_MEMORY_BASE_H
#define CORSAC_MEMORY_BASE_H

#pragma once

namespace corsac
{
    /**
     * addressof
     *
     * From the C++11 Standard, section 20.6.12.1
     *
     * В С++20 был добавлен обновлен см. стандарт
     * Возвращает фактический адрес объекта или функции, на которые ссылается r, даже при наличии перегруженного оператора &.
     */
    template<typename T>
    T* addressof(T& value) noexcept
    {
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value)));
    }
} // namespace corsac

#endif //CORSAC_MEMORY_BASE_H
