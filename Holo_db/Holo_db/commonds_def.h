#ifndef __HOLO_DB_COMMONDS_DEF_H__
#define __HOLO_DB_COMMONDS_DEF_H__

#include <string>

// 命令-0
// 该类型的命令不需要接参数
const std::string Holo_DB_exit = ".exit";    //退出
const std::string Holo_DB_quit = ".quit";    //退出
const std::string Holo_DB_q = ".q";          //退出缩写,便捷性
const std::string Holo_DB_help = ".help";    //帮助信息
const std::string Holo_DB_clear = ".clear";  //清屏

// 命令-1
// 该类型的参数需要接一个参数
const std::string Holo_DB_open = ".open";      //打开数据库
const std::string Holo_DB_create = ".create";  //创建数据库

#endif  // !__HOLO_DB_COMMONDS_DEF_H__
