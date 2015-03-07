#include <vector>
#include <map>
#include <string>

class CParseCommandLine {

public:

	// PURPOSE: Parse the command line and get the ip, port, and password
	// REQUIRES: The command line.
	static std::map <std::string, std::string> parseCmdLine(std::string CommandLine);
private:
	static std::map<std::string, std::string> CmdLine;
};