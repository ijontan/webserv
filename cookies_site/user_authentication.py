#!/usr/bin/python3

import cgi
import os
import random
import string
import sys
from datetime import datetime, timedelta, timezone

def generate_cookie():
	session_id = generate_session_id(20)
	file = open("cookies_site/databases/sessions.txt", "a")

	# creates a session (entry) in the sessions database
	file.write(f"{input_username},{session_id}\n")

	return ("Set-Cookie: sid=" + session_id + "; Expires=" + generate_expiry_date(1) + "\r\n")

# by default it creates a expiry date in the form of a cookie string
# - expires in N days
def generate_expiry_date(N):
	today_utc = datetime.now(timezone.utc)
	
	# add one day to today's date
	expiration_date = today_utc + timedelta(days=N)

	# format the expiration date string
	expiration_string = expiration_date.strftime("%a, %d %b %Y %H:%M:%S UTC")
	
	return expiration_string


def generate_session_id(length):
	characters = string.ascii_letters + string.digits
	return ''.join(random.choice(characters) for _ in range(length))


# generates a response with the html file path
def generate_response(body, cookies=False):
	print("HTTP/1.1 200 OK")
	print("Content-type: text/html")
	if cookies == True:
		cookie_string = generate_cookie()
		print(cookie_string)
	print(body)


def authenticate_user():
	file = open("cookies_site/databases/user_info.txt", "r")
	for line in file:
		username, password = line.strip().split(',')

		if input_username == username:
			if input_password == password:
				return "User Found, Correct Password"
			else:
				return "User Found, Wrong Password"
	
	return "User Not Found"


def get_file_text(file_path):
	file = open(file_path, 'r')
	return file.read()


def create_new_user():
	# creates new user if username does not exist
	if input_username and input_password:
		if check_if_user_already_exists() == False:
			file = open("cookies_site/databases/user_info.txt", "a")
			file.write(f"{input_username},{input_password}\n")

			generate_response(get_file_text("cookies_site/login_page.html"))


def check_if_user_already_exists():
	file = open("cookies_site/databases/user_info.txt", "r")
	for line in file:
		username, password = line.strip().split(',')

		if input_username == username:
			return True
	
	return False

#____________________________________________________________________
form = cgi.FieldStorage()

# Get data from fields
input_username = str(form.getvalue('username'))
input_password = str(form.getvalue('password'))

request_method = os.environ.get("REQUEST_METHOD").upper()

if request_method == "POST":
	if input_username and input_password:
		create_new_user()

elif request_method == "GET":
	auth_result = authenticate_user()

	if auth_result == "User Found, Correct Password":
		file_text = get_file_text("cookies_site/landing_page.html")
		file_text = file_text.replace("USERNAME", input_username)
		generate_response(file_text, True)
