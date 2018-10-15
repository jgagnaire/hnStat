/*
** JG - tests_hnStat
*/

#include <iostream>
#include <sstream>
#include <errno.h>

#include "File.hh"
#include "TestFile.hh"

TEST_F(TestFile, test_open_wrong_file)
{
  File file("jesuisunemiteenpullover\\o//");
  ASSERT_FALSE(file.isOpened());
  ASSERT_EQ(errno, ENOENT);
}

TEST_F(TestFile, test_open_correct_file)
{
  File file("./Makefile");
  ASSERT_TRUE(file.isOpened());
}

TEST_F(TestFile, test_set_correct_offset)
{
  File file("./Makefile");
  ASSERT_TRUE(file.isOpened());
  ASSERT_TRUE(file.setOffset(10));
}

TEST_F(TestFile, test_set_wrong_offset)
{
  // Redirect stderr to ss
  std::stringstream ss;
  std::streambuf *sbuf = std::cerr.rdbuf();
  std::cerr.rdbuf(ss.rdbuf());

  File file("./Makefile");
  ASSERT_TRUE(file.isOpened());
  ASSERT_FALSE(file.setOffset(-400));
  ASSERT_EQ(ss.str(), "Error: lseek() failed: Invalid argument\n");

  // Restore stderr
  std::cerr.rdbuf(sbuf);
}

TEST_F(TestFile, test_all_functions_wrong_file)
{
  File file("jesuisunemiteenpullover\\o//");
  ASSERT_FALSE(file.isOpened());
  ASSERT_EQ(errno, ENOENT);
  ASSERT_FALSE(file.setOffset(10));
  std::string s;
  ASSERT_FALSE(file.getLine(s));
}
