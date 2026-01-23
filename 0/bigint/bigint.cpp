#include "bigint.hpp"
#include <sstream>
#include <string>

bigint::bigint() : digits_(1, 0) {}

bigint::bigint(unsigned long long value)
{
	while (value / 10)
	{
		digits_.push_back(static_cast<unsigned char>(value % 10));
		value /= 10;
	}
	digits_.push_back(static_cast<unsigned char>(value));
}

bigint::bigint(const bigint &other) : digits_(other.digits_) {}

bigint &bigint::operator=(bigint other)
{
	if (this != &other) {
		digits_ = other.digits_;
	}
	return *this;
}

void bigint::normalize()
{
	while (digits_.size() > 1 && digits_.back() == 0) {
		digits_.pop_back();
	}
}

bool bigint::is_zero()
{
	return (digits_.size() == 1 && digits_[0] == 0);
}

bigint &bigint::operator+=(bigint other)
{
	const std::size_t max_len = (digits_.size() > other.digits_.size()) ? digits_.size() : other.digits_.size();
	digits_.resize(max_len, 0);

	unsigned int carry = 0;
	for (std::size_t i = 0; i < max_len; ++i) {
		unsigned int sum = carry + digits_[i];
		if (i < other.digits_.size()) {
			sum += other.digits_[i];
		}
		digits_[i] = static_cast<unsigned char>(sum % 10);
		carry = sum / 10;
	}
	if (carry)
		digits_.push_back(static_cast<unsigned char>(carry));
	return *this;
}

bigint &bigint::operator+=(unsigned long long value) {
	bigint temp(value);
	return (*this += temp);
}

bigint &bigint::operator<<=(std::size_t digits) {
	if (is_zero() || digits == 0) {
		return *this;
	}
	digits_.insert(digits_.begin(), digits, 0);
	normalize();
	return *this;
}

bigint &bigint::operator>>=(std::size_t digits)
{
	if (digits == 0)
		return *this;
	if (digits >= digits_.size()) {
		digits_.assign(1, 0);
		return *this;
	}
	digits_.erase(digits_.begin(), digits_.begin() + digits);
	normalize();
	return *this;
}

std::size_t bigint::to_size_t_with_cap(bool *overflow)
{
	std::string str;

	for (std::size_t i = digits_.size(); i-- > 0;)
	{
		str.push_back(static_cast<char>('0' + digits_[i]));
	}

	std::stringstream ss(str);
	std::size_t res = 0;
	ss >> res;

	if (overflow)
		*overflow = ss.fail() || !ss.eof();
	return res;
}

bigint &bigint::operator<<=(bigint digits)
{
	bool overflow = false;
	std::size_t shift = digits.to_size_t_with_cap(&overflow);
	if (overflow)
		return *this;
	return (*this <<= shift);
}

bigint &bigint::operator>>=(bigint digits) {
	bool overflow = false;
	std::size_t shift = digits.to_size_t_with_cap(&overflow);
	if (overflow) {
		digits_.assign(1, 0);
		return *this;
	}
	return (*this >>= shift);
}

bigint &bigint::operator++()
{
	return (*this += 1);
}

bigint bigint::operator++(int)
{
	bigint copy(*this);
	++(*this);
	return copy;
}

bigint operator+(bigint lhs, bigint rhs)
{
	lhs += rhs;
	return lhs;
}

bigint operator+(bigint lhs, unsigned long long rhs)
{
	lhs += rhs;
	return lhs;
}

bigint operator<<(bigint lhs, std::size_t digits)
{
	lhs <<= digits;
	return lhs;
}

bigint operator>>(bigint lhs, std::size_t digits)
{
	lhs >>= digits;
	return lhs;
}

bigint operator<<(bigint lhs, bigint digits)
{
	lhs <<= digits;
	return lhs;
}

bigint operator>>(bigint lhs, bigint digits)
{
	lhs >>= digits;
	return lhs;
}

bool operator<(bigint lhs, bigint rhs)
{
	if (lhs.digits_.size() != rhs.digits_.size()) {
		return lhs.digits_.size() < rhs.digits_.size();
	}
	for (std::size_t i = lhs.digits_.size(); i-- > 0;) {
		if (lhs.digits_[i] != rhs.digits_[i]) {
			return lhs.digits_[i] < rhs.digits_[i];
		}
	}
	return false;
}

bool operator<=(bigint lhs, bigint rhs) {
	return !(rhs < lhs);
}

bool operator>(bigint lhs, bigint rhs) {
	return rhs < lhs;
}

bool operator>=(bigint lhs, bigint rhs) {
	return !(lhs < rhs);
}

bool operator==(bigint lhs, bigint rhs) {
	return lhs.digits_ == rhs.digits_;
}

bool operator!=(bigint lhs, bigint rhs) {
	return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, bigint value) {
	for (std::size_t i = value.digits_.size(); i-- > 0;) {
		os << static_cast<char>('0' + value.digits_[i]);
	}
	return os;
}
