#include "Tools.h"

Tools::Tools(void)
{
}

Tools::~Tools(void)
{
}

string Tools::convertToString(TCHAR Array[]){
	stringstream ss;
	int i = 0;
	while ( Array[i] != '\0')
	{
		ss << char(Array[i]);
		i++;
	}
	return ss.str();
}

string Tools::convertToString(char * Array){
	stringstream ss;
		ss << Array;
	return ss.str();
}


string Tools::convertToString(int number){
	stringstream ss;
	ss << number;
	return ss.str();
}

//Gibt die Zeit und das Datum aus
//+++++++++++++++++++++++++++++++
string Tools::printTime()
{
	struct tm *localTime;
	time_t Time;
	//int sek;
	
	time(&Time);
	localTime = localtime(&Time);
	//clog << asctime(lokalZeit) << flush;
	return asctime(localTime);

}

string Tools::parseTime(string s)
{
	unsigned int position1;
	unsigned int position2;
	bool bpos1 = true;
	bool bpos2 = true;
	while(bpos1)
	{
		position1 = s.find_first_of(":");
		if (position1 != s.npos)
			s.replace(position1, 1,"xx");
		else
			bpos1 = false;
	}

	while(bpos2)
	{
		position2 = s.find_first_of(" ");
		if (position2 != s.npos)
			s.replace(position2, 1,"_");
		else
			bpos2 = false;
	}
		

	//if (position != s.npos)
	//	s.erase(position ,s.npos);
	//
	//s.erase(remove_if(s.begin(), s.end(), isspace),s.end());
	//
	return s;
}


string Tools::delSpacesAndComents(string s)
{
	unsigned int position = s.find_first_of("#");
	
	if (position != s.npos)
		s.erase(position ,s.npos);
	
	s.erase(remove_if(s.begin(), s.end(), isspace),s.end());
	
	return s;
}

void Tools::warte(int x)
{
	for (int i = 0; i < x; i++)
	{
		clog << "." << flush;
		Sleep(100);
	}
	clog << endl;
}