#include <iostream> // for std::cout
#include <memory> // for std::move
#include <string> // for std::string
#include <cassert> // for assert()
#include <regex>
#include <cctype> // for std::toupper
#include <fstream>
#include <random>
#include <algorithm> // for std::transform
#include <map> // for std::map
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

    T* begin(){ return elems; }
    T* end(){ return elems+_size;}
};

// Function objects
template<typename T> class Less_Than { const T val;
public:
        Less_Than(const T& v) : val(v) { }
        bool operator()(const T& x) const { return x<val; } // call operator
};

// Function template
template<typename C, typename Op>
void for_all(C& c, Op p){
    for(auto& x : c) p(x);
}

// Variadic template
template <typename T> void bottom(T x) { std::cout << x << " "; }
void recurse(){ std::cout << "Nothing more" << std::endl; } // base case function for termination of recursion
template <typename Head, typename... Tail>
void recurse(Head h, Tail... t) {
    bottom(h);
    recurse(t...);
}

struct Entry { std::string name; int value; };

std::string operator""_u(const char* s, size_t size){
    std::string str;
    for(int i=0; i!=size; ++i) str += std::toupper(s[i]);
    // std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

/*
template<typename T> concept Arithmetic = std::is_arithmetic<T>::value;

template<Sequence Seq, Number Num>
         requires Arithmetic<Value_type<Seq>,Num>
         Num sum(Seq s, Num n) { for(auto& v : s) n+=v; return n; };
*/

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
    for_all(v, [](int& x){ ++x; }); // function template + lambda
    for(int i=0; i!=v.size(); ++i) std::cout << v[i] << ", "; std::cout << std::endl;
    for(int& x : v) std::cout << x << ", "; std::cout << std::endl;

    // FUNCTION OBJECTS
    Less_Than<int> lt { 50 };
    assert(lt(100) == false); // => false (100 is NOT lt 50)

    recurse("abc", 1, 'a', 7.7, true);

    // STRINGS
    std::string s1 = "Niels Stroustrup";
    std::string s2 = s1.substr(6,10);    // s2 = "Stroustrup"
    s1.replace(0,5, "nicholas");    // s1 = "nicholas Stroustrup"
    s1[0] = toupper(s1[0]);         // s1 = "Nicholas Stroustrup"
    std::cout << s1 << ", " << s2 << std::endl;

    // REGEXPS
    std::regex mypattern { "\\w{2}\\s*(\\d{4})?" }; // 2 letters, some spaces, optlly 4 digits
    std::regex p2 { R"(\d-\d)" }; // notice the raw string literal R"(...)"

    std::smatch matches; // matched strings go here
    if( std::regex_search(str, matches, mypattern) ) std::cout << "Match[0]: " << matches[0] << std::endl;

    std::string someString = "hi 4000 foo ba 3333";
    for(std::sregex_iterator p(someString.begin(), someString.end(), mypattern); p != std::sregex_iterator{}; ++p)
        std::cout << "Regex iteration step: [0]=" << (*p)[0] // entire match
                  << "; [1]=" << (*p)[1] // first capture
                  << '\n';

    // NUMERICAL STUFF
    constexpr double d = 20.699;
    std::cout.precision(5);
    std::cout << d << ", " << std::scientific << d << ", " << std::hexfloat << d << ", "
      << std::fixed << d << ", " << std::defaultfloat << d << std::endl;
    // 20.7,2.0699e+01112960x1.4b2f1a9fbe76dp+4,20.6990,20.7

    // STREAMS
    std::ofstream ofs("/home/roby/Downloads/cpp_example_file");
    if(!ofs) std::cerr << "couldn't open 'target' for writing" << std::endl;
    else ofs << "hi there"; // Once properly opened can be used as an ordinary 'ostream'

    std::ostringstream ss;
    ss << 7 << true << "a";
    ss.str("..." + ss.str());
    std::cout << ss.str() << std::endl; // Output:   ...71a

    std::istringstream is { "7.5" };
    int i;
    is >> i;
    std::cout << "i = " << i << std::endl; // Output:  i = 7

    std::ostream_iterator<std::string> osi { std::cout };
    *osi = "hello ";
    osi++;
    *osi = "stream iterators :) \n";

    // RANDOM GENERATION
    std::random_device rd; // used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::normal_distribution<> normDis{5,2};
    for (int n=0; n<10; ++n) std::cout << normDis(gen) << "; "; std::cout << " (nrom distrib: mean=5, stdev=2)\n";

    // UDL
    std::string udls = "bob"_u;
    std::cout << udls << "\n";

    // PAIRS
    std::pair<int,double> pair = {1, 6.3};
    auto pair2 = std::make_pair("hello", 777);
    std::cout << "std::pair => " << pair2.first << ", " << pair2.second << "\n";

    // STRUCTURED BINDING
    std::map<std::string,int> m { {"roby", 123}, {"john",456} };
    for (const auto [key,value] : m) std::cout << "{" << key << "," << value << "} --- "; std::cout << "\n";

    Entry entry { "bob" , 777 };
    auto [entryName, entryValue] = entry;
    std::cout << entryName << " - " << entryValue << "\n";

    return 0;
}
