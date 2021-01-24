#ifndef __HOLO_DB_LOG_H__
#define __HOLO_DB_LOG_H__

#include <string.h>

#include <functional>
#include <mutex>

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

#ifdef __cplusplus
extern "C" {
#endif

#define CLR_CLR "\033[0m"      /* »Ö¸´ÑÕÉ« */
#define CLR_BLACK "\033[30m"   /* ºÚÉ«×Ö */
#define CLR_RED "\033[31m"     /* ºìÉ«×Ö */
#define CLR_GREEN "\033[32m"   /* ÂÌÉ«×Ö */
#define CLR_YELLOW "\033[33m"  /* »ÆÉ«×Ö */
#define CLR_BLUE "\033[34m"    /* À¶É«×Ö */
#define CLR_PURPLE "\033[35m"  /* ×ÏÉ«×Ö */
#define CLR_SKYBLUE "\033[36m" /* ÌìÀ¶×Ö */
#define CLR_WHITE "\033[37m"   /* °×É«×Ö */

#define CLR_BLK_WHT "\033[40;37m"     /* ºÚµ×°××Ö */
#define CLR_RED_WHT "\033[41;37m"     /* ºìµ×°××Ö */
#define CLR_GREEN_WHT "\033[42;37m"   /* ÂÌµ×°××Ö */
#define CLR_YELLOW_WHT "\033[43;37m"  /* »Æµ×°××Ö */
#define CLR_BLUE_WHT "\033[44;37m"    /* À¶µ×°××Ö */
#define CLR_PURPLE_WHT "\033[45;37m"  /* ×Ïµ×°××Ö */
#define CLR_SKYBLUE_WHT "\033[46;37m" /* ÌìÀ¶µ×°××Ö */
#define CLR_WHT_BLK "\033[47;30m"     /* °×µ×ºÚ×Ö */

// XXX(id, str, clr)
// we can select id¡¢str¡¢clr through the macro'XXX'
#define LOG_LEVEL_MAP(XXX)                 \
  XXX(LOG_LEVEL_DEBUG, "DEBUG", CLR_WHITE) \
  XXX(LOG_LEVEL_INFO, "INFO ", CLR_GREEN)  \
  XXX(LOG_LEVEL_WARN, "WARN ", CLR_YELLOW) \
  XXX(LOG_LEVEL_ERROR, "ERROR", CLR_RED)   \
  XXX(LOG_LEVEL_FATAL, "FATAL", CLR_RED_WHT)

typedef enum {
  LOG_LEVEL_VERBOSE = 0,
// through macro XXX,we chosed id
#define XXX(id, str, clr) id,
  LOG_LEVEL_MAP(XXX)
#undef XXX
      LOG_LEVEL_SILENT
} log_level_e;

// some default macro
#define DEFAULT_LOG_FILE "Holo_DB"
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO
#define DEFAULT_LOG_REMAIN_DAYS 1
#define DEFAULT_LOG_MAX_BUFSIZE (1 << 14)   // 16k
#define DEFAULT_LOG_MAX_FILESIZE (1 << 24)  // 16M

typedef void (*logger_handler)(int loglevel, const char* buf, int len);

struct HoloDBLogger {
  logger_handler handler_;
  unsigned int bufsize_;
  char* buf_;

  int log_level_;
  int enable_color_;

  char file_path_[256];
  unsigned long long max_logfilesize_;
  int enable_fsync_;  //ÊÇ·ñÐèÒªË¢ÐÂµ½´ÅÅÌ
  FILE* fp_;
  std::mutex mtx_;
};

class HoloDBLog {
 public:
  static HoloDBLog* GetInstance();

 private:
  HoloDBLog() {}

 private:
  HoloDBLogger logger_;
};

void stdout_logger(int loglevel, const char* buf, int len);
void stderr_logger(int loglevel, const char* buf, int len);
void file_logger(int loglevel, const char* buf, int len);

typedef struct logger_s logger_t;
logger_t* logger_create();
void logger_destroy(logger_t* logger);

void logger_set_handler(logger_t* logger, logger_handler fn);
void logger_set_level(logger_t* logger, int level);
// level = [VERBOSE,DEBUG,INFO,WARN,ERROR,FATAL,SILENT]
void logger_set_level_by_str(logger_t* logger, const char* level);
void logger_set_max_bufsize(logger_t* logger, unsigned int bufsize);
void logger_enable_color(logger_t* logger, int on);
int logger_print(logger_t* logger, int level, const char* fmt, ...);

// below for file logger
void logger_set_file(logger_t* logger, const char* filepath);
void logger_set_max_filesize(logger_t* logger, unsigned long long filesize);
// 16, 16M, 16MB
void logger_set_max_filesize_by_str(logger_t* logger, const char* filesize);
void logger_set_remain_days(logger_t* logger, int days);
void logger_enable_fsync(logger_t* logger, int on);
void logger_fsync(logger_t* logger);
const char* logger_get_cur_file(logger_t* logger);

// hlog: default logger instance
logger_t* hv_default_logger();

//=======================================================
// macro hlog*, users only have to focous on thoes macros
//=======================================================

// create an instance
#define hldb_log HoloDBLog::GetInstance()

#define hlog hv_default_logger()

#define hlog_set_file(filepath) logger_set_file(hlog, filepath)
#define hlog_set_level(level) logger_set_level(hlog, level)
#define hlog_set_level_by_str(level) logger_set_level_by_str(hlog, level)
#define hlog_set_max_filesize(filesize) logger_set_max_filesize(hlog, filesize)
#define hlog_set_max_filesize_by_str(filesize) \
  logger_set_max_filesize_by_str(hlog, filesize)
#define hlog_set_remain_days(days) logger_set_remain_days(hlog, days)
#define hlog_enable_fsync() logger_enable_fsync(hlog, 1)
#define hlog_disable_fsync() logger_enable_fsync(hlog, 0)
#define hlog_fsync() logger_fsync(hlog)
#define hlog_get_cur_file() logger_get_cur_file(hlog)

#define hlogd(fmt, ...)                                                   \
  logger_print(hlog, LOG_LEVEL_DEBUG, fmt " [%s:%d:%s]\n", ##__VA_ARGS__, \
               __FILENAME__, __LINE__, __FUNCTION__)
#define hlogi(fmt, ...)                                                  \
  logger_print(hlog, LOG_LEVEL_INFO, fmt " [%s:%d:%s]\n", ##__VA_ARGS__, \
               __FILENAME__, __LINE__, __FUNCTION__)
#define hlogw(fmt, ...)                                                  \
  logger_print(hlog, LOG_LEVEL_WARN, fmt " [%s:%d:%s]\n", ##__VA_ARGS__, \
               __FILENAME__, __LINE__, __FUNCTION__)
#define hloge(fmt, ...)                                                   \
  logger_print(hlog, LOG_LEVEL_ERROR, fmt " [%s:%d:%s]\n", ##__VA_ARGS__, \
               __FILENAME__, __LINE__, __FUNCTION__)
#define hlogf(fmt, ...)                                                   \
  logger_print(hlog, LOG_LEVEL_FATAL, fmt " [%s:%d:%s]\n", ##__VA_ARGS__, \
               __FILENAME__, __LINE__, __FUNCTION__)

// macro alias
#if !defined(LOGD) && !defined(LOGI) && !defined(LOGW) && !defined(LOGE) && \
    !defined(LOGF)
#define LOGD hlogd
#define LOGI hlogi
#define LOGW hlogw
#define LOGE hloge
#define LOGF hlogf
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // !__HOLO_DB_LOG_H__
