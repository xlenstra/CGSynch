//
// Created by Xander Lenstra on 23/02/2022.
//

#include <stdexcept>
#include <bit>
#include <iostream>
#include "DyadicRational.h"

DyadicRational::DyadicRational(long long numerator, long long denominator) : numerator(numerator), denominator(denominator) {
	if (!std::has_single_bit((unsigned long long) denominator)) {
		throw (std::domain_error("Trying to make the dyadic rational " + std::to_string(numerator) + "/" + std::to_string(denominator) + ".\nAll dyadic rationals must have a power of 2 as their base!"));
	}
}

DyadicRational::DyadicRational(long long value) : DyadicRational(value, 1) {}


DyadicRational::DyadicRational(const DyadicRational& other) {
	numerator = other.numerator;
	denominator = other.denominator;
}

DyadicRational& DyadicRational::operator=(const DyadicRational& other) = default;


DyadicRational DyadicRational::simplify() {
	if (denominator < 0) {
		numerator *= -1;
		denominator *= -1;
	}
	if (numerator == 0) {
		denominator = 1;
	}
	while (numerator % 2 == 0 && denominator != 1) {
		numerator /= 2;
		denominator /= 2;
	}
	return { numerator, denominator };
}

DyadicRational DyadicRational::operator-() const {
	return { -numerator, denominator };
}

DyadicRational DyadicRational::operator+(const DyadicRational& other) const {
	if (denominator == other.denominator) {
		return DyadicRational(numerator + other.numerator, denominator).simplify();
	} else if (other.denominator > denominator) {
		return { numerator * (other.denominator / denominator) + other.numerator, other.denominator };
	} else {
		return { numerator + other.numerator  * (denominator / other.denominator), denominator };
	}
}

DyadicRational DyadicRational::operator-(const DyadicRational& other) const {
	return *this + (-other);
}

void DyadicRational::operator+=(const DyadicRational& other) {
	DyadicRational sum = *this + other;
	numerator = sum.numerator;
	denominator = sum.denominator;
}

void DyadicRational::operator-=(const DyadicRational& other) {
	DyadicRational difference = *this - other;
	numerator = difference.numerator;
	denominator = difference.denominator;
}

DyadicRational DyadicRational::operator+(const long long& other) const {
	return DyadicRational(numerator + other * denominator, denominator).simplify();
}

DyadicRational DyadicRational::operator-(const long long& other) const {
	return *this + (-other);
}

void DyadicRational::operator+=(const long long& other) {
	numerator += other * denominator;
}

void DyadicRational::operator-=(const long long& other) {
	*this += -other;
}

std::strong_ordering DyadicRational::operator<=>(const DyadicRational& other) const {
	return (*this - other).simplify().numerator <=> 0;
}

std::strong_ordering DyadicRational::operator<=>(const long long& other) const {
	return numerator <=> other * denominator;
}

bool DyadicRational::operator==(const DyadicRational& other) const {
	if (denominator == other.denominator && numerator == other.numerator) return true;
	return (other.denominator * numerator == other.numerator * denominator);
}


DyadicRational DyadicRational::roundToZero() const {
	return { numerator / denominator, 1 };
}

std::optional<DyadicRational> getSimplestNumber(const DyadicRational& left, const DyadicRational& right) {
	if (left > right) return {};
	if (left == right) return left;
	// so we now know that left < right.
	// We then use the simplest number theorem to find the simplest number between the two.
	if (left >= 0) {
		DyadicRational bestGuess = left.roundToZero() + 1;
		while (bestGuess >= right) {
			bestGuess = DyadicRational(bestGuess.numerator * 2 - 1, bestGuess.denominator * 2);
			while (bestGuess <= left) {
				bestGuess = DyadicRational(bestGuess.numerator * 2 + 1, bestGuess.denominator * 2);
			}
		}
		return bestGuess;
	} else if (right <= 0) {
		DyadicRational bestGuess = right.roundToZero() - 1;
		while (bestGuess <= left) {
			bestGuess = DyadicRational(bestGuess.numerator * 2 + 1, bestGuess.denominator * 2);
			while (bestGuess >= right) {
				bestGuess = DyadicRational(bestGuess.numerator * 2 - 1, bestGuess.denominator * 2);
			}
		}
		return bestGuess;
	} else {
		// left < 0 < right, so 0 is a simplest number
		return DyadicRational(0, 1);
	}
	std::cerr << "DOMAIN ERROR! Invalid combination of left and right dyadic rational passed to getSimplestNumber!"
	          << std::endl;
	return {};
}

std::ostream& operator<<(std::ostream& os, const DyadicRational rational) {
	os << rational.numerator << "/" << rational.denominator;
	return os;
}

