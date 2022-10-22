import pandas as pd
from sklearn.datasets import load_iris
import numpy as np

from keras.models import Sequential
from keras.layers import Input, Dense
from tensorflow.keras.utils import plot_model
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.optimizers import Adam, RMSprop, SGD
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix, accuracy_score, f1_score
x, y_org = load_iris(return_X_y=True)

y = pd.Categorical(y_org)
y = pd.get_dummies(y).values
input_num = x.shape[1]
class_num = y.shape[1]

model = Sequential()
model.add(Dense(64, input_shape = (x.shape[1],), activation = 'relu'))
model.add(Dense(64, activation = 'relu'))
model.add(Dense(64, activation = 'relu'))
model.add(Dense(class_num, activation = 'softmax'))
model.summary()
plot_model(model,to_file="my_model.png")

learning_rate = 0.0001
model.compile(optimizer= Adam(learning_rate),loss='categorical_crossentropy',metrics=('accuracy'))
x_train, x_test, y_train, y_test = train_test_split(x,y,test_size = 0.2)
scaler = StandardScaler()
x_train = scaler.fit_transform(x_train)
x_test = scaler.transform(x_test)
model.fit(x_train, y_train, batch_size=32, epochs=5000, verbose=2, validation_data =(x_test, y_test))

from matplotlib import pyplot as plt
historia = model.history.history
floss_train = historia['loss']
floss_test = historia['val_loss']
acc_train = historia['accuracy']
acc_test = historia['val_accuracy']
fig,ax = plt.subplots(1,2, figsize=(20,10))
epochs = np.arange(0, 5000)
ax[0].plot(epochs, floss_train, label = 'floss_train')
ax[0].plot(epochs, floss_test, label = 'floss_test')
ax[0].set_title('Funkcje strat')
ax[0].legend()
ax[1].set_title('Dokladnosci')
ax[1].plot(epochs, acc_train, label = 'acc_train')
ax[1].plot(epochs, acc_test, label = 'acc_test')
ax[1].legend()