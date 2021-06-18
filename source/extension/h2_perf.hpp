
struct h2_perf : h2_once {
   const char* file;
   int line;
   long long ms, start;
   h2_perf(long long ms, const char* file, int line);
   ~h2_perf();
};

#define __H2PF(ms, Q) for (h2::h2_perf Q(ms, __FILE__, __LINE__); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE(t_pf))
