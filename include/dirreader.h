// Simple class to read dir contents. A file named "FILES.TXT" must be
// inside the dir with the file names.
// Example to create FILES.TXT for .png files on linux:
// ls *.png -1 >> FILES.TXT
// Example of usage:
// DirReader theDirReader("IMG/");
// while (theDirReader.next()) {
//    cout << theDirReader.get()<<"\n";
// }

#ifndef _DIRREADER_H_
#define _DIRREADER_H_

#include <stdio.h>
#include <iostream>

using namespace std;

// Dir reader constants
#define _DRD_DIRLIST_       "FILES.TXT"

class DirReader {
private:
    FILE *_fileID;          // The file handler
    string _dirName;        // Dir name. Stored to attach to file names.
    char _curFileName[256]; // File name to provide.
    int _fileOpened;        // True if file has been opened.
    int _numFiles;
    int _curFile;

public:
    // Basic constructor
    DirReader();
    // Store dir name and try to open file.
    DirReader(string dirName);
    // Close file if open.
    ~DirReader();
    // Store dir name and try to open file.
    void initialize(string dirName);
    // Store next file name.
    int next();
    // Provide full file name.
    string get();
    // Provide file name without dir name
    string get_filename();
    // Provide file count
    int get_num_files();
    // Provide cur file index
    int get_cur_file_num();
};

#endif