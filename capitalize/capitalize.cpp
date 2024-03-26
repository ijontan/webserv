
#include <cstdlib>
#include <iostream>
#include <cstring>

std::string trimString(std::string queryString)
{
	size_t equalSignPos = queryString.find('=');
	unsigned int stringLen = queryString.length();

	return (queryString.substr(equalSignPos + 1, stringLen));
}

std::string capitalizeString(std::string inputString)
{
	for (unsigned int i = 0; i < inputString.length(); i++)
	{
		if (islower(inputString[i]))
			inputString[i] = toupper(inputString[i]);
		else if (inputString[i] == '+')
			inputString[i] = ' ';
	}
	return (inputString);
}

int main()
{
	std::string queryString = std::getenv("QUERY_STRING");
	std::string inputString;
	std::string capitalizedString;

	inputString = trimString(queryString);
	capitalizedString = capitalizeString(inputString);

	// std::cout << "Content-type: text/html\r\n\r\n" << std::endl;
	std::cout << "<html>" << std::endl;
	std::cout << "<head>" << std::endl;
	// std::cout << "<meta charset=\"utf-8\">" << std::endl;
	std::cout << "<title>Result</title>" << std::endl;
	std::cout << "</head>" << std::endl;
	std::cout << "<body>" << std::endl;
	std::cout << "<h2>Result: " << capitalizedString << "</h2>" << std::endl;
	std::cout << "</body>" << std::endl;
	std::cout << "</html>" << std::endl;
}
