#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<string>

using namespace std;
map<char,char> table;
map<char,char> reverse_table;
vector<int> a,b;
struct secrete_key
{
    int ascii;
    bool visit = 0;

};
void create_table()
{
   
   
    //使用一句话密钥，剩余字母顺序排在后面
    string s = "welcom to nankai university";
    secrete_key *sk = new secrete_key[26];
    for(int i=0;i<26;i++)
    {
        sk[i].ascii = i+97; 
        a.push_back(i+97);
    }
    for(int i =  0;i<s.size();i++)  // 标记一句话中出现的字母，并按保存其顺序
    {
        if(s[i]>96&&s[i]<123)
        {
            int index = int(s[i])-97;
            if(!sk[index].visit)
            {
                sk[index].visit = 1;
                b.push_back(sk[index].ascii);
            }
                
        }
    }
    for(int i=0;i<26;i++)
        if(!sk[i].visit)
            b.push_back(sk[i].ascii);  // 将未出现的放置其中
    
    for(int i=0;i<26;i++)
    {
        table.insert(pair<char,char>(int(a[i]),int(b[i]))); // 构造置换表
        reverse_table.insert(pair<char,char>(int(b[i]),int(a[i]))); //反转置换表，decode用
    }
       
}

string encode(string m)
{
    transform(m.begin(), m.end(), m.begin(), ::tolower);
    for(int i=0;i<m.size();i++)
    {
        if(m[i]>96&&m[i]<123)
            m[i] = table[m[i]];
    }
    return m;
}

string decode(string c)
{
    for(int i=0;i<c.size();i++)
    {
        if(c[i]>96&&c[i]<123)
            c[i] = reverse_table[c[i]];
    }
    return c;
}

int main()
{

    string s = "Computer Science and Tecnology";
    create_table();
    cout<<encode(s)<<endl;
    cout<<decode(encode(s))<<endl;
}




