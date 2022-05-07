#ifndef DEMO_H
#define DEMO_H

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include<iostream>

///Taken from https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus @return a string representing the path of the working dir
std::string getWorkDir() {
    //create string buffer to hold path
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

#endif//DEMO_H
