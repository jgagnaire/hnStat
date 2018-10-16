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
  struct stat st_buf = {};

  if (::stat(name.c_str(), &st_buf) != -1)
    _block_size = st_buf.st_blksize;
  else
    _block_size = 4096; // Set a default blocksize value

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

  char *buf = new char[_block_size];
  bool ret = true;
  ssize_t retval_read;
  ssize_t idx;

  line = "";
  std::memset(buf, 0, _block_size);
  while (true)
    {
      retval_read = ::read(_file_fd, buf, _block_size);
      if (retval_read == -1)
	{
	  std::cerr << "Error: read() failed: " << ::strerror(errno) << std::endl;
	  ret = false;
	  goto end;
	}
      else if (retval_read == 0) // EOF reached
	{
	  // return false is current 'line' buffer is empty, true otherwise
	  ret = !line.empty();
	  goto end;
	}

      idx = 0;
      while (idx < _block_size)
	{
	  if (buf[idx] == '\n')
	    {
	      if (::lseek(_file_fd, -(retval_read - idx - 1), SEEK_CUR) == -1)
		{
		  std::cerr << "Error: lseek() failed: " << ::strerror(errno) << std::endl;
		  ret = false;
		  goto end;
		}
	      ret = true;
	      goto end;
	    }
	  line += buf[idx];
	  ++idx;
	}
    }

 end:
  delete[] buf;
  return ret;
}
