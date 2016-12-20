/**
 * @brief Contains the implementation of getFullNameOfFile()
 * @author MTLCRBN
 * @version 1.0
 */
#include "file_io.hpp"


std::string& getFullNameOfFile(const std::string &nameFromRoot, const char *const argv0, std::string &out)
{
	// Here, i'm using NULL and free() because realpath called malloc()
	// In order to prevent issues, we better have to call free instead of delete.
	char *absolutePath = realpath(argv0, NULL);
	if (absolutePath == NULL)
	{
		throw std::string("Error while getting absolute path of ") + nameFromRoot;
	}
	out = std::string(absolutePath);
	free(absolutePath);
	out = out.substr(0, out.find_last_of("\\/")) + "/" + nameFromRoot;
	return out;
}
