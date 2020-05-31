
h2_inline void h2_list::__add_between(h2_list* _new, h2_list* prev, h2_list* next)
{
   next->prev = _new;
   _new->next = next;
   _new->prev = prev;
   prev->next = _new;
}

h2_inline h2_list* h2_list::out()
{
   prev->next = next;
   next->prev = prev;
   next = prev = this;
   return this;
}

h2_inline int h2_list::count() const
{
   int c = 0;
   for (auto p = next; p != this; p = p->next) ++c;
   return c;
}

h2_inline void h2_list::sort(std::function<int(h2_list* a, h2_list* b)> cmp)
{
   h2_list sorted;
   h2_list *p, *q;

   while ((p = pop())) {
      for (q = sorted.next; q != &sorted; q = q->next) {
         if (0 < cmp(q, p)) {
            q->add_before(p);
            break;
         }
      }
      if (q == &sorted) sorted.add_tail(p);
   }
   while ((p = sorted.pop())) push_back(p);
}
