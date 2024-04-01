
#pragma once

#include "ABlock.hpp"
#include "IOAdaptor.hpp"
#include "ServerBlock.hpp"

#include <map>
#include <string>
#include <unistd.h>
#include <vector>

#define GET 0
#define POST 1
#define DELETE 2

#define MAX_URL_LENGTH 2048
#define MAX_CONTENT_LENGTH 1000000
class WebServer;

class MethodIO;
class MethodIO : public IOAdaptor
{
public:
	struct rInfo;
private:
	std::string statusLine;
	typedef std::string (*MethodPointer)(ServerBlock &, struct rInfo &, struct rInfo &);
	// std::map<std::string, std::string> responseHeader;
	std::string response;
	static const std::map<int, std::string> errCodeMessages;
	static const std::map<std::string, MethodPointer> methods;
	static const std::map<std::string, std::string> contentTypes;

	void tokenize(std::string s, MethodIO::rInfo &ri) const;

	static std::map<std::string, MethodPointer> initMethodsMap();
	static std::map<int, std::string> initErrCodeMessages();
	static std::map<std::string, std::string> initContentTypes();
	static std::string getMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string postMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string headMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string delMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);
	static std::string putMethod(ServerBlock &block, MethodIO::rInfo &rqi, MethodIO::rInfo &rsi);

	static std::string getDate();
	static std::string getType(std::string path);
	static std::string getPath(std::string basePath, WebServer &ws, std::string &port);
	static ServerBlock getServerBlock(MethodIO::rInfo &rqi, WebServer &ws);
	static std::string readFile(MethodIO::rInfo &rqi, MethodIO::rInfo &rsi, ServerBlock &block);
	static void writeFile(MethodIO::rInfo &rqi, ServerBlock &block, bool createNew);
	static std::string getMessage(int code);

	std::string getUpdatedContent(int fd);

public:
	struct rInfo
	{
		int code;
		std::vector<std::string> request;
		std::map<std::string, std::string> headers;
		std::string body;
		std::string port;
		std::string path;
		std::string queryPath;
		std::string query;
		bool exist;
	};
	MethodIO(void);
	~MethodIO(void);
	MethodIO(const MethodIO &src);
	MethodIO &operator=(const MethodIO &rhs);
	static std::string generateResponse(int code, MethodIO::rInfo &ri);
	std::string getMessageToSend(WebServer &ws, std::string port);
};
