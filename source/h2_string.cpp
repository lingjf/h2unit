
h2_inline h2_string::h2_string(const double d) : basic_string()
{
   char t[128];
   ::sprintf(t, "%1.15g", d);
   assign(t);
}

h2_inline bool h2_string::equals(h2_string str, bool caseless) const
{
   if (!caseless) return *this == str;
   return tolower(c_str()) == tolower(str);
}

h2_inline bool h2_string::contains(h2_string substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower(c_str()).find(tolower(substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(h2_string prefix, bool caseless) const
{
   if (!caseless) return find(prefix) == 0;
   return tolower(c_str()).find(tolower(prefix)) == 0;
}

h2_inline bool h2_string::endswith(h2_string suffix, bool caseless) const
{
   if (!caseless) return rfind(suffix) == length() - suffix.length();
   return tolower(c_str()).rfind(tolower(suffix)) == length() - suffix.length();
}

h2_inline bool h2_string::isspace() const
{
   for (auto& c : *this)
      if (!::isspace(c)) return false;
   return true;
}

h2_inline h2_string& h2_string::tolower()
{
   for (auto& c : *this) c = ::tolower(c);
   return *this;
}

h2_inline h2_string h2_string::acronym(int atmost) const
{
   char t[256];
   strncpy(t, c_str(), atmost);
   strcpy(t + atmost, SF("dark gray", "..."));
   return h2_string(t);
}

h2_inline h2_string& h2_string::center(int width)
{
   int left = (width - length()) / 2, right = width - left - length();
   insert(0, left, ' ');
   append(right, ' ');
   return *this;
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...)
{
   va_list a;
   va_start(a, format);
#if defined _WIN32
   int length = _vscprintf(format, a);
#else
   int length = vsnprintf(nullptr, 0, format, a);
#endif
   va_end(a);
   char* t = (char*)alloca(length + 1);
   va_start(a, format);
   vsprintf(t, format, a);
   va_end(a);
   append(t);
   return *this;
}
