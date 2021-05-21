import cv2 as cv
import numpy as np
from numpy.fft import fftn, ifftn, fftshift,ifftshift
import math
import argparse

#Apply the Gaussian filter to an image
def GaussianFilter(img, sigma):
    #Get Dimensions of the image
    numRows, numCols = img.shape[:2]

    #Calculate the Center
    centerI = int(numRows / 2) + 1 if numRows % 2 == 1 else int(numRows / 2)
    centerJ = int(numCols / 2) + 1 if numCols % 2 == 1 else int(numCols / 2)
    
    matrix = np.zeros((numRows, numCols, 3))

    #Store to the Matrix the Image Filtered
    for i in range(numRows):
        for j in range(numCols):
            for k in range(3):
                #Calculate the coefficient
                matrix[i][j][k] = math.exp(-1.0 * ((i - centerI) ** 2 + (j - centerJ) ** 2) / (2 * sigma ** 2))
    return matrix

#Read an image using opencv library
def ReadImage(img):
    i = cv.imread(img)
    return i

#Apply the Shifted Fourier Transform to an image  
def ShiftedFourierTransform(img):
    return fftshift(fftn(img))

#Undo the Shifted Fourier Transform 
def UndoTransformation(img):
    return ifftn(ifftshift(img))

#Save an image using opencv library
def SaveImage(img):
    cv.imwrite("result/hybrid.jpg", np.abs(img))
    
#Resize the second image with the dimensions of the first image
def ResizeBaseSecImgImage(BaseFirstImg,BaseSecImg):
    width = int(BaseFirstImg.shape[1])
    height = int(BaseFirstImg.shape[0])
    dim = (width, height)
    output = cv.resize(BaseSecImg, dim)
    return output
    
    
#Make a Hybrid Image using the definition that is in the original article
#H = I1 · G1 + I2 ·(1 − G2)
#I1=BaseFirstImg Image
#I2=BaseSecImg Image
#G1=BaseFirstImg Image with LowPass
#G2=BaseSecImg Image with HighPass
def MakeHybridImage(BaseFirstImg,BaseSecImg,LowImage,HighImage):
    i = BaseFirstImg * LowImage +BaseSecImg * (1 - HighImage)
    return i

#Get the inputs that the program need
def GetUserInput():
    parser = argparse.ArgumentParser()
    
    parser.add_argument('BaseFirstImg', help="Name of the image that will be applied low pass", type=str)
   
    parser.add_argument('BaseSecImg', help="Name of the image that will be applied high pass" , type=str)
    
    parser.add_argument('Highpass', help="Highpass  frequency", type=float)
    
    parser.add_argument('Lowpass', help="Lowpass  frequency", type=float)
    

    args = parser.parse_args()

    if args.Highpass < 0 or args.Lowpass < 0:
        print("The frequencies values must be positives!")
        exit(1)

    return [args.BaseFirstImg, args.BaseSecImg, args.Highpass, args.Lowpass]
 

def main():
    [BaseFirstImg, BaseSecImage, HPFrequency, LPFrequency] = GetUserInput()

    #User Input
    MainImg = "Images/"+ BaseFirstImg
    AltImg =  "Images/"+BaseSecImage

    # Read the input images
    I1 = ReadImage(MainImg)
    I2 = ReadImage(AltImg)

    #Resize second image(avoid bugs)
    I2=ResizeBaseSecImgImage(I1,I2)

    #Apply the Shifted Fourier Transform to the input images
    IT1=ShiftedFourierTransform(I1)
    IT2=ShiftedFourierTransform(I2)

    #Apply the Gaussian Filter with the respectives frequencies
    G1 = GaussianFilter(IT1, LPFrequency)
    G2 = GaussianFilter(IT2, HPFrequency)

    
    #Make the Hybrid Image
    H = MakeHybridImage(IT1,IT2,G1,G2)

    #Undo the Transformations and Save Image
    H=UndoTransformation(H)
    SaveImage(H)

    print("Process Done!")


if __name__ == "__main__":
    main()
