template <typename T>
struct h2_array {
   typedef typename std::decay<T>::type value_type;
   typedef value_type* pointer;
   typedef const value_type* const_pointer;
   typedef value_type& reference;
   typedef const value_type& const_reference;
   typedef value_type* iterator;
   typedef const value_type* const_iterator;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef std::reverse_iterator<iterator> reverse_iterator;
   typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

   value_type* elems;
   size_t elemc;
   h2_array(const value_type* elems_, const size_t elemc_) : elems((value_type*)elems_), elemc(elemc_) {}

   iterator begin() noexcept { return iterator(data()); }
   const_iterator begin() const noexcept { return const_iterator(data()); }
   iterator end() noexcept { return iterator(data() + elemc); }
   const_iterator end() const noexcept { return const_iterator(data() + elemc); }
   reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
   const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
   reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
   const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
   const_iterator cbegin() const noexcept { return const_iterator(data()); }
   const_iterator cend() const noexcept { return const_iterator(data() + elemc); }
   const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
   const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

   constexpr size_type size() const noexcept { return elemc; }
   constexpr size_type max_size() const noexcept { return elemc; }
   constexpr bool empty() const noexcept { return size() == 0; }

   reference operator[](size_type __n) noexcept { return elems[__n]; }
   constexpr const_reference operator[](size_type __n) const noexcept { return elems[__n]; }
   reference at(size_type __n) { return elems[__n]; }
   constexpr const_reference at(size_type __n) const { return elems[__n]; }
   reference front() noexcept { return *begin(); }
   constexpr const_reference front() const noexcept { return elems[0]; }
   reference back() noexcept { return elemc ? *(end() - 1) : *end(); }
   constexpr const_reference back() const noexcept { return elemc ? elems[elemc - 1] : elems[0]; }

   pointer data() noexcept { return elems; }
   const_pointer data() const noexcept { return elems; }
};
