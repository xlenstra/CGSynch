//
// Created by ardour on 03-05-22.
//

#include <numeric>
#include <stdexcept>

#include "Rational.h"


Rational::Rational(long long int numerator, long long int denominator) : numerator(numerator), denominator(denominator) {
	if (denominator == 0)
		throw std::domain_error("Denominator of a fraction cannot be 0!");
	this->simplifyInPlace();
}

void Rational::simplifyInPlace() {
	long long commonDenominator = std::gcd(numerator, denominator);
	numerator /= commonDenominator;
	denominator /= commonDenominator;
	if (denominator < 0) {
		numerator *= -1;
		denominator *= -1;
	}
}

Rational Rational::simplify() const {
	long long commonDenominator = std::gcd(numerator, denominator);
	if (denominator < 0) {
		return {-numerator / commonDenominator, -denominator / commonDenominator};
	}
	return {numerator / commonDenominator, denominator / commonDenominator};
}

Rational Rational::operator+(const Rational& other) const {
	return Rational(numerator * other.denominator + other.numerator * denominator, denominator * other.denominator).simplify();
}

Rational Rational::operator-(const Rational& other) const {
	return Rational(numerator * other.denominator - other.numerator * denominator, denominator * other.denominator).simplify();
}

Rational Rational::operator-() const {
	return { -numerator, denominator};
}

Rational Rational::operator*(const Rational& other) const {
	return Rational(numerator * other.numerator, denominator * other.denominator).simplify();
}

Rational Rational::operator/(const Rational& other) const {
	return Rational(numerator * other.denominator, denominator * other.numerator).simplify();
}

void Rational::operator+=(const Rational& other) {
	*this = *this + other;
}

void Rational::operator-=(const Rational& other) {
	*this = *this - other;
}

void Rational::operator*=(const Rational& other) {
	*this = *this * other;
}

void Rational::operator/=(const Rational& other) {
	*this = *this / other;
}

std::partial_ordering Rational::operator<=>(const Rational& other) const {
	return numerator * other.denominator <=> denominator * other.numerator;
}

bool Rational::operator==(const Rational& other) const {
	Rational simplifiedOther = other.simplify();
	Rational simplifiedSelf = simplify();
	return (simplifiedOther.numerator == simplifiedSelf.numerator && simplifiedOther.denominator == simplifiedSelf.denominator);
}
