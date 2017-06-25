// Some general purpose utilities.

#ifndef _HELPER_H_
#define _HELPER_H_

#include <iostream>
#include <sys/stat.h>

using namespace std;

class Helper {
public:
    // Prints a progress text and flushes out.
    static void show_progress(string theString);
    // Checks whether a file exists or not.
    static int file_exists(const std::string& name);
    // Checks whether a directory exists or not.
    static int dir_exists(const std::string& name);
    // Capitalizes a string
    static string capitalize(const std::string &theString);
};

#endif