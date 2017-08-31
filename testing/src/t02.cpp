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
        virtual void alert() const = 0;
}; // class IAlarm 


class Door {
    //protected:
    //private:
    //public:
    //    IAlarm* _alarm;
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
    //private:
    //    IAlarm* _alarm;
    char* _alarm;

    public:
        AlarmDoor() {
            //_alarm = new Alarm;
            _alarm = new char[10];
        }

        ~AlarmDoor() {
            delete _alarm;
        }

    public:
        void alert() {
            //_alarm->alert();
            _alarm[0] = 11 ;
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

    Door* door = new AlarmDoor();
    dynamic_cast<AlarmDoor*>(door) -> alert();
}




