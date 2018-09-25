#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <cstddef>
#include <type_traits>
#include <utility>

template <int... Is>
struct seq
{
};

template <int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>
{
};

template <int... Is>
struct gen_seq<0, Is...> : seq<Is...>
{
};

template <int N, int... Is>
struct gen_seq_r : gen_seq_r<N - 1, Is..., N - 1>
{
};

template <int... Is>
struct gen_seq_r<0, Is...> : seq<Is...>
{
};

template <class T>
constexpr auto v_sum(T &&v)
{
    return v;
}
template <class T, class... Ts>
constexpr auto v_sum(T &&val, Ts... vals)
{
    return val + v_sum(vals...);
}

template <class TP, int... Is>
constexpr auto do_sum(TP &&tp, seq<Is...>);

template <class... Ts, int... Is>
constexpr auto do_sum(const std::tuple<Ts...> &tp, seq<Is...>)
{
    return v_sum(std::get<Is>(tp)...);
}

template <typename... Ts, typename S = gen_seq<sizeof...(Ts)>>
constexpr auto sum(const std::tuple<Ts...> &tp)
{
    return do_sum(tp, S());
}

template <class... Ts, int... Is>
constexpr void call_each_helper(const std::tuple<Ts...> &tp, const seq<Is...>)
{
    auto l = {(std::get<Is>(tp)(), 0)...};
}

template <class... Ts, class seq = gen_seq<sizeof...(Ts)>>
constexpr void call_each(const std::tuple<Ts...> &tp)
{
    call_each_helper(tp, seq());
}

template <class... Ts>
struct Caller
{
    const std::tuple<Ts...> callbacks;
    constexpr Caller(Ts... ts) : callbacks(std::forward_as_tuple(ts...))
    {
    }
    constexpr void Call()
    {
        call_each(callbacks);
    }
};

template <class... Ts>
constexpr auto make_caller(Ts... callbacks)
{
    return Caller<Ts...>(std::forward<Ts>(callbacks)...);
}

class output
{
    int num;

  public:
    output(int n) : num(n)
    {
    }
    void p()
    {
        std::cout << num << std::endl;
    }
};
#include <algorithm>
#include <bitset>
#include <array>

template <int... Indices>
struct indices
{
    using next = indices<Indices..., sizeof...(Indices)>;
};

template <int Size>
struct build_indices
{
    using type = typename build_indices<Size - 1>::type::next;
};

template <>
struct build_indices<0>
{
    using type = indices<>;
};

template <typename T>
using Bare = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <typename Tuple>
constexpr
    typename build_indices<std::tuple_size<Bare<Tuple>>::value>::type
    make_indices()
{
    return {};
}

template <typename Tuple, int... Indices>
constexpr std::array<
    typename std::tuple_element<0, Bare<Tuple>>::type,
    std::tuple_size<Bare<Tuple>>::value>
to_array(Tuple &&tuple, indices<Indices...>)
{
    using std::get;
    return {{get<Indices>(std::forward<Tuple>(tuple))...}};
}

template <typename Tuple>
constexpr auto to_array(Tuple &&tuple)
    -> decltype(to_array(std::declval<Tuple>(), make_indices<Tuple>()))
{
    return to_array(std::forward<Tuple>(tuple), make_indices<Tuple>());
}
template <template <typename...> class T, class... Ts, int... Is>
constexpr auto do_reverse(const T<Ts...> &tp, seq<Is...>)
{
    return std::make_tuple(std::get<Is>(tp)...);
}

template <template <typename...> class T, typename... Ts, typename S = gen_seq_r<sizeof...(Ts)>>
constexpr auto reverse(const T<Ts...> &tp)
{
    return do_reverse(tp, S());
}

template <template <typename...> class T, template <typename...> class T2, class... Ts, class... Ts2, int... Is, int... Is2>
constexpr auto do_cat(const T<Ts...> &tp, const T2<Ts2...> &tp2, seq<Is...>, seq<Is2...>)
{
    return std::make_tuple(std::get<Is>(tp)..., (std::get<Is2>(tp2) + sizeof...(Ts2))...);
}

template <template <typename...> class T, template <typename...> class T2, typename... Ts, typename... Ts2, typename S = gen_seq<sizeof...(Ts)>, typename S2 = gen_seq<sizeof...(Ts2)>>
constexpr auto cat(const T<Ts...> &tp, const T2<Ts2...> &tp2)
{
    return do_cat(tp, tp2, S(), S2());
}

template <int I>
struct gc
{
    constexpr static decltype(cat(gc<I - 1>::codes, reverse(gc<I - 1>::codes))) codes = cat(gc<I - 1>::codes, reverse(gc<I - 1>::codes));

};

template <>
struct gc<1>
{
    constexpr static auto codes = std::make_tuple(0ULL, 1ULL);
};

template<int I> constexpr decltype(cat(gc<I - 1>::codes, reverse(gc<I - 1>::codes)))  gc<I>::codes;

template <int N>
struct gcArry : gc<N>
{
    constexpr static auto codes = to_array(gc<N>::codes);
    using arrT=decltype(codes);
};

template<int T,int B>
struct Greater{
    constexpr static bool v= T>B;
};
template <int N, bool  R=Greater<N,8>::v >
class GrayCodeGenN;

template <int N>
class GrayCodeGenN<N,true> : GrayCodeGenN<N - 8>
{
    bool r;
    static const typename gcArry<N>::arrT arry;
    int idx;
    static const int size;
  public:
    GrayCodeGenN() : r(false),idx(0) {}
    unsigned long long getNext(bool& end){
        end=false;
        bool in_end;
        auto ret=(arry[idx]<<8)+  GrayCodeGenN<N - 8>::getNext(in_end);
        if(in_end)
            if(!r){
                idx++;
                if(idx==size){
                    end=r=true;
                    idx--;
                }

            }else{
                idx--;
                if(idx<0){
                    end=true;
                    r=false;
                    idx=0;
                }
            }
        return ret;
    }
};

template <int N>
const typename gcArry<N>::arrT GrayCodeGenN<N,true>::arry = gcArry<8>::codes;
template <int N>
const int GrayCodeGenN<N,true>::size = gcArry<8>::codes.size();

template <int N>
class GrayCodeGenN<N,false>
{
    bool r;
    static const typename gcArry<N>::arrT arry;
    int idx;
    static const int size;
  public:
    GrayCodeGenN() : r(false),idx(0) {}
    unsigned long long getNext(bool& end){
        auto ret=arry[idx];
        end=false;
        if(!r){
            idx++;
            if(idx==size){
                end=r=true;
                idx--;
            }

        }else{
            idx--;
            if(idx<0){
                end=true;
                r=false;
                idx=0;
            }
        }
        return ret;
    }
};
template <int N>
const typename gcArry<N>::arrT GrayCodeGenN<N,false>::arry = gcArry<N>::codes;
template <int N>
const int GrayCodeGenN<N,false>::size = gcArry<N>::codes.size();

template <int N, int step = 1 << (N - 1)>
class GrayCodeGen : GrayCodeGen<N - 1>
{

    size_t bit;

  public:
    GrayCodeGen() : bit(step) {}
    std::tuple<size_t, bool> GetNext()
    {
        std::tuple<size_t, bool> c = GrayCodeGen<N - 1>::GetNext();
        if (std::get<1>(c))
        {
            size_t newbit = bit + step;
            auto x = std::make_tuple(((~((newbit >> 1) ^ newbit))) & step | std::get<0>(c), !(newbit & step));
            bit = newbit;
            return x;
        }
        else
        {
            return std::make_tuple(((~((bit >> 1) ^ bit))) & step | std::get<0>(c), false);
        }
    }
};

template <>
class GrayCodeGen<1>
{
    size_t bit;

  public:
    GrayCodeGen() : bit(1) {}
    std::tuple<size_t, bool> GetNext()
    {
        size_t newbit = bit + 1;
        auto x = std::make_tuple((~((newbit >> 1) ^ newbit)) & 1, !(newbit & 1));
        bit = newbit;
        return x;
    }
};

int main(){

    auto v = GrayCodeGen<30>();
    std::tuple<size_t, bool> t;
    for (int i = 0; i < 1<<30; i++)
    {

        t = v.GetNext();
        //std::cout << std::bitset<30>(std::get<0>(t)) << " " << std::get<1>(t) << std::endl;

        //
    }
    // Output(v,15);
    output o(3);
    output *p = new output(654);
    auto a = make_caller([&o]() { o.p(); }, [p]() { p->p(); });
    a.Call();
    constexpr auto y = v_sum(1, 2, 2.3);
    constexpr auto x = sum(std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8.7));
    std::cout << x << " " << y << std::endl;

    //std::cout<<apply_from_tuple(do_sum, std::make_tuple(10, 20))<<std::endl;
}
