
function displayResult() {
	// get the input value
	var inputValue = document.getElementById('fname').value;
	
	// display the result below the button
	document.getElementById('result').innerHTML = 'Capitalized version: ' + inputValue;
}
