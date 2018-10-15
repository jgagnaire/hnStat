/*
** JG - hnStat
*/

#ifndef FILE_HH_
# define FILE_HH_

# include <sys/types.h>
# include <cstdint>
# include <string>

// buffer size for ::read() syscall
# define BUFFER_SIZE (4096)

/*
** Class abstracting a file.
**
** The reason for re-implementing a file abstraction is
** that std::ifstream() cannot be used accross different
** threads without locking.
**
** To avoid mutexes, each thread then has to ::open() the
** file in order to get its own file descriptor, and work
** on its own offsets range.
*/
class File
{
public:
  // Performs a call to ::open() on the filename provided
  File(std::string const &name);
  ~File();

  // Returns false if the call to ::open() failed, true otherwise
  bool isOpened() const;

  /*
  ** Seeks to the provided offset in file.
  **
  ** return: true if operation succeeded,
  **         false otherwise
  */
  bool setOffset(off_t offset) const;

  /*
  ** Reads an entire line from file, and stores it in
  ** the 'line' string given as parameter without the
  ** newline character.
  **
  ** return: true if success
  **         false if EOF.
  **
  ** In case of a syscall failure, an error message is
  ** printed on the error output, and false is returned.
  */
  bool getLine(std::string &line) const;

private:
  int _file_fd;
};

#endif // !FILE_HH_
