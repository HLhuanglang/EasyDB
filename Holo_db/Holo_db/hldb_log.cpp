#include "hldb_log.h"
#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32
#include <iostream>
#include <thread>

HoloDBLog* HoloDBLog::GetInstance() {
  static HoloDBLog instance;
  return &instance;
}

void HoloDBLog::logger_init() {
  logger_->handler_ = nullptr;
  logger_->bufsize_ = DEFAULT_LOG_MAX_BUFSIZE;
  logger_->buf_ = (char*)malloc(logger_->bufsize_);
  logger_->log_level_ = DEFAULT_LOG_LEVEL;
  logger_->file_path_ = DEFAULT_LOG_FILE_PATH;
  logger_->max_logfilesize_ = DEFAULT_LOG_MAX_BUFSIZE;
  logger_->enable_fsync_ = true;
  logger_->fp_ = NULL;
  logger_->log_target_ = log_target_e::terminal;
}

void HoloDBLog::logger_set_log_target(const std::string& target) {
  int target_tmp = 0;
  if (target.compare("file") == 0) {
    target_tmp = log_target_e::file;
  } else if (target.compare("terminal") == 0) {
    target_tmp = log_target_e::terminal;
  } else if (target.compare("file_and_terminal") == 0) {
    target_tmp = log_target_e::file_and_terminal;
  }
  logger_->log_target_ = target_tmp;
}

void HoloDBLog::logger_set_filepath(const std::string& file_path) {
  logger_->file_path_ = file_path + ".log";
}

void HoloDBLog::logger_set_level(const std::string& log_level) {
  int log_level_tmp = DEFAULT_LOG_LEVEL;
  if (log_level.compare("VERBOSE") == 0) {
    log_level_tmp = LOG_LEVEL_VERBOSE;
  } else if (log_level.compare("DEBUG") == 0) {
    log_level_tmp = LOG_LEVEL_DEBUG;
  } else if (log_level.compare("INFO") == 0) {
    log_level_tmp = LOG_LEVEL_INFO;
  } else if (log_level.compare("WARN") == 0) {
    log_level_tmp = LOG_LEVEL_WARN;
  } else if (log_level.compare("FATAL") == 0) {
    log_level_tmp = LOG_LEVEL_FATAL;
  } else if (log_level.compare("NULL") == 0) {
    log_level_tmp = LOG_LEVEL_NULL;
  }
  logger_->log_level_ = log_level_tmp;
}

void HoloDBLog::logger_set_filesize(size_t file_size) {
  logger_->max_logfilesize_ = file_size;
}

void HoloDBLog::logger_set_filesize(const std::string& file_size) {
  // todo
}

void HoloDBLog::logger_enable_fsync(bool flag) {
  if (flag) {
    logger_->enable_fsync_ = true;
  } else {
    logger_->enable_fsync_ = false;
  }
}

void HoloDBLog::logger_write(const char* buf, size_t size) {
  std::unique_lock<std::timed_mutex> uqlk(this->logger_->mtx_, std::defer_lock);
  if (uqlk.try_lock_for(std::chrono::seconds(1))) {
    if (logger_->fp_ == NULL) {
      logger_->fp_ = fopen(logger_->file_path_.c_str(), "ab+");
    }
    fwrite(buf, 1, size, logger_->fp_);
    if (logger_->enable_fsync_) {
      fflush(logger_->fp_);
    }
  }
}

void HoloDBLog::logger_print(int log_level, const char* fmt, ...) {
  // if (log_level < logger_->log_level_) {
  //  // 低于设置的日志级别，则无法输出
  //  // 假如设定为 WARN，则只有ERROR、FATAL可以输出
  //  return;
  //}

  int year, month, day, hour, min, sec, ms;
#ifdef _WIN32
  SYSTEMTIME tm;
  GetLocalTime(&tm);
  year = tm.wYear;
  month = tm.wMonth;
  day = tm.wDay;
  hour = tm.wHour;
  min = tm.wMinute;
  sec = tm.wSecond;
  ms = tm.wMilliseconds;
#else
  struct timeval tv;
  struct tm* tm = NULL;
  gettimeofday(&tv, NULL);
  time_t tt = tv.tv_sec;
  tm = localtime(&tt);
  year = tm->tm_year + 1900;
  month = tm->tm_mon + 1;
  day = tm->tm_mday;
  hour = tm->tm_hour;
  min = tm->tm_min;
  sec = tm->tm_sec;
  ms = tv.tv_usec / 1000;
#endif

  const char* color = "";
  const char* level = "";

#define XXX(id, str) \
  case id:           \
    level = str;     \
    break;

  //上面为了组装出switch的内容
  switch (log_level) { LOG_LEVEL_MAP(XXX) }

#undef XXX

  char* buf = logger_->buf_;
  int bufsize = logger_->bufsize_;
  int len = 0;

  //时间
  len = snprintf(buf + len, bufsize - len,
                 "[%04d-%02d-%02d %02d:%02d:%02d.%03d][%s]", year, month, day,
                 hour, min, sec, ms, level);

  va_list ap;
  va_start(ap, fmt);
  len += vsnprintf(buf + len, bufsize - len, fmt, ap);
  va_end(ap);

  switch (logger_->log_target_) {
    case log_target_e::file: {
      logger_write(buf, len);
      break;
    }
    case log_target_e::terminal: {
      std::cout << buf << std::endl;
      break;
    }
    case log_target_e::file_and_terminal: {
      std::cout << buf << std::endl;
      logger_write(buf, len);
      break;
    }
    default: {
      break;
    }
  }
}