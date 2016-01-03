#ifndef _INSTANCECOUNTER_HPP_
#define _INSTANCECOUNTER_HPP_

#include <iostream>

template<class T_Observee> class IInstanceCounter
{
public:
	static unsigned long s_ulInstanceCount;
	static FILE* s_pLog;
	
	IInstanceCounter()
	{
		s_ulInstanceCount++;
	}
	
	virtual ~IInstanceCounter()
	{
		s_ulInstanceCount--;
		// open the report log
		//if(!s_pLog)
		//{
		//	std::string sLogFile = typeid(this).name();
		//	for each(const char& ch in sLogFile)
		//	{
		//		switch(ch)
		//		{
		//		case '<':
		//		case '>':
		//		case ':':
		//		case ' ':
		//		case '*':
		//			const_cast<char&>(ch) = '_';
		//		}
		//	}
		//	sLogFile = "E:\\InstanceCounter\\" + sLogFile + ".txt";
		//	s_pLog = fopen(sLogFile.c_str(), "a");
		//}
		//std::stringstream ss;
		//ss << s_ulInstanceCount << "\r\n";
		//fputs(ss.str().c_str(), s_pLog);
		//fclose(s_pLog);
		//s_pLog = NULL;
	}
};

template<class T_Observee> unsigned long IInstanceCounter<T_Observee>::s_ulInstanceCount = 0;
template<class T_Observee> FILE* IInstanceCounter<T_Observee>::s_pLog = NULL;

#endif