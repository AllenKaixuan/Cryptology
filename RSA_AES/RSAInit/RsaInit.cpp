#include<iostream>
#include<string>
#include"rsa.h"
#include"des.h"
#include"BigInt.h"
#include<fstream>
using namespace std;

int main()
{
	Rsa R;
	ofstream outfile1("../ServerRsa.txt");
	outfile1 << R.n;
	outfile1<< R.e;
	outfile1<< R.d;
	outfile1.close();

	ofstream outfile2("../ServerPk.txt");
	outfile2 << R.n;
	outfile2 << R.e;
	outfile2.close();

	Rsa T;
	ofstream outfile3("../ClientRsa.txt");
	outfile3 << T.n;
	outfile3 << T.e;
	outfile3 << T.d;
	outfile3.close();

	ofstream outfile4("../ClientPk.txt");
	outfile4 << T.n;
	outfile4 << T.e;
	outfile4.close();

	cout << "Server RsaKey" << endl;
	cout << "ServerRsa.n" << endl;
	R.n.display();
	cout << "ServerRsa.e" << endl;
	R.e.display();
	cout << "ServerRsa.d" << endl;
	R.d.display();

	cout << "Client RsaKey" << endl;
	cout << "ClientRsa.n" << endl;
	T.n.display();
	cout << "ClientRsa.e" << endl;
	T.e.display();
	cout << "ClientRsa.d" << endl;
	T.d.display();
}