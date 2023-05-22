#include "Error.h"
Errorexc::Errorexc(int line, const char* file)
{
	this->line = line;
	this->file = file;
}
const char* Errorexc::Result()
{
	std::ostringstream oss;
	oss << ErrorType() << std::endl << OGString();
	bufor = oss.str();
	return bufor.c_str();
}
std::string& Errorexc::GetFile()
{
	return file;
}
const char* Errorexc::ErrorType()
{
	return "I tak to moze stac puste";
}
int Errorexc::GetLine()
{
	return line;
}
std::string Errorexc::OGString()
{
	std::ostringstream oss;
	oss << "[Plik] " << file << std::endl << "[Linia] " << line;
	return oss.str();

}