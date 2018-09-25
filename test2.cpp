#include <memory>
#include <iostream>

// This CRTP class implements clone() for Derived
template <typename Derived>
class Shape {
public:
    Derived *clone() const {
        return new Derived(static_cast<Derived const&>(*this));
    }
};

// Nice macro which ensures correct CRTP usage
#define Derive_Shape_CRTP(Type) class Type: public Shape<Type>

// Every derived class inherits from Shape_CRTP instead of Shape
Derive_Shape_CRTP(Square) {};
Derive_Shape_CRTP(Circle) {};
struct A{
   void p(){
       std::cout<<"A"<<std::endl;
   }

};
struct B:public A{
   virtual void p(){
       std::cout<<"B"<<std::endl;
   }

};
struct C:public B{
   void p(){
       std::cout<<"C"<<std::endl;
   }

};

int main(){
    B* p=new C();
    p->p();
}