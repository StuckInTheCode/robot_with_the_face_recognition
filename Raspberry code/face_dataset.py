import cv2
import os
def Create_Dataset():
    cam = cv2.VideoCapture(0)
    cam.set(3, 640) # set video width
    cam.set(4, 480) # set video height

    #Load the cascade detector
    face_detector = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

    #constant face id for one single user
    face_id = 1

    print("\n Start face recognition. Please, look the camera and wait ...")

    count = 0

    while(True):

        ret, img = cam.read()
        #Convert image to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        #Collect all faces from the capture
        faces = face_detector.detectMultiScale(gray, 1.3, 5)
        
        for (x,y,w,h) in faces:

            cv2.rectangle(img, (x,y), (x+w,y+h), (255,0,0), 2)
            count += 1
            print(count)
            # Save the captured image into the datasets folder
            # Resize image to correctly train the datase
            resized_image = cv2.resize(gray[y:y+h,x:x+w],(100,100))
            cv2.imwrite("data3/subject." + str(face_id) + '.' + str(count) + ".jpg", resized_image)
            cv2.imshow('image', img)

        k = cv2.waitKey(100) & 0xff 
        if k == 27:
            break
        elif count >= 100: # Take 100 face sample and stop video
             break

    print("\n Faces were collected")
    cam.release()
    cv2.destroyAllWindows()


