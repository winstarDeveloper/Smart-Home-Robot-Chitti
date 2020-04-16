print ("Smart Home Robot Program Started")

from espeak import espeak
import RPi.GPIO as GPIO
import time
import serial
import sys
import Adafruit_DHT
import os
import vlc
import datetime
import subprocess

time.sleep(5)

p = vlc.MediaPlayer("Music/startup.mp3")
p.play()
time.sleep(1.5)

from PIL import Image                                                                                
img = Image.open('emoji/normal.png')
img.show()
time.sleep(0.5)
img.close()
img = Image.open('emoji/normal 2.png')
img.show()
time.sleep(0.5)
img = Image.open('emoji/normal 3.png')
img.show()
time.sleep(0.5)
img = Image.open('emoji/happy.png')
img.show()
time.sleep(0.5)
img = Image.open('emoji/normal.png')
img.show()

print "\nLibraries Set"
#os.system("espeak -p 50")
#os.system("espeak -s 70")
espeak.synth("Smart Home Robot Program Started")

RED = 3
GREEN = 27
YELLOW = 22
pir = 17
servo_pin = 18
dht11_pin = 24

pir_no_motion = False

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(GREEN, GPIO.OUT)
GPIO.setup(YELLOW, GPIO.OUT)
GPIO.setup(RED, GPIO.OUT)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(servo_pin, GPIO.OUT)

pwm=GPIO.PWM(servo_pin, 50)
pwm.start(0)

ser = serial.Serial('/dev/ttyACM0',9600)

def setAngle(angle):
    duty = float(angle) / 10.0 + 2.5
    pwm.ChangeDutyCycle(duty)

print "\nAll Libraries Set"

def setLED():
    GPIO.output(RED, True)
    GPIO.output(YELLOW, False)
    GPIO.output(GREEN, False)
    time.sleep(0.5)
    GPIO.output(RED, False)
    GPIO.output(YELLOW, True)
    GPIO.output(GREEN, False)
    time.sleep(0.5)
    GPIO.output(RED, False)
    GPIO.output(YELLOW, False)
    GPIO.output(GREEN, True)
    time.sleep(0.5)
    for x in range(0, 90):
        setAngle(x)
        print "Angle %d :" % (x)
    time.sleep(0.02)
    GPIO.output(RED, True)
    GPIO.output(YELLOW, True)
    GPIO.output(GREEN, False)
    time.sleep(0.5)
    GPIO.output(RED, False)
    GPIO.output(YELLOW, True)
    GPIO.output(GREEN, True)
    time.sleep(0.5)
    GPIO.output(RED, True)
    GPIO.output(YELLOW, False)
    GPIO.output(GREEN, True)
    time.sleep(0.5)
    GPIO.output(RED, True)
    GPIO.output(YELLOW, True)
    GPIO.output(GREEN, True)

setLED()
print "\nLED Set"

setAngle(90)
print "\nServo at 90 degrees"

time.sleep(1)

ser.write('A')
pwm.stop()
GPIO.cleanup()


while (1):
    #read_serial=ser.readline()
    input = ser.read()
    print ("Read input " + input.decode("utf-8") + " from Arduino")
    if (input == 'a'):
        GPIO.output(RED, True)
        GPIO.output(YELLOW, False)
        GPIO.output(GREEN, True)
        img = Image.open('emoji/happy.png')
        img.show()
        espeak.synth("Hello Sir")
    elif (input == 'b'):
        GPIO.output(RED, False)
        GPIO.output(YELLOW, False)
        GPIO.output(GREEN, True)
        img = Image.open('emoji/swag.png')
        img.show()
        espeak.synth("Hello, I am Chitti, speed 1 Giga Hertz , memory 1 giga byte, storage memory 16 giga byte, nice to meet you")
    elif (input == 'c'):
        GPIO.output(RED, True)
        GPIO.output(YELLOW, False)
        GPIO.output(GREEN, False)
        espeak.synth("Ok Sir, lets roll")
        img = Image.open('emoji/surprised.png')
        img.show()
        delay(4)
        img = Image.open('emoji/tensed 3.png')
        img.show()
        delay(5)
        img = Image.open('emoji/wink.png')
        img.show()
        delay(2)
        img = Image.open('emoji/smart.png')
        img.show()
    elif (input == 'd'):
        GPIO.output(RED, False)
        GPIO.output(YELLOW, True)
        GPIO.output(GREEN, False)
        img = Image.open('emoji/simple 1.png')
        img.show()
        delay(2)
        humidity, temperature = Adafruit_DHT.read_retry(11, dht11_pin)
        espeak.synth("the room temperature is " + temperature + "degree celsius and the Humidity is " + humidity + "percent")
        img = Image.open('emoji/normal 3.png')
        img.show()
    elif (input == 'e'):
        GPIO.output(RED, True)
        GPIO.output(YELLOW, True)
        GPIO.output(GREEN, True)
        espeak.synth("As you wish Sir")
        img = Image.open('surprised 2.png')
        img.show()
    elif (input == 'f'):
        GPIO.output(RED, False)
        GPIO.output(YELLOW, False)
        GPIO.output(GREEN, True)
        img = Image.open('emoji/making fun.png')
        img.show()
        p = vlc.MediaPlayer("Music/boomboomrobot.mp3")
        p.play()
    elif (input == 'g'):
        img = Image.open('emoji/surprised 2.png')
        img.show()
        GPIO.output(RED, True)
        GPIO.output(YELLOW, True)
        GPIO.output(GREEN, False)
        espeak.synth("Chitti Robot Taking a photo")
        p = vlc.MediaPlayer("Music/cheese.mp3")
        p.play()
        setAngle(40)
        time.sleep(0.5)
        setAngle(130)
        time.sleep(0.5)
        setAngle(70)
        
        for x in range(70, 90):
            setAngle(x)
            time.sleep(0.08)
        script_dir = os.path.dirname(__file__)
        os.system('./webcam.sh')
        currentdate = datetime.datetime.now().strftime("%Y-%m-%d_%H%M")
        rel_path = currentdate +".jpg"
        abs_file_path = os.path.join(script_dir, rel_path)
        print abs_file_path
        time.sleep(3)
        img = Image.open('teamimage.jpg')
        img.show()
        tme.sleep(2)
        img = Image.open('emoji/normal 2.png')
        img.show()
    elif (input == 'h'):
        img = Image.open('emoji/sleepy.png')
        img.show()
        GPIO.output(RED, True)
        GPIO.output(YELLOW, False)
        GPIO.output(GREEN, False)
        espeak.synth("Chitti Robot Stopped")
        espeak.synth("Command mode is ON")
        time.sleep(3)
        break
        
'''
    if GPIO.input(pir):
        print "Motion Detected!"
        ser.write('S')
        pir_no_motion = False
        break
    else:
        print "No Motion!"
        if(pir_no_motion == False):
            ser.write('A')
            pir_no_motion = True
'''
pwm.stop()
GPIO.cleanup()
