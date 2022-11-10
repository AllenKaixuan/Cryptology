#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

string encode(string m, int shift)
{
    shift = shift % 26;
    for (int i = 0; i < m.size(); i++)
    {
        if (m[i] + shift>96&& m[i] + shift < 123)
            m[i] = m[i] + shift;
        else if (m[i] + shift >= 123)
            m[i] = (m[i] + shift) % 123 + 97;
        else
            m[i] = m[i];
    }
    return m;
}
void decode(string c)
{
    for(int i = 1; i < 26; i++)
    {
        cout<<"cleartext:"<<encode(c,i)<<" shift "<<26-i<<endl;
    }

}

int main()
{

    string m;
    m = "public keys";
    transform(m.begin(), m.end(), m.begin(), ::tolower);
    cout << m<<endl;
    decode(encode(m, 3));


}