
h2_inline void h2_nm::initialize()
{
   I();
}

h2_inline unsigned long long h2_nm::get(const char* name) const
{
   if (strlen(name) == 0) return 0;
   auto it = symbols.find(name);
   return it != symbols.end() ? it->second : 0;
}

h2_inline h2_nm::h2_nm()
{
   char nm[256], line[1024], addr[128], type[32], name[1024];
   sprintf(nm, "nm %s", O.path);
   h2_with f(::popen(nm, "r"), ::pclose);
   if (f.f)
      while (::fgets(line, sizeof(line) - 1, f.f))
         if (3 == sscanf(line, "%s%s%s", addr, type, name))
            if (strchr("tTwWsSvV", type[0]))
               symbols.insert(std::make_pair(name + O.isMAC(), (unsigned long long)strtoull(addr, nullptr, 16)));
}
