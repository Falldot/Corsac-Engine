/**
 * corsac::STL
 *
 * fixed_pool.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_FIXED_POOL_H
#define CORSAC_STL_FIXED_POOL_H

#pragma once

#include "Corsac/STL/config.h"
#include "Corsac/functional.h"
#include "Corsac/memory.h"
#include "Corsac/allocator.h"
#include "Corsac/type_traits.h"

// 4275 - non dll-interface class used as base for DLL-interface classkey 'identifier'
CORSAC_DISABLE_VC_WARNING(4275);

namespace corsac
{
    /**
    * CORSAC_FIXED_POOL_DEFAULT_NAME
    *
    * Определяет имя распределителя по умолчанию при отсутствии имени, предоставленного пользователем.
    */
    #ifndef CORSAC_FIXED_POOL_DEFAULT_NAME
        #define CORSAC_FIXED_POOL_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " fixed_pool"
    #endif

    /**
    * aligned_buffer
    *
    * Это полезно для создания буфера того же размера и выравнивания данной структуры или
    * класса. Это полезно для создания пулов памяти, которые поддерживают требования как к
    * размеру, так и к выравниванию хранимых объектов, но не тратят пространство на
    * перераспределение. Обратите внимание, что мы реализуем это через специализацию
    * структур, поскольку некоторые компиляторы, такие как VC ++, не поддерживают
    * спецификацию выравнивания каким-либо образом, кроме как через интегральную константу.
    *
    * Example usage:
    *    struct Widget{ }; // Этот класс имеет заданный размер и выравнивание.
    *
    *    Объявите буфер символов равного размера и выравнивания для Widget.
    *    aligned_buffer<sizeof(Widget), CORSAC_ALIGN_OF(Widget)> mWidgetBuffer;
    *
    *    На этот раз объявите массив.
    *    aligned_buffer<sizeof(Widget), CORSAC_ALIGN_OF(Widget)> mWidgetArray[15];
    */
    using aligned_buffer_char = char CORSAC_MAY_ALIAS;

    template <size_t size, size_t alignment>
    struct aligned_buffer { aligned_buffer_char buffer[size]; };

    template<size_t size>
    struct aligned_buffer<size, 2>    { CORSAC_PREFIX_ALIGN(2) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(2); };

    template<size_t size>
    struct aligned_buffer<size, 4>    { CORSAC_PREFIX_ALIGN(4) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(4); };

    template<size_t size>
    struct aligned_buffer<size, 8>    { CORSAC_PREFIX_ALIGN(8) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(8); };

    template<size_t size>
    struct aligned_buffer<size, 16>   { CORSAC_PREFIX_ALIGN(16) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(16); };

    template<size_t size>
    struct aligned_buffer<size, 32>   { CORSAC_PREFIX_ALIGN(32) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(32); };

    template<size_t size>
    struct aligned_buffer<size, 64>   { CORSAC_PREFIX_ALIGN(64) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(64); };

    template<size_t size>
    struct aligned_buffer<size, 128>  { CORSAC_PREFIX_ALIGN(128) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(128); };

    template<size_t size>
    struct aligned_buffer<size, 256>  { CORSAC_PREFIX_ALIGN(256) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(256); };

    template<size_t size>
    struct aligned_buffer<size, 512>  { CORSAC_PREFIX_ALIGN(512) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(512); };

    template<size_t size>
    struct aligned_buffer<size, 1024> { CORSAC_PREFIX_ALIGN(1024) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(1024); };

    template<size_t size>
    struct aligned_buffer<size, 2048> { CORSAC_PREFIX_ALIGN(2048) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(2048); };

    template<size_t size>
    struct aligned_buffer<size, 4096> { CORSAC_PREFIX_ALIGN(4096) aligned_buffer_char buffer[size] CORSAC_POSTFIX_ALIGN(4096); };

    /**
    * fixed_pool_base
    *
    * Это базовый класс для реализации пулов фиксированного размера.
    * В частности, классы fixed_pool и fixed_pool_with_overflow основаны на fixed_pool_base.
    */
    struct fixed_pool_base
    {
    public:
        /// fixed_pool_base
        fixed_pool_base(void* pMemory = NULL)
                : mpHead((Link*)pMemory)
                , mpNext((Link*)pMemory)
                , mpCapacity((Link*)pMemory)
                , mnNodeSize(0) // Обычно это устанавливается в функции инициализации.
        {
            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                mnCurrentSize = 0;
                mnPeakSize = 0;
            #endif
        }

        // operator=
        fixed_pool_base& operator=(const fixed_pool_base&)
        {
            // По дизайну мы ничего не делаем. Мы не пытаемся глубоко копировать данные участников.
            return *this;
        }

        /**
        * init
        *
        * Инициализирует fixed_pool с заданным набором параметров.
        * Вы не можете вызвать эту функцию дважды, иначе результирующее
        * поведение будет неопределенным. Вы можете вызвать эту функцию
        * только после создания fixed_pool с конструктором по умолчанию.
        */
        CORSAC_API void init(void* pMemory, size_t memorySize, size_t nodeSize,
                            size_t alignment, size_t alignmentOffset = 0);

        /**
        * peak_size
        *
        * Возвращает максимальное количество невыполненных распределений, которые были в любой момент времени.
        * Это представляет собой высшую точку для подсчета распределения.
        */
        size_t peak_size() const
        {
            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                return mnPeakSize;
            #else
                return 0;
            #endif
        }

        /**
        * can_allocate
        *
        * Возвращает true, если есть бесплатные ссылки.
        */
        bool can_allocate() const
        {
            return (mpHead != NULL) || (mpNext != mpCapacity);
        }

        public:
            struct Link
            {
                Link* mpNext;
            };

            Link*   mpHead;
            Link*   mpNext;
            Link*   mpCapacity;
            size_t  mnNodeSize;

            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                uint32_t mnCurrentSize; /// Текущее количество выделенных узлов.
                uint32_t mnPeakSize;    /// Максимальное количество выделенных узлов одновременно.
            #endif

        }; // fixed_pool_base

        /**
        * fixed_pool
        *
        * Реализует простой фиксированный распределитель пула для использования контейнерами
        * фиксированного размера. Это не общий распределитель corsac, который можно подключить к
        * произвольному контейнеру corsac, так как он упрощает некоторые функции, являющиеся
        * аргументами для повышения эффективности.
        */
        class CORSAC_API fixed_pool : public fixed_pool_base
        {
        public:
            /*
            * fixed_pool
            *
            * Конструктор по умолчанию. Обычно пользователь хочет вызвать init() после создания с
            * помощью этого конструктора. Аргумент pMemory используется для временного хранения
            * указателя на буфер, который будет использоваться. Несмотря на то, что у init может
            * быть аргумент pMemory, этот аргумент полезен для временного хранения в соответствии с
            * конструкцией копии.
            */
            fixed_pool(void* pMemory = NULL)
                    : fixed_pool_base(pMemory)
            {}

            /**
            * fixed_pool
            *
            * Создает fixed_pool с заданным набором параметров.
            */
            fixed_pool(void* pMemory, size_t memorySize, size_t nodeSize,
                       size_t alignment, size_t alignmentOffset = 0)
            {
                init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);
            }

            fixed_pool& operator=(const fixed_pool&)
            {
                // По дизайну мы ничего не делаем. Мы не пытаемся глубоко копировать данные участников.
                return *this;
            }

            /**
            * allocate
            *
            * Выделяет новый объект размера, указанного при инициализации класса.
            * Возвращает NULL, если памяти больше нет.
            */
            void* allocate()
            {
                Link* pLink = mpHead;

                if(pLink) // Если у нас есть место ...
                {
                    #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                        if(++mnCurrentSize > mnPeakSize)
                            mnPeakSize = mnCurrentSize;
                    #endif

                    mpHead = pLink->mpNext;
                    return pLink;
                }
                else
                {
                    // Если в свободном списке нет свободного узла, просто
                    // выделите другой из зарезервированной области памяти.

                    if(mpNext != mpCapacity)
                    {
                        pLink = mpNext;

                        mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext) + mnNodeSize);

                        #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                            if(++mnCurrentSize > mnPeakSize)
                                    mnPeakSize = mnCurrentSize;
                        #endif

                        return pLink;
                    }

                    return NULL;
                }
            }

            void* allocate(size_t /*alignment*/, size_t /*offset*/)
            {
                return allocate();
            }

            /**
            * deallocate
            *
            * Frees the given object which was allocated by allocate().
            * If the given node was not allocated by allocate() then the behaviour
            * is undefined.
            */
            void deallocate(void* p)
            {
                #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                        --mnCurrentSize;
                #endif
                ((Link*)p)->mpNext = mpHead;
                mpHead = ((Link*)p);
            }

            using fixed_pool_base::can_allocate;

            const char* get_name() const
            {
                return CORSAC_FIXED_POOL_DEFAULT_NAME;
            }

            void set_name(const char*)
            {
                // Нечего делать. Мы не выделяем память.
            }

    }; // fixed_pool

    // fixed_pool_with_overflow
    template <typename OverflowAllocator = CORSAC_ALLOCATOR_TYPE>
    class fixed_pool_with_overflow : public fixed_pool_base
    {
    public:
        using overflow_allocator_type = OverflowAllocator;


        fixed_pool_with_overflow(void* pMemory = NULL)
                : fixed_pool_base(pMemory),
                  mOverflowAllocator(CORSAC_FIXED_POOL_DEFAULT_NAME)
        {
            // Leave mpPoolBegin, mpPoolEnd uninitialized.
        }

        fixed_pool_with_overflow(void* pMemory, const overflow_allocator_type& allocator)
                : fixed_pool_base(pMemory),
                  mOverflowAllocator(allocator)
        {
            // Leave mpPoolBegin, mpPoolEnd uninitialized.
        }

        fixed_pool_with_overflow(void* pMemory, size_t memorySize, size_t nodeSize,
                                 size_t alignment, size_t alignmentOffset = 0)
                : mOverflowAllocator(CORSAC_FIXED_POOL_DEFAULT_NAME)
        {
            fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);

            mpPoolBegin = pMemory;
        }

        fixed_pool_with_overflow(void* pMemory, size_t memorySize, size_t nodeSize,
                                 size_t alignment, size_t alignmentOffset,
                                 const overflow_allocator_type& allocator)
                : mOverflowAllocator(allocator)
        {
            fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);

            mpPoolBegin = pMemory;
        }

        fixed_pool_with_overflow& operator=(const fixed_pool_with_overflow& x)
        {
            mOverflowAllocator = x.mOverflowAllocator;
            return *this;
        }

        void init(void* pMemory, size_t memorySize, size_t nodeSize,
                  size_t alignment, size_t alignmentOffset = 0)
        {
            fixed_pool_base::init(pMemory, memorySize, nodeSize, alignment, alignmentOffset);
            mpPoolBegin = pMemory;
        }

        void* allocate()
        {
            void* p     = NULL;
            Link* pLink = mpHead;

            if(pLink)
            {
                // Отключить от цепочки
                p      = pLink;
                mpHead = pLink->mpNext;
            }
            else
            {
                // Если в свободном списке нет свободного узла, просто
                // выделите другой из зарезервированной области памяти.

                if(mpNext != mpCapacity)
                {
                    p      = pLink = mpNext;
                    mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext) + mnNodeSize);
                }
                else
                    p = mOverflowAllocator.allocate(mnNodeSize);
            }

            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                if(p && (++mnCurrentSize > mnPeakSize))
                    mnPeakSize = mnCurrentSize;
            #endif

            return p;
        }


        void* allocate(size_t alignment, size_t alignmentOffset)
        {
            void* p = NULL;
            Link* pLink = mpHead;

            if (pLink)
            {
                // Отключить от цепочки
                p = pLink;
                mpHead = pLink->mpNext;
            }
            else
            {
                // Если в свободном списке нет свободного узла, просто
                // выделите другой из зарезервированной области памяти.

                if (mpNext != mpCapacity)
                {
                    p = pLink = mpNext;
                    mpNext = reinterpret_cast<Link*>(reinterpret_cast<char*>(mpNext)+mnNodeSize);
                }
                else
                {
                    p = allocate_memory(mOverflowAllocator, mnNodeSize, alignment, alignmentOffset);
                    CORSAC_ASSERT_MSG(p != nullptr, "the behaviour of corsac::allocators that return nullptr is not defined.");
                }

            }

            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                if (p && (++mnCurrentSize > mnPeakSize))
                    mnPeakSize = mnCurrentSize;
            #endif

            return p;
        }

        void deallocate(void* p)
        {
            #if CORSAC_FIXED_SIZE_TRACKING_ENABLED
                --mnCurrentSize;
            #endif

            if((p >= mpPoolBegin) && (p < mpCapacity))
            {
                ((Link*)p)->mpNext = mpHead;
                mpHead = ((Link*)p);
            }
            else
                mOverflowAllocator.deallocate(p, (size_t)mnNodeSize);
        }

        using fixed_pool_base::can_allocate;

        const char* get_name() const
        {
            return mOverflowAllocator.get_name();
        }

        void set_name(const char* pName)
        {
            mOverflowAllocator.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const
        {
            return mOverflowAllocator;
        }

        overflow_allocator_type& get_overflow_allocator()
        {
            return mOverflowAllocator;
        }

        void set_overflow_allocator(const overflow_allocator_type& overflowAllocator)
        {
            mOverflowAllocator = overflowAllocator;
        }
    public:
        OverflowAllocator mOverflowAllocator;
        void*             mpPoolBegin;
        // В идеале нам не нужна эта переменная-член. Проблема в том, что информация о буфере
        // пула и размере объекта хранится в контейнере-владельце, и мы не можем получить к нему
        // доступ без увеличения объема кода, который нам нужен, и путем создания дополнительных
        // шаблонов кода. Может оказаться, что просто хранить здесь данные меньше в итоге.

    }; // fixed_pool_with_overflow

    ///////////////////////////////////////////////////////////////////////////
    // fixed_node_allocator
    ///////////////////////////////////////////////////////////////////////////

    /**
    * fixed_node_allocator
    *
    * Реализует fixed_pool с заданным количеством узлов, выравниванием и смещением выравнивания.
    * fixed_node_allocator похож на fixed_pool, за исключением того, что он основан на типе узла,
    * а не является универсальным распределителем. Все, что он делает, это передает выделение в базу fixed_pool.
    * Эта функция отделена от fixed_pool, потому что есть и другие способы использования fixed_pool.
    *
    * Мы используем шаблон kNodeSize вместо node_type, потому что первый позволяет двум
    * различным node_types одного размера использовать одну и ту же реализацию шаблона.
    *
    * Параметры шаблона:
    *     nodeSize               Размер выделяемого объекта.
    *     nodeCount              Количество объектов, содержащихся в пуле.
    *     nodeAlignment          Выравнивание объектов для выделения.
    *     nodeAlignmentOffset    Смещение выравнивания выделяемых объектов.
    *     bEnableOverflow        Должны ли мы использовать кучу переполнения, если наш пул объектов исчерпан.
    *     OverflowAllocator      Распределитель переполнения, который используется, только если bEnableOverflow == true. По умолчанию используется глобальная куча.
    */
    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator = CORSAC_ALLOCATOR_TYPE>
    class fixed_node_allocator
    {
    public:
        using pool_type                 = typename type_select<bEnableOverflow, fixed_pool_with_overflow<OverflowAllocator>, fixed_pool>::type;
        using this_type                 = fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof(T) как множественное alignof(T), и поэтому sizeof(T) всегда >= alignof(T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

    public:
        pool_type mPool;

    public:
        fixed_node_allocator(void* pNodeBuffer)
                : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {}

        fixed_node_allocator(void* pNodeBuffer, const overflow_allocator_type& allocator)
                : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset, allocator)
        {}

        fixed_node_allocator(const this_type& x)
                : mPool(x.mPool.mpNext, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset, x.mPool.mOverflowAllocator)
        {}

        this_type& operator=(const this_type& x)
        {
            mPool = x.mPool;
            return *this;
        }

        void* allocate(size_t n, int /*flags*/ = 0)
        {
            (void)n;
            CORSAC_ASSERT(n == kNodeSize);
            return mPool.allocate();
        }

        void* allocate(size_t n, size_t alignment, size_t offset, int /*flags*/ = 0)
        {
            (void)n;
            CORSAC_ASSERT(n == kNodeSize);
            return mPool.allocate(alignment, offset);
        }

        void deallocate(void* p, size_t)
        {
            mPool.deallocate(p);
        }

        /**
        * can_allocate
        *
        * Возвращает true, если есть свободные ссылки.
        */
        bool can_allocate() const
        {
            return mPool.can_allocate();
        }

        /**
        * reset
        *
        * Эта функция в одностороннем порядке сбрасывает фиксированный пул обратно в новое
        * инициализированное состояние. Это полезно для использования вместе с функцией сброса контейнера.
        */
        void reset(void* pNodeBuffer)
        {
            mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }

        const char* get_name() const
        {
            return mPool.get_name();
        }

        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const noexcept
        {
            return mPool.mOverflowAllocator;
        }

        overflow_allocator_type& get_overflow_allocator() noexcept
        {
            return mPool.mOverflowAllocator;
        }

        void set_overflow_allocator(const overflow_allocator_type& allocator)
        {
            mPool.mOverflowAllocator = allocator;
        }

        void copy_overflow_allocator(const this_type& x)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            mPool.mOverflowAllocator = x.mPool.mOverflowAllocator;
        }

    }; // fixed_node_allocator

    // Это почти копия приведенного выше кода, с той лишь разницей, что параметр шаблона
    // bEnableOverflow 'false', typedefs pool_type и this_type, а также функции
    // set_overflow_allocator / get_overflow_allocator.
    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, typename OverflowAllocator>
    class fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>
    {
    public:
        using pool_type                 = fixed_pool;
        using this_type                 = fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof(T) как множественное alignof(T), и поэтому sizeof(T) всегда >= alignof(T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

    public:
        pool_type mPool;

        fixed_node_allocator(void* pNodeBuffer)
                : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {}

        fixed_node_allocator(void* pNodeBuffer, const overflow_allocator_type& /*allocator*/) // распределитель не используется, потому что bEnableOverflow имеет значение false в этой специализации.
                : mPool(pNodeBuffer, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {}

        fixed_node_allocator(const this_type& x) // Нет необходимости копировать распределитель переполнения, потому что bEnableOverflow имеет значение false в этой специализации.
                : mPool(x.mPool.mpNext, kNodesSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset)
        {}

        this_type& operator=(const this_type& x)
        {
            mPool = x.mPool;
            return *this;
        }

        void* allocate(size_t n, int /*flags*/ = 0)
        {
            (void)n;
            CORSAC_ASSERT(n == kNodeSize);
            return mPool.allocate();
        }

        void* allocate(size_t n, size_t alignment, size_t offset, int /*flags*/ = 0)
        {
            (void)n;
            CORSAC_ASSERT(n == kNodeSize);
            return mPool.allocate(alignment, offset);
        }

        void deallocate(void* p, size_t)
        {
            mPool.deallocate(p);
        }

        bool can_allocate() const
        {
            return mPool.can_allocate();
        }

        void reset(void* pNodeBuffer)
        {
            mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }

        const char* get_name() const
        {
            return mPool.get_name();
        }

        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        overflow_allocator_type& get_overflow_allocator() noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        void set_overflow_allocator(const overflow_allocator_type& /*allocator*/)
        {
            // У нас нет распределителя переполнения.
            CORSAC_ASSERT(false);
        }

        void copy_overflow_allocator(const this_type&)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            // У нас нет распределителя переполнения.
        }
    }; // fixed_node_allocator

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator==(const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a == &b); // Они равны, только если являются одним и тем же объектом.
    }

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator!=(const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_node_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a != &b); // Они равны, только если являются одним и тем же объектом.
    }

    /**
    * fixed_hashtable_allocator
    *
    * Предоставляет базовый класс для фиксированных распределений хеш-таблицы.
    * Для рассмотрения: унаследовать это от fixed_node_allocator.
    *
    * Template parameters:
    *     bucketCount            Фиксированное количество сегментов хеш-таблицы, которые необходимо предоставить.
    *     nodeCount              Количество объектов, содержащихся в пуле.
    *     nodeAlignment          Выравнивание объектов для выделения.
    *     nodeAlignmentOffset    Смещение выравнивания выделяемых объектов.
    *     bEnableOverflow        Должны ли мы использовать кучу переполнения, если наш пул объектов исчерпан.
    *     OverflowAllocator      Распределитель переполнения, который используется, только если bEnableOverflow == true. По умолчанию используется глобальная куча.
    */
    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator = CORSAC_ALLOCATOR_TYPE>
    class fixed_hashtable_allocator
    {
    public:
        using pool_type                 = typename type_select<bEnableOverflow, fixed_pool_with_overflow<OverflowAllocator>, fixed_pool>::type;
        using this_type                 = fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kBucketCount         = bucketCount + 1, // '+1' потому что для хэш-таблицы нужен сегмент с завершающим нулем.
            kBucketsSize         = bucketCount * sizeof(void*),
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof (T) как множественное alignof (T), и поэтому sizeof (T) всегда> = alignof (T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset, // Не нужно включать kBucketsSize в этот расчет, поскольку контейнеры fixed_hash_xxx имеют отдельный буфер для сегментов.
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset,
            kAllocFlagBuckets    = 0x00400000 // Флаг для распределителя, который указывает, что мы выделяем сегменты, а не узлы.
        };

    protected:
        pool_type mPool;
        void*     mpBucketBuffer;

    public:
        fixed_hashtable_allocator(void* pNodeBuffer)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(NULL)
        {
            // CORSAC_ASSERT(false); // В нынешнем виде это не должно вызываться.
        }

        fixed_hashtable_allocator(void* pNodeBuffer, const overflow_allocator_type& allocator)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset, allocator),
                  mpBucketBuffer(NULL)
        {
            // CORSAC_ASSERT(false); // В нынешнем виде это не должно вызываться.
        }

        fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(pBucketBuffer)
        {}

        fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer, const overflow_allocator_type& allocator)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset, allocator),
                  mpBucketBuffer(pBucketBuffer)
        {}

        /// fixed_hashtable_allocator
        ///
        /// Обратите внимание, что мы копируем x.mpHead и mpBucketBuffer в наш собственный fixed_pool.
        /// См. Обсуждение выше в fixed_node_allocator для получения важной информации об этом.
        ///
        fixed_hashtable_allocator(const this_type& x)
                : mPool(x.mPool.mpHead, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset, x.mPool.mOverflowAllocator),
                  mpBucketBuffer(x.mpBucketBuffer)
        {}

        fixed_hashtable_allocator& operator=(const fixed_hashtable_allocator& x)
        {
            mPool = x.mPool;
            return *this;
        }

        void* allocate(size_t n, int flags = 0)
        {
            // Мы ожидаем, что вызывающая сторона использует kAllocFlagBuckets, когда хочет, чтобы мы выделяли сегменты вместо узлов.
            CORSAC_STATIC_ASSERT(kAllocFlagBuckets == 0x00400000); // В настоящее время мы ожидаем, что это будет так, потому что в хеш-таблице есть копия этого перечисления.

            if((flags & kAllocFlagBuckets) == 0) // Если мы выделяем узлы, а (возможно) не ведра...
            {
                CORSAC_ASSERT(n == kNodeSize); CORSAC_UNUSED(n);
                return mPool.allocate();
            }

            // Если размер сегмента больше не умещается в локальном буфере...
            if ((flags & kAllocFlagBuckets) == kAllocFlagBuckets && (n > kBucketsSize))
                return get_overflow_allocator().allocate(n);

            CORSAC_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }


        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
        {
            // Мы ожидаем, что вызывающая сторона использует kAllocFlagBuckets, когда хочет, чтобы мы выделяли сегменты вместо узлов.
            if ((flags & kAllocFlagBuckets) == 0) // В настоящее время мы ожидаем, что это будет так, потому что в хеш-таблице есть копия этого перечисления.
            {
                CORSAC_ASSERT(n == kNodeSize); CORSAC_UNUSED(n);
                return mPool.allocate(alignment, offset);
            }

            // Если размер сегмента больше не умещается в локальном буфере...
            if ((flags & kAllocFlagBuckets) == kAllocFlagBuckets && (n > kBucketsSize))
                return get_overflow_allocator().allocate(n, alignment, offset);

            CORSAC_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }

        void deallocate(void* p, size_t)
        {
            if(p != mpBucketBuffer)
                mPool.deallocate(p);
        }

        bool can_allocate() const
        {
            return mPool.can_allocate();
        }

        void reset(void* pNodeBuffer)
        {
            // Нет необходимости изменять mpBucketBuffer, поскольку он постоянен.
            mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }

        const char* get_name() const
        {
            return mPool.get_name();
        }

        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const
        {
            return mPool.mOverflowAllocator;
        }

        overflow_allocator_type& get_overflow_allocator()
        {
            return mPool.mOverflowAllocator;
        }

        void set_overflow_allocator(const overflow_allocator_type& allocator)
        {
            mPool.mOverflowAllocator = allocator;
        }

        void copy_overflow_allocator(const this_type& x)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            mPool.mOverflowAllocator = x.mPool.mOverflowAllocator;
        }

    }; // fixed_hashtable_allocator

    // Это почти копия приведенного выше кода, с той лишь разницей, что параметр шаблона
    // bEnableOverflow 'false', typedefs pool_type и this_type, а также функции
    // the get_overflow_allocator / set_overflow_allocator functions.
    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, typename OverflowAllocator>
    class fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>
    {
    public:
        using pool_type                 = fixed_pool;
        using this_type                 = fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kBucketCount         = bucketCount + 1, // '+1', потому что хеш-таблице нужен сегмент с завершающим нулем.
            kBucketsSize         = bucketCount * sizeof(void*),
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof (T) как множественное alignof(T), и поэтому sizeof(T) всегда> = alignof(T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset, // Не нужно включать kBucketsSize в этот расчет, поскольку контейнеры fixed_hash_xxx имеют отдельный буфер для сегментов.
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset,
            kAllocFlagBuckets    = 0x00400000 // Флаг для распределителя, который указывает, что мы выделяем сегменты, а не узлы.
        };

    protected:
        pool_type mPool;
        void*     mpBucketBuffer;

    public:

        fixed_hashtable_allocator(void* pNodeBuffer)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(NULL)
        {
            // CORSAC_ASSERT(false); // В нынешнем виде это не должно вызываться.
        }

        fixed_hashtable_allocator(void* pNodeBuffer, const overflow_allocator_type& /*allocator*/) // распределитель не используется, потому что bEnableOverflow имеет значение false в этой специализации.
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(NULL)
        {
            // CORSAC_ASSERT(false); // В нынешнем виде это не должно вызываться.
        }

        fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer)
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(pBucketBuffer)
        {}

        fixed_hashtable_allocator(void* pNodeBuffer, void* pBucketBuffer, const overflow_allocator_type& /*allocator*/) // allocator is unused because bEnableOverflow is false in this specialization.
                : mPool(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(pBucketBuffer)
        {}

        /**
        * fixed_hashtable_allocator
        *
        * Обратите внимание, что мы копируем x.mpHead и mpBucketBuffer в наш собственный fixed_pool.
        * См. Обсуждение выше в fixed_node_allocator для получения важной информации об этом.
        */
        fixed_hashtable_allocator(const this_type& x) // Нет необходимости копировать распределитель переполнения, потому что bEnableOverflow имеет значение false в этой специализации.
                : mPool(x.mPool.mpHead, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset),
                  mpBucketBuffer(x.mpBucketBuffer)
        {}

        fixed_hashtable_allocator& operator=(const fixed_hashtable_allocator& x)
        {
            mPool = x.mPool;
            return *this;
        }

        void* allocate(size_t n, int flags = 0)
        {
            // Мы ожидаем, что вызывающая сторона использует kAllocFlagBuckets, когда хочет, чтобы мы выделяли сегменты вместо узлов.
            CORSAC_STATIC_ASSERT(kAllocFlagBuckets == 0x00400000); // настоящее время мы ожидаем, что это будет так, потому что в хеш-таблице есть копия этого перечисления.
            if((flags & kAllocFlagBuckets) == 0) // Если мы выделяем узлы
            {
                CORSAC_ASSERT(n == kNodeSize);  (void)n; // Убрать предупреждение о неиспользованном var.
                return mPool.allocate();
            }

            // В этом случае не допускайте переполнения корзин хеш-таблицы.
            CORSAC_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }


        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
        {
            // Мы ожидаем, что вызывающая сторона использует kAllocFlagBuckets, когда хочет, чтобы мы выделяли сегменты вместо узлов.
            if((flags & kAllocFlagBuckets) == 0) // настоящее время мы ожидаем, что это будет так, потому что в хеш-таблице есть копия этого перечисления.
            {
                CORSAC_ASSERT(n == kNodeSize); (void)n; // Убрать предупреждение о неиспользованном var.
                return mPool.allocate(alignment, offset);
            }

            // В этом случае не допускайте переполнения корзин хеш-таблицы.
            CORSAC_ASSERT(n <= kBucketsSize);
            return mpBucketBuffer;
        }

        void deallocate(void* p, size_t)
        {
            if(p != mpBucketBuffer)
                mPool.deallocate(p);
        }

        bool can_allocate() const
        {
            return mPool.can_allocate();
        }

        void reset(void* pNodeBuffer)
        {
            // Нет необходимости изменять mpBucketBuffer, поскольку он постоянен.
            mPool.init(pNodeBuffer, kBufferSize, kNodeSize, kNodeAlignment, kNodeAlignmentOffset);
        }

        const char* get_name() const
        {
            return mPool.get_name();
        }

        void set_name(const char* pName)
        {
            mPool.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        overflow_allocator_type& get_overflow_allocator() noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        void set_overflow_allocator(const overflow_allocator_type& /*allocator*/)
        {
            // У нас нет распределителя переполнения.
            CORSAC_ASSERT(false);
        }

        void copy_overflow_allocator(const this_type&)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            // У нас нет распределителя переполнения.
        }
    }; // fixed_hashtable_allocator

    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator==(const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a == &b); // Они равны, только если являются одним и тем же объектом.
    }


    template <size_t bucketCount, size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator!=(const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_hashtable_allocator<bucketCount, nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a != &b); // Они равны, только если являются одним и тем же объектом.
    }

    /**
    * fixed_vector_allocator
    *
    * Параметры шаблона:
    *     nodeSize               The size of individual objects.
    *     nodeCount              The number of objects the pool contains.
    *     nodeAlignment          The alignment of the objects to allocate.
    *     nodeAlignmentOffset    The alignment offset of the objects to allocate.
    *     bEnableOverflow        Whether or not we should use the overflow heap if our object pool is exhausted.
    *     OverflowAllocator      Overflow allocator, which is only used if bEnableOverflow == true. Defaults to the global heap.
    */
    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator = CORSAC_ALLOCATOR_TYPE>
    class fixed_vector_allocator
    {
    public:
        using this_type                 = fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof (T) как множественное alignof(T), и поэтому sizeof(T) всегда> = alignof(T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };

    public:
        overflow_allocator_type mOverflowAllocator;
        void*                   mpPoolBegin;         // На заметку: Найдите способ избавиться от этих данных без увеличения числа шаблонов.

    public:
        fixed_vector_allocator(void* pNodeBuffer = nullptr)
                : mpPoolBegin(pNodeBuffer)
        {}

        fixed_vector_allocator(void* pNodeBuffer, const overflow_allocator_type& allocator)
                : mOverflowAllocator(allocator), mpPoolBegin(pNodeBuffer)
        {}

        fixed_vector_allocator(const fixed_vector_allocator& x)
        {
            mpPoolBegin        = x.mpPoolBegin;
            mOverflowAllocator = x.mOverflowAllocator;
        }

        fixed_vector_allocator& operator=(const fixed_vector_allocator& x)
        {
            // Мы оставляем нашу переменную mpPoolBegin в покое.
            mOverflowAllocator = x.mOverflowAllocator;
            return *this;
        }

        void* allocate(size_t n, int flags = 0)
        {
            return mOverflowAllocator.allocate(n, flags);
        }

        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
        {
            return mOverflowAllocator.allocate(n, alignment, offset, flags);
        }

        void deallocate(void* p, size_t n)
        {
            if(p != mpPoolBegin)
                mOverflowAllocator.deallocate(p, n); // Невозможно сделать это с нашим собственным выделением.
        }

        const char* get_name() const
        {
            return mOverflowAllocator.get_name();
        }

        void set_name(const char* pName)
        {
            mOverflowAllocator.set_name(pName);
        }

        const overflow_allocator_type& get_overflow_allocator() const noexcept
        {
            return mOverflowAllocator;
        }

        overflow_allocator_type& get_overflow_allocator() noexcept
        {
            return mOverflowAllocator;
        }

        void set_overflow_allocator(const overflow_allocator_type& allocator)
        {
            mOverflowAllocator = allocator;
        }

        void copy_overflow_allocator(const this_type& x)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            mOverflowAllocator = x.mOverflowAllocator;
        }
    }; // fixed_vector_allocator

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, typename OverflowAllocator>
    class fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>
    {
    public:
        using this_type                 = fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, false, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;

        enum
        {
            kNodeSize            = nodeSize,
            kNodeCount           = nodeCount,
            kNodesSize           = nodeCount * nodeSize, // Обратите внимание, что расчет kBufferSize предполагает, что компилятор устанавливает sizeof (T) как множественное alignof(T), и поэтому sizeof(T) всегда> = alignof(T).
            kBufferSize          = kNodesSize + ((nodeAlignment > 1) ? nodeSize-1 : 0) + nodeAlignmentOffset,
            kNodeAlignment       = nodeAlignment,
            kNodeAlignmentOffset = nodeAlignmentOffset
        };
        fixed_vector_allocator()
        {}

        fixed_vector_allocator(void* /*pNodeBuffer*/)
        {}

        fixed_vector_allocator(void* /*pNodeBuffer*/, const overflow_allocator_type& /*allocator*/)  // распределитель не используется, потому что bEnableOverflow имеет значение false в этой специализации.
        {}

        void* allocate(size_t /*n*/, int /*flags*/ = 0)
        {
            CORSAC_ASSERT(false); // Fixed_vector не должен перераспределяться, иначе пользователь исчерпал свое пространство.
            return NULL;
        }

        void* allocate(size_t /*n*/, size_t /*alignment*/, size_t /*offset*/, int /*flags*/ = 0)
        {
            CORSAC_ASSERT(false);
            return NULL;
        }

        void deallocate(void* /*p*/, size_t /*n*/)
        {}

        const char* get_name() const
        {
            return CORSAC_FIXED_POOL_DEFAULT_NAME;
        }

        void set_name(const char* /*pName*/)
        {}

        const overflow_allocator_type& get_overflow_allocator() const noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        overflow_allocator_type& get_overflow_allocator() noexcept
        {
            CORSAC_ASSERT(false);
            overflow_allocator_type* pNULL = NULL;
            return *pNULL; // Это некрасиво, но никогда не должно выполняться. Это здесь только для того, чтобы это было скомпилировано.
        }

        void set_overflow_allocator(const overflow_allocator_type& /*allocator*/)
        {
            // У нас нет распределителя переполнения.
            CORSAC_ASSERT(false);
        }

        void copy_overflow_allocator(const this_type&)  // Эта функция существует, поэтому мы можем написать общий код, который работает для распределителей, которые имеют и не имеют распределителей переполнения.
        {
            // У нас нет распределителя переполнения.
        }

    }; // fixed_vector_allocator

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator==(const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a == &b); // Они равны, только если являются одним и тем же объектом.
    }

    template <size_t nodeSize, size_t nodeCount, size_t nodeAlignment, size_t nodeAlignmentOffset, bool bEnableOverflow, typename OverflowAllocator>
    inline bool operator!=(const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& a,
                           const fixed_vector_allocator<nodeSize, nodeCount, nodeAlignment, nodeAlignmentOffset, bEnableOverflow, OverflowAllocator>& b)
    {
        return (&a != &b); // Они равны, только если являются одним и тем же объектом.
    }

    /**
    * fixed_swap
    *
    * Эта функция реализует своп, подходящий для фиксированных контейнеров.
    * Это проблема, потому что размер фиксированных контейнеров может быть очень большим из-за того,
    * что буфер контейнера находится внутри них. Обратите внимание, что мы имеем в виду
    * sizeof (контейнер), а не общую сумму памяти, выделенной контейнером из кучи.
    */
    template <typename Container, bool UseHeapTemporary>
    class fixed_swap_impl
    {
    public:
        static void swap(Container& a, Container& b);
    };

    template <typename Container>
    class fixed_swap_impl<Container, false>
    {
    public:
        static void swap(Container& a, Container& b)
        {
            Container temp(corsac::move(a));
            a = corsac::move(b);
            b = corsac::move(temp);
        }
    };

    template <typename Container>
    class fixed_swap_impl<Container, true>
    {
    public:
        static void swap(Container& a, Container& b)
        {
            CORSAC_ALLOCATOR_TYPE allocator(*CORSAC_DEFAULT_ALLOCATOR(), CORSAC_TEMP_DEFAULT_NAME);
            void* const pMemory = allocator.allocate(sizeof(a));

            if(pMemory)
            {
                Container* pTemp = ::new(pMemory) Container(corsac::move(a));
                a = corsac::move(b);
                b = corsac::move(*pTemp);

                pTemp->~Container();
                allocator.deallocate(pMemory, sizeof(a));
            }
        }
    };

    template<typename Container>
    void fixed_swap(Container& a, Container& b)
    {
        return fixed_swap_impl<Container, sizeof(Container) >= CORSAC_MAX_STACK_USAGE>::swap(a, b);
    }
} // namespace corsac

CORSAC_RESTORE_VC_WARNING()

#endif //CORSAC_STL_FIXED_POOL_H
