#pragma once
#include<exception>
#include<string>
#include<sstream>
class Errorexc : public std::exception
{
public:
	Errorexc(int line, const char* file);
	virtual const char* ErrorType(); // funkcja abstrakcyjna, bedzie uzywana przez klasy dziedziczące
	int GetLine();
	std::string OGString();
	std::string& GetFile();
	virtual const char* Result() ;	//musi byc wskaznikiem z powodu tego ze Messagebox przyjmuje LPCSTR
private:
	int line;
	std::string file;
protected:
	 std::string bufor; // przetrzymuje strumien string z Result

};