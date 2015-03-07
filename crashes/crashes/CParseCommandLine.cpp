#include "CParseCommandLine.h"

std::map <std::string, std::string> CParseCommandLine::CmdLine;

std::map <std::string, std::string> CParseCommandLine::parseCmdLine(std::string CommandLine)
{
	for (unsigned int i = 0; i < CommandLine.length(); ++i)
	{
		if (CommandLine.at(i) == '-' || CommandLine.at(i) == '/')
		{
			// Get past the -h or -p or -whatever
			// there's also a space after the -h or -p.
			// so i + 1 = 'h'
			// and i + 2 = ' '
			// so we want to find the next space AFTER i + 2.
			std::size_t stringPos = CommandLine.find(' ', i + 3);

			// If no space was found, assume we're at the end of the string.
			if (stringPos == std::string::npos)
			{
				stringPos = CommandLine.length();
			}

			// Make sure parameter isn't empty.
			// If it's empty, the next parameter will be collided with
			if (CommandLine.at(i + 3) != '-')
			{
				int characters = stringPos - (i + 3);
				switch (CommandLine.at(i + 1))
				{
				
					case 'h': CmdLine["Host"] = CommandLine.substr(i + 3, characters);
					case 'p': CmdLine["Port"] = CommandLine.substr(i + 3, characters);
					case 'z': CmdLine["Password"] = CommandLine.substr(i + 3, characters);
					case 'n': CmdLine["Name"] = CommandLine.substr(i + 3, characters);
				}
			}
		}
	}
	return CmdLine;
}