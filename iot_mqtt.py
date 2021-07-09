import paho.mqtt.client as mqtt
from .models import Event
import json

import smtplib
from email.mime.text import MIMEText
from email.utils import formataddr

ID="A01" # Sensor ID
mqtt_broker = "ia.ic.polyu.edu.hk" # Broker
mqtt_port = 1883 # Default
mqtt_qos = 1 # Quality of Service = 1
mqtt_topic = "iot/sensor"

def mqtt_on_message(client, userdata, msg):
    #print(123)
    d_msg = str(msg.payload.decode("utf-8"))
    iotData = json.loads(d_msg)
    #if iotData["node_id"] == ID:
    print("Received message on topic %s : %s" % (msg.topic, iotData))
    p = Event(node_id=iotData["node_id"], node_loc=iotData["loc"], temp=iotData["temp"], humid=iotData['hum'], light=iotData['light'], sound=iotData['snd'])
    p.save()
    if iotData["node_id"]=="A01":
        if int(iotData["snd"])>=-50:
            emsg=MIMEText('Sound alert','html','utf-8')
            emsg['From']=formataddr(["A01",'huangshilu1010@163.com'])
            emsg['Subject']='Sound alert'
            server=smtplib.SMTP_SSL('smtp.163.com')
            server.login('huangshilu1010@163.com','AAOTMIMLWOUNPUQO')
            server.sendmail('huangshilu1010@163.com','20040312d@connect.polyu.hk',emsg.as_string())
            server.quit()


mqtt_client = mqtt.Client("django-A01") # Create a Client Instance
mqtt_client.on_message = mqtt_on_message
mqtt_client.connect(mqtt_broker, mqtt_port) # Establish a connection to a broker
print("Connect to MQTT broker")
mqtt_client.subscribe(mqtt_topic, mqtt_qos)

mqtt_client.loop_start()