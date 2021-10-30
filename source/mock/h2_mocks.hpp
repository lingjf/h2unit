struct h2_mocks {
   static bool add(h2_list& mocks, void* mocker);
   static h2_fail* clear(h2_list& mocks, bool check);
};
