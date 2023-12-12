import numpy as np
import matplotlib.pyplot as plt

def model(X, theta):
    return np.dot(X, theta)

def cost_function(X, y, theta):
    m = len(y)
    predictions = model(X, theta)
    cost = (1/(2*m)) * np.sum(np.square(predictions - y))
    return cost

def gradient(X, y, theta):
    m = len(y)
    return (1/m) * np.dot(X.T, (model(X, theta) - y))

def gradient_descent(X, y, theta, alpha, n_iterations):
    m = len(y)
    for i in range(n_iterations):
        theta = theta - alpha * gradient(X, y, theta)
        cost = cost_function(X, y, theta)
        print(f"Iteration {i+1}/{n_iterations} - Cost: {cost}")
    return theta

# Données de test
data = np.array([
    [240000, 3650],
    [139800, 3800],
    [150500, 4400],
    [185530, 4450],
    [176000, 5250],
    [114800, 5350],
    [166800, 5800],
    [89000, 5990],
    [144500, 5999],
    [84000, 6200],
    [82029, 6390],
    [63060, 6390],
    [74000, 6600],
    [97500, 6800],
    [67000, 6800],
    [76025, 6900],
    [48235, 6900],
    [93000, 6990],
    [60949, 7490],
    [65674, 7555],
    [54000, 7990],
    [68500, 7990],
    [22899, 7990],
    [61789, 8290],
])

# Normaliser les données
max_vals = np.max(data, axis=0)
data_normalized = data / max_vals

# Séparer les features (X) et les étiquettes (y)
X = np.c_[data_normalized[:, 0], np.ones(data.shape[0])]
  # Ajouter une colonne de biais
y = data[:, 1].reshape(-1, 1)


# Initialiser les paramètres du modèle
theta = np.zeros((2, 1))

# Paramètres d'apprentissage
alpha = 1
n_iterations = 10000

# Appliquer la descente de gradient
# print("y = ", y)
# exit()
result_theta = gradient_descent(X, y, theta, alpha, n_iterations)

import matplotlib.pyplot as plt

# ... (le reste du code)
prediction = model(X, result_theta)

print(result_theta)

x = data[:, 0]
y = data[:, 1]

plt.scatter(x, y, color='blue', label='Data points')
plt.plot(x, prediction, color='red', label='Linear model prediction')

# Ajuster l'échelle de l'axe des y
plt.ylim([0, max_vals[1]])

plt.xlabel('Kilomètres')
plt.ylabel('Prix')
plt.title('Régression linéaire avec descente de gradient')

plt.legend()
plt.show()
