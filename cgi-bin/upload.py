#!/usr/bin/python3
import cgi

form = cgi.FieldStorage()
print("Content-type: text/html\n")
print("<html>")
print("<head>")
print("<title>File Upload Result</title>")
print("</head>")
print("<body>")
if "file" in form:
    fileitem = form["file"]
    if fileitem.filename:
        name = fileitem.filename
        path = "uploads/" + name
        data = fileitem.file.read()
        with open(path, 'wb') as f:
            f.write(data)
        print("<h1>Success</h1>")
    else:
        print("<h1 class='error'>Error: No file was uploaded.</h1>")
else:
    print("<h1>Fail</h1>")
print("</body>")
print("</html>")