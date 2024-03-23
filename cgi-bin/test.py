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

first_name = str(form.getvalue('fname'))
last_name = str(form.getvalue('lname'))

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

# if form:
# 	content = [
# 		"<html>",
# 		"<head>",
# 		"<title>Hello - Second CGI2 Program</title>",
# 		"</head>",
# 		"<body>",
# 		"<h2>Hello {username}, your password is {password}</h2>".format(username = first_name, password = last_name),
# 		"</body>",
# 		"</html>"
#     ]
# else:
# 	content =  ["<html>",
# 		"<head>",
# 		"<title>Hello - Second CGI Program</title>",
# 		"</head>",
# 		"<body>",
# 		"<h2>Hello {username}, your password is {password} </h2>".format(username = first_name, password = last_name),
# 		"</body>",
# 		"</html>"
# 	]

# contentLength = sum(len(s) for s in content)
# print("HTTP/1.1 200 OK")
# print("Content-Length: {clength}".format(clength = contentLength))
# print("Content-Type: text/html\r\n\r\n")

# for s in content:
# 	print(s)