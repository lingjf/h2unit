struct h2_timer : h2_once {
   const char* file;
   int line;
   int ms;
   clock_t start;
   h2_timer(int ms, const char* file, int line);
   ~h2_timer();
};

#define __H2PF(ms, Q) for (h2::h2_timer Q(ms, __FILE__, __LINE__); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE())
