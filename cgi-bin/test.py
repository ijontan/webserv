#!/usr/bin/python3

import cgi, cgitb
import sys

# Get data from fields
first_name = "Bla"
last_name = "test"
# print("HTTP/1.1 200 OK")
# print("Content-type: text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Hello - Second CGI Program</title>")
print("<html>")
print("<head>")
print("<label>First name:</label>")
print()
print("<h2>Hello %s %s</h2>" % (first_name, last_name))
print("</body>")
print("</html>")
