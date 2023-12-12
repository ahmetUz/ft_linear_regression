/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:50:28 by auzun             #+#    #+#             */
/*   Updated: 2023/12/12 14:51:00 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include "Matrix.hpp"
#include "MathFunctions.hpp"

template <typename K = int>
class Vector
{
	private:
		std::vector<K> data;
	
	public:
		Vector(const std::vector<K> & input): data(input) {}

		Vector(const size_t size, const int initNb): data(size, initNb) {}

		template <typename OtherType>
		Vector(const Vector<OtherType> &oth): data(oth.size()) {
			const int	size = oth.size();
			for (int i = 0; i < size; i++)
				data[i] = static_cast<K>(oth[i]);
		}
		
		Vector(std::initializer_list<K> input) {
			for (const auto& row: input) {
				data.emplace_back(row);
			}
		}

		~Vector() {}

		friend std::ostream&	operator<<(std::ostream& os, const Vector<K>& vector) {
			const int size = vector.size();
			os << "V" << " =  {";
			for (int i = 0; i < size; i++){
				os << vector[i];
					if (i + 1 != size)
						os << ", ";
			}
			os << "}" << std::endl;
			return os;
		}

		K&			operator[](size_t index) {
			if (index < data.size())
				return data[index];
			else
				throw std::out_of_range("Index out of range");
		}

		const K&			operator[](size_t index) const {
			if (index < data.size())
				return data[index];
			else
				throw std::out_of_range("Index out of range");
		}

		template <typename T>
		Vector<T>	operator*(T scalar) const {
			Vector<T>	result(*this);
			result.scl(scalar);
			return result;
		}

		template <typename T>
		Vector<T> operator/(T scalar) const {
			Vector<T> result(*this);
			result.div(scalar);
			return result;
		}

		template <typename T>
		Vector<T>	operator+(Vector<T> m) const {
			Vector<T>	result(*this);
			result.add(m);
			return result;
		}

		std::vector<K>	get() const { return this->data; }

		int			size() const { return data.size(); }

		void		print() const {
			const int size = this->size();
			std::cout << "V" << " =  {";
			for (int i = 0; i < size; i++){
				std::cout << data[i];
					if (i + 1 != size)
						std::cout << ", ";
			}
			std::cout << "}" << std::endl;
		}

		Matrix<K>	reshape(int rows, int cols) {
			std::vector<std::vector<K>> reshapeData;
			int							index = 0;
			for (int i = 0; i < rows; i++) {
				std::vector<K>	row;
				int				size = this->size();
				for (int j = 0; j < cols; j++) {
					if (index < size) {
						row.push_back(this->data[index]);
						index++;
					} else {
						row.push_back(K());
					}
				}
				reshapeData.push_back(row);
			}
			return Matrix<K>(reshapeData);
		}

		void ones() {
			const int	size = this->size();
			for (int i = 0; i < size; i++)
				(*this)[i] = 1;
		}

		void zeros() {
			const int	size = this->size();
			for (int i = 0; i < size; i++)
				(*this)[i] = 0;
		}

		double	sum() const {
			double	result = 0;
			const int	size = this->size();
			for (int i = 0; i < size; i++)
				result += (*this)[i];
			return result;
		}

		void	div(const K &s) {
			const int	vSize = this->size();

			for (int i = 0; i < vSize; i++)
				(*this)[i] /= s;
		}

		/*EX00 Add, Subtract and Scale*/
		void	add(const Vector<K> &v) {
			const int	vSize = this->size();

			if (vSize != v.size())
				throw std::invalid_argument("Vector addition is undefined for vectors of different sizes");
			for (int i = 0; i < vSize; i++)
				(*this)[i] += v[i];
		}

		void	sub(const Vector<K> &v) {
			const int	vSize = this->size();

			if (vSize != v.size())
				throw std::invalid_argument("Vector subtraction  is undefined for vectors of different sizes");
			for (int i = 0; i < vSize; i++)
				(*this)[i] -= v[i];
		}

		void	scl(const K &s) {
			const int	vSize = this->size();

			for (int i = 0; i < vSize; i++)
				(*this)[i] *= s;
		}
		/*----------------------------*/

		/*EX03 Dot product*/
		K	dot(const Vector<K>& v) {
			const int	uSize = this->size();
			K			result = 0;
			if (uSize != v.size())
				throw std::invalid_argument("dot product is undefined for vectors of different sizes");
			for (int i = 0; i < uSize; i++)
				result = std::fma((*this)[i], v[i], result);
			return result;
		}
		/*----------------------------*/
		
		/*EX04 Norm*/
		
		// Compute the 1-norm (Manhattan norm)
		K	norm_1() {
			K	result = 0;
			
			for (const K& value: this->data)
				result += abs<K>(value);
			return result;
		}
		/*----------------------------*/

		// Compute the norm (Euclidean norm)
		K	norm() {
			K	result = 0;
			
			for (const K& value: this->data)
				result = std::fma(value, value, result);
			return sqrt(result);
		}
		/*----------------------------*/
		
		// Compute the norm (supremum norm)
		K	norm_inf() {
			K	result = 0;
			
			for (const K& value: this->data){
				K absValue = abs(value);
				if (absValue > result)
					result = absValue;
			}
			return result;
		}
		/*----------------------------*/
		
		void	normalize(const K norm) {
			if (norm == 0)
				throw std::invalid_argument("Cannot normalize a null vector");
			this->div(norm);
		}

		void	unNormalize(const K norm) {
			if (norm == 0)
				throw std::invalid_argument("Cannot normalize a null vector");
			this->scl(norm);
		}

		/*----------------------------*/
};


#endif