
struct h2_fuzzy {
   static size_t levenshtein(const char* s1, const char* s2, size_t n1, size_t n2, bool caseless)
   {
      h2_vector<size_t> col(n2 + 1), prevCol(n2 + 1);
      for (h2_vector<size_t>::size_type i = 0; i < prevCol.size(); i++)
         prevCol[i] = (int)i;
      for (size_t i = 0; i < n1; i++) {
         col[0] = i + 1;
         for (size_t j = 0; j < n2; j++) {
            bool eq = caseless ? ::tolower(s1[i]) == ::tolower(s2[j]) : s1[i] == s2[j];
            col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (eq ? 0 : 1));
         }
         col.swap(prevCol);
      }
      return prevCol[n2];
   }

   // 1 absolute match, 0 absolute not match
   static double similarity(const char* s1, const char* s2, bool caseless)
   {
      size_t n1 = strlen(s1), n2 = strlen(s2);
      if (!n1 && !n2) return 1;
      return 1.0 - levenshtein(s1, s2, n1, n2, caseless) / ((n1 + n2) * 0.5);
   }
};

// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
struct h2_LCS {
   h2_vector<h2_string> s1, s2;
   bool caseless;
   struct matrix {
      unsigned e : 1, p : 1, d : 6, c : 24;
      matrix() : e(0), p(0), d(0), c(0) {}
   };
   h2_vector<h2_vector<matrix>> m;

   h2_LCS(h2_vector<h2_string> s1_, h2_vector<h2_string> s2_, bool caseless_ = false) : s1(s1_), s2(s2_), caseless(caseless_) {}

   void LCS_cacluate()
   {
      for (size_t i = 1; i < s1.size() + 1; i++) {
         for (size_t j = 1; j < s2.size() + 1; j++) {
            if (s1[i - 1].equals(s2[j - 1], caseless)) {
               m[i][j].c = m[i - 1][j - 1].c + 1;
               m[i][j].e = 1;
            } else {
               if (m[i - 1][j].c > m[i][j - 1].c) {
                  m[i][j].c = m[i - 1][j].c;
               } else {
                  m[i][j].c = m[i][j - 1].c;
               }
               m[i][j].e = 0;
            }
         }
      }
   }

   void LCS_traceback(size_t i, size_t j)
   {
      if (i == 0 || j == 0) return;

      if (m[i][j].e || (m[i - 1][j - 1].c >= m[i - 1][j].c && m[i - 1][j - 1].c >= m[i][j - 1].c)) {
         m[i][j].d = 10;  // 10:30 upper left
      } else if (m[i - 1][j].c > m[i][j - 1].c) {
         m[i][j].d = 12;  // 12:00 upper
      } else {
         m[i][j].d = 9;  // 9:00 left
      }

      if (m[i][j].d == 10) {  // 10:30
         m[i][j].p = m[i][j].e;
         LCS_traceback(i - 1, j - 1); // i--, j--
      } else if (m[i][j].d == 12) {  // 12:00 upper i--
         LCS_traceback(i - 1, j);
      } else {  // 9:00 left j--
         LCS_traceback(i, j - 1);
      }
   }

   std::pair<h2_vector<unsigned char>, h2_vector<unsigned char>> lcs()
   {
      for (size_t i = 0; i < s1.size() + 1; i++) m.push_back(h2_vector<matrix>(s2.size() + 1));
      LCS_cacluate();
      LCS_traceback(s1.size(), s2.size());

      h2_vector<unsigned char> l1(s1.size()), l2(s2.size());
      for (size_t i = 1; i < s1.size() + 1; i++) {
         l1[i - 1] = 0;
         for (size_t j = 1; j < s2.size() + 1; j++)
            if (m[i][j].p) l1[i - 1] = 1;
      }
      for (size_t j = 1; j < s2.size() + 1; j++) {
         l2[j - 1] = 0;
         for (size_t i = 1; i < s1.size() + 1; i++)
            if (m[i][j].p) l2[j - 1] = 1;
      }
      return {l1, l2};
   }
};
