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

   value_type* elems;
   std::size_t count;
   h2_array(const value_type* elems_, const std::size_t count_) : elems((value_type*)elems_), count(count_) {}

   iterator begin() noexcept { return iterator(data()); }
   const_iterator begin() const noexcept { return const_iterator(data()); }
   iterator end() noexcept { return iterator(data() + count); }
   const_iterator end() const noexcept { return const_iterator(data() + count); }
   const_iterator cbegin() const noexcept { return const_iterator(data()); }
   const_iterator cend() const noexcept { return const_iterator(data() + count); }

   constexpr size_type size() const noexcept { return count; }
   constexpr size_type max_size() const noexcept { return count; }
   constexpr bool empty() const noexcept { return size() == 0; }

   reference operator[](size_type __n) noexcept { return elems[__n]; }
   constexpr const_reference operator[](size_type __n) const noexcept { return elems[__n]; }
   reference at(size_type __n) { return elems[__n]; }
   constexpr const_reference at(size_type __n) const { return elems[__n]; }
   reference front() noexcept { return *begin(); }
   constexpr const_reference front() const noexcept { return elems[0]; }
   reference back() noexcept { return count ? *(end() - 1) : *end(); }
   constexpr const_reference back() const noexcept { return count ? elems[count - 1] : elems[0]; }

   pointer data() noexcept { return elems; }
   const_pointer data() const noexcept { return elems; }
};
