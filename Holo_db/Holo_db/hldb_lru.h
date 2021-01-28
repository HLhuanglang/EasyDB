/***************************************************
** 文件名：hldb_lru.h
** 作用：lru缓存算法实现
****************************************************/
#ifndef __HOLO_DB_LRU_H__
#define __HOLO_DB_LRU_H__

#include <list>
#include <mutex>
#include <unordered_map>

template <typename Key, typename Ty>
struct Node {
  Key key_;  //键
  Ty val_;   //值
  Node* head_;
  Node* tail_;
};

template <typename Key, typename Ty>
class HoloDBLRU {
 public:
  HoloDBLRU() {}
  HoloDBLRU(size_t count) : count_(count) {}

 public:
  Ty Get(Key key);
  bool Set(Key key, Ty val);

 private:
  size_t count_;  //缓存列表存放对象数量
  std::mutex mtx_;
  std::list<Node<Key, Ty>> cache_list_;
  std::unordered_map<Key, Node<Key, Ty>*> cache_map_;  //为了O(1)的查找
};

template <typename Key, typename Ty>
Ty HoloDBLRU<Key, Ty>::Get(Key key){
    // todo
};

template <typename Key, typename Ty>
bool HoloDBLRU<Key, Ty>::Set(Key key, Ty val) {
  // todo
}
#endif  // !__HOLO_DB_LRU_H__
