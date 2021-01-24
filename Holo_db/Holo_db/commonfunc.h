/***************************************************
文件名：commonfunc.h
作用：提供一些通用函数
****************************************************/
#ifndef __HOLO_DB_COMMONFUNC_H__
#define __HOLO_DB_COMMONFUNC_H__
#include <sys/stat.h>

#include <string>
namespace hldb {
/**************************************************************************
函 数 名：FileExist
函数作用：判断文件是否存在
返 回 值：true存在,false不存在
说    明：
a.使用ifstream打开文件流，成功则存在，失败则不存在
b.以fopen读方式打开文件，成功则存在，否则不存在
c.使用access函数获取文件状态，成功则存在，否则不存在
d.使用stat函数获取文件状态，成功则存在，否则不存在
**************************************************************************/
bool FileExist(const std::string& file_path) {
  struct stat buffer;
  return (stat(file_path.c_str(), &buffer) == 0);
}
}  // namespace hldb

#endif  // !__HOLO_DB_COMMONFUNC_H__
