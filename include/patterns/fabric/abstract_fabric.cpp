#include "abstract_fabric.h"
#include <memory>
#include <cassert>

#define TYPE_ASSERT(variable, type) \
    static_assert(std::is_same<decltype(variable), type>::value, \
        "Type should be "#type)

struct IUniqueProduct {
    virtual ~IUniqueProduct() = default;
};

struct ISharedProduct {
    using ret_type = std::shared_ptr<ISharedProduct>;

    virtual ~ISharedProduct() = default;
};

struct IRawProduct {
    using ret_type = IRawProduct *;
    using ctor_args = tl<bool, int>;

    virtual ~IRawProduct() = default;
};

template<typename T>
struct IValueProduct {
    using ret_type = T;
    using ctor_args = tl<T>;
};

template<int N>
struct IPrototypeProduct {
    using prototype_t = std::unique_ptr<IPrototypeProduct>;

    virtual prototype_t Clone() = 0;

    virtual ~IPrototypeProduct() = default;
};

// existing product that you don't want to change
// and that should be created as shared_ptr
struct IExistingSharedProduct {
    virtual ~IExistingSharedProduct() = default;
};

struct ExistingSharedProduct : public IExistingSharedProduct {
};

template<int N>
struct PrototypeProduct : public IPrototypeProduct<N> {
    using abstract_t = IPrototypeProduct<N>;

    typename abstract_t::prototype_t Clone() override {
        return typename abstract_t::prototype_t{new PrototypeProduct()};
    }
};

struct UniqueProduct : public IUniqueProduct {
};
struct SharedProduct : public ISharedProduct {
};

struct RawProduct : public IRawProduct {
    RawProduct(bool, int) {
    }
};

using IIntValue = IValueProduct<int>;
using IFloatValue = IValueProduct<float>;
using PrototypeProductA = PrototypeProduct<0>;
using PrototypeProductB = PrototypeProduct<1>;
using IExistingFactoryProduct = make_factory_interface<
        IExistingSharedProduct, std::shared_ptr<IExistingSharedProduct>
>;

template<typename T, typename =    void_t<>>
struct has_prototype : public std::false_type {
};

template<typename T>
struct has_prototype<T, void_t<typename T::prototype_t>> : public std::true_type {
};

#if __cplusplus >= 201402L
template<typename T>
constexpr bool has_prototype_v = has_prototype<T>::value;
#endif

template<typename...>
struct is_any_of;

template<typename Target, typename T, typename... Ts>
struct is_any_of<Target, tl<T, Ts...>> : public is_any_of<Target, tl<Ts...>> {
};

template<typename Target, typename... Ts>
struct is_any_of<Target, tl<Target, Ts...>> : public std::true_type {
};

template<typename Target>
struct is_any_of<Target, tl<>> : public std::false_type {
};

#if __cplusplus >= 201402L
template<typename... Ts>
constexpr bool is_any_of_v = is_any_of<Ts...>::value;
#endif

template<typename Context, typename Concrete, typename Base, typename Enabled = void>
class CustomConcreteCreator
        : public default_concrete_creator<Context, Concrete, Base> {
};

template<typename Abstract, typename Concrete, typename Base, typename Ret, typename Arg>
class CustomConcreteCreator<tl<Abstract, Ret, tl<Arg>>, Concrete, Base,
        typename std::enable_if<is_any_of<Abstract, tl<IIntValue, IFloatValue>>::value>::type>
        : public Base {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

    Ret create(type_identity<Abstract>, Arg arg) override {
        return arg;
    }

#ifdef __clang__
#pragma clang diagnostic pop
#endif
};

template<typename Abstract, typename Concrete, typename Base, typename Ret, typename... Args>
class CustomConcreteCreator<tl<Abstract, Ret, tl<Args...>>, Concrete, Base,
        typename std::enable_if<has_prototype<Abstract>::value>::type>
        : public Base {
public:
    friend void SetPrototype(CustomConcreteCreator &self, typename Abstract::prototype_t newPrototype) {
        self.prototype = std::move(newPrototype);
    }

private:
    typename Abstract::prototype_t prototype{};
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

    Ret create(type_identity<Abstract>, Args...) override {
        return prototype->Clone();
    }

#ifdef __clang__
#pragma clang diagnostic pop
#endif
};

using AFactory = abstract_factory<
        tl<
                IUniqueProduct, ISharedProduct, IRawProduct,
                IIntValue, IFloatValue,
                PrototypeProductA::abstract_t, PrototypeProductB::abstract_t,
                IExistingFactoryProduct
        >
>;

using CFactory = concrete_factory<AFactory, tl<
        UniqueProduct, SharedProduct, RawProduct,
        IIntValue, IFloatValue,
        PrototypeProductA::abstract_t, PrototypeProductB::abstract_t,
        ExistingSharedProduct
>,
        CustomConcreteCreator
>;
