# Car Price Prediction with Linear Regression

This project aims to introduce the basic concepts of machine learning by implementing a simple linear regression model trained with a gradient descent algorithm to predict the price of a car based on its mileage.

## Overview

In this project, i writed two programs:

- **Prediction Program**: This program will predict the price of a car for a given mileage. It will prompt the user for a mileage input and return the estimated price using the linear regression model.

- **Training Program**: This program will train the linear regression model using a dataset file containing mileage and corresponding prices. Once trained, it will save the parameters θ0 and θ1 for use in the prediction program.

## Linear Regression Model

The linear regression model used for prediction is defined by the following hypothesis:

estimatePrice(mileage) = θ0 + (θ1 * mileage)

Where:
- θ0 and θ1 are parameters to be learned.
- mileage is the input feature (in this case, the mileage of the car).

## Training the Model

To train the model, we will use the gradient descent algorithm with the following formulas:

tmpθ0 = learningRate * (1/m) * Σ (estimatePrice(mileage[i]) - price[i])
tmpθ1 = learningRate * (1/m) * Σ (estimatePrice(mileage[i]) - price[i]) * mileage[i]


Where:
- learningRate is a hyperparameter determining the step size of the gradient descent.
- m is the number of training examples.
- Σ denotes the summation over all training examples.
- estimatePrice(mileage[i]) is the predicted price for the ith mileage.
- price[i] is the actual price for the ith mileage.

Ensure that your dataset file is properly formatted with mileage and price columns.

## Getting Started

To get started with the project, follow these steps:
- Clone https://github.com/lava/matplotlib-cpp for visualization, and change the name of clonned repo to matplotlibCpp 
- Clone the repository.
- make
```
ft_linear_regression$ make
Making MATRIX
Done
Making linear_regression_train
Done
Making linear_regression_predict
Done
```

- Run the training program with your dataset file to train the model and obtain θ0 and θ1.
```
./linear_regression_train data.csv 
```

- Then run the predict program
```
./linear_regression_predict 
```

