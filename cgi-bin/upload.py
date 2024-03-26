#!/usr/bin/env python3
import cgi
import cgitb
import os

cgitb.enable()

form = cgi.FieldStorage()
ip = form.getvalue('ip')
fileitem = form['file']
data = fileitem.file.read()
filename = os.path.basename(fileitem.filename)
with open(filename, 'wb') as f:
    f.write(data)
print('Content-Type: text/plain\r\n\r\n', end='')
print('Success!')