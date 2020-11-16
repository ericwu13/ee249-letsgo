#!/usr/bin/env python3

import struct
from bluepy.btle import Peripheral, DefaultDelegate
import argparse
import struct
import paho.mqtt.client as paho
import time

ch = 0
addr = "C0:98:E5:49:00:00"

def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass

def on_message(client, userdata, msg):  # The callback for when a PUBLISH message is received from the server.
    print("Message received-> " + msg.topic + " " + str(msg.payload))  # Print a received msg
    # send the message via BLE to romi
    client.ch.write(bytes(msg.payload))


def on_connect(client, userdata, flags, rc):  # The callback for when the client connects to the broker
    print("Connected with result code {0}".format(str(rc)))  # Print result of connection attempt
    client.subscribe("command_1")# Subscribe to the topic “digitest/test1”, receive any messages published on it
    client.subscribe("command_2")# Subscribe to the topic “digitest/test1”, receive any messages published on it    


parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

broker = "54.90.30.207"
port = 1883


DISPLAY_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
DISPLAY_CHAR_UUID    = "32e6108a-2b22-4db5-a914-43ce41986c70"

try:
    print("connecting")
    buckler = Peripheral(addr)
    print("connected")
    # Get service
    sv = buckler.getServiceByUUID(DISPLAY_SERVICE_UUID)
    # Get characteristic
    ch = sv.getCharacteristics(DISPLAY_CHAR_UUID)[0]



    client1= paho.Client("digi_mqtt_test")                           #create client object
    client1.on_connect = on_connect  # Define callback function for successful connection
    client1.on_message = on_message  # Define callback function for receipt of a message
    client1.connect(broker,port)                                 #establish connection
    client1.ch = ch
    client1.loop_forever()  # Start networking daemon

finally:
    buckler.disconnect()
