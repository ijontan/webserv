#!/usr/bin/python3

import cgi
# import cgitb
# import sys
# import os
# Create instance of FieldStorage
form = cgi.FieldStorage()


# Get data from fields
first_name = form.getvalue('fname')
last_name = form.getvalue('lname')
# print("HTTP/1.1 200 OK")
# print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Hello - Second CGI Program</title>")
print("</head>")
print("<body>")
print("<h2>Hello %s %s</h2>" % (first_name, last_name))
print("</body>")
print("</html>")
