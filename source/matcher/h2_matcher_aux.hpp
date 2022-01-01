static inline h2::h2_string File(const char* filename)  // slurp
{
   FILE* f = fopen(filename, "rb");
   if (!f) return "";
   fseek(f, 0, SEEK_END);
   auto l = ftell(f);
   fseek(f, 0, SEEK_SET);
   h2::h2_string s(l, ' ');
   fread((char*)s.data(), 1, l, f);
   ((char*)s.data())[l] = '\0';
   fclose(f);
   return s;
}

H2MATCHER(IsEven) { return a % 2 == 0; }
H2MATCHER(IsOdd) { return a % 2 != 0; }
H2MATCHER(IsDivable, expect) { return a % expect == 0; }
H2MATCHER(IsInteger) { return ::floor(a) == a; }
H2MATCHER(IsNaN) { return std::isnan(a); }
