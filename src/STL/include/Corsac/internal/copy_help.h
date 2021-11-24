/**
 * corsac::STL
 *
 * internal/copy_help.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_COPY_HELP_H
#define CORSAC_COPY_HELP_H

#pragma once

#include "Corsac/internal/config.h"

#include "Corsac/type_traits.h"
#include "Corsac/iterator.h"

#include <string.h> // memcpy, memcmp, memmove

namespace corsac
{
    /**
     *
     * move / move_n / move_backward
     * copy / copy_n / copy_backward
     *
     * Мы хотим оптимизировать move, move_n, move_backward, copy, copy_backward, copy_n для выполнения
     * операций memmove, когда это возможно
     *
     * Мы могли бы использовать memcpy, хотя у него более строгие требования к перекрытию, чем у алгоритмов
     * перемещения и копирования, и в противном случае потребуется среда выполнения, чтобы выбрать его вместо
     * memmove. В частности, memcpy вообще не допускает перекрытия диапазонов, тогда как movecopy разрешает
     * перекрытие конца вывода, а move_backwardcopy_backward разрешает начало перекрытия вывода. Несмотря на
     * это, может быть полезно использовать memcpy для любых платформ, где memcpy значительно быстрее, чем
     * memmove, и поскольку в большинстве случаев операция copymove фактически не нацелена на перекрывающуюся
     * память, поэтому memcpy можно использовать.
     *
     * Мы можем использовать memmovememcpy, если выполняется следующее:
     *      InputIterator and OutputIterator are of the same type.
     *      InputIterator and OutputIterator are of type contiguous_iterator_tag or simply are pointers (the two are virtually synonymous).
     *      is_trivially_copyable<T>::value is true. i.e. the constructor T(const T& t) (or T(T&& t) if present) can be replaced by memmove(this, &t, sizeof(T))
     *
     * copy обычно отличается от move, но есть случай, когда copy совпадает с move: когда copy используется с
     * move_iterator. Мы обрабатываем этот случай здесь, обнаруживая, что копирование выполняется с помощью
     * move_iterator, и перенаправляем его на перемещение (что может использовать memmovememcpy).
     *
     * Класс generic_iterator обычно используется для обертывания указателей необработанной памяти, чтобы они
     * могли действовать как формальные итераторы. Поскольку указатели предоставляют возможность для операций
     * memmovememcpy, мы можем обнаружить общий итератор и использовать его обернутый тип в качестве
     * указателя, если он окажется таковым.
     */

    // Реализация перемещает копирование как тривиальных, так и нетривиальных данных через меньший итератор, чем при произвольном доступе.
    template <typename /*InputIteratorCategory*/, bool /*isMove*/, bool /*canMemmove*/>
    struct move_and_copy_helper
    {
        template <typename InputIterator, typename OutputIterator>
        static OutputIterator move_or_copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            for(; first != last; ++result, ++first)
                *result = *first;
            return result;
        }
    };

    /**
     * Специализация на копирование нетривиальных данных с помощью итератора с произвольным доступом.
     * Теоретически это быстрее, потому что компилятор может видеть счет, когда это константа времени
     * компиляции. Эта специализация преобразует InputIterator с произвольным доступом последним и первым в
     * целочисленный тип. У нас есть простой способ воспользоваться преимуществами итератора вывода с
     * произвольным доступом, так как диапазон задается входом, а не выходом, а расстояние (first, last)
     * для итератора без произвольного доступа потенциально медленное.
     */
    template <>
    struct move_and_copy_helper<corsac::random_access_iterator_tag, false, false>
    {
        template <typename InputIterator, typename OutputIterator>
        static OutputIterator move_or_copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            using difference_type = typename corsac::iterator_traits<InputIterator>::difference_type;

            for(difference_type n = (last - first); n > 0; --n, ++first, ++result)
                *result = *first;

            return result;
        }
    };


    template <bool isMove, typename InputIterator, typename OutputIterator>
    inline OutputIterator move_and_copy_chooser(InputIterator first, InputIterator last, OutputIterator result)
    {
        using IIC               = typename corsac::iterator_traits<InputIterator>::iterator_category;
        using OIC               = typename corsac::iterator_traits<OutputIterator>::iterator_category;
        using value_type_input  = typename corsac::iterator_traits<InputIterator>::value_type;
        using value_type_output = typename corsac::iterator_traits<OutputIterator>::value_type;

        const bool canBeMemmoved = corsac::is_trivially_copyable<value_type_output>::value &&
                                   corsac::is_same<value_type_input, value_type_output>::value &&
                                   (corsac::is_pointer<InputIterator>::value  || corsac::is_same<IIC, corsac::contiguous_iterator_tag>::value) &&
                                   (corsac::is_pointer<OutputIterator>::value || corsac::is_same<OIC, corsac::contiguous_iterator_tag>::value);

        // Необходимо выбирать на основе тега итератора ввода, а не тега итератора вывода, потому что контейнеры принимают диапазоны ввода типов итератора, отличных от self.
        return corsac::move_and_copy_helper<IIC, isMove, canBeMemmoved>::move_or_copy(first, last, result);
    }

    // У нас есть второй уровень вызовов unwrap_iterator, потому что исходный итератор может быть чем-то вроде move_iterator<generic_iterator<int*> > (i.e. doubly-wrapped).
    template <bool isMove, typename InputIterator, typename OutputIterator>
    inline OutputIterator move_and_copy_unwrapper(InputIterator first, InputIterator last, OutputIterator result)
    {
        // Придется преобразовать в OutputIterator потому-что result.base() может быть как T*
        return OutputIterator(corsac::move_and_copy_chooser<isMove>(corsac::unwrap_iterator(first), corsac::unwrap_iterator(last), corsac::unwrap_iterator(result)));
    }

    /**
     * move
     *
     * После этой операции элементы в перемещаемом диапазоне по-прежнему будут содержать допустимые значения
     * соответствующего типа, но не обязательно те же значения, что и до перемещения. Возвращает конец
     * диапазона результатов. Примечание. При перемещении между контейнерами диапазон dest должен быть
     * допустимым; эта функция не изменяет размер контейнеров. Примечание: если результат находится в пределах
     * [first, last), вместо move следует использовать move_backward.
     *
     * Example usage:
     *      corsac::move(myArray.begin(), myArray.end(), myDestArray.begin());
     *
     * Reference implementation:
     *      template <typename InputIterator, typename OutputIterator>
     *      OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
     *      {
     *          while(first != last)
     *              *result++ = corsac::move(*first++);
     *          return result;
     *      }
     */
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
    {
        return corsac::move_and_copy_unwrapper<true>(corsac::unwrap_iterator(first), corsac::unwrap_iterator(last), result);
    }

    // Специализация для перемещения нетривиальных данных через меньший итератор, чем при произвольном доступе.
    template <typename InputIteratorCategory>
    struct move_and_copy_helper<InputIteratorCategory, true, false>
    {
        template <typename InputIterator, typename OutputIterator>
        static OutputIterator move_or_copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            for(; first != last; ++result, ++first)
                *result = corsac::move(*first);
            return result;
        }
    };

    // Специализация для перемещения нетривиальных данных через итератор с произвольным доступом. Теоретически это быстрее, потому что компилятор может видеть счет, когда это константа времени компиляции.
    template <>
    struct move_and_copy_helper<corsac::random_access_iterator_tag, true, false>
    {
        template <typename InputIterator, typename OutputIterator>
        static OutputIterator move_or_copy(InputIterator first, InputIterator last, OutputIterator result)
        {
            using difference_type = typename corsac::iterator_traits<InputIterator>::difference_type;

            for(difference_type n = (last - first); n > 0; --n, ++first, ++result)
                *result = corsac::move(*first);

            return result;
        }
    };

    // Специализация на то, когда мы можем использовать memmove/memcpy. См. Примечания выше, какие условия позволяют это.
    template <bool isMove>
    struct move_and_copy_helper<corsac::random_access_iterator_tag, isMove, true>
    {
        template <typename T>
        static T* move_or_copy(const T* first, const T* last, T* result)
        {
            if (CORSAC_UNLIKELY(first == last))
                return result;
            // Мы могли бы использовать memcpy здесь, если нет перекрытия диапазонов, но memcpy редко бывает намного быстрее, чем memmove.
            return (T*)memmove(result, first, (size_t)((uintptr_t)last - (uintptr_t)first)) + (last - first);
        }
    };

    /**
     * copy
     *
     * Эффекты: копирует элементы из диапазона [first, last) в диапазон [result, result + (last - first)),
     * начиная с первого и заканчивая последним. Для каждого неотрицательного целого числа n <(last - first)
     * выполняется (result + n) = (first + n).
     *
     * Возвращает: результат + (последний - первый). То есть возвращает конец результата.
     * Обратите внимание, что это отличается от того, как работают memmovememcpy, поскольку они возвращают начало результата.
     *
     * Требуется: результат не должен быть в диапазоне [first, last). Но на самом деле конец диапазона
     * результатов может находиться в пределах входного диапазона.
     *
     * Сложность: точно «last - first» задания.
     */
    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        const bool isMove = corsac::is_move_iterator<InputIterator>::value;
        return corsac::move_and_copy_unwrapper<isMove>(corsac::unwrap_iterator(first), corsac::unwrap_iterator(last), result);
    }
}


#endif //CORSAC_COPY_HELP_H
