import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestClassifier #随机森林分类
from sklearn.metrics import confusion_matrix, accuracy_score
from joblib import dump, load
import pickle
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt
import math
import seaborn as sns

# 训练数据和测试数据集
train_data = pd.read_csv('./testedpy_dataset.csv')
test_data = pd.read_csv('./trainedpy_dataset.csv')

X_train = train_data[['AX','AY','AZ','GX','GY','GZ']]
y_train = train_data['Type']
X_test =  test_data[['AX','AY','AZ','GX','GY','GZ']]
y_test = test_data['Type']

# create Random Forest Classifier
rf_classifier = RandomForestClassifier(n_estimators=500, random_state=42, class_weight='balanced')
# use trainning dataset for fitting
rf_classifier.fit(X_train, y_train)
# use trained model for prediction
y_pred_randomForest = rf_classifier.predict(X_test)
# calculate accuracy for each class
conf_matrix = confusion_matrix(y_test, y_pred_randomForest)
class_accuracies = []
for i in range(5):
    class_accuracy = conf_matrix[i, i] / sum(conf_matrix[i, :])
    class_accuracies.append(class_accuracy)

# calculate overall accuracy
multi_class_accuracy = sum(class_accuracies) / 5

print(class_accuracies)
print(f'overall={multi_class_accuracy}')
#  calculate MSE
ResidualSquare = (y_pred_randomForest - y_test)**2    
MSE = np.mean(ResidualSquare)
print(f'MSE={MSE}')


# # get feature importances
# feature_importances = rf_classifier.feature_importances_

# # draw the graph
# indices = np.argsort(feature_importances)[::-1]
# plt.figure(figsize=(10, 6))
# plt.title("Feature Importance")
# plt.bar(range(X_train.shape[1]), feature_importances[indices], align="center")
# plt.xticks(range(X_train.shape[1]), X_train.columns[indices], rotation=90)
# plt.tight_layout()
# plt.show()

# class_labels = ['standing', 'sitting', 'walking', 'lying', 'running']
# accuracies = class_accuracies

# sns.barplot(x=class_labels, y=accuracies, color='blue')
# plt.title('Model Accuracy by Class')
# plt.xlabel('Class')
# plt.ylabel('Accuracy')
# plt.ylim(0, 1)  
# plt.show()

 # save model
pickle.dump(rf_classifier, open('model.pkl','wb'))
