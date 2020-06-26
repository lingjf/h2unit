
struct h2_nm {
   h2_singleton(h2_nm);
   static void initialize();
   std::map<std::string, unsigned long long> symbols;
   unsigned long long get(const char* name) const;
   h2_nm();
};
