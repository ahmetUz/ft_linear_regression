/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Matrix.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: auzun <auzun@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/10 17:06:57 by auzun			 #+#	#+#			 */
/*   Updated: 2023/12/10 15:58:25 by auzun			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include "Vector.hpp"

template <typename K>
class Vector;

template <typename K = int>
class Matrix
{
	private:
		std::vector<std::vector<K>> data;

	public:
		Matrix(const std::vector<std::vector<K>> & input) : data(input) {}
		
		 Matrix(const size_t numRows, const size_t numCols, const K initNb) {
			data.resize(numRows, std::vector<K>(numCols, K(initNb)));
		}

		/* template <typename OtherType>
		Matrix(const Matrix<OtherType> &oth): data(oth.shape().first,\
			std::vector<K>(oth.shape().second)) {

			const std::pair<int, int> shape = oth.shape();
			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					data[i][j] = static_cast<K>(oth[i][j]);
			}
		} */

		Matrix(std::initializer_list<std::initializer_list<K>> input) {
			for (const auto& row: input) {
				data.emplace_back(row);
			}
		}
		
		~Matrix() {}

		friend std::ostream&	operator<<(std::ostream& os, const Matrix<K>& matrix) {
			const std::pair<int, int> shape = matrix.shape();
			for (int i = 0; i < shape.first; i++) {
				os << "V" << i << " = { ";
				for (int j = 0; j < shape.second; j++) {
					os << matrix[i][j];
					if (j + 1 != shape.second)
						os << ", ";
				}
				os << "}" << std::endl;
			}
			return os;
		}
		
		std::vector<K>&	operator[](size_t index) {
			if (index < data.size())
				return data[index];
			else
				throw std::out_of_range("Index out of range");
		}

		const std::vector<K>&	operator[](size_t index) const {
			if (index < data.size())
				return data[index];
			else
				throw std::out_of_range("Index out of range");
		}

		template <typename T>
		Matrix<T>	operator*(T scalar) const {
			Matrix<T>	result(*this);
			result.scl(scalar);
			return result;
		}

		template <typename T>
		T	operator*(const Matrix<T>& other) const {
			T	result = T(0);
			result = this->dot(other);
			return result;
		}

		template <typename T>
		Matrix<T>	operator+(Matrix<T> m) const {
			Matrix<T>	result(*this);
			result.add(m);
			return result;
		}

		template <typename T>
		Matrix<T>	operator-(Matrix<T> m) const {
			Matrix<T>	result(*this);
			result.sub(m);
			return result;
		}

		template <typename T>
		Matrix<T>	operator-(T m) const {
			Matrix<T>	result(*this);
			result.sub(Matrix<T>(this->shape().first, this->shape().second, m));
			return result;
		}

		std::vector<std::vector<K>>	get() const { return this->data; }
		
		std::pair<int, int>	shape() const {
			int rows = data.size();
			int cols = (rows > 0) ? data[0].size() : 0;
			return std::make_pair(rows, cols);
		}

		bool				isSquare() const {
			const std::pair<int, int> shape = this->shape();
			return shape.first == shape.second;
		}

		void				print() const {
			const std::pair<int, int> shape = this->shape();
			for (int i = 0; i < shape.first; i++) {
				std::cout << "V" << i << " = { ";
				for (int j = 0; j < shape.second; j++) {
					std::cout << data[i][j];
					if (j + 1 != shape.second)
						std::cout << ", ";
				}
				std::cout << "}" << std::endl;
			}
		}

		Vector<K>			reshape(void) const {
			std::vector<K> reshapeData;
			const std::pair<int, int> shape = this->shape();
			for (int i = 0; i < shape.first; i++)
				reshapeData.insert(reshapeData.end(), data[i].begin(), data[i].end());
			return Vector<K>(reshapeData);
		}
		
		void	ones(const std::pair<int, int> shape) {
			data = std::vector<std::vector<K>>(shape.first, std::vector<K>(shape.second, K(1)));
		}

		void	zeros(const std::pair<int, int> shape) {
			data = std::vector<std::vector<K>>(shape.first, std::vector<K>(shape.second, K(0)));
		}

		double	sum() const {
			double	result = 0;
			const std::pair<int, int> shape = this->shape();
			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					result += (*this)[i][j];
			}
			return result;
		}

		K	dot(const Matrix<K>& other) const {
			const std::pair<int, int> shape1 = this->shape();
			const std::pair<int, int> shape2 = other.shape();

			if (shape1 != shape2) {
				throw std::invalid_argument("Matrix dimensions are not compatible for dot product.");
			}

			K result = K(0);
			for (int i = 0; i < shape1.first; ++i) {
				for (int j = 0; j < shape1.second; ++j) {
					result += (*this)[i][j] * other[i][j];
				}
			}

			return result;
		}

		K	dot(const Vector<K>& other) const {
			const std::pair<int, int> shape = this->shape();

			if (shape.second != other.size()) {
				throw std::invalid_argument("Matrix and vector dimensions are not compatible for dot product.");
			}

			K result = K(0);
			for (int i = 0; i < shape.first; ++i) {
				for (int j = 0; j < shape.second; ++j) {
					result += (*this)[i][j] * other[j];
				}
			}

			return result;
		}

		/*EX00 Add, Subtract and Scale*/
		void	add(const Matrix<K> &m2) {
			const std::pair<int, int>	m1Shape = this->shape();
			const std::pair<int, int>	m2Shape = m2.shape();

			if (m1Shape.first != m2Shape.first || m1Shape.second != m2Shape.second)
				throw std::invalid_argument("Matrix addition is undefined for matrix of different shape");
			for (int i = 0; i < m1Shape.first; i++) {
				for (int j = 0; j < m1Shape.second; j++)
					(*this)[i][j] += m2[i][j];
			}
		}

		void	sub(const Matrix<K> &m2) {
			const std::pair<int, int>	m1Shape = this->shape();
			const std::pair<int, int>	m2Shape = m2.shape();

			if (m1Shape.first != m2Shape.first || m1Shape.second != m2Shape.second)
				throw std::invalid_argument("Matrix subtraction is undefined for matrix of different shape");
			for (int i = 0; i < m1Shape.first; i++) {
				for (int j = 0; j < m1Shape.second; j++)
					(*this)[i][j] -= m2[i][j];
			}
		}

		void	sub(const K &s) {
			const std::pair<int, int>	shape = this->shape();

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					(*this)[i][j] -= s;
			}
		}

		void	scl(const K &s) {
			const std::pair<int, int>	shape = this->shape();

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					(*this)[i][j] *= s;
			}
		}
		/*----------------------------*/

		void	div(const K &s) {
			const std::pair<int, int>	shape = this->shape();

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					(*this)[i][j] /= s;
			}
		}

		template <typename T>
		Matrix<K>	pow(const T &nb) {
			const std::pair<int, int>	shape = this->shape();

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					(*this)[i][j] = std::pow((*this)[i][j], nb);
			}
			return *this;
		}

		K	mean() {
			const std::pair<int, int>	shape = this->shape();
			return this->sum() / (shape.first * shape.second);
		}

		/*EX07 Linear map, Matrix multiplication*/

		//Vector * matrix
		Vector<K>	mul_vec(const Vector<K>& vec) {
			const std::pair<int, int>	shape = this->shape();

			if (shape.second != vec.size())
			{
				std::cout << "Matrix: " << shape.first << "x" << shape.second << std::endl;
				std::cout << "Vector: " << vec.size() << std::endl;
				throw std::invalid_argument("Matrix and vector dimensions are not compatible for multiplication.");
			}
			std::vector<K>	result(shape.second);
			for (int i = 0; i < shape.first; i++) {
				K	nb = 0;
				for (int j = 0; j < shape.second; j++)
					nb = std::fma((*this)[i][j], vec[j], nb);
				result[i] = nb;
			}
			return Vector<K>(result);
		}

		Matrix<K>	mul_mat(const Matrix<K>& m) {
			const std::pair<int, int>	m1Shape = this->shape();
			const std::pair<int, int>	m2Shape = m.shape();

			if (m1Shape.second != m2Shape.first)
				throw std::invalid_argument("Matrix dimensions are not compatible for multiplication.");
			std::vector<std::vector<K>>	result(m1Shape.first, std::vector<K>(m2Shape.second));
			for (int i = 0; i < m1Shape.first; i++) {
				for (int j = 0; j < m2Shape.second; j++) {
					K	nb = 0;
					for (int k = 0; k < m1Shape.second; k++)
						nb = std::fma((*this)[i][k], m[k][j], nb);
					result[i][j] = nb;
				}
			}
			return Matrix<K>(result);
		}
		/*----------------------------*/

		/*EX08 trace*/
		K	trace() {
			if (!this->isSquare())
				throw std::invalid_argument("Matrix is not square.");
			K	result = 0;
			const std::pair<int, int>	shape = this->shape();
			for (int i = 0; i < shape.first; i++) {
				result += (*this)[i][i];
			}
			return result;
		}
		/*----------------------------*/

		/*EX09 transpose*/
		Matrix<K>	transpose() {
			const std::pair<int, int>	shape = this->shape();
			std::vector<std::vector<K>>	result(shape.second, std::vector<K>(shape.first));
			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++)
					result[j][i] = (*this)[i][j];
			}
			return Matrix<K>(result);
		}
		/*----------------------------*/

		/*EX10 row-echelon*/

		Matrix<K>	row_echelon() {
			const std::pair<int, int>	shape = this->shape();
			Matrix<K>	result = *this;

			for (int row = 0, col = 0; row < shape.first && col < shape.second; col++) {
				int	pivotRow = row;

				// find the pivot using "pivoting partiel"
				for (int i = row + 1; i < shape.first; i++) {
					if (abs(result[i][col]) > abs(result[pivotRow][col]))
						pivotRow = i;
				}
				if (result[pivotRow][col] == K(0))
					continue;
				std::swap(result[row], result[pivotRow]);
				//Scale the row to make to pivot element 1
				K pivotValue = result[row][col];
				for (int j = col; j < shape.second; j++) {
					result[row][j] /= pivotValue;
				}
				for (int i = row + 1; i < shape.first; i++) {
					K factor = result[i][col];
					for (int j = col; j < shape.second; j++)
						result[i][j] -= factor * result[row][j];
				}
				row++;
			}
			return result;
		}

		/*----------------------------*/
		
		/*EX11  Determinant*/
		K	det_2x2(std::vector<std::vector<K>> m) const {
			return m[0][0] * m[1][1] - m[0][1] * m[1][0];
		}
		
		K	det_3x3(std::vector<std::vector<K>> m) const {
			std::vector<std::vector<K>> submatrix1 = {{m[1][1], m[1][2]}, {m[2][1], m[2][2]}};
			std::vector<std::vector<K>> submatrix2 = {{m[1][0], m[1][2]}, {m[2][0], m[2][2]}};
			std::vector<std::vector<K>> submatrix3 = {{m[1][0], m[1][1]}, {m[2][0], m[2][1]}};

			return m[0][0] * det_2x2(submatrix1) - m[0][1] * det_2x2(submatrix2) + m[0][2] * det_2x2(submatrix3);
		}

		K	det_4x4(std::vector<std::vector<K>> m) const {
			std::vector<std::vector<K>> submatrix1 = {{m[1][1], m[1][2], m[1][3]}, {m[2][1], m[2][2], m[2][3]}, {m[3][1], m[3][2], m[3][3]}};
			std::vector<std::vector<K>> submatrix2 = {{m[1][0], m[1][2], m[1][3]}, {m[2][0], m[2][2], m[2][3]}, {m[3][0], m[3][2], m[3][3]}};
			std::vector<std::vector<K>> submatrix3 = {{m[1][0], m[1][1], m[1][3]}, {m[2][0], m[2][1], m[2][3]}, {m[3][0], m[3][1], m[3][3]}};
			std::vector<std::vector<K>> submatrix4 = {{m[1][0], m[1][1], m[1][2]}, {m[2][0], m[2][1], m[2][2]}, {m[3][0], m[3][1], m[3][2]}};

			return m[0][0] * det_3x3(submatrix1) - m[0][1] * det_3x3(submatrix2) + m[0][2] * det_3x3(submatrix3) - m[0][3] * det_3x3(submatrix4);
		}

		K	determinant() {
			if (!isSquare())
				throw std::invalid_argument("Matrix is not square.");
			const std::pair<int, int>	shape = this->shape();
			if (shape.first == 1)
				return (*this)[0][0];
			else if (shape.first == 2)
				return det_2x2(this->data);
			else if (shape.first == 3)
				return det_3x3(this->data);
			else if (shape.first == 4)
				return det_4x4(this->data);
			else
				 throw std::invalid_argument("matrix larger than 4x4.");
		}
		/*----------------------------*/
		
		/*EX12  Inverse*/
		Matrix<K>	identity() {
			const std::pair<int, int>	shape = this->shape();
			std::vector<std::vector<K>> identity(shape.first, std::vector<K>(shape.second));

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++) {
					if (i == j)
						identity[i][j] = K(1);
					else
						identity[i][j] = K(0);
				}
			}
			return Matrix<K>(identity);
		}

		Matrix<K>	inverse() {
			if (!this->isSquare())
				throw std::invalid_argument("Matrix is not square.");
			if (this->determinant() == K(0))
				throw std::invalid_argument("Matrix is singular.");

			const std::pair<int, int>	shape = this->shape();
			Matrix<K>					m(*this);
			Matrix<K>					mInverse = this->identity();

			//square matrix so shape.first == shape.second
			for (int col = 0; col < shape.first; col++) {
				int pivotRow = col;

				for (int i = col + 1; i < shape.first; i++) {
					if (abs(m[i][col]) > abs(m[pivotRow][col]))
						pivotRow = i;
				}
				if (m[pivotRow][col] == K(0))
					throw std::invalid_argument("Matrix is singular.");
				std::swap(m[pivotRow], m[col]);
				std::swap(mInverse[pivotRow], mInverse[col]);
				K	pivotValue = m[col][col];
				for (int j = 0; j < shape.first; j++) {
					m[col][j] /= pivotValue;
					mInverse[col][j] /= pivotValue;
				}
				for (int i = 0; i < shape.first; i++) {
					if (i != col) {
						K	factor = m[i][col];
						for (int j = 0; j < shape.first; j++) {
							m[i][j] -= factor * m[col][j];
							mInverse[i][j] -= factor * mInverse[col][j];
						}
					}
				}
			}
			return (mInverse);
		}
		/*----------------------------*/

		/*EX13  Rank*/
		size_t	rank() {
			Matrix<K>	mRowEchelonForm = this->row_echelon();
			const std::pair<int, int>	shape = mRowEchelonForm.shape();
			size_t		rank = 0;

			for (int i = 0; i < shape.first; i++) {
				for (int j = 0; j < shape.second; j++) {
					if (mRowEchelonForm[i][j] != K(0)) {
						rank++;
						break;
					}
				}
			}
			return rank;
		}
		/*----------------------------*/
};


#endif
