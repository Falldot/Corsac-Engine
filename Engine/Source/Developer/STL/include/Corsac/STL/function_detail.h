//
// Created by Falldot on 16.11.2021.
//

#ifndef CORSAC_FUNCTION_DETAIL_H
#define CORSAC_FUNCTION_DETAIL_H

#pragma once

#include "Corsac/Base/base.h"

#include "Corsac/STL/config.h"
#include "Corsac/STL/functional_base.h"
#include "Corsac/STL/help_move.h"

#include "Corsac/type_traits.h"
//#include "Corsac/utility.h"
#include "Corsac/allocator.h"

#if CORSAC_RTTI_ENABLED
    #include <typeinfo>
#endif

namespace corsac
{
    class bad_function_call : public std::exception
    {
    public:
        bad_function_call() noexcept = default;

        const char* what() const noexcept override
        {
            return "bad function_detail call";
        }
    };

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

        class unused_class {};

        union functor_storage_alignment
        {
            void (*unused_func_ptr)(void);
            void (unused_class::*unused_func_mem_ptr)(void);
            void* unused_ptr;
        };

        template <int SIZE_IN_BYTES>
        struct functor_storage
        {
            static_assert(SIZE_IN_BYTES >= 0, "local buffer storage cannot have a negative size!");
            template <typename Ret>
            Ret& GetStorageTypeRef() const
            {
                return *reinterpret_cast<Ret*>(const_cast<char*>(&storage[0]));
            }

            union
            {
                functor_storage_alignment align;
                char storage[SIZE_IN_BYTES];
            };
        };

        template <>
        struct functor_storage<0>
        {
            template <typename Ret>
            Ret& GetStorageTypeRef() const
            {
                return *reinterpret_cast<Ret*>(const_cast<char*>(&storage[0]));
            }

            union
            {
                functor_storage_alignment align;
                char storage[sizeof(functor_storage_alignment)];
            };
        };

        template <typename Functor, int SIZE_IN_BYTES>
        struct is_functor_inplace_allocatable
        {
            static constexpr bool value =
                    sizeof(Functor) <= sizeof(functor_storage<SIZE_IN_BYTES>) &&
                    (corsac::alignment_of_v<functor_storage<SIZE_IN_BYTES>> % corsac::alignment_of_v<Functor>) == 0;
        };


        /// function_base_detail
        template <int SIZE_IN_BYTES>
        class function_base_detail
        {
        public:
            using FunctorStorageType = functor_storage<SIZE_IN_BYTES>;
            FunctorStorageType mStorage;

            enum ManagerOperations : int
            {
                MGROPS_DESTRUCT_FUNCTOR = 0,
                MGROPS_COPY_FUNCTOR = 1,
                MGROPS_MOVE_FUNCTOR = 2,
                #if CORSAC_RTTI_ENABLED
                    MGROPS_GET_TYPE_INFO = 3,
                    MGROPS_GET_FUNC_PTR = 4,
                #endif
            };

            // Functor can be allocated inplace
            template <typename Functor, typename = void>
            class function_manager_base
            {
            public:

                static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept
                {
                    return &(storage.template GetStorageTypeRef<Functor>());
                }

                template <typename T>
                static void CreateFunctor(FunctorStorageType& storage, T&& functor)
                {
                    ::new (GetFunctorPtr(storage)) Functor(corsac::forward<T>(functor));
                }

                static void DestructFunctor(FunctorStorageType& storage)
                {
                    GetFunctorPtr(storage)->~Functor();
                }

                static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from)
                {
                    ::new (GetFunctorPtr(to)) Functor(*GetFunctorPtr(from));
                }

                static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from) noexcept
                {
                    ::new (GetFunctorPtr(to)) Functor(corsac::move(*GetFunctorPtr(from)));
                }

                static void* Manager(void* to, void* from, typename function_base_detail::ManagerOperations ops) noexcept
                {
                    switch (ops)
                    {
                        case MGROPS_DESTRUCT_FUNCTOR:
                        {
                            DestructFunctor(*static_cast<FunctorStorageType*>(to));
                        }
                            break;
                        case MGROPS_COPY_FUNCTOR:
                        {
                            CopyFunctor(*static_cast<FunctorStorageType*>(to),
                                        *static_cast<const FunctorStorageType*>(from));
                        }
                            break;
                        case MGROPS_MOVE_FUNCTOR:
                        {
                            MoveFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
                            DestructFunctor(*static_cast<FunctorStorageType*>(from));
                        }
                            break;
                        default:
                            break;
                    }
                    return nullptr;
                }
            };

            template <typename Functor>
            class function_manager_base<Functor, typename corsac::enable_if<!is_functor_inplace_allocatable<Functor, SIZE_IN_BYTES>::value>::type>
            {
            public:
                static Functor* GetFunctorPtr(const FunctorStorageType& storage) noexcept
                {
                    return storage.template GetStorageTypeRef<Functor*>();
                }

                static Functor*& GetFunctorPtrRef(const FunctorStorageType& storage) noexcept
                {
                    return storage.template GetStorageTypeRef<Functor*>();
                }

                template <typename T>
                static void CreateFunctor(FunctorStorageType& storage, T&& functor)
                {
                    allocator allocator = corsac::allocator();
                    Functor* func = static_cast<Functor*>(allocator.allocate(sizeof(Functor), alignof(Functor), 0));

                    ::new (static_cast<void*>(func)) Functor(corsac::forward<T>(functor));
                    GetFunctorPtrRef(storage) = func;
                }

                static void DestructFunctor(FunctorStorageType& storage)
                {
                    Functor* func = GetFunctorPtr(storage);
                    if (func)
                    {
                        allocator allocator = corsac::allocator();
                        func->~Functor();
                        allocator.deallocate(static_cast<void*>(func), sizeof(Functor));
                    }
                }

                static void CopyFunctor(FunctorStorageType& to, const FunctorStorageType& from)
                {
                    allocator allocator = corsac::allocator();
                    Functor* func = static_cast<Functor*>(allocator.allocate(sizeof(Functor), alignof(Functor), 0));

                    ::new (static_cast<void*>(func)) Functor(*GetFunctorPtr(from));
                    GetFunctorPtrRef(to) = func;
                }

                static void MoveFunctor(FunctorStorageType& to, FunctorStorageType& from) noexcept
                {
                    Functor* func = GetFunctorPtr(from);
                    GetFunctorPtrRef(to) = func;
                    GetFunctorPtrRef(from) = nullptr;
                }

                static void* Manager(void* to, void* from, typename function_base_detail::ManagerOperations ops) noexcept
                {
                    switch (ops)
                    {
                        case MGROPS_DESTRUCT_FUNCTOR:
                        {
                            DestructFunctor(*static_cast<FunctorStorageType*>(to));
                        }
                            break;
                        case MGROPS_COPY_FUNCTOR:
                        {
                            CopyFunctor(*static_cast<FunctorStorageType*>(to),
                                        *static_cast<const FunctorStorageType*>(from));
                        }
                            break;
                        case MGROPS_MOVE_FUNCTOR:
                        {
                            MoveFunctor(*static_cast<FunctorStorageType*>(to), *static_cast<FunctorStorageType*>(from));
                        }
                            break;
                        default:
                            break;
                    }
                    return nullptr;
                }
            };

            template <typename Functor, typename R, typename... Args>
            class function_manager final : public function_manager_base<Functor>
            {
            public:
                using Base = function_manager_base<Functor>;

            #if CORSAC_RTTI_ENABLED
                static void* GetTypeInfo() noexcept
				{
					return reinterpret_cast<void*>(const_cast<std::type_info*>(&typeid(Functor)));
				}

				static void* Manager(void* to, void* from, typename function_base_detail::ManagerOperations ops) noexcept
				{
					switch (ops)
					{
						case MGROPS_GET_TYPE_INFO:
						{
							return GetTypeInfo();
						}
						break;
						case MGROPS_GET_FUNC_PTR:
						{
							return static_cast<void*>(Base::GetFunctorPtr(*static_cast<FunctorStorageType*>(to)));
						}
						break;
						default:
						{
							return Base::Manager(to, from, ops);
						}
						break;
					}
				}
                #endif // corsac_RTTI_ENABLED

                /**
                 * NOTE:
                 *
                 * The order of arguments here is vital to the call optimization. Let's dig into why and look at some asm.
                 * We have two invoker signatures to consider:
                 *   R Invoker(const FunctorStorageType& functor, Args... args)
                 *   R Invoker(Args... args, const FunctorStorageType& functor)
                 *
                 * Assume we are using the Windows x64 Calling Convention where the first 4 arguments are passed into
                 * RCX, RDX, R8, R9. This optimization works for any Calling Convention, we are just using Windows x64 for
                 * this example.
                 *
                 * Given the following member function: void TestMemberFunc(int a, int b)
                 *  RCX == this
                 *  RDX == a
                 *  R8  == b
                 *
                 * All three arguments to the function including the hidden this pointer, which in C++ is always the first argument
                 * are passed into the first three registers.
                 * The function call chain for corsac::function<>() is as follows:
                 *  operator ()(this, Args... args) -> Invoker(Args... args, this->mStorage) -> StoredFunction(Args... arg)
                 *
                 * Let's look at what is happening at the asm level with the different Invoker function signatures and why.
                 *
                 * You will notice that operator ()() and Invoker() have the arguments reversed. operator ()() just directly calls
                 * to Invoker(), it is a tail call, so we force inline the call operator to ensure we directly call to the Invoker().
                 * Most compilers always inline it anyways by default; have been instances where it doesn't even though the asm ends
                 * up being cheaper.
                 * call -> call -> call versus call -> call
                 *
                 * corsac::function<int(int, int)> = FunctionPointer
                 *
                 * Assume we have the above corsac::function object that holds a pointer to a function as the internal callable.
                 *
                 * Invoker(this->mStorage, Args... args) is called with the follow arguments in registers:
                 *  RCX = this  |  RDX = a  |  R8 = b
                 *
                 * Inside Invoker() we use RCX to deference into the corsac::function object and get the function pointer to call.
                 * This function to call has signature Func(int, int) and thus requires its arguments in registers RCX and RDX.
                 * The compiler must shift all the arguments towards the left. The full asm looks something as follows.
                 *
                 * Calling Invoker:                       Inside Invoker:
                 *
                 * mov rcx, this                          mov rax, [rcx]
                 * mov rdx, a                             mov rcx, rdx
                 * mov r8, b                              mov rdx, r8
                 * call [rcx + offset to Invoker]         jmp [rax]
                 *
                 * Notice how the compiler shifts all the arguments before calling the callable and also we only use the this pointer
                 * to access the internal storage inside the corsac::function object.
                 *
                 * Invoker(Args... args, this->mStorage) is called with the following arguments in registers:
                 *  RCX = a  |  RDX = b  |  R8 = this
                 *
                 * You can see we no longer have to shift the arguments down when going to call the internal stored callable.
                 *
                 * Calling Invoker:                      Inside Invoker:
                 *
                 * mov rcx, a                            mov rax, [r8]
                 * mov rdx, b                            jmp [rax]
                 * mov r8, this
                 * call [r8 + offset to Invoker]
                 *
                 * The generated asm does a straight tail jmp to the loaded function pointer. The arguments are already in the correct
                 * registers.
                 *
                 * For Functors or Lambdas with no captures, this gives us another free register to use to pass arguments since the this
                 * is at the end, it can be passed onto the stack if we run out of registers. Since the callable has no captures; inside
                 * the Invoker(), we won't ever need to touch this thus we can just call the operator ()() or let the compiler inline it.
                 *
                 * For a callable with captures there is no perf hit since the callable in the common case is inlined and the pointer to the callable
                 * buffer is passed in a register which the compiler can use to access the captures.
                 *
                 * For corsac::function<void(const T&, int, int)> that a holds a pointer to member function. The this pointers is implicitly
                 * the first argument in the argument list, const T&, and the member function pointer will be called on that object.
                 * This prevents any argument shifting since the this for the member function pointer is already in RCX.
                 *
                 * This is why having this at the end of the argument list is important for generating efficient Invoker() thunks.
                 */
                static R Invoker(Args... args, const FunctorStorageType& functor)
                {
                    return corsac::invoke(*Base::GetFunctorPtr(functor), corsac::forward<Args>(args)...);
                }
            };

            function_base_detail() noexcept = default;
            ~function_base_detail() noexcept = default;
        };

        #define CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(FUNCTOR, RET, ARGS, BASE, MYSELF)  \
                    typename corsac::enable_if_t<corsac::is_invocable_r_v<RET, FUNCTOR, ARGS> &&         \
                                                !corsac::is_base_of_v<BASE, corsac::decay_t<FUNCTOR>> && \
                                                !corsac::is_same_v<corsac::decay_t<FUNCTOR>, MYSELF>>

        #define CORSAC_INTERNAL_FUNCTION_DETAIL_VALID_FUNCTION_ARGS(FUNCTOR, RET, ARGS, MYSELF) \
                    CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(FUNCTOR, RET, ARGS, MYSELF, MYSELF)


        /// function_detail
        ///
        template <int, typename>
        class function_detail;

        template <int SIZE_IN_BYTES, typename R, typename... Args>
        class function_detail<SIZE_IN_BYTES, R(Args...)> : public function_base_detail<SIZE_IN_BYTES>
        {
        public:
            using result_type = R;

        protected:
            using Base = function_base_detail<SIZE_IN_BYTES>;
            using FunctorStorageType = typename function_base_detail<SIZE_IN_BYTES>::FunctorStorageType;
            using Base::mStorage;

        public:
            function_detail() noexcept = default;
            function_detail(std::nullptr_t) noexcept {}

            function_detail(const function_detail& other)
            {
                if (this != &other)
                {
                    Copy(other);
                }
            }

            function_detail(function_detail&& other)
            {
                if (this != &other)
                {
                    Move(corsac::move(other));
                }
            }

            template <typename Functor, typename = CORSAC_INTERNAL_FUNCTION_DETAIL_VALID_FUNCTION_ARGS(Functor, R, Args..., function_detail)>
            function_detail(Functor functor)
            {
                CreateForwardFunctor(corsac::move(functor));
            }

            ~function_detail() noexcept
            {
                Destroy();
            }

            function_detail& operator=(const function_detail& other)
            {
                if (this != &other)
                {
                    Destroy();
                    Copy(other);
                }

                return *this;
            }

            function_detail& operator=(function_detail&& other)
            {
                if(this != &other)
                {
                    Destroy();
                    Move(corsac::move(other));
                }

                return *this;
            }

            function_detail& operator=(std::nullptr_t) noexcept
            {
                Destroy();
                mMgrFuncPtr = nullptr;
                mInvokeFuncPtr = &DefaultInvoker;

                return *this;
            }

            template <typename Functor, typename = CORSAC_INTERNAL_FUNCTION_DETAIL_VALID_FUNCTION_ARGS(Functor, R, Args..., function_detail)>
            function_detail& operator=(Functor&& functor)
            {
                Destroy();
                CreateForwardFunctor(corsac::forward<Functor>(functor));
                return *this;
            }

            template <typename Functor>
            function_detail& operator=(corsac::reference_wrapper<Functor> f) noexcept
            {
                Destroy();
                CreateForwardFunctor(f);
                return *this;
            }

            void swap(function_detail& other) noexcept
            {
                if(this == &other)
                    return;

                FunctorStorageType tempStorage;
                if (other.HaveManager())
                {
                    (void)(*other.mMgrFuncPtr)(static_cast<void*>(&tempStorage), static_cast<void*>(&other.mStorage),
                                               Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
                }

                if (HaveManager())
                {
                    (void)(*mMgrFuncPtr)(static_cast<void*>(&other.mStorage), static_cast<void*>(&mStorage),
                                         Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
                }

                if (other.HaveManager())
                {
                    (void)(*other.mMgrFuncPtr)(static_cast<void*>(&mStorage), static_cast<void*>(&tempStorage),
                                               Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
                }

                corsac::swap(mMgrFuncPtr, other.mMgrFuncPtr);
                corsac::swap(mInvokeFuncPtr, other.mInvokeFuncPtr);
            }

            explicit operator bool() const noexcept
            {
                return HaveManager();
            }

            constexpr R operator ()(Args... args) const
            {
                return (*mInvokeFuncPtr)(corsac::forward<Args>(args)..., this->mStorage);
            }

        #if CORSAC_RTTI_ENABLED
            const std::type_info& target_type() const noexcept
				{
					if (HaveManager())
					{
						void* ret = (*mMgrFuncPtr)(nullptr, nullptr, Base::ManagerOperations::MGROPS_GET_TYPE_INFO);
						return *(static_cast<const std::type_info*>(ret));
					}
					return typeid(void);
				}

				template <typename Functor>
				Functor* target() noexcept
				{
					if (HaveManager() && target_type() == typeid(Functor))
					{
						void* ret = (*mMgrFuncPtr)(static_cast<void*>(&mStorage), nullptr,
												   Base::ManagerOperations::MGROPS_GET_FUNC_PTR);
						return ret ? static_cast<Functor*>(ret) : nullptr;
					}
					return nullptr;
				}

				template <typename Functor>
				const Functor* target() const noexcept
				{
					if (HaveManager() && target_type() == typeid(Functor))
					{
						void* ret = (*mMgrFuncPtr)(static_cast<void*>(&mStorage), nullptr,
												   Base::ManagerOperations::MGROPS_GET_FUNC_PTR);
						return ret ? static_cast<const Functor*>(ret) : nullptr;
					}
					return nullptr;
				}
        #endif // CORSAC_RTTI_ENABLED

        private:
            bool HaveManager() const noexcept
            {
                return (mMgrFuncPtr != nullptr);
            }

            void Destroy() noexcept
            {
                if (HaveManager())
                {
                    (void)(*mMgrFuncPtr)(static_cast<void*>(&mStorage), nullptr,
                                         Base::ManagerOperations::MGROPS_DESTRUCT_FUNCTOR);
                }
            }

            void Copy(const function_detail& other)
            {
                if (other.HaveManager())
                {
                    (void)(*other.mMgrFuncPtr)(static_cast<void*>(&mStorage),
                                               const_cast<void*>(static_cast<const void*>(&other.mStorage)),
                                               Base::ManagerOperations::MGROPS_COPY_FUNCTOR);
                }

                mMgrFuncPtr = other.mMgrFuncPtr;
                mInvokeFuncPtr = other.mInvokeFuncPtr;
            }

            void Move(function_detail&& other)
            {
                if (other.HaveManager())
                {
                    (void)(*other.mMgrFuncPtr)(static_cast<void*>(&mStorage), static_cast<void*>(&other.mStorage),
                                               Base::ManagerOperations::MGROPS_MOVE_FUNCTOR);
                }

                mMgrFuncPtr = other.mMgrFuncPtr;
                mInvokeFuncPtr = other.mInvokeFuncPtr;
                other.mMgrFuncPtr = nullptr;
                other.mInvokeFuncPtr = &DefaultInvoker;
            }

            template <typename Functor>
            void CreateForwardFunctor(Functor&& functor)
            {
                using DecayedFunctorType = typename corsac::decay<Functor>::type;
                using FunctionManagerType = typename Base::template function_manager<DecayedFunctorType, R, Args...>;

                if (internal::is_null(functor))
                {
                    mMgrFuncPtr = nullptr;
                    mInvokeFuncPtr = &DefaultInvoker;
                }
                else
                {
                    mMgrFuncPtr = &FunctionManagerType::Manager;
                    mInvokeFuncPtr = &FunctionManagerType::Invoker;
                    FunctionManagerType::CreateFunctor(mStorage, corsac::forward<Functor>(functor));
                }
            }

        private:
            using ManagerFuncPtr = void* (*)(void*, void*, typename Base::ManagerOperations);
            using InvokeFuncPtr = R (*)(Args..., const FunctorStorageType&);

            // Мы не можем предполагать, что R конструктивно по умолчанию.
            // Эта функция вызывается только тогда, когда объект функции НЕ МОЖЕТ быть вызван, потому что он пуст,
            // он всегда будет генерировать или утверждать, поэтому мы никогда не будем использовать возвращаемое значение в любом случае, и вызывающий не должен.
            static R DefaultInvoker(Args... /*args*/, const FunctorStorageType& /*functor*/)
            {};

            ManagerFuncPtr mMgrFuncPtr = nullptr;
            InvokeFuncPtr mInvokeFuncPtr = &DefaultInvoker;
        };

    } // namespace internal
}

#endif //CORSAC_FUNCTION_DETAIL_H
