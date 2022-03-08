//
// Created by s1935534 on 23/02/2022.
//

#include <stdexcept>
#include <bit>
#include <iostream>
#include "DyadicRational.h"

DyadicRational::DyadicRational(int numerator, int denominator) : numerator(numerator), denominator(denominator) {
    if (!std::has_single_bit((unsigned) denominator))
        throw(std::domain_error("All dyadic rationals must have a power of 2 as their base!"));
}

DyadicRational::DyadicRational(int integer) : DyadicRational(integer, 1) {}


DyadicRational::DyadicRational(const DyadicRational& other) {
    numerator = other.numerator;
    denominator = other.denominator;
}

DyadicRational& DyadicRational::operator=(const DyadicRational& other) = default;



DyadicRational DyadicRational::simplify() {
    while (numerator % 2 != 0 && denominator != 1) {
        numerator /= 2;
        denominator /= 2;
    }
    return { numerator, denominator };
}

DyadicRational DyadicRational::operator-() const {
    return { -numerator, denominator };
}

DyadicRational DyadicRational::operator+(const DyadicRational& other) const {
    if (denominator == other.denominator) return DyadicRational(numerator + other.numerator, denominator).simplify();
    return {
        numerator * other.denominator + other.numerator * denominator,
        denominator * other.denominator
    };
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

DyadicRational DyadicRational::operator+(const int& other) const {
    return DyadicRational(numerator + other * denominator, denominator).simplify();
}

DyadicRational DyadicRational::operator-(const int& other) const {
    return *this + (-other);
}

void DyadicRational::operator+=(const int& other) {
	numerator += other * denominator;
}

void DyadicRational::operator-=(const int& other) {
	*this += -other;
}

std::strong_ordering DyadicRational::operator<=>(const DyadicRational& other) const {
    if (denominator == other.denominator)
        return numerator <=> other.numerator;
    return numerator * other.denominator <=> other.numerator * denominator;
}

std::strong_ordering DyadicRational::operator<=>(const int& other) const {
    return numerator <=> other * denominator;
}

bool DyadicRational::operator==(const DyadicRational& other) const {
    if (denominator == other.denominator && numerator == other.numerator) return true;
    return (other.denominator * numerator == other.numerator * denominator);
}


DyadicRational DyadicRational::roundToZero() const {
    return {numerator/denominator, 1};
}

std::optional<DyadicRational> getSimplestNumber(const DyadicRational& left, const DyadicRational& right) {
    if (left > right) return {};
    if (left == right) return left;
    // so we now know that left < right.
    // We then use the simplest number theorem to find the simplest number between the two.
    if (left >= 0) {
        DyadicRational bestGuess = left.roundToZero()+1;
        while (bestGuess >= right) {
            bestGuess = DyadicRational(bestGuess.numerator*2-1, bestGuess.denominator*2);
            while (bestGuess <= left) {
                bestGuess = DyadicRational(bestGuess.numerator*2+1, bestGuess.denominator*2);
            }
        }
        return bestGuess;
    } else if (right <= 0) {
        DyadicRational bestGuess = right.roundToZero()-1;
        while (bestGuess <= left) {
            bestGuess = DyadicRational(bestGuess.numerator*2+1, bestGuess.denominator*2);
        }
        return bestGuess;
    }
	std::cerr << "DOMAIN ERROR! Invalid combination of left and right dyadic rational passed to getSimplestNumber!" << std::endl;
    return std::optional<DyadicRational>();
}

