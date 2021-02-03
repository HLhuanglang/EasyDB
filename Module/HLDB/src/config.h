/***************************************************
文件名：config.h
作用：用于生成dll等配置
****************************************************/
#ifndef __HOLO_DB_CONFIG_H__
#define __HOLO_DB_CONFIG_H__

#if defined(_MSC_VER)
#ifndef HOLO_API
#define HOLO_API __declspec(dllexport)
#else
#define HOLO_API __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define HOLO_API __attribute__((visibility("default")))
#else
#define HOLO_API
#endif

#endif  // __HOLO_DB_CONFIG_H__
