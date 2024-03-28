#!/usr/bin/python3

import cgi
import os
from user_authentication import generate_response, get_file_text

def get_username_from_sessions_database(current_session_id):
	file = open("cookies_site/databases/sessions.txt", "r")
	for line in file:
		username, session_id = line.strip().split(',')
		
		if current_session_id == session_id:
			return username

#_________________________________________________________________

request_method = os.environ.get("REQUEST_METHOD").upper()
request_cookie = os.environ.get("HTTP_COOKIE")

if request_method == "GET":
	# extracts the session_id: sid=...
	session_id = request_cookie[4:]
	current_user = get_username_from_sessions_database(session_id)
 
	file_text = get_file_text("cookies_site/profile_page.html")
	file_text = file_text.replace("USERNAME", current_user)
	generate_response(file_text, False)