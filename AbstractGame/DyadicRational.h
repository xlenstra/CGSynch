//
// Created by s1935534 on 23/02/2022.
//

#ifndef CGSYNCH_2_DYADICRATIONAL_H
#define CGSYNCH_2_DYADICRATIONAL_H

#include <optional>

class DyadicRational {
public:
    int numerator;
    int denominator;
    DyadicRational() = delete;
    DyadicRational(int numerator, int denominator);
    explicit DyadicRational(int integer);
    DyadicRational(const DyadicRational& other);
    DyadicRational& operator=(const DyadicRational& other);

    DyadicRational operator-() const;
    DyadicRational operator+(const DyadicRational& other) const;
    DyadicRational operator-(const DyadicRational& other) const;
    DyadicRational operator+(const int& other) const;
    DyadicRational operator-(const int& other) const;
    std::strong_ordering operator<=>(const DyadicRational& other) const;
    bool operator==(const DyadicRational& other) const;
    std::strong_ordering operator<=>(const int& other) const;

    DyadicRational simplify();
    std::pair<int,int> getDatabaseVersion() { return { numerator, denominator }; }
    bool isInteger() { return simplify().denominator == 1; }
    [[nodiscard]] DyadicRational roundToZero() const;
};

std::optional<DyadicRational> getSimplestNumber(const DyadicRational& left, const DyadicRational& right);


#endif //CGSYNCH_2_DYADICRATIONAL_H
