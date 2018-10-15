/*
** JG - hnStat
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
#include <string>
#include <array>

#include "File.hh"

File::File(std::string const &name)
{
  _file_fd = ::open(name.c_str(), O_RDONLY);
}

File::~File()
{
  if (_file_fd != -1)
    ::close(_file_fd);
}

bool File::isOpened() const
{
  return _file_fd != -1;
}

bool File::setOffset(off_t offset) const
{
  if (_file_fd == -1)
    return false;

  if (::lseek(_file_fd, offset, SEEK_SET) == -1)
    {
      std::cerr << "Error: lseek() failed: " << ::strerror(errno) << std::endl;
      return false;
    }
  return true;
}

bool File::getLine(std::string &line) const
{
  if (_file_fd == -1)
    return false;

  char buf[BUFFER_SIZE] = {};
  ssize_t ret;
  ssize_t idx;

  line = "";
  while (true)
    {
      ret = ::read(_file_fd, buf, BUFFER_SIZE);
      if (ret == -1)
	{
	  std::cerr << "Error: read() failed: " << ::strerror(errno) << std::endl;
	  return false;
	}
      else if (ret == 0) // EOF reached
	{
	  // return false is current 'line' buffer is empty, true otherwise
	  return !line.empty();
	}

      idx = 0;
      while (idx < BUFFER_SIZE)
	{
	  if (buf[idx] == '\n')
	    {
	      if (::lseek(_file_fd, -(ret - idx - 1), SEEK_CUR) == -1)
		{
		  std::cerr << "Error: lseek() failed: " << ::strerror(errno) << std::endl;
		  return false;
		}
	      return true;
	    }
	  line += buf[idx];
	  ++idx;
	}
    }
  return true;
}
