#!/usr/bin/env python3

import struct
from bluepy.btle import Peripheral, DefaultDelegate
import argparse
import struct
import paho.mqtt.client as paho
import time

SEND_COMMAND_DEBUG = False

# class IMUData():
#     def __init__(self):
#         # 14 float numbers
#         # 9 from IMU, 5 from flex sensor
#         self.data = [0]*3
#     def unpack(self, data): 
#         for i in range(3):
#             print(data[(4*i):(4*i)+4])
#             self.data[i] = struct.unpack('f', data[(4*i):(4*i)+4])[0]
#         print(self.data)

class LetsGoDelegate(DefaultDelegate):
    def __init__(self, params):
        DefaultDelegate.__init__(self)
        # ... initialise here
        self.LetsGo_cHandle = params

    def handleNotification(self, cHandle, data):
        #print("Notification!")
        #check cHandle
        if(cHandle == self.LetsGo_cHandle):
            gesture_char = struct.unpack('c', data)[0]
            send_str = str(gesture_char, 'utf-8')
            print("Receive command: ", send_str)
            self.client.publish("command_1",send_str) 
        # ... process 'data'



    def setLetsGoCHandle(self, cHandle, client):
        self.LetsGo_cHandle = cHandle
        self.client = client

def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass


parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

# C0:98:E5:49:00:00
broker = "54.90.30.207"
port = 1883

LETSGO_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
LETSGO_Gesture_UUID = "32e6108a-2b22-4db5-a914-43ce41986c70"

try:
    client1= paho.Client("control1")                           #create client object
    client1.on_publish = on_publish                          #assign function to callback
    client1.connect(broker,port)     
    
    if SEND_COMMAND_DEBUG:
        while True:
            command = input("Enter a command character:\n")
            client1.publish("command_1", command)   
    else :
        print("connecting")
        buckler = Peripheral(addr)
        delegate = LetsGoDelegate(0)
        buckler.withDelegate(delegate)
        print("connected")

                            #establish connection
        # imu_data = IMUData()


        # Get service
        sv = buckler.getServiceByUUID(LETSGO_SERVICE_UUID)
        # Get characteristic
        ch = sv.getCharacteristics(LETSGO_Gesture_UUID)[0]
        #ch.write(bytes("testing", 'utf-8'))
        delegate.setLetsGoCHandle(ch.getHandle(), client1)
        dess = ch.getDescriptors()
        dess[0].write(bytes([0x01]))


        # while True:
        #     display = input("Enter a message to write to the display:\n")
        #     ch.write(bytes(display, 'utf-8'))
        #     print(bytes(display, 'utf-8'))

        while True:
            if buckler.waitForNotifications(10.0):
                print("Gesture received and published!!")
            else:
                print("No notifications")
                #display = input("Enter a message to write to the display:\n")
                #ch.write(bytes(display, 'utf-8'))
    # while True:
    #     # return type is "bytes"
    #     readbytes = ch.read()
    #     # define the data format of the bytes

    #     #processing the data

finally:
    buckler.disconnect()
