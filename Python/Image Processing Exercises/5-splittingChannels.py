import cv2

input_image = cv2.imread("python.png", cv2.IMREAD_GRAYSCALE)
red = input_image[:, :, 2]
green = input_image[:, :, 1]
blue = input_image[:, :, 0]

cv2.imshow("Input image", input_image)
cv2.imshow("Red channel", red)
cv2.imshow("Green channel", green)
cv2.imshow("Blue channel", blue)
cv2.waitKey(0)