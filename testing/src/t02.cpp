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
        virtual void alert( string ___src ) const = 0;
}; // class IAlarm 


class Door {
    public:
        string _objName ;
    public:
        Door() { _objName = "Door" ; }
        virtual void open() const {
            cout << "open horizontally" << endl;
        };

        virtual void close() const {
            cout << "close horizontally" << endl;
        };
        virtual void addName( string ___doorName ) {
            _objName.append( ___doorName ) ;
        };
        virtual void alertX() {
            cout << " not Alterm door : " << _objName << endl ; 
        }

        virtual ~Door() { cout << " door : _objName destory : " << _objName << endl ; }
        //virtual ~Door(){};
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
        void alert( string ___src ) const {
            cout << "ring,ring,ring : " << _objName << " : " << ___src << endl;
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
        void alertX() {
            _alarm->alert( _objName );
        }
};

class DoorController {
    protected:
        vector<Door*> _doorVec;

    private:
        struct {           
            void operator() (Door* ii) { delete ii ; printf( "delete %p\n" , ii) ;};
        } deleteDoorObj ;
        struct {           
            //void operator() (Door* ii) { dynamic_cast<AlarmDoor*>(ii) -> alertX() ; }
            void operator() (Door* ii) { ii -> alertX() ; }
        } alertObj ;
    public :
        string _objName ;
    public:
        DoorController () { _objName = "DoorController" ; cout << " DoorController create " << endl; }
        ~DoorController () { 
            for_each(_doorVec.begin(), _doorVec.end(), deleteDoorObj ); cout << " DoorController destory " << endl;  }

        void addDoor(Door* aDoor, string ___doorName ) {
            aDoor -> addName( ___doorName ) ;
            _doorVec.push_back(aDoor);
        }

        template <class TTT>
        void addDoor2( string ___doorName ){
            addDoor( new TTT() , ___doorName );
        }

        void openDoor() {
            for_each(_doorVec.begin(), _doorVec.end(), mem_fun(&Door::open));
        }
        void alertDoor() {
            //for_each(_doorVec.begin(), _doorVec.end(), mem_fun(&dynamic_cast<AlarmDoor*>Door::alertX));
            for_each(_doorVec.begin(), _doorVec.end(), alertObj ); 
        }
};

int main() {
    DoorController dc;
    dc.addDoor(new HorizontalDoor , " 1st door " );
    dc.addDoor(new VerticalDoor , " 2rd door " );
    dc.addDoor(new AlarmDoor , " 3nd door " );
    dc.addDoor2<AlarmDoor>( " 4th door ");
    dc.addDoor2<AlarmDoor>( " 5th door ");
    dc.openDoor();
    dc.alertDoor();

    Door* door1 = new AlarmDoor();
    Door* door2 = new AlarmDoor();

    dynamic_cast<AlarmDoor*>(door1) -> alertX();
    dynamic_cast<AlarmDoor*>(door1) -> alertX();

    dynamic_cast<AlarmDoor*>(door2) -> alertX();
    dynamic_cast<AlarmDoor*>(door2) -> alertX();
    dynamic_cast<AlarmDoor*>(door2) -> alertX();
    dynamic_cast<AlarmDoor*>(door2) -> alertX();

    dynamic_cast<AlarmDoor*>(door1) -> alertX();
    dynamic_cast<AlarmDoor*>(door2) -> alertX();

    AlarmDoor* door3 = new AlarmDoor();

    door3 -> alertX(); 

}




