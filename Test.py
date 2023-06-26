import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split


############### Logistic Regression Class
def sigmoid(x):
    return 1/(1+np.exp(-x))
class LogisticRegression:
    def __init__(self, lr=0.001, n=1000) :
        self.lr = lr
        self.n = n
        self.weights = None
        self.bias = None

    def fit(self,X, y):

        sample, feature = X.shape
        self.weights = np.zeros(feature)
        self.bias = 0

        for _ in range(self.n):
            linear_prediction = np.dot(X, self.weights) + self.bias
            predictions = sigmoid(linear_prediction)

            dw = (1/sample) * np.dot(X.T, (predictions-y))
            db =  (1/sample) * np.sum(predictions-y)

            self.weights = self.weights - self.lr * dw
            self.bias = self.bias - self.lr * db

    def predict(self, X):

        linear_prediction = np.dot(X, self.weights) + self.bias
        y_pred = sigmoid(linear_prediction)
        class_pre = [ 0 if y <= 0.5 else 1 for y in y_pred ]

        return class_pre

#################Load data

dataset = pd.read_csv('heart_disease_data.csv')

print(dataset)
# dataset.tail()
# dataset.shape
# dataset.info()
dataset.isnull().sum()
dataset['target'].value_counts() # Checking the distribution of Target Variable

X = dataset.drop(columns='target', axis=1)
y = dataset['target']
X

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=2)
# # X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=2)
print( y_train.shape, y_test.shape)
model = LogisticRegression(lr=0.01)
model.fit(X_train, y_train)

prediction = model.predict(X_test)


def accuracy(prediction, y_test):
    return np.sum(prediction == y_test)/len(y_test)

acc = accuracy(prediction, y_test) 

print(acc)

input_data = (57,	0,	0,	140,	241,	0,	1,	123,	1,	0.2,	1,	0,	3)
# input_data
convert_input_data_to_arr = np.asarray(input_data)
# convert_input_data_to_arr

reshape_input_data = convert_input_data_to_arr.reshape(1,-1)
# reshape_input_data

predict_input = model.predict(reshape_input_data)
print(predict_input)


if predict_input[0] == 0:
  print("Your heart is healthy")
if predict_input[0] ==1 :
  print("Your heart is not healthy")

