struct h2_mocks {
   h2_list mocks;
   bool add(void* mock);
   h2_fail* clear(bool check);
};
