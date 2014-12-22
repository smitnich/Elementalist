#include "base.h"
//The file to be written to
extern FILE *outputFile;
string convertInt(int);
//Various ways of writing to a file
void outputLog(char output[])
{
	if (outputFile == NULL)
		return;
	fputs(output,outputFile);
}
void outputLog(const char output[])
{
	if (outputFile == NULL)
		return;
	fputs(output,outputFile);
}
void outputLog(int output)
{
	if (outputFile == NULL)
		return;
	fputs(const_cast<char *>(convertInt(output).data()),outputFile);
}