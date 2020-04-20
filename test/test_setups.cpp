#include "../source/h2_unit.cpp"

#include <time.h>

static char*
my_asctime_r(const struct tm* tm, char* buf)
{
  return "2020-04-09";
}

GlobalSetup()
{
  STUB(asctime_r, my_asctime_r);
  MOCK(asctime, char*(const struct tm*)).any().returns("2020-04-10");
}

static char*
my_ctime_r(const time_t* timep, char* buf)
{
  return "2020-04-09";
}

GlobalSuiteSetup()
{
  STUB(ctime_r, my_ctime_r);
  MOCK(ctime, char*(const time_t*)).any().returns("2020-04-10");
}

struct tm*
my_gmtime_r(const time_t* timep, struct tm* result)
{
  return (struct tm*)1024;
}

GlobalCaseSetup()
{
  STUB(gmtime_r, my_gmtime_r);
  MOCK(gmtime, struct tm * (const time_t*)).any().returns((struct tm*)1024);
}

static time_t
my_mktime(struct tm* tm)
{
  return 1024;
}

SUITE(Stub / Mock in Setups)
{
  STUB(mktime, my_mktime);
  MOCK(time, time_t(time_t*)).any().returns(1024);

  Case(a)
  {
    OK("2020-04-09", asctime_r(NULL, NULL));
    OK("2020-04-09", ctime_r(NULL, NULL));
    OK((struct tm*)1024, gmtime_r(NULL, NULL));
    OK(1024, mktime(NULL));

    OK("2020-04-10", asctime(NULL));
    OK("2020-04-10", ctime(NULL));
    OK((struct tm*)1024, gmtime(NULL));
    OK(1024, time(NULL));
  }

  Case(b)
  {
    OK("2020-04-09", asctime_r(NULL, NULL));
    OK("2020-04-09", ctime_r(NULL, NULL));
    OK((struct tm*)1024, gmtime_r(NULL, NULL));
    OK(1024, mktime(NULL));

    OK("2020-04-10", asctime(NULL));
    OK("2020-04-10", ctime(NULL));
    OK((struct tm*)1024, gmtime(NULL));
    OK(1024, time(NULL));
  }
}
