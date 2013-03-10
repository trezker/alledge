#ifndef Exception_h
#define Exception_h

#include <exception>
#include <string>

class Exception {
public:
	Exception(const std::string& s);
	virtual const std::string& Message() const;
private:
	std::string message;
};

#endif
