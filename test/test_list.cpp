#include "../source/h2_unit.cpp"

struct list_host {
   int a;
   float b;
   h2::h2_list link;
   char c[343];
};

SUITE(list)
{
   int u = 0;
   h2::h2_list root;
   h2::h2_list root123;  // root <-> h1 <-> h2 <-> h3

   list_host h1, h2, h3;

   h1.a = 1;
   h2.a = 2;
   h3.a = 3;

   root123.add_tail(&h1.link);
   root123.add_tail(&h2.link);
   root123.add_tail(&h3.link);

   int e123[] = {1, 2, 3};
   int e321[] = {3, 2, 1};

   Case(empty)
   {
      OK(root.empty());
      OK(!root123.empty());
   }

   Case(add_head)
   {
      root.add_head(&h1.link);
      root.add_head(&h2.link);
      root.add_head(&h3.link);

      h2_list_for_each_entry (p, &root, list_host, link) {
         OK(e321[u++], p->a);
      }
   }

   Case(add_tail)
   {
      root.add_tail(&h1.link);
      root.add_tail(&h2.link);
      root.add_tail(&h3.link);

      h2_list_for_each_entry (p, &root, list_host, link) {
         OK(e123[u++], p->a);
      }
   }

   Case(push and pop)
   {
      root.push(&h1.link);
      root.push(&h2.link);
      root.push(&h3.link);

      h2_list_for_each_entry (p, &root, list_host, link) {
         OK(e321[u++], p->a);
      }

      h2::h2_list* p3 = root.pop();
      OK(&h3.link, p3);
      h2::h2_list* p2 = root.pop();
      OK(&h2.link, p2);
      h2::h2_list* p1 = root.pop();
      OK(&h1.link, p1);
      h2::h2_list* p0 = root.pop();
      OK(IsNull, p0);
   }

   Case(is_first and is_last)
   {
      OK(root123.is_first(&h1.link));
      OK(root123.is_last(&h3.link));
   }

   Case(get_first and get_last)
   {
      OK(&h1.link, root123.get_first());
      OK(&h3.link, root123.get_last());
   }

   Case(out)
   {
      h2_list_for_each_entry (p, &root123, list_host, link) {
         p->link.out();
         OK(e123[u++], p->a);
      }
   }
}

CASE(list sort)
{
   h2::h2_list root;

   list_host h1, h2, h3;

   h1.a = 1;
   h2.a = 2;
   h3.a = 3;

   root.add_tail(&h2.link);
   root.add_tail(&h1.link);
   root.add_tail(&h3.link);

   root.sort([](h2::h2_list* a, h2::h2_list* b) {
      list_host* a_ = h2_list_entry(a, list_host, link);
      list_host* b_ = h2_list_entry(b, list_host, link);
      return a_->a - b_->a;
   });

   h2_list_for_each_entry (p, &root, list_host, link) {
      OK(i + 1, p->a);
   }
}