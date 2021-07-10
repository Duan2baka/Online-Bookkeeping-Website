import paho.mqtt.client as mqtt
from .models import Event
import json
import smtplib
from email.mime.text import MIMEText
from email.header import Header
import time
from playsound import playsound
ID="A01" # Sensor ID
mqtt_broker = "ia.ic.polyu.edu.hk" # Broker
mqtt_port = 1883 # Default
mqtt_qos = 1 # Quality of Service = 1
mqtt_topic = "iot/sensor"

mail_host="smtp.qq.com" 
#mail_user="964069049@qq.com" 
#mail_pass="dptn phyu xorr bdec" 

#sender = '964069049@qq.com'
#receivers = ['964069049@qq.com'] 
mail_user="1955828561@qq.com" 
mail_pass="ouimpzqjjzradbid" 

sender = '1955828561@qq.com'
receivers = ['1955828561@qq.com'] 
def send_email(location):
    message = MIMEText('Someone unidentified has entered {}, pls stop him/her asap ...'.format(location), 'plain', 'utf-8')
    message['From'] = Header("Alert system", 'utf-8')
    message['To'] =  Header("Development team", 'utf-8')
    
    subject = 'Alert in {}'.format(location)
    message['Subject'] = Header(subject, 'utf-8')
    
    #aDict=json.loads(data1)

    try:
        smtpObj = smtplib.SMTP_SSL(mail_host,465) 
        #smtpObj.connect(mail_host,25)  
        smtpObj.login(mail_user,mail_pass)
        smtpObj.sendmail(sender, receivers, message.as_string())
        smtpObj.quit()
        print ("Sending sucessfully...")

    except smtplib.SMTPException:
        print ("Error: fail to send the email...")

def mqtt_on_message(client, userdata, msg):
    #print(123)
    d_msg = str(msg.payload.decode("utf-8"))
    iotData = json.loads(d_msg)
    #if iotData["node_id"] == ID:
    print("Received message on topic %s : %s" % (msg.topic, iotData))
    #print(iotData['light'])
    #print(iotData['snd'])
    #print(time.localtime().tm_hour)
    if iotData['light']>=40 and iotData['snd']>=30 and (time.localtime().tm_hour>=22 or time.localtime().tm_hour<=7):
        print('email sent!')
        send_email(iotData['loc'])
        playsound('C:/Users/ic2140/Desktop/Alarm.mp3')
    p = Event(node_id=iotData["node_id"], node_loc=iotData["loc"], temp=iotData["temp"], humid=iotData['hum'], light=iotData['light'], sound=iotData['snd'])
    p.save()

mqtt_client = mqtt.Client("Duan2baka") # Create a Client Instance
mqtt_client.on_message = mqtt_on_message
mqtt_client.connect(mqtt_broker, mqtt_port) # Establish a connection to a broker
print("Connect to MQTT broker")
mqtt_client.subscribe(mqtt_topic, mqtt_qos)

mqtt_client.loop_start()

'''
 
 
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
