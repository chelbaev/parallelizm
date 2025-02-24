from matplotlib import pyplot as plt

# 20.000
time_array = [206, 101, 50, 28, 25, 12, 11, 5]
tread_array = [1, 2, 4, 7, 8, 16, 20, 40]

#40.000
time_array_4 = [819, 414, 197, 132, 100, 49, 40, 20]
tread_array_4 = [1, 2, 4, 7, 8, 16, 20, 40]



plt.subplot(2,2,1)
plt.plot(tread_array, time_array)
plt.scatter(tread_array, time_array)
plt.subplot(2,2,2)
plt.plot(tread_array_4, time_array_4)
plt.scatter(tread_array_4, time_array_4)
plt.show()