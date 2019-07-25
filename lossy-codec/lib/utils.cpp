#include "utils.h"

using namespace std;

std::string expand_user(std::string path) {
	if (not path.empty() and path[0] == '~') {
		assert(path.size() == 1 or path[1] == '/');  // or other error handling
		char const* home = getenv("HOME");
		if (home or ((home = getenv("USERPROFILE")))) {
			path.replace(0, 1, home);
		}
		else {
			char const *hdrive = getenv("HOMEDRIVE"),
			*hpath = getenv("HOMEPATH");
			assert(hdrive);  // or other error handling
			assert(hpath);
			path.replace(0, 1, std::string(hdrive) + hpath);
		}
	}
	return path;
}

long get_file_size(std::string filename) {
    struct stat stat_buf; 
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}