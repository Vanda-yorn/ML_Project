import cv2 as cv 
import numpy as np 
img = cv.imread("img/fruit3.jpg")
image = cv.imshow("Image",img)

def darkness():
    darkness =np.copy(img)
    modify_value= 100
    for x in range(len(img)):
        for y in range(len(img[x])):
            #Operation 
            new_R = img[x,y,2] - modify_value 
            new_G = img[x,y,1] - modify_value 
            new_B = img[x,y,0] - modify_value 
            # check condition of the intensity which is smaller than 0
            if new_B < 0:
                new_B = 0
            if new_G < 0:
                new_G=0
            if new_R <0:
                new_R=0
            #Add new intensity which has modified value
            darkness[x,y] = [new_B,new_G,new_R]
    cv.imshow("Darkness Image",darkness)
    
def brightness():
    brightness= np.copy(img)
    modify_value= 100
    for x in range(len(img)):
        for y in range(len(img[x])):
            #Operation 
            new_R = modify_value + img[x,y,2]
            new_G = modify_value + img[x,y,1]
            new_B = modify_value + img[x,y,0]
            # check condition of the intensity which is bigger than 255
            if new_B > 255:
                new_B = 255
            if new_G > 255:
                new_G = 255
            if new_R > 255:
                new_R = 255
            #Add new intensity which has modified value
            brightness[x,y] = [new_B,new_G,new_R]
    cv.imshow("Brightness Image",brightness)
    
image
darkness()
brightness()
cv.waitKey(0)
