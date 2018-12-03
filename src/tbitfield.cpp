// ����, ���, ���� "������ ����������������-2", �++, ���
//
// tbitfield.cpp - Copyright (c) ������� �.�. 07.05.2001
//   ������������ ��� Microsoft Visual Studio 2008 �������� �.�. (19.04.2015)
//
// ������� ����


#include "tbitfield.h"
#include <math>

TBitField::TBitField(int len)

{

	if (len < 0)
		throw "error";

	MemLen = ceil(static_cast<double>(len) / (sizeof(TELEM)* 8));
	BitLen = len;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // ����������� �����������
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = MemLen - 1; i >= 0; i--)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // ������ ��� ��� ���� n
{
	if (n < 0 || n > BitLen)
		throw "error";
	else if (n != 0)
		return MemLen - ceil(static_cast<double>(n) / (sizeof(TELEM)* 8));
	else
		return MemLen - 1;
}

TELEM TBitField::GetMemMask(const int n) const // ������� ����� ��� ���� n
{
	if (n < 0 || n > BitLen)
		throw "error";
	else
		return 1 << n;
}

// ������ � ����� �������� ����

int TBitField::GetLength(void) const // �������� ����� (�-�� �����)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // ���������� ���
{
	if (n < 0 || n > BitLen)
		throw "error";
	else if (GetBit(n) != 1)
		pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // �������� ���
{
	if (n < 0 || n > BitLen)
		throw "error";
	else if (GetBit(n) != 0)
		pMem[GetMemIndex(n)] ^= GetMemMask(n);
}

int TBitField::GetBit(const int n) const // �������� �������� ����
{
	if (n < 0 || n > BitLen)
		throw "Unexpected bit index";
	else
		return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0 ? 1 : 0;
}

// ������� ��������

TBitField& TBitField::operator = (const TBitField &bf) // ������������
{
	if (bf.MemLen == 0)
		throw "error";
	else
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];

		for (int i = MemLen - 1; i >= 0; i--)
		{
			pMem[i] = bf.pMem[i];
		}

		return *this;
	}
}

int TBitField::operator == (const TBitField &bf) const // ���������
{
	if (bf.MemLen == 0)
		throw "error";
	else
	{
		bool check = 1;
		int length;

		if (BitLen >= bf.BitLen)
		{
			length = bf.MemLen - 1;
			for (int i = MemLen - 1; i >= bf.MemLen - 1; i--, length--)
			{
				check &= pMem[i] == bf.pMem[length];
			}
		}
		else
		{
			length = MemLen - 1;
			for (int i = bf.MemLen - 1; i >= MemLen - 1; i--, length--)
			{
				check &= pMem[length] == bf.pMem[i];
			}
		}

		return check;
	}
}

int TBitField::operator != (const TBitField &bf) const // ���������
{
	return !(operator==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // �������� "���"
{
	int len = BitLen;

	if (BitLen < bf.BitLen)
		len = bf.BitLen;

	TBitField tmp(len);

	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];

	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator & (const TBitField &bf) // �������� "�"
{
	int len = BitLen;

	if (bf.BitLen > len)
		len = bf.BitLen;

	TBitField temp(len);

	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];

	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];

	return temp;
}

TBitField TBitField::operator~ (void) // ���������
{
	TBitField tmp(BitLen);
	TELEM mask = 0;

	for (int i = (MemLen - 1) * sizeof(TELEM)* 8; i < BitLen; i++)
		mask += GetMemMask(i);
	tmp.pMem[0] = pMem[0] ^ mask;

	for (int i = MemLen - 1; i > 0; i--)
		tmp.pMem[i] = ~pMem[i];
	return tmp;
}

// ����/�����

istream &operator>>(istream &istr, TBitField &bf) // ����
{
	int i = 0;
	char ch;
	do { istr >> ch; } while (ch != ' ');
	while (1)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else if (ch == '1')
			bf.SetBit(i++);
		else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // �����
{
	for (int i = 0; i < bf.GetLength(); i++)
	if (bf.GetBit(i)) ostr << '1';
	else ostr << '0';
	return ostr;
}