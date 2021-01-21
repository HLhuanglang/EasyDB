#include<iostream>
#include<string>
#include<algorithm>

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
|Holo_DB xxx.db     |open a database
|.help              |show info
|.clear             |clear screen
|.cls               |clear screen
|.q                 |quit Holo_DB
|.exit              |quit Holo_DB
|.quit              |quit Holo_DB
==========================================
	)";
	std::cout << display_msg << std::endl;
}

int main()
{
	char InputBuffer[2048];	//存放用户输入

	while (true) {
		display_HoloDB();
		std::cin.getline(InputBuffer, sizeof(InputBuffer));
		std::string cmd = InputBuffer;

		//过滤空格
		size_t cmd_begin_pos = cmd.find_first_not_of(" ");
		if (cmd_begin_pos != std::string::npos) {
			size_t cmd_end_pos = cmd.find_first_of(" ", cmd_begin_pos);
			if (cmd_end_pos != std::string::npos) {
				cmd = cmd.substr(cmd_begin_pos, cmd_end_pos - cmd_begin_pos);
			}
			else {
				cmd = cmd.substr(cmd_begin_pos);
			}
		}
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
		if (cmd.compare(".exit") == 0
			|| cmd.compare(".quit") == 0
			|| cmd.compare(".q")==0) {
			exit(EXIT_SUCCESS);
		} else if (cmd.compare(".help") == 0) {
			display_HelpMsg();
		} else if (cmd.compare(".clear") == 0
			|| cmd.compare(".cls") == 0) {
			system("cls");
		}
		else {
			printf("Error: unkonw commond or invalid arguments:\"%s\". Enter \".help\" for help\n", cmd.c_str());
		}
	}
	return 0;
}
