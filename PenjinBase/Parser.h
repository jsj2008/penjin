#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include "Command.h"
#include "TextDoc.h"
#include "StringUtility.h"
using namespace StringUtility;
#include "ErrorHandler.h"
using namespace std;

/// The keyword class stores the info needed to parse each keyword
//  They are stored in an array so the ID is the array index.
#include "KeyWord.h"

class Parser
{
    public:
        PENJIN_ERRORS loadParserConfigFile(CRstring fileName);       //  Load a config script that tell the parser the format of what it will parse
        PENJIN_ERRORS createCommandList(CRstring fileName);		    //	Load a script file, parses it and creates a commandlist

        uint getNumCommands(){return commandList.size();}
        uint countNumCommands(CRint commandType);          //  counts the number of a specific command
        void setCommandList(const queue<Command>& comList){commandList = comList;}
        queue<Command> getCommandList(){return commandList;}             //  Returns a copy of the commandList

        Command getNextCommand();	//	Gets the next command from the commandlist

        string getExtensionlessFilename(string name);	//	gets a filename minus the extension
        string getExtension(string name);
        string stripParentDirectory(string name);		//	removes the parent directory and just leaves the filename
        string getParentDirectory(string fileName);		//	Strips the filename and only leaves the path to the parent folder
        string getValue(string variable);		        //	Get a single value with the specified variable name.
        string stripValue(string variable);				//	Removes the latest value from the string

        // string* getValues(string variable);		//	Get multiple values with the specified variable name.

        string getVariableName(string line);	//	Extracts the name of a variable from a raw string
        string stripVariableName(string line);	//	Removes the variable name from a string and leaves the values only.

    private:
        uint countColons(CRstring line);
        queue <Command> commandList;	//	stores all decoded commands
        vector<KeyWord> keyWords;       //  List of key words the parser will look for
        TextDoc doc;
};

#endif	//	PARSER_H
