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

Matrix<double>	model(Matrix<double>& x, Matrix<double>& theta) {
	return x.mul_mat(theta);
}

double	costFunction(Matrix<double>& x, Matrix<double>& y, Matrix<double>& theta) {

	double	m = y.shape().first;
	return 1.0 / (2.0 * m) * ((model(x, theta) - y).pow(2)).sum();
}

double	gradientDescent(Matrix<double>& x, Matrix<double>& y, Matrix<double>& theta, double alpha, int iterations) {
	double	m = y.shape().first;
	double	j = 0.0;

	for (int i = 0; i < iterations; ++i) {
		theta = theta - (alpha / m) * (x.transpose() * (model(x, theta) - y));
		j = costFunction(x, y, theta);
	}
	return j;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::vector<double>	kilometers;
	std::vector<double>	prices;

	extractData(argv[1], kilometers, prices);

	Matrix<double>	km({kilometers});
	Matrix<double>	pr({prices});

	/*get matrix x + ones col*/
	km = km.transpose();
	Matrix<double>	X = hstack(km, Vector<double>(km.shape().first, 1));
	std::cout << "X:\n" << X << std::endl;
	/*----------------------*/

	/*get theta vector */
	Matrix<double>	theta(2, 1, 1);
	std::cout << "theta:\n" << theta << std::endl;
	/*-----------------*/

	/*model*/
	Matrix<double>	modelM = model(X, theta);
	// std::cout << "model" << modelM << std::endl;
	/*----*/

	pr = pr.transpose();
	std::cout << "pr:\n" << pr << std::endl;
	/*cost function*/
	std::cout << "cost function: " << std::fixed << std::setprecision(10) << costFunction(X, pr, theta) << std::endl;
	/*------*/

	std::vector<double>	modelV = (modelM.transpose()).get()[0];
	plt::plot(kilometers, modelV, "r-");

	/*display*/
	// Affichage des points
	plt::scatter(kilometers, prices, 15);

	// Afficher le graphique
	plt::show();
	/*------*/
	return 0;
}