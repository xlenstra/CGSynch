//
// Created by ardour on 03-05-22.
//

#ifndef CGSYNCH_2_RATIONAL_H
#define CGSYNCH_2_RATIONAL_H

#include <string>

class Rational {
public:
	Rational(long long numerator, long long denominator);
	explicit Rational(long long integer) : Rational(integer, 1) {};
	Rational() : Rational(0,1) {};
	Rational(const Rational& other) : Rational(other.numerator, other.denominator) {};
	Rational& operator=(const Rational& other) = default;


	[[nodiscard]] std::string getDisplayString() const { return std::to_string(numerator) + '/' + std::to_string(denominator); }

	void simplifyInPlace();
	[[nodiscard]] Rational simplify() const;

	Rational operator+(const Rational& other) const;
	Rational operator-(const Rational& other) const;
	Rational operator*(const Rational& other) const;
	Rational operator/(const Rational& other) const;
	Rational operator-() const;
	void operator+=(const Rational& other);
	void operator-=(const Rational& other);
	void operator*=(const Rational& other);
	void operator/=(const Rational& other);

	std::partial_ordering operator<=>(const Rational& other) const;
	bool operator==(const Rational& other) const;

private:
	long long numerator = 0;
	long long denominator = 1;
};


#endif //CGSYNCH_2_RATIONAL_H
