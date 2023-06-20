import cv2 as cv 

img = cv.imread('img/img1.jpg')
# cv.imshow('Image', img)
height, width = img.shape[:2]

for h in range(height):
    for w in range(width):

        # img[h,w,0]= 255-img[h,w,0]
        # img[h,w,1] = 255-img[h,w,1]
        # img[h,w,2] = 255-img[h,w,2]
        
        B = 255-img[h,w,0]
        G = 255-img[h,w,1]
        R = 255-img[h,w,2]

        img[h,w,0] = B
        img[h,w,1] = G
        img[h,w,2] = R


cv.imshow('Test',img)

cv.waitKey(0)
cv.destroyAllWindows()