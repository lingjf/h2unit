
#define __H2OK(Q, expression, ...) \
   for (h2::h2_defer_failure Q("", "", expression, __FILE__, __LINE__); Q;) h2::h2_OK(&Q, __VA_ARGS__)

#define __H2JE3(Q, expect, actual) \
   for (h2::h2_defer_failure Q(#expect, #actual, "", __FILE__, __LINE__); Q;) h2::h2_JE(&Q, expect, actual, "")

#define __H2JE4(Q, expect, actual, selector) \
   for (h2::h2_defer_failure Q(#expect, #actual, "", __FILE__, __LINE__); Q;) h2::h2_JE(&Q, expect, actual, selector)

#define H2OK(...) __H2OK(H2PP_UNIQUE(t_defer_failure), (#__VA_ARGS__), __VA_ARGS__)

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2PP_UNIQUE(t_defer_failure), __VA_ARGS__)

/* clang-format off */
using h2::_;
using h2::Any;
using h2::IsNull;
using h2::NotNull;
using h2::IsTrue;
using h2::IsFalse;
using h2::Eq;
using h2::Nq;
using h2::Ge;
using h2::Gt;
using h2::Le;
using h2::Lt;
using h2::Me;
using h2::M1e;
using h2::M8e;
using h2::M16e;
using h2::M32e;
using h2::M64e;
using h2::Re;
using h2::We;
using h2::Je;
using h2::Se;
using h2::Substr;
using h2::StartsWith;
using h2::EndsWith;
using h2::CaseLess;
#ifndef _WIN32
using h2::operator~;
#endif
using h2::Pointee;
using h2::Not;
using h2::operator!;
using h2::operator&&;
using h2::operator||;
using h2::AllOf;
using h2::AnyOf;
using h2::NoneOf;
using h2::ListOf;
using h2::CountOf;
using h2::Have;
using h2::Has;
using h2::In;
using h2::Pair;
