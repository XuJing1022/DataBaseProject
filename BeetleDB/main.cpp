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
			cout << "\'" + tmp + "\'" + "������ȷ�����������help�鿴������" << endl;
		}
	}
}