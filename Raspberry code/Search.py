import face_dataset as fd
import face_training as ft
import cv2
import time
import serial
import os
from serial import Serial

import struct
import sys
import math

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=19200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

#Play the start sound
os.system('mpg321 /home/pi/Downloads/R2D2Bible.mp3')
fd.Create_Dataset()
#Play sound after create dataset
os.system('mpg321 /home/pi/Downloads/Whoa.mp3')


ft.Train_LBPH()

#Set the initial head position
ser.write("v0"+"\n")
time.sleep(1)
ser.write("h40"+"\n")

# Path for dataset
path = './data3'

detector = cv2.CascadeClassifier("haarcascade_frontalface_default.xml");

recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('trainer/trainer.yml')
cascadePath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);
font = cv2.FONT_HERSHEY_SIMPLEX

# iniciate id counter
id = 0

names = ['None', 'User']

cam = cv2.VideoCapture(0)
cam.set(3, 640)  # set video widht
cam.set(4, 480)  # set video height

# Define min window size to be recognized as a face
minW = 0.1 * cam.get(3)
minH = 0.1 * cam.get(4)


def Monitor_User(x, y, w, h):
    print(x,y,w,h)
    os.system('mpg321 /home/pi/Downloads/blip.mp3')

    if((x + w/2 < 300) | (x+ w/2 > 340)):
        distX = 85 - (((x + w/2) * 85) / 640)
        print("h" + str(distX) + "\n")
        time.sleep(1)
        ser.write("h" + str(distX) + "\n")
        time.sleep(1)

    if((y + h/2 < 200) | (y + h/2 > 280)):
        distY = ((y + h/2) * 40) / 480
        print("v" + str(distY) + "\n")
        time.sleep(1)
        ser.write("v" + str(distY) + "\n")
        time.sleep(1)

    if( h < 100):
        timeDeltaSmall = ((100 -h) /100)  + 1
        print("f" + str(timeDeltaSmall) + "\n")
        time.sleep(1)
        ser.write("f" + "\n")
        time.sleep(timeDeltaSmall)
        ser.write("s" + "\n")
        time.sleep(1)

    if( h > 300):
        timeDeltaBig = h - 300 / 180 + 1
        print("f" + str(timeDeltaBig) + "\n")
        time.sleep(1)
        ser.write("b" + "\n")
        time.sleep(timeDeltaBig)
        ser.write("s" + "\n")
        time.sleep(1)


def Find_Face():

    ret, img = cam.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    found = False

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=5,
        minSize=(int(minW), int(minH))
    )

    for (x, y, w, h) in faces:

        resized_image = cv2.resize(gray[y:y + h, x:x + w], (100, 100))
        id, confidence = recognizer.predict(resized_image)
        if(confidence < 95):
            Monitor_User(x, y, w, h)
            found = True

    return found



def Search():
    count = 0
    detected = 0
    result = False
    while (count < 30):
        if Find_Face():
            result = True
            print('see you')
            if(detected > 10):
                count = 0
                detected = 0
            detected += 1
        else:
            count+=1
            print('not see you')

    return result


def Find_User(prev_direction):

    if(prev_direction == 0):

        print('finding you forward...')
        ser.write("f" + "\n")
        time.sleep(5)
        ser.write("s" + "\n")
        time.sleep(1)
    elif(prev_direction == 1):

        print('finding you left...')
        ser.write("l1000" + "\n")
        time.sleep(2)
        ser.write("f" + "\n")
        time.sleep(5)
        ser.write("s" + "\n")
        time.sleep(1)
    elif(prev_direction == 2):

        print('finding you right...')
        ser.write("r1000" + "\n")
        time.sleep(2)
        ser.write("f" + "\n")
        time.sleep(5)
        ser.write("s" + "\n")
        time.sleep(1)
    else:

        print('finding you back...')
        ser.write("b" + "\n")
        time.sleep(2)
        ser.write("s" + "\n")
        time.sleep(2)


def Turn_Around_Head(turn):

    if(turn == 0):
        print('turn up...')
        ser.write("v5" + "\n")
        time.sleep(2)
    elif(turn == 1):
        print('turn down...')
        ser.write("v40" + "\n")
        time.sleep(2)
    elif (turn == 2):
        print('turn left...')
        ser.write("h5" + "\n")
        time.sleep(2)
    else:
        print('turn right...')
        ser.write("h85" + "\n")
        time.sleep(2)

head_turns = 0
prev_direction = 0
#prev_direction = 0 - Forward , 1 - Left, 2 - Right, 3 - back


while True:
    #Search() is an infinite loop if user captured by the robot
    while not Search():
        os.system('mpg321 /home/pi/Downloads/blip2.mp3')
        # Send commands to arduino
        if(head_turns < 4):
            Turn_Around_Head(head_turns)
            head_turns+=1
        else:
            Find_User(prev_direction)
            if(prev_direction == 3):
                prev_direction = 0
            else:
                prev_direction+=1
            head_turns = 0
            
    k = cv2.waitKey(100) & 0xff # Press 'ESC' for exiting 
    if k == 27:
        break

print("\n Process shut down. Exit programm \n")
cam.release()
cv2.destroyAllWindows()
