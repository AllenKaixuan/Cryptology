#include"rsa.h"

// ����һ��������Ϊ��������
void SortPrime(BigInt& n)
{
	int i = 0;
	BigInt divisor;
	const int length = sizeof(prime) / sizeof(int);

	while (i != length)
	{
		n.Random();
		while (!n.IsOdd())
			n.Random();

		i = 0;
		for (; i < length; i++)
			if ((n % prime[i]) == 0)
				break;
	}
}

// RabinMiller���ĳ�����Ƿ�������
bool RabinMiller(const BigInt& n)
{
	BigInt r, a, y;
	unsigned int s, j;
	r = n - 1;
	s = 0;

	while (!r.IsOdd())
	{
		s++;
		r >> 1;
	}

	a.RandomSmall();	// �������һ��С��N-1�ļ����a

	y = PowerMode(a, r, n);	//y = a��r����ģn

	//���J=2��J<S��
	if ((!(y == 1)) && (!(y == (n - 1))))
	{
		j = 1;

		while ((j <= s - 1) && (!(y == (n - 1))))
		{
			y = (y * y) % n;
			if (y == 1)
				return false;
			j++;
		}

		if (!(y == (n - 1)))
			return false;
	}
	return true;
}

//����һ������
BigInt GeneratePrime()
{
	srand((unsigned)time(NULL));
	BigInt n;
	int time = 0;	// ��¼������������
	bool success=false;
	
	// ѭ������������ͨ��5��RabinMiller����
	while (!success)
	{
		time++;
		//����һ����������
		cout << "���ɵ�" << time << "����������" << endl;
		SortPrime(n);
		n.display();

		//��������RABINMILLER����,����ȫ��ͨ���������ϸ�
		for (int i=0; i < 5; i++)
		{
			cout << "��" << i + 1 << "��RabinMiller���ԣ�";
			if (!RabinMiller(n))
			{
				cout << "Fail.." << endl;
				break;
			}
			cout << "Pass!" << endl;
			success=true;
		}
		cout<<endl;
	}
	return n;
}

// շת��������������������Լ��
BigInt Gcd(const BigInt& m, const BigInt& n)
{
	if (n == 0)
		return m;
	else
		return Gcd(n, m % n);
}

// ��չŷ�������˷���Ԫ
BigInt ExtendedGcd(const BigInt& a, const BigInt& b, BigInt& x, BigInt& y)
{
	BigInt t, d;

	// ���һ��������Ϊ�����޷����г�������
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}

	d = ExtendedGcd(b, a % b, x, y);
	t = x;
	x = y;
	y = t - ((a / b) * y);
	return d;
}

BigInt StringToBigInt(string str)
{
	BigInt a(0);
	for(int i=str.length()-1;i>=0;i--)
	{
		int x=str[i]+128;
		BigInt temp=a*256;
		a=temp+x;
	}
	//cout << "a" << a.GetLenth() << endl;
	return a;
}

string BigIntToString(BigInt num)
{
	string str="";
	while(!(num==0))
	{
		BigInt temp=num%256;
		int x=temp.ToInt();
		num=num/256;
		str+=(char)(x-128);
	}
	return str;
}

string BigIntToHex(BigInt &num)
{
	return num.ToHex();
}

BigInt HexToBigInt(string hex)
{
	BigInt result(0);
	for(int i=0;i<hex.length();i++)
	{
		int val;
		switch (hex[i])
		{
		case '0':val=0;break;
		case '1':val=1;break;
		case '2':val=2;break;
		case '3':val=3;break;
		case '4':val=4;break;
		case '5':val=5;break;
		case '6':val=6;break;
		case '7':val=7;break;
		case '8':val=8;break;
		case '9':val=9;break;
		case 'A':val=10;break;
		case 'B':val=11;break;
		case 'C':val=12;break;
		case 'D':val=13;break;
		case 'E':val=14;break;
		case 'F':val=15;break;
		}
		result = result * 16 + val;
	}
	return result;
}

Rsa::Rsa()
{
    BigInt p = GeneratePrime();
    BigInt q = GeneratePrime();
    n = p * q;
    BigInt f = (p - 1) * (q - 1);
    BigInt x,temp;

	// ѭ������e��ֱ����������
    while (1)
    {
        //������fn���ʵ�e
        e.Random();
        while (!(Gcd(e, f) == 1))
            e.Random();

        //����չŷ������㷨��ͼ���eģt�ĳ˷���Ԫ
        temp = ExtendedGcd(e, f, d, x);

        // e*dģt���Ϊ1 -> d��eģt�ĳ˷���Ԫ���������
        temp = (e * d) % f;
        if (temp == 1)
            break;
    }
}

Rsa::Rsa(BigInt &N, BigInt &E)
{
	n=N;
	e=E;
}

Rsa::Rsa(BigInt &N, BigInt &E, BigInt &D)
{
	n=N;
	e=E;
	d=D;
}

