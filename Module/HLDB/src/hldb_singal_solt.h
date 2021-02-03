/***************************************************
** 文件名：hldb_singal_solt.h
** 作用：模拟实现qt的信号和槽机制
** 参考：
* https://www.cnblogs.com/dankye/archive/2012/08/25/2655816.html
* https://www.bbsmax.com/A/gGdXXVjGd4/
* https://blog.csdn.net/qq_35718950/article/details/84031368
** 使用：
* sender只负责定义自己的信号,并在合适的时候发送信息
* receiver只负责接收信号,并处理收到信号后应该做什么
****************************************************/
#ifndef __HOLO_DB_SINGAL_SOLT_H__
#define __HOLO_DB_SINGAL_SOLT_H__

#include <functional>
#include <memory>
#include <vector>

#define emit
#define slots
#define signals public
#define connect(sender, signal, slot) ((sender)->signal.bind(slot))

template <typename... Args>
class Slot {
 public:
  using OnFunc = std::function<void(Args&&...)>;

  Slot(const OnFunc& func) : m_func(func) {
    // Do nothing
  }

  void exec(Args&&... args) { m_func(std::forward<Args>(args)...); }

 private:
  OnFunc m_func = nullptr;
};

template <typename... Args>
class Signal {
 public:
  using SlotPtr = std::shared_ptr<Slot<Args&&...>>;
  using OnFunc = std::function<void(Args&&...)>;

  void bind(const OnFunc& func) {
    m_slotVec.push_back(SlotPtr(new Slot<Args&&...>(func)));
  }

  void operator()(Args&&... args) {
    for (auto& iter : m_slotVec) {
      iter->exec(std::forward<Args>(args)...);
    }
  }

 private:
  std::vector<SlotPtr> m_slotVec;
};

#endif  // !__HOLO_DB_SINGAL_SOLT_H__
