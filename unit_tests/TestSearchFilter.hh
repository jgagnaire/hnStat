/*
** JG - tests_hnStat
*/

#ifndef TESTCLASS_HH_
# define TESTCLASS_HH_

# include "gtest/gtest.h"

# include "SearchFilter.hh"

class TestSearchFilter : public ::testing::Test
{
public:
  TestSearchFilter() {}
  virtual ~TestSearchFilter() {}
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
  virtual void SetUp();
  virtual void TearDown();

  static SearchFilter *s_search_filter;
};

#endif // !TESTCLASS_HH_
