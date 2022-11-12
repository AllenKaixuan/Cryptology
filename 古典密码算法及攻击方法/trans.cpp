#include<iostream>
#include<map>
#include<string>
#include<algorithm>
using namespace std;
map<char, char> table;
int main()
{
    string c = "csnmjbprigxaheqyfzvdto";
    string m = "ETAOINRSHCPLBGMYFUWDVZ";
    transform(m.begin(), m.end(), m.begin(), ::tolower);
    for (int i = 0; i < c.size(); i++)
        table.insert(pair<char, char>(c[i], m[i]));
    string s;
    getline(cin, s);
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    for (int i = 0; i < s.size(); i++)
    {
        if (c.find(s[i], 0)!=-1)
        {
            cout << table[s[i]];
        }
        else
        {
            cout << s[i];
        }
    }


}