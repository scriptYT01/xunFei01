#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Interface
{
    public:
    //virtual void doSomething() = 0 ;
    virtual void doSomething() {
        cout << " ---- doSomething Interface --- " << endl ;
    }
    Interface(){
        cout << " ---- Interface 1 --- " << endl ;
    };
#if 1 
    virtual ~Interface(){ ;
        cout << " ---- Interface 2 --- " << endl ;
    } ;
#else
    virtual ~Interface(){};
#endif
};

class Derived : public Interface
{
    public :
    Derived(){
        cout << " ---- Derived 1 --- " << endl ;
    }
#if 1
    ~Derived() 
    {
        cout << " ---- Derived 2 --- " << endl ;
    }
#endif
#if 1
    void doSomething() {
        cout << " ---- doSomething Derived --- " << endl ;
    }
#endif
};
                           
int main( int __argc , char ** __argv )
{
#if 1
    //Interface* p = new Derived();
    Interface* p = new Interface();
#else
    Derived* p = new Derived();
#endif
    p -> doSomething();
    delete p; 
} // main
