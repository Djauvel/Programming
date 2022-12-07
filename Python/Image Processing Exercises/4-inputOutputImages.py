import cv2
import numpy as np

input_image = cv2.imread("python.png")
output_image = np.zeros(input_image.shape, dtype=input_image.dtype)

for y, row in enumerate(input_image):
    for x, pixel in enumerate(row):
        output_image[y, x] = pixel

cv2.imshow("Input image", input_image)
cv2.imshow("Output image", output_image)
cv2.waitKey(0)