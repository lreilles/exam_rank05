#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

class bigint {
	public:
		bigint();
		bigint(unsigned long long value);
		bigint(const bigint &other);
		bigint &operator=(bigint other);

		bigint &operator+=(bigint other);
		bigint &operator+=(unsigned long long value);

		bigint &operator<<=(std::size_t digits);
		bigint &operator>>=(std::size_t digits);
		bigint &operator<<=(bigint digits);
		bigint &operator>>=(bigint digits);

		bigint &operator++();     // prefix
		bigint operator++(int);   // postfix

		friend bigint operator+(bigint lhs, bigint rhs);
		friend bigint operator+(bigint lhs, unsigned long long rhs);

		friend bigint operator<<(bigint lhs, std::size_t digits);
		friend bigint operator>>(bigint lhs, std::size_t digits);
		friend bigint operator<<(bigint lhs, bigint digits);
		friend bigint operator>>(bigint lhs, bigint digits);

		friend bool operator<(bigint lhs, bigint rhs);
		friend bool operator<=(bigint lhs, bigint rhs);
		friend bool operator>(bigint lhs, bigint rhs);
		friend bool operator>=(bigint lhs, bigint rhs);
		friend bool operator==(bigint lhs, bigint rhs);
		friend bool operator!=(bigint lhs, bigint rhs);

		friend std::ostream &operator<<(std::ostream &os, bigint value);

	private:
		std::vector<unsigned char> digits_;

		void normalize();
		bool is_zero();
		std::size_t to_size_t_with_cap(bool *overflow);
};
