import cv2 as cv

img = cv.imread('.img/download.jpeg')

height, width = img.shape[:2]
# print(height)
# print(width)

for h in range(height) :
    for w in range(width)  :
        B = img[h,w,0]
        G = img[h,w,1]
        R = img[h,w,2]

        # img[h,w] = R + G + B

        img[h,w] = 0.299*R + 0.587*G + 0.114*B
        
cv.imshow('GrayScale',img)
cv.waitKey(0)

