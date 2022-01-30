#include "local_file_system.h"

sb::FileSystem sb::getLocalFileSystem() 
{
    return {
        .open_file_read = &openLocalFileRead,
        .open_file_write = &openLocalFileWrite,
        .read_file = &readLocalFile,
        .write_file = &writeLocalFile,
        .get_file_length = &getLocalFileLength,
        .close_file = &closeLocalFile,
        .file_exists = &localFileExists,
        .is_file_valid = &isLocalFileValid
    };
}
