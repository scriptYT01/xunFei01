/* 
(C) OOMusou 2007 http://oomusou.cnblogs.com

Filename    : DoorInheritance.cpp
Compiler    : Visual C++ 8.0 / BCB 6.0 / gcc 3.4.2 / ISO C++
Description : Demo how to use interface
Release     : 05/07/2007 1.0
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class IAlarm {
    public:
        //virtual void alert() const = 0;
        virtual void alert() const ;
}; // class IAlarm 

class XXX1 {
}; // class XXX1 

class XXX21 {
    public:
        virtual void alert() {
            cout << " alert XXX21 " << endl;
        };
}; // class XXX21 

class XXX22 : public XXX21 {
    public:
        void alert() {
            cout << " alert XXX22 " << endl;
        };
}; // class XXX22 

class Door {
    public:
        virtual void open() const {
            cout << "open horizontally" << endl;
        }

        virtual void close() const {
            cout << "close horizontally" << endl;
        }
}; // class Door

class HorizontalDoor : public Door {
}; // class HorizontalDoor 

class VerticalDoor : public Door {
    public:
        void open() const {
            cout << "open vertically" << endl;
        }

        void close() const {
            cout << "close vertically" << endl;
        }
}; // class VerticalDoor 

class Alarm : public IAlarm {
    public:
        void alert() const {
            cout << "ring,ring,ring" << endl;
        }
};

class AlarmDoor : public Door {
    //protected:
    private:
        //VerticalDoor* _alarm;
        //Door* _alarm;
        //char* _alarm;
        //Alarm* _alarm;
        //XXX1 *_alarm ;
        //XXX21 *_alarm ;
        XXX22 *_alarm ;
        int idx ;

    public:
        AlarmDoor() {
            //_alarm = new Alarm;
            //_alarm = new char[10];
            //_alarm = new VerticalDoor();
            //_alarm = new Door() ;
            //_alarm = new XXX1() ;
            //_alarm = new XXX21() ;
            if ( _alarm == NULL ) {
                cout << " NULL , create alarm only. " << (int64_t) _alarm << endl ;

                _alarm = new XXX22() ;
            } else {
                cout << " not NULL , free it and re-create alarm . " << (int64_t) _alarm << endl ;
                //free( _alarm );
                _alarm = new XXX22() ;
            }
            idx += 100 ;
        }

        ~AlarmDoor() {
            //delete _alarm;
        }

    public:
        void alert() {
            _alarm->alert();
            //_alarm[0] = 11 ;
            cout << " now idx is : " << idx << endl ;
            idx += 1 ;
        }
};

class DoorController {
    protected:
        vector<Door*> _doorVec;

    public:
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
}




