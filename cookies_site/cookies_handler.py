
import cgi
import os

def get_username_from_sessions_database(current_session_id):
	file = open("cookies_site/databases/sessions.txt", "r")
	for line in file:
		username, session_id = line.strip().split(',')
		
		if current_session_id == session_id:
			return username

def add_user_preference(current_user, info):
	file = open("cookies_site/databases/user_info.txt", "a")
	for line in file:
		username, password = line.strip().split(',')

		if current_user == username:
		                                 
			

#_________________________________________________________________
form = cgi.FieldStorage()

input_color = str(form.getvalue('color'))

request_method = os.environ.get("REQUEST_METHOD").upper()
request_cookie = os.environ.get("HTTP_COOKIE").upper()


if request_method == "POST":
	if input_color:
		# extracts the session_id: sid=...
		session_id = request_cookie[4:-1]
		current_user = get_username_from_sessions_database(session_id)

		add_user_preference(current_user, input_color)