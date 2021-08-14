
GlobalSetup()
{
}

GlobalCleanup()
{
}

GlobalSuiteSetup()
{
}

GlobalSuiteCleanup()
{
}

GlobalCaseSetup()
{
}
GlobalCaseCleanup()
{
}

SUITE(a)
{
}

SUITE(b)
{
   Cleanup()
   {
   }

   Case(1)
   {
   }
   Todo(2)
   {
   }
}

time_t time_fake(time_t*) { return 0; }

SUITE(c)
{
   Case(1)
   {
      OK(true);
      OK(1, 1);
      OK("", "");
   }

   Case(2)
   {
      OK(Ge(0), 1);
      OK(Lt(0), 1);
      OK(We(""), "");
      OK(Re(""), "");
      OK(Re(""), "");
      OK(Substr(""), "");
      OK(Je(""), "");
      JE("", "");
   }

   Case(3)
   {
      OK(_, 0);
      OK(_, 1);
      OK(Any, 0);
      OK(Any, 1);
      OK(IsNull, nullptr);
      OK(NotNull, NULL);
      OK(IsTrue, true);
      OK(IsTrue, false);
      OK(IsFalse, false);
      OK(IsFalse, true);
   }

   Case(4)
   {
      OK(!Not(0), 1);
      OK(AllOf(_), 1);
      OK(AnyOf(_), 1);
      OK(NoneOf(_), 1);
      OK(!Eq(1) && (Ge(1) || Lt(1)), 1);
   }

   Case(5)
   {
      BLOCK()
      {
      }
   }

   Case(6)
   {
      MOCK(time, time_t, (time_t*)).Once().Return((time_t)0);
      MOCKS(time, time_t, (time_t*), Once()) { return 0; };
      STUB(time, time_t, (time_t*), time_fake);
   }
}
