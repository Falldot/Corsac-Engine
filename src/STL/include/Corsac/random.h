//
// Created by Falldot on 16.11.2021.
//

#ifndef CORSAC_RANDOM_H
#define CORSAC_RANDOM_H

#pragma once

#include "internal/config.h"
#include "numeric_limits.h"

namespace corsac
{
    // Реализует равномерное распределение значений, генерируемых генератором,
    // где генератор обычно является генератором случайных или псевдослучайных чисел.
    // Обратите внимание, что диапазон minmax для этого класса является включительным,
    // поэтому, если вам нужны случайные целые числа 0, 1, 2 и 3,
    // вам нужно инициализировать этот класс с помощью (0, 3), а не (0, 4).
    template<class IntType = int>
    class uniform_int_distribution
    {
        static_assert(corsac::is_integral<IntType>::value, "uniform_int_distribution: IntType must be integral.");

    public:
        using result_type = IntType;

        // Для uniform_int_distribution param_type просто определяет минимальное и максимальное значения диапазона,
        // возвращаемого operator (). Это может означать что-то еще для других типов дистрибутивов.
        struct param_type
        {
            explicit param_type(IntType a = 0, IntType b = corsac::numeric_limits<IntType>::max());

            result_type a() const;
            result_type b() const;

            bool operator==(const param_type& x) { return (x.mA == mA) && (x.mB == mB); }
            bool operator!=(const param_type& x) { return (x.mA != mA) || (x.mB != mB); }

        protected:
            IntType mA;
            IntType mB;
        };

        uniform_int_distribution(IntType a = 0, IntType b = corsac::numeric_limits<IntType>::max());
        uniform_int_distribution(const param_type& params);

        void reset();

        template<class Generator>
        result_type operator()(Generator& g);

        template<class Generator>
        result_type operator()(Generator& g, const param_type& params);

        result_type a() const;
        result_type b() const;

        param_type param() const;
        void param(const param_type& params);

        result_type min() const;
        result_type max() const;

    protected:
        param_type mParam;
    };

    // uniform_int_distribution
    template<class IntType>
    inline uniform_int_distribution<IntType>::param_type::param_type(IntType aValue, IntType bValue)
            : mA(aValue), mB(bValue)
    {
        CORSAC_ASSERT(aValue <= bValue);
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::param_type::a() const
    {
        return mA;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::param_type::b() const
    {
        return mB;
    }

    template<class IntType>
    inline uniform_int_distribution<IntType>::uniform_int_distribution(IntType aValue, IntType bValue)
            : mParam(aValue, bValue)
    {}

    template<class IntType>
    inline uniform_int_distribution<IntType>::uniform_int_distribution(const param_type& params)
            : mParam(params)
    {}

    template<class IntType>
    void uniform_int_distribution<IntType>::reset()
    {}

    template<class IntType>
    template<class Generator>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::operator()(Generator& g)
    {
        return operator()(g, mParam);
    }

    template<class IntType>
    template<class Generator>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::operator()(Generator& g, const param_type& params)
    {
        // Эту функцию сложно реализовать универсальным способом для всех интегральных типов.
        // Решение будет включать обработку случаев со знаковыми типами и 64-битными типами, вероятно,
        // таким образом, чтобы использовать метапрограммирование шаблонов для работы со знаковыми диапазонами.

        // Временное решение, пока мы ищем полное решение. Поддерживает только uint8_t,
        // uint16_t, and uint32_t uniform_int_distribution types.
        static_assert(corsac::is_unsigned<result_type>::value && (sizeof(result_type) <= 4), "uniform_int_distribution currently supports only uint8_t, uint16_t, uint32_t.");

        result_type v = g();    // Создает значение в диапазоне (numeric_limits<result_type>::min(), numeric_limits<result_type>::max()).
        result_type r = (result_type)((v * (uint64_t)((params.b() - params.a()) + 1)) >> (sizeof(result_type) * 8)); // +1 потому что диапазоны включены.
        return params.a() + r;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::a() const
    {
        return mParam.mA;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::b() const
    {
        return mParam.mB;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::param_type
    uniform_int_distribution<IntType>::param() const
    {
        return mParam;
    }

    template<class IntType>
    inline void
    uniform_int_distribution<IntType>::param(const param_type& params)
    {
        mParam = params;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::min() const
    {
        return mParam.mA;
    }

    template<class IntType>
    inline typename uniform_int_distribution<IntType>::result_type
    uniform_int_distribution<IntType>::max() const
    {
        return mParam.mB;
    }

    template<class ResultType>
    inline bool operator==(const uniform_int_distribution<ResultType>& lhs,
                           const uniform_int_distribution<ResultType>& rhs)
    {
        return (lhs.param() == rhs.param());
    }

    template<class ResultType>
    inline bool operator!=(const uniform_int_distribution<ResultType>& lhs,
                           const uniform_int_distribution<ResultType>& rhs)
    {
        return (lhs.param() != rhs.param());
    }
}

#endif //CORSAC_RANDOM_H
