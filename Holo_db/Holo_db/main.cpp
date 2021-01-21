#include<iostream>
#include<string>
#include<algorithm>
#include"commonds_def.h"


void dispaly_introduce_Holo_DB()
{
	std::string display_introduce_msg = R"(
==============================================
  This is a personal project, do this project 
I hope to understand how does a database work, 
especiall how to achieve.
  this db i plan to clone sqlite, just achieve
some feature i want. And Why I use c++? because
i work with it. Maybe in the future, I will 
consider implementing a C language version.
  if you have any question or sugesstion,you can
contact me by e-mail.
===============================================
author:HLhuanglang
email:1282424466@qq.com
===============================================
	)";
	std::cout << display_introduce_msg << std::endl;
}

void display_HoloDB()
{
	std::cout << "Holo_DB> ";
}

void display_HelpMsg()
{
	std::string display_msg = R"(
================Holo_DB===================
|--------cmd--------|-------description--|
==========================================
|.help              |show info
|.clear             |clear screen
|.exit              |quit Holo_DB
|.quit              |quit Holo_DB
|.open /x/x/xxx.db  |open a database(Abs path)
|.create xxx.db     |create a database under current path
==========================================
	)";
	std::cout << display_msg << std::endl;
}

bool is_invalid_cmd(const std::string& cmd)
{
	if (cmd.empty()) {
		return false;
	} else if (cmd[0] != '.') {
		return false;
	} else if (cmd == Holo_DB_exit
		|| cmd == Holo_DB_quit
		|| cmd == Holo_DB_help
		|| cmd == Holo_DB_clear) {
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

int main()
{
	dispaly_introduce_Holo_DB();

	char InputBuffer[2048];	//存放用户输入

	while (true) {
		display_HoloDB();
		std::cin.getline(InputBuffer, sizeof(InputBuffer));
		std::string cmd = InputBuffer;
		trim_cmd(cmd);
		if (is_invalid_cmd(cmd)) {
			if (cmd.compare(Holo_DB_exit) == 0 || cmd.compare(Holo_DB_quit) == 0) {
				exit(EXIT_SUCCESS);
			} else if (cmd.compare(Holo_DB_help) == 0) {
				display_HelpMsg();
			} else if (cmd.compare(Holo_DB_clear) == 0) {
				system("cls");
			}
		} else {
			printf("Error: unkonw commond or invalid arguments:\"%s\". Enter \".help\" for help\n", cmd.c_str());
		}
	}
	return 0;
}
