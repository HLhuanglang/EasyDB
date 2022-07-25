#include <sys/stat.h>

#include <algorithm>
#include <iostream>
#include <string>

#include "display_msg.h"

void trim_whitespace(std::string &cmd) {
	size_t cmd_begin_pos = cmd.find_first_not_of(" ");
	if (cmd_begin_pos != std::string::npos) {
		size_t cmd_end_pos = cmd.find_first_of(" ", cmd_begin_pos);
		if (cmd_end_pos != std::string::npos) {
			cmd = cmd.substr(cmd_begin_pos, cmd_end_pos - cmd_begin_pos);
		} else {
			cmd = cmd.substr(cmd_begin_pos);
		}
	}
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
}

int main(int count, char *input_parameters[]) {
	easydb_helper::dispaly_introduce_easydb();
	easydb_helper::display_help_msg();
	return 0;
}