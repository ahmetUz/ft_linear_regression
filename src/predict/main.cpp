
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "MathFunctions.hpp"

Matrix<double> extractTheta(const std::string& filename) {
	std::ifstream file(filename);
	
	if (file.is_open()) {
		std::vector<double> thetaValues;
		double value;

		while (file >> value) {
			thetaValues.push_back(value);
		}
		file.close();
		Matrix<double> theta({thetaValues});
		return theta;
	} else {
		std::cerr << "Error: Unable to open file for reading - " << filename << std::endl;
		exit(1);
	}
}

double extractKmNorm(const std::string& filename) {
	std::ifstream file(filename);

	if (file.is_open()) {
		double kmNorm;
		if (file >> kmNorm) {
			file.close();
			return kmNorm;
		} else {
			std::cerr << "Error: Unable to read kmNorm value from file - " << filename << std::endl;
			exit(1);
		}
	} else {
		std::cerr << "Error: Unable to open file for reading - " << filename << std::endl;
		exit(1);
	}
}

double	predictPrice(double km, Matrix<double> theta, double kmNorm) {
	double	kmNormed = km / kmNorm;
	Matrix<double>	input({{kmNormed}, {1.0}});
	input = input.transpose();

	double	price = input.mul_mat(theta).get()[0][0];
	return price;
}

int main () {
	Matrix<double> theta = extractTheta("theta.csv");
	theta = theta.transpose();
	double kmNorm = extractKmNorm("kmNorm.csv");

	while (1)
	{
		std::cout << "Enter a mileage (type 'STOP' to exit): ";
		std::string input;
		std::cin >> input;

		 if (input == "STOP") {
			std::cout << "Exiting the program." << std::endl;
			break;
		}

		try {
			double km = std::stod(input);
			double price = predictPrice(km, theta, kmNorm);
			std::cout << "Estimated price: " << price << std::endl;
		}
		catch (std::exception& e) {
			std::cerr << "Error converting to double: " << e.what() << std::endl;
			continue;
		}
	}
	return 0;
}