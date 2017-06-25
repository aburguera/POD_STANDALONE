#include "helper.h"

void Helper::show_progress(string theString){
    cout << "[PROGRESS: " << theString << "]\n" << std::flush;
}

int Helper::file_exists(const string &name) {
    struct stat info;
    return (stat(name.c_str(), &info)!=0)?0:(!(info.st_mode & S_IFDIR));
}

int Helper::dir_exists(const string &name) {
    struct stat info;
    return (stat(name.c_str(), &info)!=0)?0:(info.st_mode & S_IFDIR);
}

string Helper::capitalize(const string &theString) {
    string outStr=theString;
    for (int i=0;i<theString.length();i++) outStr[i]=toupper(theString[i]);
    return outStr;
}