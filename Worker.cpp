#include "Worker.h"

void Worker::Execute(FILE* inputFile)
{
	try
	{
		TList<char>* inputString = _parser.ReadString(inputFile);
		int inputStringLength = inputString->GetCount();
		int leafNumber = _linearizator.Linearize(inputString);

		_parser.PrintResult(leafNumber, inputString, inputStringLength);

	}
	catch (AppException &exception)
	{
		_parser.PrintError(exception.GetMessage());
	}
	//catch (std::bad_alloc &exception)
	//{
	//	_parser.PrintError(ErrorMessages::MEMORY_ERROR);
	//}
}

