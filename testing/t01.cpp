/* 
 * http://www.cnblogs.com/oomusou/archive/2007/05/07/738311.html
(C) OOMusou 2007 http://oomusou.cnblogs.com

Filename    : DoorInheritance.cpp
Compiler    : Visual C++ 8.0 / BCB 6.0 / gcc 3.4.2 / ISO C++
Description : Demo how to use abstract class
Release     : 05/07/2007 1.0
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Door {
public:
  virtual void open() const {
    cout << "open horizontally" << endl;
  }
  
  virtual void close() const {
    cout << "close horizontally" << endl;
  }
};

class HorizontalDoor : public Door {
};

class VerticalDoor : public Door {
public:
  void open() const {
    cout << "open vertically" << endl;
  }
  
  void close() const {
    cout << "close vertically" << endl;
  }
};

class DoorController {
protected:
  vector<Door*> _doorVec;
  
public:
  void addDoor(Door* aDoor) {
    _doorVec.push_back(aDoor);
  }
  
  void openDoor() const {
    for_each(_doorVec.begin(), _doorVec.end(), mem_fun(&Door::open));
  }
};


int main() {
  DoorController dc;
  dc.addDoor(new HorizontalDoor());
  dc.addDoor(new VerticalDoor());
  dc.addDoor(new HorizontalDoor());
  dc.openDoor();
}
