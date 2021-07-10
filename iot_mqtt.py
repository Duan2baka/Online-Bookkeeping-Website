import paho.mqtt.client as mqtt
from .models import Event
import json
import signal
import time
ID="A01" # Sensor ID
mqtt_broker = "ia.ic.polyu.edu.hk" # Broker
mqtt_port = 1883 # Default
mqtt_qos = 1 # Quality of Service = 1
mqtt_topic = "iot/sensor"

import smtplib
from email.mime.text import MIMEText
from email.header import Header

mail_host="smtp.163.com" 
mail_user="huangshilu1010@163.com" 
mail_pass="AAOTMIMLWOUNPUQO" 
 
sender = 'huangshilu1010@163.com'
receivers = ['20040312d@connect.polyu.hk'] 
 
message = MIMEText('Someone unidentified has entered W311, pls stop him/her asap ...', 'plain', 'utf-8')
message['From'] = Header("Alert system", 'utf-8')
message['To'] =  Header("Development team", 'utf-8')
 
subject = 'Alert in W311'
message['Subject'] = Header(subject, 'utf-8') 

flag=False 

def mqtt_on_message(client, userdata, msg):
    #print(123)
    d_msg = str(msg.payload.decode("utf-8"))
    iotData = json.loads(d_msg)
    #if iotData["node_id"] == ID:
    print("Received message on topic %s : %s" % (msg.topic, iotData))   
    p = Event(node_id=iotData["node_id"], node_loc=iotData["loc"], temp=iotData["temp"], humid=iotData['hum'], light=iotData['light'], sound=iotData['snd'])
    p.save()
    
    try:
        smtpObj = smtplib.SMTP() 
        smtplib.SMTP_SSL('smtp.163.com')  
        smtpObj.login(mail_user,mail_pass)
        while int(iotData["snd"])>-50:
            if ID=="A01":
                flag=True
                smtpObj.sendmail(sender, receivers, message.as_string())
                print ("Sending sucessfully...")
                signal.signal(signal.SIGALRM, handler)
                signal.alarm(5)
                time.sleep(10)

    except smtplib.SMTPException:
        print ("Error: fail to send the email...")
    

mqtt_client = mqtt.Client("django-20099033D") # Create a Client Instance
mqtt_client.on_message = mqtt_on_message
mqtt_client.connect(mqtt_broker, mqtt_port) # Establish a connection to a broker
print("Connect to MQTT broker")
mqtt_client.subscribe(mqtt_topic, mqtt_qos)
mqtt_client.loop_start()

'''
import smtplib
from email.mime.text import MIMEText
from email.header import Header
import json

mail_host="smtp.qq.com" 
mail_user="964069049@qq.com" 
mail_pass="dptn phyu xorr bdec" 
 
sender = '964069049@qq.com''
receivers = ['964069049@qq.com'] 
 
message = MIMEText('Someone unidentified has entered W311, pls stop him/her asap ...', 'plain', 'utf-8')
message['From'] = Header("Alert system", 'utf-8')
message['To'] =  Header("Development team", 'utf-8')
 
subject = 'Alert in W311'
message['Subject'] = Header(subject, 'utf-8')
 
aDict=json.loads(data1)

flag=False 
try:
    smtpObj = smtplib.SMTP() 
    smtpObj.connect(mail_host, 25)  
    smtpObj.login(mail_user,mail_pass)
    while flag==False:
        if aDict['sound']>30:
            flag=True
            smtpObj.sendmail(sender, receivers, message.as_string())
            print ("Sending sucessfully...")

except smtplib.SMTPException:
    print ("Error: fail to send the email...")
'''
