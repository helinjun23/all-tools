#ifndef _FILE_HELPER_H_
#define _FILE_HELPER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct FileHelper
{
    static int read_file_content(const char* file_path, long& length, char** memory)
    {
        std::fstream fs;
        fs.open(file_path, std::ios::in | std::ios::binary);
        if (!fs)
        {
            return -1;
        }
        fs.seekg(0, std::ios::end);
        length = fs.tellg();
        fs.seekg(0, std::ios::beg);
        *memory = new char[length];
        fs.read(*memory, length);
        fs.close();
        return 0;
    }

    static int write_file(const char *file_path, char *buf, int buflen)
    {
        std::ofstream fs;
        fs.open(file_path, std::ios::out | std::ios::binary);
        if (!fs)
        {
            return -1;
        }
        fs.write((const char *)buf, buflen);
        fs.close();
        return 0;
    }

    static void release_file_buf(char** memory)
    {
       if((*memory) != NULL)
       {
           delete [](*memory);
           (*memory)=NULL;
       }
    }
};


#endif
