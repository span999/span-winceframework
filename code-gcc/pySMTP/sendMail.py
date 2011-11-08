#!/usr/bin/python
# to send an email...


# Import smtplib for the actual sending function
import smtplib  

import email


from email.mime.multipart import MIMEMultipart
# Import the email modules we'll need
from email.mime.text import MIMEText

#tmsg = MIMEMultipart('alternative')
#tmsg = MIMEMultipart()


# Open a plain text file for reading.  For this example, assume that
# the text file contains only ASCII characters.
fp = open('email.msg', 'rb')
# Create a text/plain message
#tmsg = MIMEText(fp.read())
#tmsg.attach( MIMEText(fp.read()) )
tmpMsg = fp.read()
fp.close()


# Open a plain text file for reading.  For this example, assume that
# the text file contains only ASCII characters.
fp = open('email.end', 'rb')
# Create a text/plain message
#tmsg.attach( MIMEText(fp.read()) )
tmpMsg = tmpMsg + fp.read()
fp.close()


tmsg = MIMEText(tmpMsg)

  
#fromaddr = 'spdroid@gmail.com'  
fromaddr = 'project-N399@mic.com.com'  
toaddrs  = ['span.liu@mic.com.tw','aiken.chou@mic.com.tw']
#msg = ("From: %s\r\nTo: %s\r\n\r\n" % (fromaddr, ", ".join(toaddrs)))
#msg = 'There was a terrible error that occured and I wanted you to know!'  


tmsg['Subject'] = '[N399]The message FYI broadcasting...'
tmsg['From'] = fromaddr
tmsg['To'] = ','.join(toaddrs)



   
# Credentials (if needed)  
username = 'spdroid@gmail.com'  
password = 'gmail999'  
   
# The actual mail send  
server = smtplib.SMTP('smtp.gmail.com:587')  
server.starttls()  
server.login(username,password)  
#server.sendmail(fromaddr, toaddrs, msg)  
server.sendmail(fromaddr, toaddrs, tmsg.as_string())  
server.quit()  



"""
main
"""

