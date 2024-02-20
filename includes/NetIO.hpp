#pragma once

#include "IOAdaptor.hpp"
#include <string>
#include <vector>

class NetIO : public IOAdaptor
{
private:
public:
	NetIO(void);
	~NetIO(void);
	NetIO(const NetIO &src);
	NetIO &operator=(const NetIO &rhs);
	std::string getMessageToSend() const;
	std::vector<std::string> tokenize(std::string s) const;
};
