#ifndef __HOLO_DB_TEST_H__
#define __HOLO_DB_TEST_H__

#include <iostream>

#include "hldb_lru.h"
#include "hldb_singal_solt.h"

class Sender {
 public:
  // void emit(int t, std::string str) { singal_(t, str); }
  void open() { singal_isopen(true); }
  void close() { singal_isopen(false); }

 public:
  Signal<bool> singal_isopen;
};

class Revicer {
 public:
  void handle(bool flag) {
    if (flag) {
      std::cout << "收到开启指令,正在开门!" << std::endl;
    } else {
      std::cout << "收到关闭指令,正在开门!" << std::endl;
    }
  }
};

void test1() {
  Sender send;
  Revicer recv;
  connect(&send, singal_isopen,
          std::bind(&Revicer::handle, &recv, std::placeholders::_1));
  send.open();
}

#endif  // !__HOLO_DB_TEST_H__
