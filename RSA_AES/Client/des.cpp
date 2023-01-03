#include "des.h"

// 初始置换IP
int IP[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7 };

// 逆初始置換IP^-1
int IIP[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25 };

// 选择扩展运算E
int E[48] = { //扩展换位表
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1 };

// 置换运算P
int P[32] = {
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25 };

// 置换选择PC-1
int PC1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4 };

// 置换选择PC-2
int PC2[56] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32 };

// 循环位移表
int MOV[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// S盒
int S[8][4][16] = {
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11} };

const struct des_test_case {
	int num, mode; // mode 1 = encrypt
	unsigned char key[8], txt[8], out[8];
} cases[] = {
	{ 1, 1,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
				{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 } },
	{ 2, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 },
				{ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 3, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 },
				{ 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 4, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA },
				{ 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 5, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F },
				{ 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 6, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 },
				{ 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 7, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF },
				{ 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 8, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F },
				{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 9, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 },
				{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{10, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A },
				{ 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

	{ 1, 0,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
				{ 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 },
				{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
	{ 2, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 } },
	{ 3, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 } },
	{ 4, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA } },
	{ 5, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F } },
	{ 6, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 } },
	{ 7, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF } },
	{ 8, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F } },
	{ 9, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 } },
	{10, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
				{ 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
				{ 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A } }
};

bitset<64> getM(string input)
{
	int index = 0;
	char* temp = new char[16];
	int indexoftemp = 0;
	while (input[index] != '\0')
	{
		if (index > 1 && (input[index - 1] == 'x' || input[index - 2] == 'x'))
		{
			if (indexoftemp == 16)
				break;
			temp[indexoftemp] = input[index];
			indexoftemp++;
		}
		index++;
	}

	bitset<64> result;
	for (index = 0; index < 16; index++)
	{
		int point = 63 - 4 * index;
		switch (temp[index])
		{
		case '0':
			result[point] = 0;
			result[point] = 0;
			result[point] = 0;
			result[point] = 0;
			break;
		case '1':
			result[point] = 0;
			result[point] = 0;
			result[point] = 0;
			result[point] = 1;
			break;
		case '2':
			result[point] = 0;
			result[point] = 0;
			result[point] = 1;
			result[point] = 0;
			break;
		case '3':
			result[point] = 0;
			result[point] = 0;
			result[point] = 1;
			result[point] = 1;
			break;
		case '4':
			result[point] = 0;
			result[point] = 1;
			result[point] = 0;
			result[point] = 0;
			break;
		case '5':
			result[point] = 0;
			result[point] = 1;
			result[point] = 0;
			result[point] = 1;
			break;
		case '6':
			result[point] = 0;
			result[point] = 1;
			result[point] = 1;
			result[point] = 0;
			break;
		case '7':
			result[point] = 0;
			result[point] = 1;
			result[point] = 1;
			result[point] = 1;
			break;
		case '8':
			result[point] = 1;
			result[point] = 0;
			result[point] = 0;
			result[point] = 0;
			break;
		case '9':
			result[point] = 1;
			result[point] = 0;
			result[point] = 0;
			result[point] = 1;
			break;
		case 'A':
			result[point] = 1;
			result[point] = 0;
			result[point] = 1;
			result[point] = 0;
			break;
		case 'B':
			result[point] = 1;
			result[point] = 0;
			result[point] = 1;
			result[point] = 1;
			break;
		case 'C':
			result[point] = 1;
			result[point] = 1;
			result[point] = 0;
			result[point] = 0;
			break;
		case 'D':
			result[point] = 1;
			result[point] = 1;
			result[point] = 0;
			result[point] = 1;
			break;
		case 'E':
			result[point] = 1;
			result[point] = 1;
			result[point] = 1;
			result[point] = 0;
			break;
		case 'F':
			result[point] = 1;
			result[point] = 1;
			result[point] = 1;
			result[point] = 1;
			break;
		default:
			break;
		}
	}

	return result;
}

// shift
bitset<28> leftshift(bitset<28> a, int n)
{
	bitset<28> result;
	for (int i = 0; i < 28; i++)
		result[(i + n) % 28] = a[i];
	return result;
}

// 子密钥生成
void initkey(bitset<64> sk, bitset<48>* K)
{
	bitset<56> key;
	bitset<28> C, D;

	// 64->56
	for (int i = 0; i < 56; i++)
		key[55-i] = sk[64-PC1[i]];

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			C[j] = key[28 + j];
			D[j] = key[j];
		}
		
		C = leftshift(C, MOV[i]);
		D = leftshift(D, MOV[i]);

		for (int j = 0; j < 28; j++)
		{
			key[j] = D[j];
			key[j + 28] = C[j];
		}

		bitset<48>tempkey;
		for (int j = 0; j < 48; j++)
			tempkey[47-j] = key[56-PC2[j]];

		K[i] = tempkey;
	}
}

// 初始IP置换
bitset<64> InitReplace(bitset<64> a)
{
	bitset<64> result;
	for (int i = 0; i < 64; i++)
		result[63-i] = a[64-IP[i] ];
	return result;
}

// E, XOR, S-box, P
bitset<32> f(bitset<32> R, bitset<48> currentKey)
{
	bitset<48> expand;
	for (int i = 0; i < 48; i++)
		expand[47-i] = R[32-E[i] ];
	expand = expand ^ currentKey;
	bitset<32> temp;
	int x = 0;
	for (int i = 0; i < 48; i += 6)
	{
		int row = expand[47-i] * 2 + expand[47-i- 5];
		int col = expand[47-i- 1] * 8 + expand[47-i- 2] * 4 + expand[47-i- 3] * 2 + expand[47-i- 4];
		int num = S[i / 6][row][col];
		bitset<4> t(num);
		temp[31-x] = t[3];
		temp[31-x- 1] = t[2];
		temp[31-x- 2] = t[1];
		temp[31-x- 3] = t[0];
		x += 4;
	}
	bitset<32> result;
	for (int i = 0; i < 32; i++)
		result[31-i] = temp[32-P[i] ];

	return result;
}

bitset<64> InverseIP(bitset<64> a)
{
	bitset<64> r;
	for (int i = 0; i < 64; i++)
		r[63-i] = a[64-IIP[i] ];
	return r;
}

// 加密
bitset<64> encrypt(bitset<64> text, bitset<64> key)
{
	bitset<48>* K = new bitset<48>[16];
	initkey(key, K);

	bitset<64> IPtext = InitReplace(text);
	bitset<32> L, R;

	for (int i = 0; i < 32; i++)
	{
		L[i] = IPtext[i + 32];
		R[i] = IPtext[i];
	}
	bitset<32> temp;
	for (int round = 0; round < 16; round++)
	{
		temp = R;
		R = L ^ f(R, K[round]);
		L = temp;
	}

	bitset<64> result;
	for (int i = 0; i < 32; i++)
	{
		result[i] = L[i];
		result[i + 32] = R[i];
	}
	result = InverseIP(result);
	return result;
}

// 解密函数
bitset<64> decrypt(bitset<64> text, bitset<64> key)
{
	bitset<48>* K = new bitset<48>[16];
	initkey(key, K);
	bitset<32> L, R;
	bitset<64> IPtext = InitReplace(text);

	for (int i = 0; i < 32; i++)
	{
		L[i] = IPtext[i + 32];
		R[i] = IPtext[i];
	}
	bitset<32> temp;
	for (int round = 0; round < 16; round++)
	{
		temp = R;
		R = L ^ f(R, K[15 - round]);
		L = temp;
	}

	bitset<64> result;
	for (int i = 0; i < 32; i++)
	{
		result[i] = L[i];
		result[i + 32] = R[i];
	}
	result = InverseIP(result);
	return result;
}

int encryptForRSA(int high32, int low32) {
    // 首先使用两个int进行一个处理，让他们随机选择一组key
    int keyNum = (high32 % 4 * low32 / 10 + 1) % 10;
	bitset<64>key;
    // 获取key
    for (int countKey = 0; countKey < 8; countKey++) {
        bitset<8> tempKey(int(cases[keyNum].key[countKey]));
        for (int countOfBit = 0; countOfBit < 8; countOfBit++) {
            key[63 - (countKey * 8 + countOfBit)] = tempKey[7 - countOfBit];
        }
    }

    // 对两个输入的int进行处理，使其保存在bitset<64> m中
    bitset<32> tempHigh(high32);
    bitset<32> tempLow(low32);
    bitset<64> m;
	for (int i = 0; i < 64; i++) {
        if (i < 32)
            m[i] = tempLow[i];
        else
            m[i] = tempHigh[i - 32];
    }

    // DES加密
	bitset<64> c;
    c = encrypt(m,key);

    // 选取其中的32位，作为一个int进行返回，从第一位不是0开始（保证是奇数，有利于后面的检测）
    bitset<32> resultBitset;
    int indexNotZero = 0;
    for (; indexNotZero < 64; indexNotZero++)
        if (c[indexNotZero])
            break;
    for (int i = 0; i < 32; i++)
        resultBitset[i] = c[indexNotZero++];

    // 转换成int，返回
    int result = int(resultBitset.to_ulong());
    return result;
}