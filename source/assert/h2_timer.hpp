struct h2_timer : h2_once {
   const char* file;
   int cpu_ms;
   clock_t start;
   h2_timer(int ms, const char* file);
   ~h2_timer();
};

#define __H2PF(ms, Q) for (h2::h2_timer Q(ms, H2_FILE); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE())
