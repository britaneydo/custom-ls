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
// opendir(), readdir(), closedir()
#include <dirent.h>
// stat() and stat struct
#include <sys/stat.h>
// type definitions that stat() depends on
#include <sys/types.h>
// getpwuid() (user ID numbers --> user names)
#include <pwd.h>
// getgrgid() (group ID numbers --> group names)
#include <grp.h>
// strftime(), localtime()
#include <ctime>
using namespace std;

// ============================================================================
// getPermissions
//
// This is a function that will take as input a number that has the user's 
// permissions stored, and will turn that number into a rwx readable 
// permission line using bitmasks that displays permissions.
//
// INPUT: st_mode number (number that has user permissions stored)
//
// OUTPUT: human-readable rwx string (chmod)
// ============================================================================

string getPermissions(mode_t mode) 
{
    // string that will be replaced by rwx
    string perms = "----------";

    // FILE TYPE (first character)

    // S_ISDIR() is a macro that checks if the file type bits say "directory"
    if (S_ISDIR(mode))  
        perms[0] = 'd';

    // S_ISLNK() checks if it's a symbolic link (like a shortcut)
    else if (S_ISLNK(mode)) 
        perms[0] = 'l';

    // if neither, regular file; leave as '-'

    // OWNER PERMISSIONS (characters 1, 2, 3)

    // S_IRUSR is a bitmask for "owner read permission"
    // The & operator checks if that specific bit is set in mode
    // If the bit is set, the permission exists — replace '-' with the letter
    if (mode & S_IRUSR) perms[1] = 'r'; // owner can read
    if (mode & S_IWUSR) perms[2] = 'w'; // owner can write
    if (mode & S_IXUSR) perms[3] = 'x'; // owner can execute

    // GROUP PERMISSIONS (characters 4, 5, 6)

    if (mode & S_IRGRP) perms[4] = 'r'; // group can read
    if (mode & S_IWGRP) perms[5] = 'w'; // group can write
    if (mode & S_IXGRP) perms[6] = 'x'; // group can execute

    // OTHERS' PERMISSIONS (characters 7, 8, 9)

    if (mode & S_IROTH) perms[7] = 'r'; // others can read
    if (mode & S_IWOTH) perms[8] = 'w'; // others can write
    if (mode & S_IXOTH) perms[9] = 'x'; // others can execute

    return perms;

} // end of "getPermissions"

// ============================================================================
// getTimestamp
//
// This is a function that will take as input a number that has the user's 
// permissions stored, and will turn that number into a rwx readable 
// permission line using bitmasks that displays permissions.
//
// INPUT: time_t value
//
// OUTPUT: readable string (buffer)
// ============================================================================

string getTimestamp(time_t mtime) 
{

    // localtime() converts the raw Unix timestamp into a tm struct
    // tm struct breaks the time down into fields: tm_hour, tm_min, tm_mon, etc.
    struct tm* timeInfo = localtime(&mtime);

    // Create a char array (buffer) to hold the formatted string
    char buffer[20];

    // strftime() formats the tm struct into a human readable string
    // and writes it into our buffer
    // "%b" = abbreviated month name (Mar)
    // "%d" = day of month (21)
    // "%H:%M" = hour and minute in 24hr format (14:32)
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);

    // convert array to C++ string
    return string(buffer);
}

// ============================================================================
// listDirectory
//
// This function takes as an argument a directory path and lists out all the
// files in that directory, calling getPermissions() when needed to get
// owner, group, and others' perimissions.
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

        // build full path to file for stat() function
        string fullPath = path + "/" + entry->d_name;

        // create stat struct 
        struct stat fileStat;

        // call stat() to fill fileStat with data, will return -1 on failure
        if (stat(fullPath.c_str(), &fileStat) == -1) 
        {
            // If stat() fails (permission denied), skip this file
            cerr << "Cannot stat: " << fullPath << "\n";
            continue;
        }

        // getting owner/group names
        struct passwd* pw = getpwuid(fileStat.st_uid);
        string owner = (pw != nullptr) ? pw->pw_name : "unknown";
        struct group* gr = getgrgid(fileStat.st_gid);
        string grpName = (gr != nullptr) ? gr->gr_name : "unknown";

        
        // print: permissions, owner, group, size, timestamp, filename
        cout << getPermissions(fileStat.st_mode) << "\t"
                << owner << "\t"        // owner username
                << grpName << "\t"      // group name
                << fileStat.st_size << "\t"   // size in bytes
                << getTimestamp(fileStat.st_mtime) << "\t"  // last modified
                << entry->d_name << "\n";     // filename
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