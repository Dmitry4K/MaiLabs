
#include<iostream>
#include<type_traits>
#include<tuple>
template <typename T1, typename T2, typename = std::void_t<>> struct IsSummable : std::false_type {};
template <typename T1, typename T2> struct IsSummable<T1, T2, std::void_t<decltype(T1() + T2())>> : std::true_type {};

template <typename...> struct IsSummable_v;

template <typename T1>
struct IsSummable_v<T1>
{
    static const bool value = IsSummable<T1, T1>::value;
};

template <typename T1, typename T2, typename... TT>
struct IsSummable_v<T1, T2, TT...>
{
    static const bool value = IsSummable<T1, T2>::value && IsSummable_v<T2, TT...>::value;
};

template <typename... TT> struct IsSummable_v<std::tuple<TT...>> : IsSummable_v<TT...> {};

static_assert(IsSummable_v<int, double, char>::value);
static_assert(IsSummable_v<std::tuple<int, double, char>>::value);
static_assert(!IsSummable_v<int, std::string>::value);
static_assert(!IsSummable_v<std::tuple<int, std::string>>::value);

int main() {
    return 0;
}
