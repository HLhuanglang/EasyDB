#ifndef __HOLO_DB_LOG_H__
#define __HOLO_DB_LOG_H__
/***************************************************
文件名：hldb_log.h
作用：Holo_DB的日志模块
****************************************************/
#include <functional>
#include <mutex>
#include <string>

#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#define DIR_SEPARATOR_STR "\\"
#else
#define DIR_SEPARATOR '/'
#define DIR_SEPARATOR_STR "/"
#endif

#ifndef __FILENAME__
#define __FILENAME__                                                       \
  (strrchr(__FILE__, DIR_SEPARATOR) ? strrchr(__FILE__, DIR_SEPARATOR) + 1 \
                                    : __FILE__)
#endif

#define CLR_CLR "\033[0m"      /* 恢复颜色 */
#define CLR_BLACK "\033[30m"   /* 黑色字 */
#define CLR_RED "\033[31m"     /* 红色字 */
#define CLR_GREEN "\033[32m"   /* 绿色字 */
#define CLR_YELLOW "\033[33m"  /* 黄色字 */
#define CLR_BLUE "\033[34m"    /* 蓝色字 */
#define CLR_PURPLE "\033[35m"  /* 紫色字 */
#define CLR_SKYBLUE "\033[36m" /* 天蓝字 */
#define CLR_WHITE "\033[37m"   /* 白色字 */

#define CLR_BLK_WHT "\033[40;37m"     /* 黑底白字 */
#define CLR_RED_WHT "\033[41;37m"     /* 红底白字 */
#define CLR_GREEN_WHT "\033[42;37m"   /* 绿底白字 */
#define CLR_YELLOW_WHT "\033[43;37m"  /* 黄底白字 */
#define CLR_BLUE_WHT "\033[44;37m"    /* 蓝底白字 */
#define CLR_PURPLE_WHT "\033[45;37m"  /* 紫底白字 */
#define CLR_SKYBLUE_WHT "\033[46;37m" /* 天蓝底白字 */
#define CLR_WHT_BLK "\033[47;30m"     /* 白底黑字 */

// XXX(id, str, clr)
#define LOG_LEVEL_MAP(XXX)                 \
  XXX(LOG_LEVEL_DEBUG, "DEBUG", CLR_WHITE) \
  XXX(LOG_LEVEL_INFO, "INFO ", CLR_GREEN)  \
  XXX(LOG_LEVEL_WARN, "WARN ", CLR_YELLOW) \
  XXX(LOG_LEVEL_ERROR, "ERROR", CLR_RED)   \
  XXX(LOG_LEVEL_FATAL, "FATAL", CLR_RED_WHT)

// 日志级别：
// [VERBOSE、DEBUG、INFO、WARN、ERROR、FATAL、NULL(不打印)]
typedef enum {
  LOG_LEVEL_VERBOSE = 0,
#define XXX(id, str, clr) id,
  LOG_LEVEL_MAP(XXX)
#undef XXX
      LOG_LEVEL_NULL
} log_level_e;

typedef enum {
  file,              //输出到文件
  terminal,          //输出到终端
  file_and_terminal  //文件和终端
} log_target_e;

// some default macro
#define DEFAULT_LOG_FILE "Holo_DB"
#define DEFAULT_LOG_LEVEL LOG_LEVEL_DEBUG
#define DEFAULT_LOG_MAX_BUFSIZE (1 << 14)   // 16k
#define DEFAULT_LOG_MAX_FILESIZE (1 << 24)  // 16M

using logger_hander =
    std::function<void*(int log_level, const char* buf, int len)>;

struct HoloDBLogger {
  logger_hander handler_;
  unsigned int bufsize_;  //待输出数组的大小
  char* buf_;             //指向待输出数组的指针
  int log_level_;
  std::string file_path_;
  size_t max_logfilesize_;
  int enable_fsync_;
  int enable_color_;
  FILE* fp_;
  std::mutex mtx_;
  int log_target_;
};

class HoloDBLog {
 public:
  static HoloDBLog* GetInstance();
  void logger_init();
  void logger_print(int log_level, const char* fmt, ...);
  void logger_set_log_target(const std::string& target);
  void logger_set_filepath(const std::string& file_path);
  void logger_set_level(const std::string& log_level);
  void logger_set_filesize(size_t file_size);
  void logger_set_filesize(const std::string& file_size);

 private:
  HoloDBLog() { logger_ = new HoloDBLogger(); }

 private:
  HoloDBLogger* logger_;
};

/***************************************************************
 * 供用户使用的宏
 **************************************************************/

//日志初始化
#define hldb_log_init() HoloDBLog::GetInstance()->logger_init()

//设置日志信息输出地,若设置为仅输出到终端,则后续宏不起作用
#define hldb_log_set_target_by_str(target) \
  HoloDBLog::GetInstance()->logger_set_log_target(target)

//设置日志文件存放地点
//不设置绝对路径,则存放在当前程序运行路径下
#define hldb_log_set_filepath_by_str(filepath) \
  HoloDBLog::GetInstance()->logger_set_filepath(filepath)

//设置日志级别
//[VERBOSE,DEBUG,INFO,WARN,ERROR,FATAL,SILENT]
#define hldb_log_set_log_level_by_str(level) \
  HoloDBLog::GetInstance()->logger_set_level(level)

//设置日志文件的大小,超过限制则清理
//默认16MB
#define hldb_log_set_filesize(filesize) \
  HoloDBLog::GetInstance()->logger_set_filesize(filesize)

#define hldb_log_set_filesize_by_str(filesize_str) \
  HoloDBLog::GetInstance()->logger_set_filesize(filesize_str)

// 日志信息
#define hldb_log_debug(fmt, ...)                                          \
  HoloDBLog::GetInstance()->logger_print(                                 \
      LOG_LEVEL_DEBUG, "[%s][%d行][%s]" fmt "\n", __FILENAME__, __LINE__, \
      __FUNCTION__, ##__VA_ARGS__)

#define hldb_log_info(fmt, ...)                                          \
  HoloDBLog::GetInstance()->logger_print(                                \
      LOG_LEVEL_INFO, "[%s][%d行][%s]" fmt "\n", __FILENAME__, __LINE__, \
      __FUNCTION__, ##__VA_ARGS__)

#define hldb_log_warn(fmt, ...)                                          \
  HoloDBLog::GetInstance()->logger_print(                                \
      LOG_LEVEL_WARN, "[%s][%d行][%s]" fmt "\n", __FILENAME__, __LINE__, \
      __FUNCTION__, ##__VA_ARGS__)

#define hldb_log_error(fmt, ...)                                          \
  HoloDBLog::GetInstance()->logger_print(                                 \
      LOG_LEVEL_ERROR, "[%s][%d行][%s]" fmt "\n", __FILENAME__, __LINE__, \
      __FUNCTION__, ##__VA_ARGS__)

#define hldb_log_fatal(fmt, ...)                                          \
  HoloDBLog::GetInstance()->logger_print(                                 \
      LOG_LEVEL_FATAL, "[%s][%d行][%s]" fmt "\n", __FILENAME__, __LINE__, \
      __FUNCTION__, ##__VA_ARGS__)

// 别名
#define LOGDEBUG hldb_log_debug
#define LOGINFO hldb_log_info
#define LOGWARN hldb_log_warn
#define LOGERROR hldb_log_error
#define LOGFATAL hldb_log_fatal

#endif __HOLO_DB_LOG_H__