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

constexpr size_t TSIZE=8;

template <size_t N, bool S = N <= TSIZE>
class GrayCodeGen;

template <size_t N>
class GrayCodeGen<N, true>
{
    bool reverse;
    typename std::array<size_t,1<<N>::const_iterator itr;
    typename std::array<size_t,1<<N>::const_reverse_iterator ritr;
    static const typename std::array<size_t,1<<N> code ;

  public:
    GrayCodeGen() : itr(code.begin()),ritr(code.rbegin()),reverse(false)
    {
    }
    auto Next()
    {
        if (reverse)
        {
            if (ritr != code.rend())
            {
                return std::make_tuple(false, *(ritr++));
            }
            else
            {
                reverse=false;
                itr=code.begin();
                return std::make_tuple(true, *(itr++));
            }
        }
        else
        {
            if (itr != code.end())
            {
                return std::make_tuple(false, *(itr++));
            }
            else
            {
                reverse=true;
                ritr=code.rbegin();
                return std::make_tuple(true, *(ritr++));
            }
        }
    }
};
template <size_t N>
const typename std::array<size_t,1<<N> GrayCodeGen<N, true>::code = toArray(GrayCode<N>::gc);

template <size_t N>
class GrayCodeGen<N, false>:protected GrayCodeGen<N-TSIZE>
{
    bool reverse;
    typename std::array<size_t,1<<TSIZE>::const_iterator itr;
    typename std::array<size_t,1<<TSIZE>::const_reverse_iterator ritr;
    static const typename std::array<size_t,1<<TSIZE> code ;

  public:
    GrayCodeGen() : itr(code.begin()),ritr(code.rbegin()),reverse(false)
    {
    }
    size_t Current(){
        if (reverse)
        {
            return *ritr<<(N-TSIZE);
        }
        else
        {
            return *itr<<(N-TSIZE);
        }
    }

    auto NextInternal()
    {
        if (reverse)
        {
            if (ritr+1 != code.rend())
            {
                return std::make_tuple(false, *(++ritr)<<(N-TSIZE));
            }
            else
            {
                reverse=false;
                itr=code.begin();
                return std::make_tuple(true, *(itr)<<(N-TSIZE));
            }
        }
        else
        {
            if (itr+1 != code.end())
            {
                return std::make_tuple(false, *(++itr)<<(N-TSIZE));
            }
            else
            {
                reverse=true;
                ritr=code.rbegin();
                return std::make_tuple(true, *(ritr)<<(N-TSIZE));
            }
        }
    }
    auto Next()
    {
        size_t Val;
        bool II=false;
        auto NextSub= GrayCodeGen<N-TSIZE>::Next();

        if(std::get<0>(NextSub)){
            std::tie(II,Val)=NextInternal();
        }else{
            Val=Current();
        }
        return std::make_tuple(II,Val|std::get<1>(NextSub));
    }
};
template <size_t N>
const typename std::array<size_t,1<<TSIZE> GrayCodeGen<N, false>::code = toArray(GrayCode<TSIZE>::gc);

int main()
{
    constexpr int N=30;
    GrayCodeGen<N> gc;
    for (int i=0;i<(1<<N);i++)
    {
        auto xx=gc.Next();
        //std::cout<< std::bitset<N>(std::get<1>(xx)) << std::endl;
    }
}
