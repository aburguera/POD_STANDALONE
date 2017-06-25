#include "dirreader.h"

DirReader::DirReader() {
    _fileOpened=0;
}

DirReader::DirReader(string dirName){
    initialize(dirName);
}

DirReader::~DirReader() {
    if (_fileOpened) fclose(_fileID);
}

void DirReader::initialize(string dirName) {
    _dirName=dirName;
    _fileOpened=((_fileID=fopen((dirName+_DRD_DIRLIST_).c_str(),"r"))!=NULL);
    for (_numFiles=0;next();_numFiles++);
    if (_fileOpened) rewind(_fileID);
    _curFile=0;
}

int DirReader::next(){
    _curFile++;
    return _fileOpened?fscanf(_fileID,"%s",_curFileName)!=-1:0;
}

string DirReader::get(){
    return _fileOpened?_dirName+string(_curFileName):"";
}

string DirReader::get_filename(){
    return _fileOpened?string(_curFileName):"";
}

int DirReader::get_num_files() {
    return _numFiles;
}

int DirReader::get_cur_file_num() {
    return _curFile;
}
