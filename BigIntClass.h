//
// Created by я on 30.09.2022.
//

#ifndef BINT_BIGINTCLASS_H
#define BINT_BIGINTCLASS_H


#include <iostream>
#include <vector>
#include <iomanip>

class BigInt {
public:
    char sign = '+';
    std::vector<int> number;

    void DeleteNulls();
    BigInt SumDifSign(BigInt tmp_1, BigInt tmp_2, char sign_of_res);
    BigInt BinarySearch(BigInt division, BigInt divider);
    BigInt Abs(BigInt input);
    BigInt FromBinToDecPos(std::string value);
    BigInt FromBinToDecNeg(std::string value);
    std::string ChangeToBinPositive(BigInt value, char value_sign);
    std::string ChangeToBinNegative(std::string value, char value_sign, int size);

    BigInt();
    BigInt(int);
    explicit BigInt(std::string);
    BigInt(const BigInt&);
    //~BigInt();

    BigInt& operator=(const BigInt&);

    BigInt operator~() const;

    BigInt& operator++();
    const BigInt operator++(int);
    BigInt& operator--();
    const BigInt operator--(int);

    BigInt& operator+=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator^=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);

    BigInt operator+() const;  // unary +
    BigInt operator-();  // unary -

    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    explicit operator int() const;
    operator std::string() const;

    size_t size() const;
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);


std::ostream& operator<<(std::ostream& o, const BigInt& i);


#endif //BINT_BIGINTCLASS_H
