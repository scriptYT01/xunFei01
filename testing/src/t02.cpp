/* 
(C) OOMusou 2007 http://oomusou.cnblogs.com

Filename    : DoorInheritance.cpp
Compiler    : Visual C++ 8.0 / BCB 6.0 / gcc 3.4.2 / ISO C++
Description : Demo how to use interface
Release     : 05/07/2007 1.0
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class IAlarm {
    public :
        string _objName ;
    public:
        virtual ~IAlarm(){};
        virtual void alert() const = 0;
        //virtual void alert() const ;
}; // class IAlarm 


class Door {
    public:
        string _objName ;
    public:
        Door() { _objName = "Door" ; }
        virtual void open() const {
            cout << "open horizontally" << endl;
        }

        virtual void close() const {
            cout << "close horizontally" << endl;
        }
        virtual ~Door(){};
}; // class Door

class HorizontalDoor : public Door {
    public:
    HorizontalDoor () { _objName = "HorizontalDoor" ; cout << " HorizontalDoor create " << endl; }
    ~HorizontalDoor () { cout << " HorizontalDoor destory " << endl;  }
}; // class HorizontalDoor 

class VerticalDoor : public Door {
    public:
        void open() const {
            cout << "open vertically" << endl;
        }

        void close() const {
            cout << "close vertically" << endl;
        }
        VerticalDoor () { _objName = "VerticalDoor" ; cout << " VerticalDoor create " << endl; }
        ~VerticalDoor () { cout << " VerticalDoor destory " << endl;  }
}; // class VerticalDoor 

class Alarm : public IAlarm {
    public:
        void alert() const {
            cout << "ring,ring,ring : " << _objName << endl;
        }
        Alarm() { _objName = "Alarm" ; }
        //~Alarm(){};
};

class AlarmDoor : public Door {
    //protected:
    private:
        Alarm* _alarm;

    public:
        AlarmDoor() {
            _objName = "AlarmDoor" ;
            //_alarm = new Alarm();
            _alarm = new Alarm;
            cout << " AlarmDoor create " << endl; 
        }

        ~AlarmDoor() {
            delete _alarm;
            cout << " AlarmDoor destory " << endl;  
        }

    public:
        void alert() {
            _alarm->alert();
        }
};

class DoorController {
    protected:
        vector<Door*> _doorVec;

    private:
    public :
        void _deleteDoor( Door* ___door ) {
            //delete ___door ;
        };
        struct myclass {           // function object type:
            //void operator() (int i) {std::cout << ' ' << i;}
            void operator() (Door* ii) { delete ii ; printf( "delete %p\n" , ii) ;};
        } myobject;
    public :
        string _objName ;
    public:
        DoorController () { _objName = "DoorController" ; cout << " DoorController create " << endl; }
        ~DoorController () { 
            //for(auto it = _doorVec.begin(); it != _doorVec.end() ; ++it){ delete it ; }
            //for_each(_doorVec.begin(), _doorVec.end(), mem_fun( delete &));
            //for_each(_doorVec.begin(), _doorVec.end(), delete &);
            //for(Door** it = _doorVec.begin(); it != _doorVec.end() ; ++it){ delete it ; }
            //for( Door* it : _doorVec ) {;}
            //for_each(_doorVec.begin(), _doorVec.end(), _deleteDoor);
            for_each(_doorVec.begin(), _doorVec.end(), myobject);
            cout << " DoorController destory " << endl;  }

        void addDoor(Door* aDoor) {
            _doorVec.push_back(aDoor);
        }

        void openDoor() {
            for_each(_doorVec.begin(), _doorVec.end(), mem_fun(&Door::open));
        }
};

int main() {
    DoorController dc;
    dc.addDoor(new HorizontalDoor());
    dc.addDoor(new VerticalDoor());
    dc.addDoor(new AlarmDoor());
    dc.openDoor();

    Door* door1 = new AlarmDoor();
    Door* door2 = new AlarmDoor();

    dynamic_cast<AlarmDoor*>(door1) -> alert();
    dynamic_cast<AlarmDoor*>(door1) -> alert();

    dynamic_cast<AlarmDoor*>(door2) -> alert();
    dynamic_cast<AlarmDoor*>(door2) -> alert();
    dynamic_cast<AlarmDoor*>(door2) -> alert();
    dynamic_cast<AlarmDoor*>(door2) -> alert();

    dynamic_cast<AlarmDoor*>(door1) -> alert();
    dynamic_cast<AlarmDoor*>(door2) -> alert();

    AlarmDoor* door3 = new AlarmDoor();

    door3 -> alert(); 

}




