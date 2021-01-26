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
 private:
  size_t count_;  //缓存列表存放对象数量
  std::mutex mtx_;
  std::list<Node> list_;
  std::unordered_map<Key, Node<Key, Ty>*> unorder_map_;  //为了O(1)的查找
};

#endif  // !__HOLO_DB_LRU_H__
