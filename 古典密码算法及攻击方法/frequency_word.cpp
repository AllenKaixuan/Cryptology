#include<iostream>
#include<sstream>
#include<map>
#include<string>
#include<algorithm>
using namespace std;

//通过STL的transform算法配合的toupper和tolower来实现该功能，只针对26个英文字母，其余的不会改变。
//输入的字符串中只考虑了空格这一种特殊字符，没有考虑其它特殊字符
int main()
{
	string str;
	getline(cin, str);
	transform(str.begin(), str.end(), str.begin(), ::tolower);//将大写全部转换为小写
	stringstream ss(str);
    map<string, int> m;
	map<string, int>::iterator it;
	pair<map<string, int>::iterator, bool> flag;
	while (ss >> str)    //以空格为分隔符读入每一个单词
	{
	    flag = m.insert(pair<string, int>(str, 1));
		if ( ! flag.second)
		{
			it = m.find(str);  //map自动排序，需要先找到其位置
			it->second++;      //出现相同单词value值+1
		}
	}
	multimap<int, string>m1;
	for (it = m.begin(); it != m.end(); it++)
	{
		m1.insert(make_pair(it->second, it->first));  //将m中的单词转存到m1中，方便按value值排序
	}
	for (multimap<int,string>::iterator iter = --m1.end();iter != --m1.begin(); iter--)   //降序输出
		if(iter->first>=1)
			cout << iter->second << " " << iter->first<< endl;
	return 0;
}