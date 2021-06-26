
struct h2_backtrace {
   int count = 0, shift = 0;
   void* frames[100];

   h2_backtrace() = default;
   h2_backtrace(int shift_);

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(const h2_backtrace&);

   bool has(void* func, int size) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(int pad) const;
};
