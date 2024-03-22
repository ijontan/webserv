#!/usr/bin/python3

import cgi
# import cgitb
# import sys
# import os

def main():
	form = cgi.FieldStorage()

	# Get data from fields
	input_username = form.getvalue('username')
	input_password = form.getvalue('password')

	file = open("user_credentials.txt", "r")
	for line in file:
		username, password = line.strip().split(',')

		if input_username == username:
			if input_password == password:
				return True
			else:
				return False

	# creates new user if username does not exist
	if input_username and input_password:
		file = open("user_credentials.txt", "a")
		file.write(f"{input_username},{input_password}\n")
		return True


if __name__ == 'main':
	main()
