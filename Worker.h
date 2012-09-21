#ifndef H_WORKER
#define H_WORKER

#include "Parser.h"
#include "Linearizator.h"

class Worker
{
public:
	void Execute(FILE* inputFile = stdin);
private:
	Parser _parser;
	Linearizator _linearizator;
};

#endif


