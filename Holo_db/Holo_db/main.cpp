#include<iostream>
#include<string>
#include<algorithm>
#include"commonds_def.h"
#include"display_msg.h"
#include"log.h"

bool is_invalid_cmd(const std::string& cmd)
{
	if (cmd.empty()) {
		return false;
	} else if (cmd[0] != '.') {
		return false;
	} else if (cmd == Holo_DB_exit
		|| cmd == Holo_DB_quit
		|| cmd == Holo_DB_help
		|| cmd == Holo_DB_clear
		|| cmd == Holo_DB_q) {
		return true;
	} else {
		return false;
	}
}

void trim_cmd(std::string& cmd) 
{
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

int main(int count, char* input_parameters[])
{
	std::string db_path = "";	//数据库文件的路径
	if (count > 1) {
		//count表示程序入参个数,默认为1,参数是程序的名字
		db_path = input_parameters[1];
	}
	
	DisplayMsg dis_msg;
	dis_msg.dispaly_introduce_Holo_DB();
	char InputBuffer[2048];	//存放用户输入

	while (true) {
		dis_msg.display_HoloDB();
		std::cin.getline(InputBuffer, sizeof(InputBuffer));
		std::string cmd = InputBuffer;
		trim_cmd(cmd);
		if (is_invalid_cmd(cmd)) {
			if (cmd.compare(Holo_DB_exit) == 0 || cmd.compare(Holo_DB_quit) == 0 || cmd.compare(Holo_DB_q) == 0) {
				exit(EXIT_SUCCESS);
			} else if (cmd.compare(Holo_DB_help) == 0) {
				dis_msg.display_HelpMsg();
			} else if (cmd.compare(Holo_DB_clear) == 0) {
				system("cls");
			}
		} else {
			printf("Error: unkonw commond or invalid arguments:\"%s\". Enter \".help\" for help\n", cmd.c_str());
		}
	}
	return 0;
}
