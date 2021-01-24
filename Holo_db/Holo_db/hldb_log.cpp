#include "hldb_log.h"

#include <iostream>

HoloDBLog* HoloDBLog::GetInstance() {
  static HoloDBLog instance;
  if (instance.logger_ == nullptr) {
    instance.logger_->handler_ = nullptr;
    instance.logger_->bufsize_ = DEFAULT_LOG_MAX_BUFSIZE;
    instance.logger_->buf_ = (char*)malloc(instance.logger_->bufsize_);
    instance.logger_->log_level_ = DEFAULT_LOG_LEVEL;
    instance.logger_->file_path_ = "";
    instance.logger_->max_logfilesize_ = DEFAULT_LOG_MAX_BUFSIZE;
    instance.logger_->enable_fsync_ = 1;
    instance.logger_->fp_ = NULL;
    instance.logger_->log_target_ = log_target_e::file;
  }
  return &instance;
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
  logger_->file_path_ = file_path;
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
  } else if (log_level.compare("SILENT") == 0) {
    log_level_tmp = LOG_LEVEL_SILENT;
  }
  logger_->log_level_ = log_level_tmp;
}

void HoloDBLog::logger_set_filesize(size_t file_size) {
  logger_->max_logfilesize_ = file_size;
}

void HoloDBLog::logger_set_filesize(const std::string& file_size) {
  // todo
}

//
void HoloDBLog::logger_print(int log_level, const char* fmt, ...) {
  if (log_level < logger_->log_level_) {
    return;
  }

  switch (logger_->log_target_) {
    case log_target_e::file: {
      std::cout << "test file" << std::endl;
      break;
    }
    case log_target_e::terminal: {
      std::cout << "test terminal" << std::endl;
      break;
    }
    case log_target_e::file_and_terminal: {
      std::cout << "test all" << std::endl;
      break;
    }
    default: {
      break;
    }
  }
}