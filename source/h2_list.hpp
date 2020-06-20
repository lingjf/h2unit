
#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))
#define h2_list_for_each_entry(p, head, type, link) \
   for (int li = 0; li == 0; ++li)                  \
      for (type* p = h2_list_entry((head).next, type, link), *_t = h2_list_entry(p->link.next, type, link); &p->link != &(head); p = _t, _t = h2_list_entry(_t->link.next, type, link), ++li)

#define h2_list_pop_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry(&(head).pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry((head).next, type, link))
#define h2_list_bottom_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry((head).prev, type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* thus, h2_list* prev, h2_list* next);

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list& entry) { __add_between(&entry, this, this->next); }
   void add_tail(h2_list& entry) { __add_between(&entry, this->prev, this); }
   void add_before(h2_list& entry) { __add_between(&entry, this->prev, this); }
   void add_after(h2_list& entry) { __add_between(&entry, this, this->next); }

   void push_back(h2_list& entry) { add_tail(entry); }
   void push(h2_list& entry) { add_head(entry); }
   h2_list& pop() { return next->out(); }
   h2_list& pop_back() { return prev->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }
   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list& out();

   bool empty() const { return next == this; }
   int count() const;
   void sort(std::function<int(h2_list* a, h2_list* b)> cmp);
};
