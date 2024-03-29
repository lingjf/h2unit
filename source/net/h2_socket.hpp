struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_sock : h2_once {
   h2_sock();
   ~h2_sock();
   static void clear();
   static void inject(const void* packet, size_t size, const char* attributes = "");  // from=1.2.3.4:5678, to=4.3.2.1:8765
   static h2_packet* fetch();
   static void failing(h2_fail* fail);
   template <typename M1 = h2_polymorphic_matcher<h2_matches_any>, typename M2 = h2_polymorphic_matcher<h2_matches_any>, typename M3 = h2_polymorphic_matcher<h2_matches_any>, typename M4 = h2_polymorphic_matcher<h2_matches_any>>
   static void check(const char* filine, const char* e, M1 from = _, M2 to = _, M3 payload = _, M4 size = _)
   {
      h2_fail* fail = nullptr;
      h2_packet* p = h2_sock::fetch();
      if (!p) {
         h2_line t = "Outgoing packet miss Ptx(";
         t.printf("green", "%s", e).printf("", ")");
         fail = h2_fail::new_normal(t, filine);
      } else {
         h2_fail* fails = nullptr;
         h2_fail* fail_from = h2_matcher_cast<const char*>(We(from)).matches(p->from.c_str());
         if (fail_from) {
            fail_from->explain = "from address";
            h2_fail::append_subling(fails, fail_from);
         }
         h2_fail* fail_to = h2_matcher_cast<const char*>(We(to)).matches(p->to.c_str());
         if (fail_to) {
            fail_to->explain = "to address";
            h2_fail::append_subling(fails, fail_to);
         }
         h2_fail* fail_payload = h2_matcher_cast<const unsigned char*>(payload).matches((unsigned char*)p->data.data());
         if (fail_payload) {
            fail_payload->explain = "payload";
            h2_fail::append_subling(fails, fail_payload);
         }
         h2_fail* fail_size = h2_matcher_cast<const int>(size).matches(p->data.size());
         if (fail_size) {
            fail_size->explain = "payload length";
            h2_fail::append_subling(fails, fail_size);
         }
         if (fails) {
            h2_line t = "Outgoing packet unexpected Ptx(";
            t.printf("green", "%s", e).printf("", ")");
            fail = h2_fail::new_normal(t, filine);
            h2_fail::append_child(fail, fails);
         }
      }
      failing(fail);
   }
};

#define __H2SOCK(Q) for (h2::h2_sock Q; Q;)
#define H2SOCK(...) __H2SOCK(H2PP_UNIQUE())

#define Ptx(...) h2::h2_sock::check(H2_FILINE, h2::ss(#__VA_ARGS__), __VA_ARGS__)
#define Pij(Packet_, Size_, ...) h2::h2_sock::inject(Packet_, Size_, h2::ss(#__VA_ARGS__))
