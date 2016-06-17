#include"QueryParser.h"
#include<iostream>
using namespace std;
int main()
{
	string tmp;
	QueryParser t;
	t.ExecuteSQL("help;");
	while (getline(cin, tmp))
	{
		if (tmp.find(';') != string::npos)
		{
			t.ExecuteSQL(tmp);
		}
		else
		{
			cout << "\'" + tmp + "\'" + "不是正确的命令。请输入help查看帮助。" << endl;
		}
	}
}