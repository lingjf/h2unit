struct h2_backtrace {
   int count = 0, shift = 0;
   void* frames[100];

   h2_backtrace(int shift_ = 0) : shift(shift_) {}
   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;
   void clear() { count = 0; }

   bool operator==(const h2_backtrace&) const;
   static h2_backtrace& dump(int shift_);

   bool in(void* fps[]) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(size_t pad) const;
};
