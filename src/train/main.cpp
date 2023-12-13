#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "matplotlibcpp.h"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "MathFunctions.hpp"

namespace plt = matplotlibcpp;

bool isValidDouble(const std::string& str) {
	size_t len = str.length();
	
	if (len == 0)
		return false;

	size_t start = 0;

	if (str[0] == '+' || str[0] == '-') {
		start = 1;
	}

	bool isDouble = false;
	for (size_t i = start; i < len; ++i) {
		if (std::isdigit(str[i])) {
			isDouble = true;
		} else if (str[i] == '.') {
			if (isDouble) {
				return false;
			}
		} else {
			return false;
		}
	}
	return isDouble;
}

void	extractData(const std::string& filename, std::vector<double>& kilometers, std::vector<double>& prices) {
	std::ifstream	file(filename);

	if (!file.is_open()) {
		std::cerr << "Error: cannot open file " << filename << std::endl;
		exit(1);
	}

	std::string	line;
	while (std::getline(file, line)) {
		std::istringstream	iss(line);
		std::string			kilometer;
		std::string			price;

		if (std::getline(iss, kilometer, ',') && std::getline(iss, price, ',')) {
			try {
				if (isValidDouble(kilometer) && isValidDouble(price)) {
					kilometers.push_back(std::stod(kilometer));
					prices.push_back(std::stod(price));
				}
			} catch (std::exception& e) {
				std::cerr << "Error converting to double: " << e.what() << std::endl;
				std::cerr << "Invalid data: " << kilometer << ", " << price << std::endl;
			}
		}
	}
	file.close();
}

void	saveThetaToFile(const Matrix<double>& theta, const std::string& filename) {
	std::ofstream file(filename);

	if (file.is_open()) {
		for (int i = 0; i < theta.shape().first; ++i) {
			file << theta[i][0] << std::endl;
		}
		std::cout << "Theta saved to file: " << filename << std::endl;
		file.close();
	} else {
		std::cerr << "Error: Unable to open file for writing - " << filename << std::endl;
	}
}

void	saveKmNormToFile(double kmNorm, const std::string& filename) {
	std::ofstream file(filename);

	if (file.is_open()) {
		file << kmNorm << std::endl;
		std::cout << "kmNorm saved to file: " << filename << std::endl;
		file.close();
	} else {
		std::cerr << "Error: Unable to open file for writing - " << filename << std::endl;
	}
}

Matrix<double>	model(Matrix<double>& x, Matrix<double>& theta) {
	return x.mul_mat(theta);
}

double	costFunction(Matrix<double>& x, Matrix<double>& y, Matrix<double>& theta) {

	double	m = y.shape().first;
	return 1.0 / (2.0 * m) * ((model(x, theta) - y).pow(2)).sum();
}


Matrix<double>	grad(Matrix<double>& x, Matrix<double>& y, Matrix<double>& theta) {
	double	m = y.shape().first;
	return x.transpose().mul_mat(model(x, theta) - y) * (1.0 / m) ;
}

Matrix<double>	gradientDescent(Matrix<double>& x, Matrix<double>& y, Matrix<double>& theta, double alpha, int iterations) {

	Vector<double>	cost_history(iterations, 0.0);

	for (int i = 0; i < iterations; ++i) {
		theta.sub(grad(x, y, theta) * alpha);
		cost_history[i] = costFunction(x, y, theta);
		std::cout << iterations << " / " << i + 1 << " cost: " << cost_history[i] << std::endl;
	}
	plt::plot(cost_history.get(), "r-");
	plt::show();
	return theta;
}

double	coefDetermination(Matrix<double> y, Matrix<double> pred) {
	double	u = ((y - pred).pow(2)).sum();
	double	v = ((y - y.mean()).pow(2)).sum();
	return 1 - u / v;
}

double	predictPrice(double km, Matrix<double> theta, double kmNorm) {
	double	kmNormed = km / kmNorm;
	Matrix<double>	input({{kmNormed}, {1.0}});
	input = input.transpose();

	double	price = model(input, theta).get()[0][0];
	return price;
}

/*f(x) = ax + b*/

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	/*extract data from data.csv*/
	std::vector<double>	kilometers;
	std::vector<double>	prices;
	extractData(argv[1], kilometers, prices);
	/*-------------------------*/


	/*normalize kilometers*/
	Vector<double>	km(kilometers);
	double	kmNorm = km.norm();
	saveKmNormToFile(kmNorm, "kmNorm.csv");
	km.normalize(kmNorm);
	/*--------------------*/

	/*Create the X matrix with 1 cls*/
	Matrix<double>	X({km.get()});
	X = X.transpose();
	X = hstack(X, Vector<double>(km.size(), 1.0));
	/*-----------------------------*/

	/*Create the y matrix*/
	Matrix<double>	y({prices});
	y = y.transpose();
	/*-------------------*/

	/*
	std::cout << "X: " << X << std::endl;
	std::cout << "y: " << y << std::endl;
	*/

	/*Create theta*/
	Matrix<double>	theta({{0.0}, {0.0}});
	/*-----------*/

	/*learning rate*/
	double	alpha = 1;
	/*-------------*/

	/*number of iterations*/
	int		iterations = 1000;
	/*--------------------*/


	/*get final theta*/
	Matrix<double>	theta_opt = gradientDescent(X, y, theta, alpha, iterations);
	std::cout << "theta: " << theta_opt << std::endl;
	saveThetaToFile(theta_opt, "theta.csv");
	/*---------------*/

	/*get prediction model */
	Matrix<double>	prediction = model(X, theta_opt);
	/*--------------*/


	std::cout << "coefDetermination: " << coefDetermination(y, prediction) << std::endl;

	std::cout << "predictioPrice: " << predictPrice(48235, theta_opt, kmNorm) << std::endl;

	/*show in graph*/
	plt::scatter(kilometers, prices, 15);
	plt::plot(kilometers, prediction.transpose().get()[0], "r-");
	plt::show();
	/*-----------*/
	return 0;
}