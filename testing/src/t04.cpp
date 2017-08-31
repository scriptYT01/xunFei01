#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Interface
{
    public:
        virtual void doSomething() = 0 ;
        virtual ~Interface(){} ;
};

class Derived : public Interface
{
    public :
        void doSomething() {
            cout << " ---- doSomething Derived --- " << endl ;
        }
};

int main( int __argc , char ** __argv )
{
#if 1
    Interface* p = new Derived();
#else
    Derived* p = new Derived();
#endif
    p -> doSomething();
    delete p; 
} // main



