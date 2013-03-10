#include "../alledge/Exception.h"

Exception::Exception(const std::string& s) {
	message = s;
}
const std::string& Exception::Message() const {
	return message;
}
