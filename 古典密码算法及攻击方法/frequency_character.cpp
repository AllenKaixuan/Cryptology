#include<iostream>
#include<sstream>
#include<map>
#include<string>
#include<algorithm>
using namespace std;

int main()
{
	string str;
	getline(cin, str);
	transform(str.begin(), str.end(), str.begin(), ::tolower);//将大写全部转换为小写
	map<char, int> table;
	map<char, int>::iterator it;
	pair<map<char, int>::iterator, bool> flag;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] < 123 && str[i]>96)
		{
			flag = table.insert(pair<char, int>(str[i], 1));
			if (!flag.second)
			{
				it = table.find(str[i]);  //map自动排序，需要先找到其位置
				it->second++;      //出现相同字符value值+1
			}
		}

	}


	multimap<int, char>reverse_table;
	for (it = table.begin(); it != table.end(); it++)
	{
		reverse_table.insert(make_pair(it->second, it->first));  //将m中的单词转存到m1中，方便按value值排序
	}
	for (multimap<int, char>::iterator iter = --reverse_table.end(); iter != --reverse_table.begin(); iter--)   //降序输出
		if (iter->first >= 1)
			cout << iter->second << " " << double(iter->first) / str.size() << endl;
				
	return 0;
}