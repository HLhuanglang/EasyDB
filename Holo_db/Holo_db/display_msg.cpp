#include "display_msg.h"

#include <iostream>
#include <string>

void DisplayMsg::display_HoloDB() { std::cout << "Holo_DB> "; }

void DisplayMsg::dispaly_introduce_Holo_DB() {
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

void DisplayMsg::display_HelpMsg() {
  std::string display_msg = R"(
================Holo_DB===================
|--------cmd--------|-------description--|
==========================================
|.help              |show info
|.clear             |clear screen
|.exit              |quit Holo_DB
|.quit              |quit Holo_DB
|.create xxx.db     |create a database under current path
|.open /x/x/xxx.db  |open a database(Abs path)
|.close             |close the currrent database
|.database          |Show the currently used database
==========================================
	)";
  std::cout << display_msg << std::endl;
}