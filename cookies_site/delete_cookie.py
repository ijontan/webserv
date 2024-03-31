#!/usr/bin/python3

import os
from user_authentication import generate_expiry_date, get_file_text

def delete_session(current_session_id):
	file = open("cookies_site/databases/sessions.txt", "r")
	text = file.readlines()
	new_text = []
	for line in text:
		username, session_id = line.strip().split(',')
		
		if current_session_id != session_id:
			new_text.append(line)

	file = open("cookies_site/databases/sessions.txt", "w")
	file.truncate(0)
	file.writelines(new_text)

request_method = os.environ.get("REQUEST_METHOD").upper()
request_cookie = os.environ.get("HTTP_COOKIE")

if request_method == "GET": 
	# extracts the session_id: sid=...
	session_id = request_cookie[4:]
	delete_session(session_id)

	print("HTTP/1.1 200 OK")
	print("Content-type: text/html")
	print("Set-Cookie: sid=" + session_id + "; Expires=" + generate_expiry_date(0))
	print(get_file_text("cookies_site/login_page.html"))