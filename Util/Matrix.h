//
// Created by Xander Lenstra on 22-05-22.
//

#ifndef CGSYNCH_MATRIX_H
#define CGSYNCH_MATRIX_H

#include <vector>

template<typename value_type>
class Matrix {
public:
	Matrix() : Matrix(0,0,value_type()) {};
	Matrix(size_t width, size_t height, value_type value = {});
	Matrix(std::vector<std::vector<value_type>>); // NOLINT(google-explicit-constructor)
	void resize(size_t newWidth, size_t newHeight);
	void resize(size_t newHeight, std::vector<value_type> newValue);
	[[nodiscard]] size_t getHeight() const { return data.size(); }
	[[nodiscard]] size_t getWidth() const;
	[[nodiscard]] size_t size() const { return getHeight(); };
	[[nodiscard]] bool empty() const { return size() == 0; };
	typename std::vector<std::vector<value_type>>::const_iterator begin() const { return data.begin(); };
	typename std::vector<std::vector<value_type>>::const_iterator end() const { return data.end(); };
	constexpr std::vector<value_type>& operator[](size_t i) {
		return data[i];
	}
	constexpr const std::vector<value_type>& operator[](size_t i) const {
		return data[i];
	}
	bool operator==(const Matrix& other) const;
	void push_back(const std::vector<value_type>& newRow);
private:
	std::vector<std::vector<value_type>> data;
};

template<typename T>
void Matrix<T>::resize(size_t newWidth, size_t newHeight) {
	data.resize(newHeight);
	for (auto& row : data) {
		row.resize(newWidth);
	}
}

template<typename T>
void Matrix<T>::resize(size_t newHeight, std::vector<T> newValue) {
	data.resize(newHeight, newValue);
}

template<typename T>
size_t Matrix<T>::getWidth() const {
	if (data.empty())
		return 0;
	return data[0].size();
}

template<typename T>
bool Matrix<T>::operator==(const Matrix& other) const {
	if (other.getHeight() != getHeight()) return false;
	if (other.getWidth() != getWidth()) return false;
	for (size_t i = 0; i < size(); ++i) {
		if (data[i] != other.data[i]) return false;
	}
	return true;
}

template<typename T>
Matrix<T>::Matrix(size_t width, size_t height, T value) {
	data = std::vector(height, std::vector(width, value));
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>> matrix) {
	data = matrix;
}

template<typename T>
void Matrix<T>::push_back(const std::vector<T>& newRow) {
	if (!empty() && newRow.size() != data[0].size()) {
		throw(std::logic_error("Invalid row length!"));
	}
	data.push_back(newRow);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
	for (const auto& row : matrix) {
		for (const auto& value : row) {
			os << value << ',';
		}
		os << std::endl;
	}
	return os;
}

#endif //CGSYNCH_MATRIX_H
