
#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))

#define _h2_h2_list_for_each_safe(p, t, head) for (h2_list* p = (head)->next, *t = p->next; p != (head); p = t, t = t->next)
#define h2_list_for_each(p, head) _h2_h2_list_for_each_safe(p, H2Q(t), head)

#define _h2_list_for_each_entry_safe(p, t, head, type, link) for (type* p = h2_list_entry((head)->next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != (head); p = t, t = h2_list_entry(t->link.next, type, link))
#define h2_list_for_each_entry(p, head, type, link) _h2_list_for_each_entry_safe(p, H2Q(t), head, type, link)

#define h2_list_pop_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_first(), type, link))
#define h2_list_bottom_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_last(), type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* new_, h2_list* prev, h2_list* next) {
      next->prev = new_;
      new_->next = next;
      new_->prev = prev;
      prev->next = new_;
   }

   // void init() { next = prev = this; }

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list* entry) { __add_between(entry, this, this->next); }

   void add_tail(h2_list* entry) { __add_between(entry, this->prev, this); }

   void push(h2_list* entry) { add_head(entry); }

   h2_list* pop() { return empty() ? nullptr : next->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }

   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list* get_first() const { return empty() ? nullptr : next; }

   h2_list* get_last() const { return empty() ? nullptr : prev; }

   h2_list* out() {
      prev->next = next;
      next->prev = prev;
      next = prev = this;
      return this;
   }

   bool empty() const { return next == this; }
};
