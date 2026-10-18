// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>
#include <string>
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        len = 1;
    BitLen = len;
    MemLen = ((len - 1) >> 5) + 1;
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen ;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i <MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1u << (n&31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen)) 
        throw out_of_range("index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) 
        throw out_of_range("index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) 
        throw out_of_range("index out of range");
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0 ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) 
        return *this;
    else {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        copy(bf.pMem, bf.pMem + MemLen, pMem);
        /*for (int i = 0; i < MemLen; i++) 
            pMem[i] = bf.pMem[i];*/
        return *this;
    }
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return  0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    for (int i = 0; i < MemLen; i++) 
        result.pMem[i] |= pMem[i];

    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] |= bf.pMem[i];

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int minLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(minLen);   
    for (int i = 0; i < result.MemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];

    return result;

}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];
    int last_bits = BitLen & 31;
    TELEM mask;
    if (last_bits != 0) { 
        TELEM mask = (1u << last_bits) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string s;
    istr >> s;
    int len = s.length();
    for (int i = 0; i < bf.MemLen; i++)
        bf.pMem[i] = 0;
    if (len > bf.BitLen)
        len = bf.BitLen;
    for (int i = 0; i < len; i++) {
        if (s[i] == '1')
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << (bf.GetBit(i) ? 1 : 0);

    return ostr;
}
