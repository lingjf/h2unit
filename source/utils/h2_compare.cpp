h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
#if !defined _WIN32 || !defined NDEBUG  // Windows regex suck under release version and memory check
   h2_memory::hook(false);
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   h2_memory::hook();
#endif
   return result;
}

h2_inline bool h2_pattern::wildcard_match(const char* pattern, const char* subject, bool caseless)
{
#if defined _WIN32 || defined __CYGWIN__
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*pcur == '?') {
         ++scur;
         ++pcur;
      } else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      } else if (caseless ? ::tolower(*scur) == ::tolower(*pcur) : *scur == *pcur) {
         ++scur;
         ++pcur;
      } else if (pstar) {
         pcur = pstar + 1;
         scur = ++sstar;
      } else
         return false;
   }
   while (*pcur == '*') ++pcur;
   return !*pcur;
#else
   return !fnmatch(pattern, subject, caseless ? FNM_CASEFOLD : 0);
#endif
}

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}

struct h2_fuzzy {
   static int levenshtein(const char* s1, const char* s2, int n1, int n2)
   {
      h2_vector<unsigned int> col(n2 + 1), prevCol(n2 + 1);

      for (unsigned int i = 0; i < prevCol.size(); i++)
         prevCol[i] = i;
      for (unsigned int i = 0; i < n1; i++) {
         col[0] = i + 1;
         for (unsigned int j = 0; j < n2; j++)
            col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (s1[i] == s2[j] ? 0 : 1));
         col.swap(prevCol);
      }

      return prevCol[n2];
   }
   static int LCS(const char* s1, const char* s2, int n1, int n2)
   {
      return 1;
   }
   static double jaro(const char* s1, const char* s2, int n1, int n2)
   {
      if (n1 == 0) return n2 == 0 ? 1.0 : 0.0;

      // max distance between two chars to be considered matching floor() is ommitted due to integer division rules
      int match_distance = std::max(n1, n2) / 2 - 1;

      // arrays of bools that signify if that char in the matcing string has a match
      int* s1_matches = (int*)calloc(n1, sizeof(int));
      int* s2_matches = (int*)calloc(n2, sizeof(int));

      // number of matches and transpositions
      double matches = 0.0;
      double transpositions = 0.0;

      // find the matches
      for (int i = 0; i < n1; i++) {
         // start and end take into account the match distance
         int start = std::max(0, i - match_distance);
         int end = std::min(i + match_distance + 1, n2);

         for (int k = start; k < end; k++) {
            // if s2 already has a match continue
            if (s2_matches[k]) continue;
            // if s1 and s2 are not
            if (::tolower(s1[i]) != ::tolower(s2[i])) continue;
            // otherwise assume there is a match
            s1_matches[i] = 1;
            s2_matches[k] = 1;
            matches++;
            break;
         }
      }

      // if there are no matches return 0
      if (matches == 0) {
         free(s1_matches);
         free(s2_matches);
         return 0.0;
      }

      // count transpositions
      int k = 0;
      for (int i = 0; i < n1; i++) {
         // if there are no matches in s1 continue
         if (!s1_matches[i]) continue;
         // while there is no match in s2 increment k
         while (!s2_matches[k]) k++;
         // increment transpositions
         if (::tolower(s1[i]) != ::tolower(s2[k])) transpositions++;
         k++;
      }

      // divide the number of transpositions by two as per the algorithm specs this division is valid because the counted transpositions include both instances of the transposed characters.
      transpositions /= 2.0;

      free(s1_matches);
      free(s2_matches);

      return ((matches / n1) + (matches / n2) + ((matches - transpositions) / matches)) / 3.0;
   }

   static double jaro_winkler(const char* s1, const char* s2, int n1, int n2)
   {
      double distance = jaro(s1, s2, n1, n2);

      // finds the number of common terms in the first 3 strings, max 3.
      int prefix_length = 0;
      if (n1 != 0 && n2 != 0)
         while (prefix_length < 3 && ::tolower(*s1++) == ::tolower(*s2++)) prefix_length++;

      // 0.1 is the default scaling factor
      return distance + prefix_length * 0.1 * (1 - distance);
   }

   static double likes(const char* s1, const char* s2, bool caseless = false)
   {
      const size_t n1 = strlen(s1), n2 = strlen(s2);
      if (caseless ? strcasecmp(s1, s2) == 0 : strcmp(s1, s2) == 0) return 1;
      return jaro_winkler(s1, s2, n1, n2);
   }
};
