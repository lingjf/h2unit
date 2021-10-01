struct h2_timer : h2_once {
   h2_sz sz;
   int cpu_ms;
   clock_t start;
   h2_timer(int ms, const h2_sz& sz);
   ~h2_timer();
};

#define __H2PF(ms, Q) for (h2::h2_timer Q(ms, {__FILE__, __LINE__}); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE())
