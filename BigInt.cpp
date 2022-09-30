#include "BigIntClass.h"

enum { CountOfMaxElementsInSell = 9, Base = 1000000000 };

void BigInt::DeleteNulls() {
    if (number.size() > 1) {
        for (int i = number.size() - 1; i > 0; i--) {
            if (number[i] == 0) {
                auto it = number.begin();
                advance(it, i);
                number.erase(it);
            } else break;
        }
    }
    if (this->number.size() == 1 && this->number[0] == 0)
        sign = '+';
}

BigInt::BigInt() {
    number.push_back(0);
    sign = '+';
}

BigInt::BigInt(int input) {
    if (input < 0)
        sign = '-';
    input = abs(input);
    if (input == 0) number.push_back(0);
    while (input > 0) {
        number.push_back(input % Base);
        input /= Base;
    }
}

BigInt::BigInt(std::string input) {
    if (input[0] == '-' && input.size() < 2)
        throw std::invalid_argument("Wrong input.");

    if (input[0] != '-' && (input[0] < '0' || input[0] > '9'))
        throw std::invalid_argument("Wrong input.");

    for (int i = 1; i < input.size(); i++) {
        if (input[i] < '0' || input[i] > '9')
            throw std::invalid_argument("Wrong input.");
    }

    if (input[0] == '-') {
        sign = '-';
        input.erase(0, 1);
    }

    std::string tmp;
    while (input.size() >= 9) {
        auto size = input.size() - 9;
        tmp = input.substr(size);
        input.erase(size);
        number.push_back(stoi(tmp));
        tmp.clear();
    }
    if (!input.empty())
        number.push_back(stoi(input));

    DeleteNulls();
}

BigInt::BigInt(const BigInt &input) {
    this->sign = input.sign;
    this->number = input.number;
}
//~BigInt();

BigInt & BigInt::operator=(const BigInt &input) {
    this->sign = input.sign;
    this->number = input.number;
    return *this;
}

BigInt BigInt::operator~() const {
    BigInt copy(*this);
    ++copy;
    return -copy;
}

BigInt &BigInt::operator++() {
    BigInt tmp(1);
    *this += tmp;
    return *this;
}

const BigInt BigInt::operator++(int) {
    BigInt tmp = *this;
    ++*this;
    return tmp;
}

BigInt &BigInt::operator--() {
    BigInt tmp(1);
    *this -= tmp;
    return *this;
}

const BigInt BigInt::operator--(int) {
    BigInt tmp = *this;
    --*this;
    return tmp;
}


BigInt BigInt::SumDifSign(BigInt tmp_1, BigInt tmp_2, char sign_of_res) {
    int dif, overflow = 0;
    for (int i = 0; i < std::max(tmp_1.number.size(), tmp_2.number.size()); i++) {
        dif = i < tmp_2.number.size() ? tmp_2.number[i] + overflow : overflow;
        overflow = dif > tmp_1.number[i] ? 1 : 0;
        if (dif > tmp_1.number[i]) dif -= Base;
        if (i < tmp_1.number.size()) tmp_1.number[i] -= dif;
    }
    *this = tmp_1;
    DeleteNulls();
    if (this->number[this->number.size() - 1] != 0)
        this->sign = sign_of_res;
    return *this;
}

int max(int val_1, int val_2) {
    if (val_1 > val_2)
        return val_1;
    return val_2;
}

BigInt & BigInt::operator+=(const BigInt &input) {
    BigInt tmp(input);
    int overflow = 0;
    char sign_1 = this->sign, sign_2 = tmp.sign;
    if (sign == tmp.sign) {
        for (int i = 0; i < max(this->number.size(), tmp.number.size()) || overflow != 0; i++) {
            if (i == this->number.size()) this->number.push_back(0);
            this->number[i] += overflow + (i < tmp.number.size() ? tmp.number[i] : 0);
            overflow = this->number[i] >= Base;
            if (overflow == 1) this->number[i] -= Base;
        }
        DeleteNulls();
    } else {
        this->sign = '+';
        tmp.sign = '+';
        *this = (*this >= tmp) ? SumDifSign(*this, tmp, sign_1) : SumDifSign(tmp, *this, sign_2);
    }
    return *this;
}

BigInt & BigInt::operator*=(const BigInt &input) {
    this->sign = (this->sign != input.sign) ? '-' : '+';
    long long val_1, val_2 = 0;
    int cur = 0, tmp_index = 0;
    std::vector<int> tmp;
    tmp.resize(this->number.size() + input.number.size());
    for (auto i: this->number) {
        for (auto j: input.number) {
            val_1 = (long long) i * (long long) j + val_2 + tmp[cur];
            tmp[cur++] = val_1 % Base;
            val_2 = val_1 / Base;
        }
        if (val_2 != 0) tmp[cur] = val_2;
        val_2 = 0;
        cur = ++tmp_index;
    }
    this->number = tmp;
    DeleteNulls();
    return *this;
}

BigInt & BigInt::operator-=(const BigInt &input) {
    BigInt tmp = input;
    *this += -tmp;
    return *this;
}


BigInt BigInt::BinarySearch(BigInt division, BigInt divider) {
    BigInt compare(0), remain, mul;
    if (divider > division) return compare;
    int cur = Base / 2, min = 0, max = Base;
    while (min <= max) {
        mul = cur;
        remain = -(divider * mul - division);
        if (remain < divider && remain >= compare) return BigInt(cur);
        if (remain < compare) max = cur;
        else min = cur;
        cur = (min + max) / 2;
    }
    return remain;
}

BigInt BigInt::Abs(BigInt input) {
    input.sign = '+';
    return input;
}

BigInt & BigInt::operator/=(const BigInt &input) {
    BigInt compare(0), diff(0), base(Base);
    if (input == compare) throw std::invalid_argument("Divison by zero.");
    this->sign = (this->sign != input.sign) ? '-' : '+';
    std::vector<int> result;
    if (Abs(*this) < Abs(input)) {
        *this = compare;
        return *this;
    }
    BigInt tmp(0);
    for (int i = this->number.size(); i >= 0; i--) {
        BigInt cur(i > 0 ? this->number[i - 1] : 0);
        BigInt remain = BinarySearch(Abs(tmp), Abs(input));
        if (remain != compare) tmp -= remain * Abs(input);
        tmp = (tmp * base) + cur;
        result.emplace(result.begin(), remain);
    }
    this->number = result;
    DeleteNulls();
    return *this;
}

BigInt & BigInt::operator^=(const BigInt &input) {
    std::string bin_val_1 = ChangeToBinPositive(Abs(*this), this->sign);
    std::string bin_val_2 = ChangeToBinPositive(Abs(input), input.sign);
    int size = max(bin_val_1.size(), bin_val_2.size());
    bin_val_1 = ChangeToBinNegative(bin_val_1, this->sign, size);
    bin_val_2 = ChangeToBinNegative(bin_val_2, input.sign, size);
    std::string result;
    char cur_sign = (this->sign == '-' || input.sign == '-') ? '-' : '+';
    for (int i = 0; i < size; i++)
        result += (bin_val_2[i] == bin_val_1[i]) ? '0' : '1';
    *this = (cur_sign == '+') ? FromBinToDecPos(result) : FromBinToDecNeg(result);
    this->sign = cur_sign;
    DeleteNulls();
    return *this;
}

BigInt & BigInt::operator%=(const BigInt &input) {
    BigInt div(Abs(*this));
    div /= Abs(input);
    div *= Abs(input);
    *this += (sign == '-') ? div : -div;
    return *this;
}


std::string BigInt::ChangeToBinPositive(BigInt value, char value_sign) {
    if (value_sign == '-') value--;
    std::string tmp;
    BigInt base(2), compare(0);
    while (value > compare) {
        BigInt cur = value;
        cur %= base;
        tmp += cur.number[0] + '0';
        value /= base;
    }
    return tmp;
}


std::string BigInt::ChangeToBinNegative(std::string value, char value_sign, int size) {
    for (int i = 0; i < size; i++) {
        if (value.size() == i) value.push_back(value_sign == '+' ? '0' : '1');
        else if (value_sign == '-') value[i] = (value[i] == '1') ? '0' : '1';
    }
    return value;
}

BigInt BigInt::FromBinToDecPos(std::string value) {
    BigInt tmp(0), cur(1), base(2);
    for (int i = 0; i < value.size(); i++) {
        if (value[i] != '0') tmp += cur;
        cur *= base;
    }
    return tmp;
}


BigInt BigInt::FromBinToDecNeg(std::string value) {
    BigInt tmp(0), cur(1), base(2);
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == '0')
            tmp += cur;
        cur *= 2;
    }
    tmp++;
    return tmp;
}


BigInt & BigInt::operator&=(const BigInt &input) {
    std::string bin_val_1 = ChangeToBinPositive(Abs(*this), this->sign);
    std::string bin_val_2 = ChangeToBinPositive(Abs(input), input.sign);
    int size = max(bin_val_1.size(), bin_val_2.size());
    bin_val_1 = ChangeToBinNegative(bin_val_1, this->sign, size);
    bin_val_2 = ChangeToBinNegative(bin_val_2, input.sign, size);
    std::string result;
    char cur_sign = (this->sign == '-' && input.sign == '-') ? '-' : '+';
    for (int i = 0; i < std::min(bin_val_1.size(), bin_val_2.size()); i++)
        result += ((bin_val_2[i] == '1') && (bin_val_1[i] == '1')) ? '1' : '0';
    *this = (cur_sign == '+') ? FromBinToDecPos(result) : FromBinToDecNeg(result);
    this->sign = cur_sign;
    DeleteNulls();
    return *this;
}

BigInt & BigInt::operator|=(const BigInt &input) {
    std::string bin_val_1 = ChangeToBinPositive(Abs(*this), this->sign);
    std::string bin_val_2 = ChangeToBinPositive(Abs(input), input.sign);
    int size = max(bin_val_1.size(), bin_val_2.size());
    bin_val_1 = ChangeToBinNegative(bin_val_1, this->sign, size);
    bin_val_2 = ChangeToBinNegative(bin_val_2, input.sign, size);
    std::string result;
    char cur_sign = (this->sign == '-' || input.sign == '-') ? '-' : '+';
    for (int i = 0; i < max(bin_val_1.size(), bin_val_2.size()); i++) {
        result += ((bin_val_2[i] == '1') || (bin_val_1[i] == '1')) ? '1' : '0';
    }
    *this = (cur_sign == '+') ? FromBinToDecPos(result) : FromBinToDecNeg(result);
    this->sign = cur_sign;
    DeleteNulls();
    return *this;
}

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() {
    if (this->number[this->number.size() - 1] == 0) return *this;
    BigInt copy(*this);
    if (copy.sign == '-')
        copy.sign = '+';
    else
        copy.sign = '-';
    return copy;
}

bool BigInt::operator==(const BigInt &input) const {
    return (this->number == input.number && this->sign == input.sign);
}

bool BigInt::operator!=(const BigInt &input) const {
    return (this->number != input.number || this->sign != input.sign);
}

bool BigInt::operator<(const BigInt &input) const {
    if (this->sign != input.sign)
        return (this->sign == '-');

    if (this->number.size() != input.number.size()) {
        bool compare = this->number.size() < input.number.size();
        if (this->sign == '+')
            return compare;
        else
            return !compare;
    }

    for (int i = this->number.size() - 1; i >= 0; i--) {
        bool compare = this->number[i] < input.number[i];
        if (this->number[i] != input.number[i]) {
            if (this->sign == '+')
                return compare;
            else
                return !compare;
        }
    }

    return false;
}

bool BigInt::operator>(const BigInt &input) const {
    if (*this == input)
        return false;
    return !(*this < input);
}

bool BigInt::operator<=(const BigInt &input) const {
    if (*this == input)
        return true;
    return (*this < input);
}

bool BigInt::operator>=(const BigInt &input) const {
    if (*this == input)
        return true;
    return !(*this < input);
}

BigInt::operator int() const {
    int value = 0;
    if (number.size() <= 1 || (number.size() == 2 && number[0] <= 147483647 && number[1] <= 2)) {
        for (int i = 0; i < number.size(); i++)
            value += number[i] * (1 - i) + number[i] * i * Base;
        value *= (this->sign == '-')? -1: 1;
        return value;
    } else {
        throw std::invalid_argument("Too big for int.");
    }
}

BigInt::operator std::string() const {
    std::string cur;
    if (sign == '-' && this->number[number.size() - 1] != 0) cur += '-';
    cur += std::to_string(this->number[number.size() - 1]);
    for (int i = number.size() - 2; i >= 0; i--) {
        std::string value = std::to_string(number[i]);
        for (int j = value.size(); j < CountOfMaxElementsInSell; j++)
            cur += '0';
        cur += value;
    }
    return cur;
}

size_t BigInt::size() const {
    return this->number.size() * 4;
}


BigInt operator+(const BigInt &inp_1, const BigInt &inp_2) {
    BigInt tmp(inp_1);
    tmp += inp_2;
    return tmp;
}

BigInt operator-(const BigInt &inp_1, const BigInt &inp_2) {
    BigInt tmp(inp_1);
    tmp -= inp_2;
    return tmp;
}

BigInt operator*(const BigInt &inp_1, const BigInt &inp_2) {
    BigInt copy(inp_1);
    copy *= inp_2;
    return copy;
}

BigInt operator/(const BigInt &input_1, const BigInt &input_2) {
    BigInt copy(input_1);
    copy /= input_2;
    return copy;
}

BigInt operator^(const BigInt &input_1, const BigInt &input_2) {
    BigInt tmp(input_1);
    tmp ^= input_2;
    return tmp;
}

BigInt operator%(const BigInt &input_1, const BigInt &input_2) {
    BigInt copy(input_1);
    copy %= input_2;
    return copy;
}

BigInt operator&(const BigInt &input_1, const BigInt &input_2) {
    BigInt tmp(input_1);
    tmp &= input_2;
    return tmp;
}

BigInt operator|(const BigInt &input_1, const BigInt &input_2) {
    BigInt tmp(input_1);
    tmp |= input_2;
    return tmp;
}

std::ostream& operator<<(std::ostream& o, const BigInt& input) {
    if (input.sign == '-') o << input.sign;
    std::cout << input.number[input.number.size() - 1];
    for (int i = input.number.size() - 2; i >= 0; i--)
        o<<std::setw(9)<<std::setfill('0')<<input.number[i];
    return o;
}
