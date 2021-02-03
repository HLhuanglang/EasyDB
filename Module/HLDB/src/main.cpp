#include <sys/stat.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "commonds_def.h"
#include "commonfunc.h"
#include "display_msg.h"
#include "hldb_log.h"

bool is_invalid_cmd(const std::string& cmd) {
  if (cmd.empty()) {
    return false;
  } else if (cmd[0] != '.') {
    return false;
  } else if (cmd == Holo_DB_exit || cmd == Holo_DB_quit ||
             cmd == Holo_DB_help || cmd == Holo_DB_clear || cmd == Holo_DB_q) {
    return true;
  } else {
    return false;
  }
}

void trim_cmd(std::string& cmd) {
  //过滤空格
  size_t cmd_begin_pos = cmd.find_first_not_of(" ");
  if (cmd_begin_pos != std::string::npos) {
    size_t cmd_end_pos = cmd.find_first_of(" ", cmd_begin_pos);
    if (cmd_end_pos != std::string::npos) {
      cmd = cmd.substr(cmd_begin_pos, cmd_end_pos - cmd_begin_pos);
    } else {
      cmd = cmd.substr(cmd_begin_pos);
    }
  }
  //将命令转为小写字母
  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
}

int main(int count, char* input_parameters[]) {
  // 日志组件初始化
  hldb_log_init();
  hldb_log_set_target_by_str("file");

  // windows下通过cmd或git bash以"start Holo_DB.exe db_path"
  // linux下直接 Holo_DB db_path
  // 以上述两种方式启动时,获取指定的数据库文件路径
  std::string db_path = "";
  if (count > 1) {
    // count表示程序入参个数,默认为1,参数是程序的名字
    // input_parameters[0] = "Holo_DB.exe"
    db_path = input_parameters[1];
  }
  LOGINFO("db_path:%s", db_path.c_str());

#ifdef HLDB_DEBUG
  int i = 0;
  do {
    LOGINFO("TEST:%d", i);
    LOGWARN("TEST:%d", i);
    LOGERROR("TEST:%d", i);
    LOGFATAL("TEST:%d", i);
    i++;
  } while (i < 100);
#endif  // HLDB_DEBUG

  // 信息打印对象
  DisplayMsg dis_msg;
  dis_msg.dispaly_introduce_Holo_DB();

  // 如果指定了数据库地址,则需要判断是否有效
  if (!db_path.empty() && !hldb::FileExist(db_path)) {
    LOGINFO("数据库文件不存在:%s", db_path.c_str());
  }

  // 存放用户输入的命令
  char InputBuffer[2048];

  while (true) {
    dis_msg.display_HoloDB();

    // 获取用户输入的命令或是sql语句
    std::cin.getline(InputBuffer, sizeof(InputBuffer));
    std::string cmd = InputBuffer;

    // 初步解析命令
    trim_cmd(cmd);
    if (is_invalid_cmd(cmd)) {
      // 普通的命令
      if (cmd.compare(Holo_DB_exit) == 0 || cmd.compare(Holo_DB_quit) == 0 ||
          cmd.compare(Holo_DB_q) == 0) {
        exit(EXIT_SUCCESS);
      } else if (cmd.compare(Holo_DB_help) == 0) {
        dis_msg.display_HelpMsg();
      } else if (cmd.compare(Holo_DB_clear) == 0) {
        system("cls");
      } else {
        // sql语句
      }
    } else {
      printf(
          "Error:unkonw commond or invalid arguments:[%s]. Enter[.help] for "
          "help!\n",
          cmd.c_str());
    }
  }
  return 0;
}
