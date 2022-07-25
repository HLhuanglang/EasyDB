#include "display_msg.h"

#include <iostream>
#include <string>

void easydb_helper::display_easydb() { std::cout << "easydb> "; }

void easydb_helper::dispaly_introduce_easydb() {
	std::string display_introduce_msg = R"(==============================================
  This is a personal project, do this project
I hope to understand how does a database work,
especiall how to achieve.
  this db I plan to clone sqlite, just achieve
some feature I want. And Why I use c++? because
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

void easydb_helper::display_help_msg() {
	std::string display_msg = R"(
===================EasyDB=======================
|--------cmd-----------|-------description-----|
================================================
|.help                 |show help info
|.clear                |clear screen
|.exit                 |quit EasyDB
|.quit                 |quit EasyDB
|.create xxx.edb       |create a database under current path
|.open /x/x/xxx.edb    |open a database(Abs path)
|.close                |close the currrent database
|.database             |Show the currently used database
================================================
	)";
	std::cout << display_msg << std::endl;
}