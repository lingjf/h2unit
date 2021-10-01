
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
   h2_vector<h2_vector<int>> c, d, p;

   h2_LCS(h2_vector<h2_string> s1_, h2_vector<h2_string> s2_, bool caseless_ = false) : s1(s1_), s2(s2_), caseless(caseless_) {}

   void LCS_table()
   {
      for (size_t i = 0; i < s1.size() + 1; i++) {
         c.push_back(h2_vector<int>(s2.size() + 1));
         d.push_back(h2_vector<int>(s2.size() + 1));
         p.push_back(h2_vector<int>(s2.size() + 1));
      }

      for (size_t i = 0; i < s1.size() + 1; i++) c[i][0] = 0;
      for (size_t i = 0; i < s2.size() + 1; i++) c[0][i] = 0;

      for (size_t i = 1; i < s1.size() + 1; i++) {
         for (size_t j = 1; j < s2.size() + 1; j++) {
            if (s1[i - 1].equals(s2[j - 1], caseless)) {
               c[i][j] = c[i - 1][j - 1] + 1;
               d[i][j] = 1030; // 10:30 upper left
            } else if (c[i - 1][j] > c[i][j - 1]) {
               c[i][j] = c[i - 1][j];
               d[i][j] = 900; // 9:30 left
            } else {
               c[i][j] = c[i][j - 1];
               d[i][j] = 1200; // 12:00 upper
            }
         }
      }
   }

   void LCS_traceback(size_t i, size_t j)
   {
      if (i == 0 || j == 0) return;
      if (d[i][j] == 1030) { // 10:30
         p[i][j] = 1;
         LCS_traceback(i - 1, j - 1);
      } else if (d[i][j] == 900) { // 9:00
         LCS_traceback(i - 1, j);
      } else { // 1200 12:00
         LCS_traceback(i, j - 1);
      }
   }

   std::pair<h2_vector<int>, h2_vector<int>> lcs()
   {
      LCS_table();
      LCS_traceback(s1.size(), s2.size());

      h2_vector<int> l1(s1.size()), l2(s2.size());
      for (size_t i = 1; i < s1.size() + 1; i++) {
         l1[i - 1] = 0;
         for (size_t j = 1; j < s2.size() + 1; j++)
            if (p[i][j]) l1[i - 1] = 1;
      }
      for (size_t j = 1; j < s2.size() + 1; j++) {
         l2[j - 1] = 0;
         for (size_t i = 1; i < s1.size() + 1; i++)
            if (p[i][j]) l2[j - 1] = 1;
      }
      return {l1, l2};
   }
};
