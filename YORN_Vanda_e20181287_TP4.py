import cv2
readimg =  cv2.imread('img/fruit2.jpg')
maskSobelX= [[-1,0,1],
             [-2,0,2],
             [-1,0,1]]
maskSobelY=[[1,2,1],
            [0,0,0],
            [-1,-2,-1]]

#extract height and width
mOutImg=cv2.imread('img/fruit2.jpg')
(height,width,kernel)=readimg.shape
print(width,height,kernel)

pTmpXB= (width*height)*[0]
pTmpXG= (width*height)*[0]
pTmpXR= (width*height)*[0]
pTmpYB=(width*height)*[0]
pTmpYG=(width*height)*[0]
pTmpYR= (width*height)*[0]
#initailize 0
mOutImg=mOutImg*0
#apply mask to original image
for i in range(1,height-1):
    for j in range(1,width-1):
        newValueBx=0
        newValueGx=0
        newValueRx=0
        newValueB=0
        newValueG =0
        newValueR=0
        for mr in range(3):
            for mc in range(3):
            # I1(i+k-1,j+l-1)k(k,l)
                r,g,b=readimg[i+mc-1,j+mr-1]
                newValueBx += maskSobelX[mr][mc]*b
                newValueGx += maskSobelX[mr][mc]*g
                newValueRx += maskSobelX[mr][mc]*r
                newValueB += maskSobelY[mr][mc]*b
                newValueG += maskSobelY[mr][mc]*g
                newValueR += maskSobelY[mr][mc]*r
        pTmpYB[i*width+j]= newValueB
        pTmpYG[i*width+j]= newValueG
        pTmpYR[i*width+j]= newValueR
        pTmpXB[i *width +j]= newValueBx
        pTmpXG[i * width +j]=newValueGx
        pTmpXR[i * width +j]= newValueRx
#convert to positive
for i in range(1,height-1):
    for j in range(1,width-1):
        constBVal1,constGVal1,constRVal1=pTmpXB[i*width+j],pTmpXG[i*width+j],pTmpXR[i*width+j]
        constBVal2,constGVal2,constRVal2=pTmpYB[i*width+j],pTmpYG[i*width+j],pTmpYR[i*width+j]
        if constBVal1<0:
            constBVal1=-constBVal1
        if constGVal1<0:
            constGVal1=-constGVal1
        if constRVal1<0:
            constRVal1=-constRVal1
        if constBVal2<0:
            constBVal2=-constBVal2
        if constGVal2<0:
            constGVal2=-constGVal2
        if constRVal2<0:
            constRVal2=-constRVal2
        pTmpXB[i*width+j]=constBVal1+constBVal2
        pTmpXG[i*width+j]=constGVal1+constGVal2
        pTmpXR[i*width+j]=constRVal1+constRVal2
#new max and min of picture
minB=minG=minR=100000000
maxB=maxG=maxR=-100000000
for i in range(1,height-1):
    for j in range(1,width-1):
        newValueB=pTmpXB[i*width+j]
        newValueG=pTmpXG[i*width+j]
        newValueR=pTmpXR[i*width+j]
        if(newValueB<minB):
            minB=newValueB
        if(newValueB>maxB):
            maxB=newValueB
        if(newValueG<minG):
            minG=newValueG
        if(newValueG>maxG): 
            maxG=newValueG
        if(newValueR<minR):
            minR = newValueR
        if(newValueR>maxR):
            maxR=newValueR
#optimize number 
constBVal1 = (float(255.0/(maxB-minB)))
constBVal2= (float(-255.0*minB/(maxB-minB)))
constGVal1=(float(255.0/(maxG-minG)))
constGVal2 = (float(-255.0*minG/(maxG-minG)))
constRVal1=(float(255.0/(maxR-minR)))
constRVal2=(float(-255.0 * minR/(maxR-minR)))
# apply new RGB
for i in range(1,height-1):
    for j in range(1,width-1):
        # Write your code here
        newB = pTmpXB[i * width +j] * (constBVal1-constBVal2) + constBVal2
        newG = pTmpXG[i * width +j] * (constGVal1-constGVal2) + constGVal2
        newR = pTmpXR[i * width +j] * (constRVal1-constRVal2) + constRVal2
        mOutImg[i, j] = [newB, newG, newR]

pTmpXB=[]
pTmpXG=[]
pTmpXR=[]
pTmpYB=[]
pTmpYG=[]
pTmpYR=[]
# print(mOutImg)
cv2.imshow("Sobel Edge Detection",mOutImg)
cv2.imshow("Origin",readimg)
cv2.waitKey()