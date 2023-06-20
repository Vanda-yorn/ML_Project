import cv2 as cv 

img = cv.imread('img/facetree.jpg')


height, width = img.shape[:2]

mirImg = img.copy()

for h in range(height):
    for w in range(width):

        mirImg[h,w]=img[1-h,w]
       
cv.imshow('Test',mirImg)
cv.waitKey(0)
cv.destroyAllWindows(0)