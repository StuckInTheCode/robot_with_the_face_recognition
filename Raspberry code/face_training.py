import cv2
import numpy as np
from PIL import Image
import os

# Path for face image database
path = './data3'

def Train_LBPH():

    recognizer = cv2.face.LBPHFaceRecognizer_create()
    cascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml");

    #Input the current dataset path
    def loadFaces(path):

        imagePaths = [os.path.join(path,f) for f in os.listdir(path)]
        faceArray=[]
        ids = []
        i = 0
        for imagePath in imagePaths:

            PIL_img = Image.open(imagePath).convert('L')
            
            # Load images from dataset
            img_numpy = np.array(PIL_img,'uint8')
            
            faces = cascade.detectMultiScale(img_numpy)
            id = int(os.path.split(imagePath)[-1].split(".")[1])
            
            for (x,y,w,h) in faces:
                faceArray.append(img_numpy[y:y+h,x:x+w])
                ids.append(id)

        return faceSamples,ids

    print ("\n Start training the recognizer \n")
    faces,ids = loadFaces(path)
    
    # Train the model
    recognizer.train(faces, np.array(ids))
    # Save the model 
    recognizer.write('trainer/trainer.yml') 
    print("\n {0} faces trained ".format(len(np.unique(ids))))
