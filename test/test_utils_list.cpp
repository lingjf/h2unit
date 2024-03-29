#include "../source/h2_unit.cpp"

struct list_host {
   int a;
   float b;
   h2::h2_list link;
   const char* c;
};

SUITE(list)
{
   h2::h2_list root;
   h2::h2_list root123;  // root <-> h1 <-> h2 <-> h3

   list_host h1, h2, h3;

   h1.a = 1;
   h2.a = 2;
   h3.a = 3;

   root123.add_tail(h1.link);
   root123.add_tail(h2.link);
   root123.add_tail(h3.link);

   int e123[] = {1, 2, 3};
   int e321[] = {3, 2, 1};

   Case(empty)
   {
      OK(root.empty());
      OK(!root123.empty());
   }

   Case(add_head)
   {
      root.add_head(h1.link);
      root.add_head(h2.link);
      root.add_head(h3.link);

      int i = 0;
      h2_list_for_each_entry (p, root, list_host, link) {
         OK(e321[i], p->a);
         i++;
      }
   }

   Case(add_tail)
   {
      root.add_tail(h1.link);
      root.add_tail(h2.link);
      root.add_tail(h3.link);

      h2_list_for_each_entry (p, i, root, list_host, link) {
         OK(e123[i], p->a);
      }
   }

   Case(add_before)
   {
      root.add_before(h1.link);
      root.add_before(h2.link);
      root.add_before(h3.link);

      h2_list_for_each_entry (p, i, root, list_host, link) {
         OK(e123[i], p->a);
      }
   }

   Case(add_after)
   {
      root.add_after(h1.link);
      root.add_after(h2.link);
      root.add_after(h3.link);

      int i = 0;
      h2_list_for_each_entry (p, root, list_host, link) {
         OK(e321[i], p->a);
         i++;
      }
   }

   Case(push and pop)
   {
      root.push(h1.link);
      root.push(h2.link);
      root.push(h3.link);

      h2_list_for_each_entry (p, i, root, list_host, link) {
         OK(e321[i], p->a);
      }

      h2::h2_list& p3 = root.pop();
      OK(&h3.link, &p3);
      h2::h2_list& p2 = root.pop();
      OK(&h2.link, &p2);
      h2::h2_list& p1 = root.pop();
      OK(&h1.link, &p1);
   }

   Case(push back and pop back)
   {
      root.push_back(h1.link);
      root.push_back(h2.link);
      root.push_back(h3.link);

      h2_list_for_each_entry (p, i, root, list_host, link) {
         OK(e123[i], p->a);
      }

      h2::h2_list& p3 = root.pop_back();
      OK(&h3.link, &p3);
      h2::h2_list& p2 = root.pop_back();
      OK(&h2.link, &p2);
      h2::h2_list& p1 = root.pop_back();
      OK(&h1.link, &p1);
   }

   Case(is_first and is_last)
   {
      OK(root123.is_first(&h1.link));
      OK(root123.is_last(&h3.link));
   }

   Case(out)
   {
      h2_list_for_each_entry (p, i, root123, list_host, link) {
         p->link.out();
         OK(e123[i], p->a);
      }
   }

   Case(count)
   {
      OK(3, root123.count());
      OK(0, root.count());
   }

   Case(reverse)
   {
      int i = 0;
      h2_list_for_each_reverse_entry (p, root123, list_host, link) {
         OK(e321[i], p->a);
         ++i;
      }
   }

   Case(pop entry)
   {
      root.push(h1.link);
      root.push(h2.link);
      root.push(h3.link);

      list_host* a1 = h2_list_pop_entry(root, list_host, link);
      OK((a1 == &h3));
      list_host* a2 = h2_list_pop_entry(root, list_host, link);
      OK((a2 == &h2));
      list_host* a3 = h2_list_pop_entry(root, list_host, link);
      OK((a3 == &h1));
   }

   Case(top entry and bottom entry)
   {
      root.push(h1.link);
      root.push(h2.link);
      root.push(h3.link);

      list_host* a1 = h2_list_top_entry(root, list_host, link);
      OK((a1 == &h3));
      list_host* a3 = h2_list_bottom_entry(root, list_host, link);
      OK((a3 == &h1));
   }
}

static int list_host_cmp_by_a(h2::h2_list* a, h2::h2_list* b)
{
   list_host* a_ = h2_list_entry(a, list_host, link);
   list_host* b_ = h2_list_entry(b, list_host, link);
   return a_->a - b_->a;
}

static int list_host_cmp_by_c(h2::h2_list* a, h2::h2_list* b)
{
   list_host* a_ = h2_list_entry(a, list_host, link);
   list_host* b_ = h2_list_entry(b, list_host, link);
   return strcmp(a_->c, b_->c) * -1;
}

CASE(list sort)
{
   h2::h2_list root;

   list_host h1, h2, h3;

   h1.a = 1;
   h2.a = 2;
   h3.a = 3;

   h1.c = "c";
   h2.c = "b";
   h3.c = "a";

   root.add_tail(h2.link);
   root.add_tail(h3.link);
   root.add_tail(h1.link);

   root.sort(list_host_cmp_by_a);
   h2_list_for_each_entry (p, i, root, list_host, link) {
      OK(i + 1, p->a);
   }

   root.sort(list_host_cmp_by_c);
   h2_list_for_each_entry (p, i, root, list_host, link) {
      OK(i + 1, p->a);
   }
}
