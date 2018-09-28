#include <iostream>
#include <typeinfo>
#include <functional>
namespace pk
{
class any final
{
    struct content
    {
        virtual content *move() = 0;
        virtual content *copy() = 0;
        virtual const std::type_info & type() = 0;
        virtual ~content() {}
    };
    template <class T>
    struct _content : content
    {
        typename std::decay<T>::type V;
        _content(T v) : V(v) {}
        virtual content *copy()
        {
            return new _content(*this);
        }
        virtual content *move()
        {
            return new _content(std::move(*this));
        }
        virtual const std::type_info & type()
        {
            return typeid(T);
        }

    };
    content *pVal{nullptr};

  public:
    template <class T>
    any(T v)
    {
        pVal = new _content<T>(v);
    }
    ~any()
    {
        if (pVal)
        {
            delete pVal;
        }
    }
    any& operator=(any&& other)
    {
        std::swap(pVal,other.pVal);
        return *this;
    }
    any(const any& other)
    {
        pVal=other.pVal->copy();
    }
    template<class T>
    T cast(){
        if(typeid(T)==pVal->type()){
            return static_cast<_content<T>*>(pVal)->V;
        }
        throw std::bad_cast();
    }
};
} // namespace pk

class test
{
    char t{0};


};
int main()
{
    test ss=test();
    pk::any x = test();
    x=1;
    pk::any y = std::reference_wrapper<int>(x.cast<int&>());
    x.cast<int&>()=2;
    std::cout<<x.cast<int>()<<std::endl;
    std::cout<<y.cast<std::reference_wrapper<int>>().get()<<std::endl;
    x=ss;
}