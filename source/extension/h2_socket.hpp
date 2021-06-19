
struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_sock : h2_once {
   h2_sock();
   ~h2_sock();
   static void clear();
   static void inject(const void* packet, size_t size, const char* attributes);  // from=1.2.3.4:5678, to=4.3.2.1:8765
   static h2_packet* fetch();

   template <typename M1 = h2_polymorphic_matcher<h2_matches_any>, typename M2 = h2_polymorphic_matcher<h2_matches_any>, typename M3 = h2_polymorphic_matcher<h2_matches_any>, typename M4 = h2_polymorphic_matcher<h2_matches_any>>
   static void check(const char* file, int line, const char* e, M1 from = Any, M2 to = Any, M3 payload = Any, M4 size = Any)
   {
      h2_assert_g();
      h2_packet* p = h2_sock::fetch();
      h2_fail* fails = nullptr;
      h2_fail* fail_from = h2_matcher_cast<const char*>(from).matches(p->from.c_str(), 0, false, false);
      if (fail_from) {
         fail_from->explain = "from address";
         h2_fail::append_subling(fails, fail_from);
      }
      h2_fail* fail_to = h2_matcher_cast<const char*>(to).matches(p->to.c_str(), 0, false, false);
      if (fail_to) {
         fail_to->explain = "to address";
         h2_fail::append_subling(fails, fail_to);
      }
      h2_fail* fail_payload = h2_matcher_cast<const unsigned char*>(payload).matches((unsigned char*)p->data.data(), 0, false, false);
      if (fail_payload) {
         fail_payload->explain = "payload";
         h2_fail::append_subling(fails, fail_payload);
      }
      h2_fail* fail_size = h2_matcher_cast<const int>(size).matches(p->data.size(), 0, false, false);
      if (fail_size) {
         fail_size->explain = "payload length";
         h2_fail::append_subling(fails, fail_size);
      }

      if (fails) {
         h2_row r = "Outgoing packet unexpected Ptx(";
         r.printf("green", "%s", e).printf("", ")");
         h2_fail* fail = h2_fail::new_normal(r, file, line);
         h2_fail::append_child(fail, fails);
         h2_fail_g(fail, false);
      }
   }
};

#define __H2SOCK(Q) for (h2::h2_sock Q; Q;)
#define H2SOCK(...) __H2SOCK(H2PP_UNIQUE(t_sock))

#define Ptx(...) h2::h2_sock::check(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
#define Pij(_Packet, _Size, ...) h2::h2_sock::inject(_Packet, _Size, #__VA_ARGS__)
