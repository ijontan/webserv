#!/usr/bin/python3
import cgi, os, sys

form = cgi.FieldStorage()

for line in sys.stdin:
    if 'Exit' == line.rstrip():
        break
    print(f'Processing Message from sys.stdin: {line}', file=sys.stderr)
print("Done", file=sys.stderr)
print("TEST", file=sys.stderr)
print(form.keys(), file=sys.stderr)
print("TEST", file=sys.stderr)
# Get filename here
# fileitem = form['file']

# # Test if the file was uploaded
# if fileitem.filename:
#    open(os.getcwd() + '/cgi-bin/uploads/' + fileitem.filename, 'wb').write(fileitem.file.read())
#    message = 'The file "' + fileitem.filename + '" was uploaded to ' + os.getcwd() + '/cgi-bin/uploads'
# else:
#    message = 'Uploading Failed'

# print("HTTP/1.1 200 OK")
# print("Content-Type: text/html;charset=utf-8")
# print ("Content-type:text/html\r\n")
# print("<html>")
# print("<head>")
# print("</head>")
# print("<body>")
# print("<h1> " + message + " </h1>")
# print("</body>")
# print("</html>")