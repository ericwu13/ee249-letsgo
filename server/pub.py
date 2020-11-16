# pip3 install paho-mqtt
import paho.mqtt.client as paho
broker = "54.90.30.207"
port = 1883
def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass
client1= paho.Client("control1")                           #create client object
client1.on_publish = on_publish                          #assign function to callback
client1.connect(broker,port)                                 #establish connection
ret= client1.publish("commands","on")                   #publish
