// ============================================================================
// File: lscommand.cpp
// ============================================================================
// This program implements a new way the Linux command "ls" can be used.
//
// Input:
// One or more directory names.
//
// Output:
// All the files in said directory, one line per file, formatted properly.
//
// ============================================================================

#include <iostream>
// opendir, readdir, closedir
#include <dirent.h>
using namespace std;

// ============================================================================
// listDirectory
//
// INPUT: directory path (string)
//
// OUTPUT: all files in directory
// ============================================================================

void listDirectory(const string &path) 
{
    // opendir() --> asks OS to open directory at given path
    // returns ptr to a DIR object, returns nullptr if fails
    // since opendir() is C and not C++, use path.c_str() to convert C++ str to C str
    DIR* dir = opendir(path.c_str());

    // error handling
    if (dir == nullptr) 
    {
        cerr << "Cannot open directory: " << path << "\n";
        return;
    }

    // create dirent struct that holds info about one directory (file/folder)
    struct dirent* entry;

    // readdir() reads next file entry every time it is called, once no more
    // files are found, returns nullptr --> loop ends
    while ((entry = readdir(dir)) != nullptr) 
    {
        // skip all ., .., and hidden "files" (parent, .git)
        if (entry->d_name[0] == '.') 
        {
            continue;
        }

        // print file followed by newline
        cout << entry->d_name << "\n";
    }

    // finished with this directory, close and move on to free space
    closedir(dir);

} // end of "listDirectory"

// ============================================================================
// main()
// ============================================================================

// argc = number of command-line arguments (including the program name itself)
// argv = array of those arguments as C-style strings
int main(int argc, char* argv[]) 
{
    // no arguments given, list current directory
    if (argc < 2) 
    {   
        // "." means current path
        listDirectory(".");

    } 
    
    // loop thru all user arguments. start at i = 1 since argv[] is program name (./myls)
    else
    {
        for (int i = 1; i < argc; i++) 
        {   
            // print directory name first
            cout << argv[i] << ":\n";

            // function call to list out all files in this directory, \n in between
            listDirectory(argv[i]);
            cout << "\n";
        }
    }
    return 0;

} // end of "main"