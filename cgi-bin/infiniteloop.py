#!/usr/bin/python3

import cgi
import os
import sys
import cgitb
# import urllib
# Create instance of FieldStorage
# print(os.environ.get('CONTENT_LENGTH', 0))
# data = ""

# if int(os.environ.get('CONTENT_LENGTH', 0)) != 0:
# #     for i in range(int(os.environ.get('CONTENT_LENGTH', 0))):
#     data = sys.stdin.read(1)

# print(data)

form = cgi.FieldStorage()

first_name = str(form.getvalue('finame'))
last_name = str(form.getvalue('laname'))

print("HTTP/1.1 200 OK")
print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Hello - Second CGI Program</title>")
print("</head>")
print("<body>")
print("<h2>Hello %s %s</h2>" % (first_name, last_name))
print("</body>")
print("</html>")
while True:
	print(1)
