#ifndef H_APP_EXCEPTION
#define H_APP_EXCEPTION

class AppException 
{
public:
	AppException(const char* message)
	{
		_message = message;
	}

	const char* GetMessage() 
	{ 
		return _message;
	};
private:
	const char* _message;
};

#endif


