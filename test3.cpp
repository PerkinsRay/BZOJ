#include <tuple>
#include <iostream>
#include <bitset>
template <size_t... Idx>
struct seq
{
};

template <size_t... Idx>
struct seq_gen;

template <size_t N, size_t... Idx>
struct seq_gen<N, Idx...> : seq_gen<N - 1, N - 1, Idx...>
{
};

template <size_t... Idx>
struct seq_gen<0, Idx...> : seq<Idx...>
{
};

template <size_t... Idx>
struct rseq_gen;

template <size_t N, size_t... Idx>
struct rseq_gen<N, Idx...> : rseq_gen<N - 1, Idx..., N - 1>
{
};

template <size_t... Idx>
struct rseq_gen<0, Idx...> : seq<Idx...>
{
};

template <class... Ts, size_t... Idx>
constexpr auto do_tuple_arrange_add(const std::tuple<Ts...> &tp, seq<Idx...>, size_t v)
{
    return std::make_tuple((std::get<Idx>(tp) + v)...);
}

template <class... Ts, class SEQ = rseq_gen<sizeof...(Ts)>>
constexpr auto tuple_reverse_add(const std::tuple<Ts...> &tp, size_t v)
{
    return do_tuple_arrange_add(tp, SEQ(), v);
}

template <int Size>
struct GrayCode
{
    static constexpr auto gc = std::tuple_cat(GrayCode<Size - 1>::gc, tuple_reverse_add(GrayCode<Size - 1>::gc, 1 << (Size - 1)));
};

template <>
struct GrayCode<1>
{
    static constexpr auto gc = std::make_tuple(0UL, 1UL);
};

template <class... Ts, size_t... Idx>
std::ostream &PrintTuple(std::ostream &s, const std::tuple<Ts...> &tp, seq<Idx...>)
{
    auto l = {(s << std::bitset<8>(std::get<Idx>(tp)) << "\n", 0)...};
    return s;
}

template <class... Ts, class SEQ = seq_gen<sizeof...(Ts)>>
std::ostream &operator<<(std::ostream &s, const std::tuple<Ts...> &tp)
{
    return PrintTuple(s, tp, SEQ());
}

#include <array>

template <class... Ts, size_t... Idx>
constexpr std::array<size_t,sizeof...(Idx)> do_toArray(const std::tuple<Ts...> &T, seq<Idx...>)
{
    return {std::get<Idx>(T)...};
}

template <class... Ts>
constexpr const std::array<size_t,sizeof...(Ts)> toArray(const std::tuple<Ts...> &T)
{
    return do_toArray(T, seq_gen<sizeof...(Ts)>());
}


constexpr bool equal(size_t  a,size_t  b){
    return a==b;
}


template<size_t I>
auto constexpr make_gc(typename std::enable_if<equal(I,1),bool>::type=false){
    return std::make_tuple(0UL,1UL);
}
template<size_t I>
auto constexpr make_gc(typename std::enable_if<!equal(I,1),bool>::type=false){
    return std::tuple_cat(make_gc<I - 1>(), tuple_reverse_add(make_gc<I  - 1>(), 1 << (I - 1)));
}


int main()
{
   constexpr auto x= make_gc<8>();
   std::cout<<x;
}
