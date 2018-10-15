/*
** JG - tests_hnStat
*/

#ifndef TESTFILE_HH_
# define TESTFILE_HH_

# include "gtest/gtest.h"

# include "File.hh"

class TestFile : public ::testing::Test
{
public:
  TestFile() {}
  virtual ~TestFile() {}
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
};

#endif // !TESTFILE_HH_
                     
