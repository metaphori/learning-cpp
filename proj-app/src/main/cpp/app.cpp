#include <iostream>
#include <memory>
#include <string>
//#include <stdlib.h>
#include "app.h"
#include "lib.hpp"

std::string learning_cpp::Greeter::greeting() {
    return std::string("Hello, World!");
}

namespace foo {
    void bar();
}

void foo::bar(){ std::cout << "local foo:bar" << std::endl; }

class Shape {
public:
    virtual double area() const = 0; // virtual marks the member CAN be overridden; part "=0" says it MUST
    virtual ~Shape() { }
};

class Square : public Shape {
public:
    Square(int s) : _side { s } { };
    double area() const override; // override specifier checks that this member actually overrides a virtual base function
    double side() const { return _side; } // const specifier indicates the function doesn't modify the receiver object
private:
    int _side;
};
double Square::area() const { return this->side() * this->side(); }

class Rect : public Shape {
public:
    Rect(int w, int h) : _w {w}, _h {h} { };
    double area() const override { return _w*_h; }
    double width() const { return _w; } double height() const { return _h; }
private:
    int _w, _h;
};

// Goal: show "enum classes", which are (1) strongly typed enumerations and (2) enumerations are scoped
// NB: enum class is not actually a class (i.e., can't have methods)
enum class Light { green, yellow, red };
Light operator++(Light& l){
    switch(l){
        case Light::green: return Light::yellow;
        case Light::yellow: return Light::red;
        case Light::red: return Light::green;
    }
}
// Plain enums also exist (no 'class' after 'enum')
enum FooBarEnum { FooEnumValue, BarEnumValue };

// Goal: show issue derived by DEFAULT MEMBERWISE COPY, which will copy the POINTER to the content "elems"
class VectorFlawed {
    int sz; double *elems;
public:
    VectorFlawed(int s) : sz{s}, elems { new double[s] } {}
    int size() const { return sz; }
    ~VectorFlawed() { delete[] elems; }
    double& operator[](int pos){ return elems[pos]; }
};

// Goal: show the peculiarities of OOP in C++ e.g. copy/move
class Vector {
    int sz; double *elems;
public:
    Vector(int s) : sz{s}, elems { new double[s] } {}
    ~Vector() { delete[] elems; }
    Vector(const Vector& v) : sz{v.sz}, elems{ new double[v.sz] } { // copy constructor
        for(int i=0; i != sz; ++i) elems[i] = v.elems[i]; // notice: cannot do v[i] since v is a const Vector&
    }
    double& operator[](int pos){ return elems[pos]; }
    int size() const { return sz; }
};

void printVector(Vector& v, std::string s){ std::cout << s << " => "; for(int i=0; i!=v.size(); ++i) std::cout << v[i] << ", "; std::cout << std::endl; }

void comp(char arr[10]){ std::cout << arr << std::endl; } // arr is double*

std::string gen_str(){ return std::string("hello"); }

template<typename T>
class Box {
    T value;
public:
    Box(T content) : value { content } {};
    Box(const Box& b) : value { b.value } {  // copy constructor
        std::cout << "[COPYING into new " << this << " the box " << &b << "(" << b.value << ")] ";
    }
    Box(const Box&& b) : value { b.value } {  // move constructor
        std::cout << "[MOVING into new " << this << " the box " << &b << "(" << b.value << ")] ";
    }
    Box& operator=(const Box& b) {  // copy assignment
        std::cout << "[COPYING into " << this << "(" << value << ") the box " << &b << "(" << b.value << ")] ";
        value = b.value;
    }
    /*
    Box& operator=(const Box&& b) {  // move assignment
        std::cout << "[ASSIGNING box " << this << "(" << value << ") by MOVING " << &b << "(" << b.value << ")] ";
        value = b.value;
    }
    */
    T content() { return value; }
};
// Goal: show how functions may be optimised for rvalues (i.e., for moves rather than copies)
template<typename T> void swap_naive(T& a, T& b){
    T tmp {a}; // tmp's copy constructor called
    a = b; // a's copy assignment called
    b = tmp; // b's copy assignment called
}
template<typename T> void swap(T& a, T& b){
    T tmp { std::move(a) }; // tmp's move constructor called
    a = std::move(b); // a's move assignment called
    b = std::move(tmp); // b's move
}

// TODO: templates
template<typename T>
class Vec {
    int _size;
    T* elems;
public:
    Vec(const std::initializer_list<T>& initl) : _size{ initl.size() }, elems { new T[initl.size()] }  {
        T* content = elems;
        for(auto& v : initl) { *content = v; content++; }
    }

    T operator[](int pos) const { return elems[pos]; }
    int size() const { return _size; }
};

int main () {
    learning_cpp::Greeter greeter;
    std::cout << greeter.greeting() << std::endl;
    foo::bar(); // NB: foo:bar() would call bar() since "foo:" would be a label
    bar(); // imported from "lib.hpp"
    proj_lib::foo();

    auto lst = { 1, 0, 7 };
    for(auto v : lst) std::cout << v << "," ; std::cout << std::endl;

    Shape* s = new Square(10);
    std::cout << "Area of square of size: " << dynamic_cast<Square*>(s)->side() << "x" << dynamic_cast<Square&>(*s).side() << " is " << s->area() << std::endl;
    std::cout << "Our square is in memory at the following location: " << dynamic_cast<void*>(s) << std::endl;
    delete s; // remember to delete from the free store

    auto r = std::unique_ptr<Shape>{ new Rect(5,2) };
    auto rptr = dynamic_cast<Rect&>(*r.get()); // NB: BAD PRACTICE -- MAY LEAK THE OBJECT IF THE CONVERTION FAILS => USE shared_ptr<> and std::dynamic_pointer_cast<>
    std::cout << "Area of rect " << rptr.width() << "x" << rptr.height() << " is " << r->area() << std::endl;

    auto rectShared = std::make_shared<Rect>( 1, 3 );
    std::cout << "Area of rect " << rectShared->width() << "x" << rectShared->height() << " is " << rectShared->area() << std::endl;

    auto light = Light::yellow;
    auto newLight = ++light;
    std::cout << "Light was " << (int)light << " and now is " << (int)newLight << std::endl;

    VectorFlawed vecFlawed { 3 };
    auto vecf = vecFlawed; // will copy the POINTER to the content
    vecf[1] = 77;
    std::cout << vecFlawed[1] << std::endl; // will print 77 (!!) I.e., the shallow copy
    // PLUS: there will also be a DOUBLE FREE

    Vector v1 { 3 };
    Vector v2 = v1;
    v2[1] = 77;
    printVector(v1, std::string{"v1 (original vector)"});
    printVector(v2, std::string{"v2 (copied from v1)"});

    //comp("ciao"); // ok but raises a warning: ISO c++ forbids converting a string constant to char*

    std::string str { "foo" };

    std::string& r1 {str};    // lvalue ref, bind r1 to s (an lvalue)
    //std::string& r2 {gen_str()};  // ERROR: lvalue ref, but f() is an rvalue
    //std::string& r3 {"bar"};// ERROR: lvalue ref, but "bar" is an rvalue (temporary)
    const std::string& r2 {gen_str()};  // ERROR: lvalue ref, but f() is an rvalue
    const std::string& r3 {"bar"};// ERROR: lvalue ref, but "bar" is an rvalue (temporary)

    //std::string&& rr1 {str};     // ERROR: rvalue ref, but s is lvalue
    std::string&& rr2 {gen_str()};   // OK: bind rr2 to a rvalue (a temporary)
    std::string&& rr3 {"bar"}; // OK: bind rr3 to a rvalue (a temporary)

    // swap: copy vs. move
    Box<int> b1 { 1 }; Box<int> b2 { 2 };
    std::cout << b1.content() << ", " << b2.content() << " -- ";
    swap_naive(b1, b2);
    std::cout << b1.content() << ", " << b2.content() << " -- ";
    swap(b1, b2);
    std::cout << b1.content() << ", " << b2.content() << std::endl;

    // Generic Vec
    Vec<int> v { 1,2,3 };
    //for(int& x : v)
    for(int i=0; i!=v.size(); ++i) std::cout << v[i] << ", "; std::cout << std::endl;

    return 0;
}
