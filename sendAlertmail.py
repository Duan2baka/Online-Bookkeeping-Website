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