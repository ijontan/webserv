#!/usr/bin/python3
import cgi, os, sys

# if ("multipart/form-data" in os.environ["CONTENT_TYPE"]):
# 	print("content type", file=sys.stderr)

form = cgi.FieldStorage()

# print("TEST", file=sys.stderr)
# # print(os.environ, file=sys.stderr)
# print(form['filename'].filename, file=sys.stderr)
# print("TEST", file=sys.stderr)

# for line in sys.stdin:
#     if 'Exit' == line.rstrip():
#         break
#     print(f'Processing Message from sys.stdin: {line}', file=sys.stderr)
# print("Done", file=sys.stderr)
# Get filename here
fileitem = form['filename']

# # Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/cgi-bin/uploads/' + fileitem.filename, 'wb').write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded to ' + os.getcwd() + '/cgi-bin/uploads'
else:
   message = 'Uploading Failed'

print("HTTP/1.1 200 OK")
print("Content-Type: text/html;charset=utf-8")
print ("Content-type:text/html\r\n")
print("<html>")
print("<head>")
print("</head>")
print("<body>")
print("<h1> " + message + " </h1>")
print("</body>")
print("</html>")