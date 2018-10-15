/*
** JG - tests_hnStat
*/

#include <iostream>

#include "SearchFilter.hh"
#include "TestSearchFilter.hh"



SearchFilter *TestSearchFilter::s_search_filter = nullptr;

void TestSearchFilter::SetUp()
{
  if (TestSearchFilter::s_search_filter != nullptr)
    delete s_search_filter;
  TestSearchFilter::s_search_filter = new SearchFilter();
}

void TestSearchFilter::TearDown()
{
  delete TestSearchFilter::s_search_filter;
  TestSearchFilter::s_search_filter = nullptr;
}




TEST_F(TestSearchFilter, test_noargs_parseCriteria)
{
  const char *argv[] = {"hnStat", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_double_command_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "top", "3", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_no_low_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--from", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_no_low_timestamp_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--from", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_no_up_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--to", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_no_up_timestamp_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--to", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_distinct_no_low_and_up_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--from", "--to", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_no_low_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--from", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_no_low_timestamp_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--from", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_no_up_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--to", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_no_up_timestamp_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--to", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_no_low_and_up_timestamp_nofile_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--from", "--to", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_leading_zeros_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "000030", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_TRUE(retval);
  ASSERT_EQ(30, TestSearchFilter::s_search_filter->getTopQueriesNumber());
}

TEST_F(TestSearchFilter, test_topx_leading_zeros_timestamps_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--from", "000000",
			"--to", "01234", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_TRUE(retval);
  ASSERT_EQ("0", TestSearchFilter::s_search_filter->getLowerTimestamp());
  ASSERT_EQ("1234", TestSearchFilter::s_search_filter->getUpperTimestamp());
}

TEST_F(TestSearchFilter, test_distinct_leading_zeros_timestamps_parseCriteria)
{
  const char *argv[] = {"hnStat", "distinct", "--from", "0",
			"--to", "004321", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_TRUE(retval);
  ASSERT_EQ("0", TestSearchFilter::s_search_filter->getLowerTimestamp());
  ASSERT_EQ("4321", TestSearchFilter::s_search_filter->getUpperTimestamp());
}

TEST_F(TestSearchFilter, test_wrong_command_parseCriteria)
{
  const char *argv[] = {"hnStat", "jesuisunemiteenpullover", "000030", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_wrong_number_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "jesuisunemiteenpullover", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}

TEST_F(TestSearchFilter, test_topx_low_timestamp_bigger_than_up_parseCriteria)
{
  const char *argv[] = {"hnStat", "top", "3", "--from", "1234",
			"--to", "123", "./Makefile", NULL};
  bool retval = TestSearchFilter::s_search_filter->parseCriteria(const_cast<char **>(argv));
  ASSERT_FALSE(retval);
}
