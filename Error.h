#pragma once
#include<exception>
#include<string>
#include<sstream>
class Errorexc : public std::exception
{
public:
	Errorexc(int line, const char* file);
	virtual const char* ErrorType(); // funkcja abstrakcyjna, bedzie uzywana przez klasy dziedzicz¹ce
	int GetLine();
	std::string OGString();
	std::string& GetFile();
	const char* Result() ;	//musi byc wskaznikiem z powodu tego ze Messagebox przyjmuje LPCSTR
private:
	int line;
	std::string file;
protected:
	mutable std::string bufor; // przetrzymuje strumien string z Result

};