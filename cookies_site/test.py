
current_session_id = "sid=Lg9N5S5aG5SAe6uDhFx1"

print(current_session_id[4:])

file = open("databases/sessions.txt", "r")
for line in file:
	username, session_id = line.strip().split(',')
	
	if current_session_id == session_id:
		print(username)