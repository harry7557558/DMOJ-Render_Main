print("import modules...")
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import SGD


print("load dataset...")

def load_data(filename):
    dat = np.loadtxt(filename, delimiter=',')
    if len(dat[0])==206:
        return (dat[:,0:196],dat[:,196:])
    if len(dat[0])==794:
        return (dat[:,0:784],dat[:,784:])

X, Y = load_data("mnist/mnist_test_2.csv")
INPUT_SIZE = len(X[0])
OUTPUT_SIZE = 10
print(X)
print(Y)


print("define and compile keras model...")
model = Sequential()
model.add(Dense(10, input_dim=INPUT_SIZE, activation='softmax'))
opt = SGD(lr=0.01, momentum=0.9)
model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])


print("fit keras model...")
old_accuracy = 0
for i in range(20):
    model.fit(X, Y, epochs=10, batch_size=100)
    _, accuracy = model.evaluate(X, Y)
    if abs(accuracy-old_accuracy) < 0.001:
        break
    old_accuracy = accuracy


def output_to_class(output):
    res = []
    for item in output:
        maxi = -1
        maxval = float("-inf")
        for i in range(len(item)):
            if item[i]>maxval:
                maxi, maxval = i, item[i]
        res.append(maxi)
    return res

expected = output_to_class(Y)
predicted = output_to_class(model.predict(X))
correct_count = 0
for i in range(len(expected)):
    if expected[i]==predicted[i]:
        correct_count += 1
print(f"Accuracy: {correct_count/len(expected)}")


# get weights and biases
weights, biases = model.layers[0].get_weights()
print("weights =", weights.tolist())
print("biases =", biases.tolist())


