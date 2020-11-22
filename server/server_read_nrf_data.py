#!/usr/bin/env python3

import struct
from bluepy.btle import Peripheral, DefaultDelegate
import argparse
import struct
import paho.mqtt.client as paho
import time


class IMUData():
    def __init__(self):
        # 14 float numbers
        # 9 from IMU, 5 from flex sensor
        self.data = [0]*3
    def unpack(self, data): 
        for i in range(3):
            #print(data[(4*i):(4*i)+4])
            self.data[i] = struct.unpack('f', data[(4*i):(4*i)+4])[0]
        print(self.data, flush = True)

class LetsGoDelegate(DefaultDelegate):
    def __init__(self, params):
        DefaultDelegate.__init__(self)
        # ... initialise here
        self.LetsGo_cHandle = params
        self.ch_pair = {}

    def handleNotification(self, cHandle, data):
        print("Notification! %d %d", cHandle)
        #check cHandle
        if cHandle in self.ch_pair:
            print(self.ch_pair[cHandle])
            imu_data.unpack(data)
        #ret = self.client.publish("command_1",data) 
        # ... process 'data'

    def setLetsGo(self, imu_data, client):
        self.imu_data = imu_data
        self.client = client

    def addPairCHandle(self, ch_pair):
        self.ch_pair[ch_pair[0]] = ch_pair[1]

def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass


parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

broker = "54.90.30.207"
port = 1883


LETSGO_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
LETSGO_ACCEL_UUID    = "32e6108a-2b22-4db5-a914-43ce41986c70"
LETSGO_GYRO_UUID    = "32e6108b-2b22-4db5-a914-43ce41986c70"
LETSGO_MAGNET_UUID    = "32e6108c-2b22-4db5-a914-43ce41986c70"
LETSGO_FLEX_UUID    = "32e6108d-2b22-4db5-a914-43ce41986c70"

CH_UUIDs = [LETSGO_ACCEL_UUID, LETSGO_GYRO_UUID, LETSGO_MAGNET_UUID, LETSGO_FLEX_UUID]
CH_NAME = ["accel", "gyro", "magnet", "flex"]
try:
    print("connecting")
    buckler = Peripheral(addr)
    delegate = LetsGoDelegate(0)
    buckler.withDelegate(delegate)
    print("connected")

    client1= paho.Client("control1")                           #create client object
    client1.on_publish = on_publish                          #assign function to callback
    client1.connect(broker,port)                                 #establish connection
    imu_data = IMUData()


    # Get service
    sv = buckler.getServiceByUUID(LETSGO_SERVICE_UUID)
    # Get characteristic
    chs = [sv.getCharacteristics(uuid)[0] for uuid in CH_UUIDs]
    delegate.setLetsGo(imu_data, client1)
    #ch.write(bytes("testing", 'utf-8'))
    for i, ch in enumerate(chs):
        delegate.addPairCHandle((ch.getHandle(), CH_NAME[i]))
        dess = ch.getDescriptors()
        dess[0].write(bytes([0x01]))


    # if buckler.waitForNotifications(100.0):
    #     display = input("Enter a message to write to the display:\n")
    #     ch.write(bytes(display, 'utf-8'))
    while True:
        if buckler.waitForNotifications(10.0):
            print("completated!")
    # while True:
    #     # return type is "bytes"
    #     readbytes = ch.read()
    #     # define the data format of the bytes

    #     #processing the data

finally:
    buckler.disconnect()
