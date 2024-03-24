#!/usr/bin/python3

import cgi
# import cgitb
# import sys
import os

def generate_response(file_path):
	file = open(file_path, "r")
	
	print("HTTP/1.1 200 OK")
	print("Content-type: text/html\r\n\r\n")
	print(file.read())

def authenticate_user(input_username, input_password):
	file = open("cookies_site/user_credentials.txt", "r")
	for line in file:
		username, password = line.strip().split(',')

		if input_username == username:
			if input_password == password:
				return "User Found, Correct Password"
			else:
				return "User Found, Wrong Password"
	
	return "User Not Found"


def create_new_user(input_username, input_password):
	# creates new user if username does not exist
	if input_username and input_password:
		file = open("cookies_site/user_credentials.txt", "a")
		file.write(f"{input_username},{input_password}\n")

		generate_response("cookies_site/login_page.html")


form = cgi.FieldStorage()

# Get data from fields
input_username = str(form.getvalue('username'))
input_password = str(form.getvalue('password'))

auth_result = authenticate_user(input_username, input_password)
if auth_result == "User Found, Correct Password":
	generate_response("cookies_site/landing_page.html")

elif auth_result == "User Found, Wrong Password":
	generate_response("cookies_site/wrong_password.html")

elif auth_result == "User Not Found":
	create_new_user(input_username, input_password)


