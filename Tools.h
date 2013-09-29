/*
 *	Class with diferent methods to do repeting operations across the project
 */



#ifndef _TOOLS_H
#define _TOOLS_H

#include "Constants.h"

#include <string.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <time.h> 
#include <vector>

using namespace std;

class Tools
{
public:
	Tools(void);
	~Tools(void);

//------------------------------------------------------------------------------
//Methods
	string convertToString(char * szArray[]);
	string convertToString(char * Array);
	string convertToString(int number);
	string printTime();
	//string parseTime(string s);
	string delSpaces(string s);
	vector<string> parseCmdLine(char * argv[], int argc);
	void wait(int x);
	void printErrorVector(bool bPrintLog, vector<int> ivTestErrors);
	string replaceASCII(string sToSend);
	string errorCodeParser(int iError);
	void showCmdHelp();

//------------------------------------------------------------------------------
//Template for vector operation "remove_if"

	template <class ForwardIterator, class UnaryPredicate>
	  ForwardIterator remove_if (ForwardIterator first, ForwardIterator last,
								 UnaryPredicate pred)
	{
	  ForwardIterator result = first;
	  while (first!=last) {
		if (!pred(*first)) {
		  *result = *first;
		  ++result;
		}
		++first;
	  }
	  return result;
	}
};
#endif