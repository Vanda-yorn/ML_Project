import numpy as np 
import pandas as pd 
from sklearn import datasets
from sklearn.model_selection import train_test_split

class DecisionStump():
    def __init__(self):
        self.priority = 1
        self.feature_xcol = None
        self.threshold = None
        self.alpha = None

    def predict(self, X):
        sample = X.shape[0]
        X_col = X[:, self.feature_xcol]
        predictions = np.ones(sample)

        if self.priority == 1 :
            predictions[X_col < self.threshold]==-1
        else :
            predictions[X_col > self.threshold]==-1

        return predictions


class Adaboost():

    def __init__(self, num_classified=7):
        self.num_classified = num_classified
    
    def fit(self, X, y):

        sample, features = X.shape
        weight = np.full(sample, (1/sample))

        self.classifieds = []

        for _ in range(self.num_classified):

            classified = DecisionStump()
            min_err = float('inf')

            for feature_i in range(features):

                X_col = X[:, feature_i]

                #Find the unique value in array
                thresholds = np.unique(X_col)

                for threshold in thresholds:
                    p = 1 
                    predictions = np.ones(sample)
                    predictions[X_col < threshold] = -1

                    missClassified = weight[y != predictions]
                    error = sum(missClassified)

                    if error > 0.5:
                        error = 1 - error
                        p = -1

                    if error < min_err :
                        classified.priority = p
                        classified.threshold = threshold
                        classified.feature_xcol = feature_i
                        min_err = error

            EPS = 1e-10

        
            classified.alpha = 0.5 * np.log((1.0 - min_err + EPS)/(min_err+EPS))

            predictions = classified.predict(X)

            weight *= np.exp( - classified.alpha * y * predictions)
            weight /= np.sum(weight)



            self.classifieds.append(classified)

    def predict(self, X):
        classified_pred = [classified.alpha * classified.predict(X) for classified in self.classifieds]
        y_pred = np.sum(classified_pred, axis=0)
        y_pred = np.sign(y_pred)

        return y_pred


def accuracy(prediction, y_test):
    return np.sum(prediction == y_test)/len(y_test)

datasets = datasets.load_breast_cancer()

# datasets = pd.read_csv('heart_disease_data.csv')
# print(datasets)

# X = datasets.drop(columns='target', axis=1)
# y = datasets['target']
# print(X)
# print(y)
X = datasets.data
y = datasets.target
y[y==0] = -1

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=2)

model = Adaboost(num_classified=5)

model.fit(X_train, y_train)
y_pred = model.predict(X_test)

acc = accuracy(y_test, y_pred)

print(acc)
