# To run this, you need a Raspberry Pi 2 (or greater) with face_recognition and
# the picamera[array] module installed.
# Installation steup provided by: https://gist.github.com/ageitgey/1ac8dbe8572f3f533df6269dab35df65
# Intergration on RPi Zero W By "Group Name" Final Project

import face_recognition
import picamera
import numpy as np
import os
import time
import serial

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate = 115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
    )

# Get a reference to the Raspberry Pi camera.
# If this fails, make sure you have a camera connected to the RPi and that you
# enabled your camera in raspi-config and rebooted first.
camera = picamera.PiCamera()
camera.resolution = (320, 240)
output = np.empty((240, 320, 3), dtype=np.uint8)

# Load a sample picture and learn how to recognize it.
print("Loading known face image(s)")


# Initialize some variables
face_locations = []
face_encodings = []
face_names = []

#new variables
known_person=[]
known_image=[]
known_face_encoding=[]

#Loop to add images in friends folder
for file in os.listdir("friends"):
    try:
        #Extracting person name from the image filename eg: david.jpg
        known_person.append(file.replace(".jpg", ""))
        file=os.path.join("friends/", file)
        known_image = face_recognition.load_image_file(file)
        known_face_encoding.append(face_recognition.face_encodings(known_image)[0])
    except Exception as e:
        pass



while True:
    print("Capturing image.")
    # Grab a single frame of video from the RPi camera as a numpy array
    camera.capture(output, format="rgb")

    # Find all the faces and face encodings in the current frame of video
    face_locations = face_recognition.face_locations(output)
    print("Found {} faces in image.".format(len(face_locations)))
    face_encodings = face_recognition.face_encodings(output, face_locations)

    # Loop over each face found in the frame to see if it's someone we know.
    for face_encoding in face_encodings:

        match = face_recognition.compare_faces(known_face_encoding, face_encoding)
        matches=np.where(match)[0] #Checking which image is matched
        if len(matches)>0:
          name = str(known_person[matches[0]])
          ser.write('a'.encode('utf-8'))
          print("Im Stopping The Turret!")
        else:
          name = ""

        print("I see someone named {}!".format(name))



