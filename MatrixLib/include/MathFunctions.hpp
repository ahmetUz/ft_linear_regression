/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MathFunctions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzun <auzun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 17:06:41 by auzun             #+#    #+#             */
/*   Updated: 2023/12/08 17:07:21 by auzun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHFUNCTIONS_HPP
#define MATHFUNCTIONS_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

/*EX01 Linear combination*/

template <typename K>
Vector<K>	linear_combination(const std::vector<Vector<K>>& u, const std::vector<K>& coefs) {
	size_t	uSize = u.size();

	if (uSize != coefs.size())
		throw std::invalid_argument("Vector size mismatch");

	const size_t	resSize(u[0].size());
	std::vector<K>	result(resSize, 0);

	for (size_t i = 0; i < uSize; i++){
		const size_t	vSize = u[i].size();
		if (vSize != resSize)
			throw std::invalid_argument("Vector size mismatch");
		for (size_t j = 0; j < vSize; j++)
			result[j] = std::fma(u[i][j], coefs[i], result[j]);
	}
	return Vector<K>(result);
}

/*----------------------------*/

/*EX02 Linear interpolation*/

template <typename T = float>
T lerp(const T& u, const T& v, const float t) {
	if (t <= 0.0f)
		return u;
	else if (t >= 1.0f)
		return v;
	return u * (1.0f - t) + v * t;
}

/*----------------------------*/

template <typename T = float>
T	abs(const T& value) {
	return value < 0 ? -value : value;
}

double	sqrt(double x) {
	if (x < 0)
		return -NAN;
	if (x == 0)
		return 0;

	double epsilon = 1e-15;
	double guess = x;
	while (1)
	{
		double next = std::fma(0.5, (guess + x / guess), 0);
		if (abs(next - guess) < epsilon)
			return next;
		guess = next;
	}
}

/*EX05 Cosine*/
template <typename T = float>
T	angle_cos(Vector<T>& u, Vector<T>& v) {
	const int	uSize = u.size();
	const int	vSize = v.size();
	
	if (uSize != vSize || uSize == 0 || vSize == 0)
		throw std::invalid_argument("Invalid vector size");
	return u.dot(v) / (u.norm() * v.norm());
}
/*----------------------------*/

/*EX06 Cross product*/

// [0] = x
// [1] = y
// [2] = z

template <typename T = float>
Vector<T>	cross_product(const Vector<T>& u, const Vector<T>& v) {
	if (u.size() != 3 || v.size() != 3)
		throw std::invalid_argument("Invalid vector size");
	return Vector<T>(
		{	u[1] * v[2] - u[2] * v[1],
			u[2] * v[0] - u[0] * v[2],
			u[0] * v[1] - u[1] * v[0],
		}
	);
}

/*----------------------------*/

/*EX14  Bonus: Projection*/
Matrix<float>	projection(const float fov, const float ratio, const float near, const float far) {
	const float	scale = 1 / std::tan(fov / 2);
	const float	depth = far - near;
	
	std::vector<std::vector<float>>	mProjection(4, std::vector<float>(4, 0));
	mProjection[0][0] = scale / ratio;
	mProjection[1][1] = scale;
	mProjection[2][2] = -(far + near) / depth;
	mProjection[2][3] = -2.0 * far * near /depth;
	mProjection[3][2] = -1.0;
	mProjection[3][3] = 0;
	return Matrix<float>(mProjection);
}
/*----------------------------*/

template <typename T = double>
Matrix<T>	hstack(const Matrix<T>& m1, const Vector<T>& v1) {
	const std::pair<int, int>	m1Shape = m1.shape();
	const int	v1Size = v1.size();
	
	if (m1Shape.first != v1Size)
		throw std::invalid_argument("Invalid vector size");
	Matrix<T>	result(m1Shape.first, m1Shape.second + 1, 0);
	for (int i = 0; i < m1Shape.first; i++)
		for (int j = 0; j < m1Shape.second; j++)
			result[i][j] = m1[i][j];
	for (int i = 0; i < m1Shape.first; i++)
		result[i][m1Shape.second] = v1[i];
	return result;
}
#endif