#!/usr/bin/python3

import cgi
# import cgitb
# import sys
# import os

def main():
	form = cgi.FieldStorage()

	# Get data from fields
	username = form.getvalue('username')
	password = form.getvalue('password')
	# print("HTTP/1.1 200 OK")
	# print("Content-type: text/html\r\n\r\n")

if __name__ == 'main':
	main()
