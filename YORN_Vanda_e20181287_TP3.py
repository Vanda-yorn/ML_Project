import cv2 as cv 

img = cv.imread('img/facetree.jpg')


height, width = img.shape[:2]

mirImg1 = mirImg2 = mirImg3 = img.copy()



def  miro_f1(img):
        for h in range(height):
            for w in range(width):
                mirImg1[h,w]=img[1-h,w]
        return mirImg1


def miro_f2(img):
    for h in range(height):
        for w in range(width):
            mirImg2[h,w]=img[h,1-w]
    return mirImg2

def miro_f3(img):
    for h in range(height):
        for w in range(width):
            mirImg3[h,w]=img[1-h,1-w]
    return mirImg3




cv.imshow('Test1',img)

cv.imshow('Test2', miro_f1(img))

cv.imshow('Test3', miro_f2(img))

cv.imshow('Test4', miro_f3(img))


cv.waitKey(0)
cv.destroyAllWindows()