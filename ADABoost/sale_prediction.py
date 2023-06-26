import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
import seaborn as sns 
from sklearn.preprocessing import LabelEncoder 
from sklearn.model_selection import train_test_split 
from sklearn import metrics
from xgboost import XGBRegressor


dataset = pd.read_csv('Train.csv')

# print(dataset)

# print(dataset.head(5))

# print(dataset.info())

# print(dataset.isnull().sum())

# print(dataset['Item_Weight'].mean())

dataset['Item_Weight'].fillna(dataset['Item_Weight'].mean(), inplace=True)

print(dataset.isnull().sum())
