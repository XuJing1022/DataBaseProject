//
// main.cpp
//
// Created by Chenbjin on 2015/12/18.
// Copyright(C) 2015, Chenbjin. All rights reserved.
//
#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/timer.hpp>

#include "Interpreter.h"
using namespace std;

void Welcome();

int main()
{
	Welcome();
	string sql;			//sql statement
	string line;        //input line
	size_t end;         //end pos
	Interpreter BeetleInterpreter;
	int n = 100;
	while (true)
	{
		
		/*std::ostringstream oss;
		std::string id = "";
		std::string ss = "";
		int a = 200 - n;
		float s = (float)(200 - n) / 2;
		oss << a;
		id += oss.str();
		oss.clear();
		oss << s;
		ss += oss.str();

		if (n == 100) {
			line = "use SystemForTest;";
			n--;
		}
		else if(n) {
			line = "insert into T values(" + id + "," + ss + ",'KKL');";
			n--;
		}
		else*/
		{
			cout<< endl << "BeetleDB> ";
			getline(cin, line);
		}
	
	
		
		sql = string(line);
		if (sql == "") { continue; } /* Only is an empty input line */

		boost::algorithm::trim(sql);  /* remove blank ahead or behind of the sql */

		if (sql.compare(0,4,"exit") == 0 || sql.compare(0,4,"quit") == 0 || sql.compare(0,2,"\\q") == 0 || sql.compare(0,2,"\\e") == 0)
		{
			BeetleInterpreter.ExecSQL("quit");
			break;
		}

		while ((end = sql.find(";")) == string::npos) /* not end */
		{
			cout <<"       -> ";
			getline(cin, line);
			if (line == "") { continue; } /* Only is an empty input line */
			sql += "\n" + string(line);
		}
		boost::timer ter;
		BeetleInterpreter.ExecSQL(sql);  /* Execute sql statement */
		cout << "(" << ter.elapsed() << " sec)" << endl;
	}
	system("pause");
	return 0;
}

void Welcome()
{
	cout << "Welcome to BeetleDB. Commands end with ';'." << endl;
	cout << "Copyright (c) 2015, Beetle Group. All rights reserved." << endl << endl;
	cout <<	"Type 'help;' for help." << endl;
}
