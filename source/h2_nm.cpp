
struct h2_symbol : h2_libc {
   h2_list x;
   char name[128];
   unsigned long long addr;
   h2_symbol(char* _name, unsigned long long _addr) : addr(_addr) { strncpy(name, _name, sizeof(name)); }
};

h2_inline h2_nm::h2_nm()
{
   char nm[256], line[1024], addr[128], type[32], name[1024];
   sprintf(nm, "nm %s", O.path);
   h2_with f(::popen(nm, "r"), ::pclose);
   if (f.f)
      while (::fgets(line, sizeof(line) - 1, f.f))
         if (3 == sscanf(line, "%s%s%s", addr, type, name))  // if (strchr("tTwWsSvV", type[0]))
            symbols.push_back((new h2_symbol(name + O.isMAC(), (unsigned long long)strtoull(addr, nullptr, 16)))->x);
}

h2_inline unsigned long long h2_nm::get(const char* name)
{
   if (!name || strlen(name) == 0) return 0;
   h2_list_for_each_entry (p, I().symbols, h2_symbol, x)
      if (!strcmp(p->name, name))
         return p->addr;
   return 0;
}

struct h2_offset {
   h2_offset() {}
   virtual ~h2_offset() {}
   virtual void dummy() {}

   static long long vtable_offset()
   {
      static long long s = get_vtable_offset();
      return s;
   }

   static long long get_vtable_offset()
   {
      char vtable_symbol[256];
      h2_offset t;
      long long absolute_vtable = (long long)*(void***)&t;
      sprintf(vtable_symbol, "_ZTV%s", typeid(h2_offset).name());  // mangled for "vtable for h2_offset"
      long long relative_vtable = (long long)h2_nm::get(vtable_symbol);
      return absolute_vtable - relative_vtable;
   }

   static long long text_offset()
   {
      static long long s = get_text_offset();
      return s;
   }

   static long long get_text_offset()
   {
      long long relative_vtable = (long long)h2_nm::get("main");
      return (long long)&main - relative_vtable;
   }
};

h2_inline long long h2_nm::text_offset()
{
   return h2_offset::text_offset();
}
h2_inline long long h2_nm::vtable_offset()
{
   return h2_offset::vtable_offset();
}

h2_inline bool h2_nm::in_main(unsigned long long addr)
{
   static unsigned long long main_addr = get("main");
   if (main_addr == 0) return false;
   /* main() 52~60 bytes code in linux MAC */
   return main_addr < addr && addr < main_addr + 128;
}
