import gzip
import numpy as np
import matplotlib.pyplot as plt


f = gzip.open(r'C:\Users\xauvlad\Downloads\train-images-idx3-ubyte.gz','r')

image_size = 28
num_images = 100

f.read(16)
buf = f.read(image_size * image_size * num_images)
data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
data = data.reshape(num_images, image_size, image_size, 1)

image = np.asarray(data[99]).squeeze()
plt.imshow(image)
plt.show()