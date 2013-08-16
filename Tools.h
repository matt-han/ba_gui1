/*
 *	Class with diferent methods to do repeting operations across the project
 *  
 *  Methods:
 *		convertToString
 *		printTime and parseTime
 *		deleteSpacesandComents
 *		wait
 */



#ifndef _TOOLS_H
#define _TOOLS_H

#include "Constants.h"

#include <string.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <time.h> 

using namespace std;

class Tools
{
public:
	Tools(void);
	~Tools(void);

//------------------------------------------------------------------------------
//Methods
	string convertToString(TCHAR Array[]);
	string convertToString(char * Array);
	string convertToString(int number);
	string printTime();
	string parseTime(string s);
	string delSpacesAndComents(string s);
	void wait(int x);

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