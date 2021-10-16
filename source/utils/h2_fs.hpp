struct h2_fs {
   const char* file;
   int line;
   const char* func;
   h2_fs(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr) : file(file_), line(line_), func(func_) {}
   bool empty() const;
   const char* basefile() const;
};
