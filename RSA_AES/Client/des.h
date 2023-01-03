#include<iostream>
#include<bitset>
#include<string>
#include<algorithm>

using namespace std;
void initkey(bitset<64> sk, bitset<48>* K);
bitset<64> InitReplace(bitset<64> a);
bitset<32> f(bitset<32>R, bitset<48>currentKey);
bitset<64> InverseIP(bitset<64>a);
bitset<64> encrypt(bitset<64> text, bitset<64>key);
bitset<64> decrypt(bitset<64> text, bitset<64>key);
bitset<64> getM(string input);
bitset<28> leftshift(bitset<28> a, int n);
int encryptForRSA(int high32, int low32);