
function init() {
	let button = document.getElementById('hello')
	console.log(button)
	if (!button) return
	button.onclick = () => {
		window.open("https://www.youtube.com/watch?v=dQw4w9WgXcQ")
	}

}

window.onload = init

