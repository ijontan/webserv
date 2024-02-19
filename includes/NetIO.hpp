#pragma once

#include "IOAdaptor.hpp"

class NetIO : public IOAdaptor
{
private:
public:
	NetIO(void);
	~NetIO(void);
	NetIO(const NetIO &src);
	NetIO &operator=(const NetIO &rhs);
	std::string getMessageToSend() const;
};
