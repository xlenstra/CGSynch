//
// Created by Xander Lenstra on 23/02/2022.
//

#ifndef CGSYNCH_2_DYADICRATIONAL_H
#define CGSYNCH_2_DYADICRATIONAL_H

#include <optional>

class DyadicRational {
public:
	long long numerator;
	long long denominator;
    DyadicRational() = delete;
    DyadicRational(long long numerator, long long denominator);
    explicit DyadicRational(long long integer);
    DyadicRational(const DyadicRational& other);
    DyadicRational& operator=(const DyadicRational& other);

    DyadicRational operator-() const;
    DyadicRational operator+(const DyadicRational& other) const;
    DyadicRational operator-(const DyadicRational& other) const;
	void operator+=(const DyadicRational& other);
	void operator-=(const DyadicRational& other);

    DyadicRational operator+(const long long& other) const;
    DyadicRational operator-(const long long& other) const;
	void operator +=(const long long& other);
	void operator -=(const long long& other);

    std::strong_ordering operator<=>(const DyadicRational& other) const;
    bool operator==(const DyadicRational& other) const;
    std::strong_ordering operator<=>(const long long& other) const;

    DyadicRational simplify();
    [[nodiscard]] DyadicRational roundToZero() const;
};

std::ostream& operator<<(std::ostream& os, const DyadicRational rational);

std::optional<DyadicRational> getSimplestNumber(const DyadicRational& left, const DyadicRational& right);


#endif //CGSYNCH_2_DYADICRATIONAL_H
