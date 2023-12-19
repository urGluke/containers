#include <gtest/gtest.h>

#include <stack>

#include "../s21_containers.h"
#ifdef ALPINE
#define CORRECTION_FOR_TEST 2
#else
#define CORRECTION_FOR_TEST 1
#endif

template <class T>
bool checkEq(s21::stack<T> a, std::stack<T> b) {
  if (a.size() != b.size()) return false;
  if (a.empty() != b.empty()) return false;
  if (a.empty()) return true;
  for (size_t i = 0; i < a.size(); i++) {
    if (a.top() != b.top()) return false;
    a.pop();
    b.pop();
  }
  return true;
}

TEST(Init, EmptyInt) {
  s21::stack<int> a;
  std::stack<int> b;
  ASSERT_EQ(true, checkEq(a, b));
}
TEST(Init, EmptyVec) {
  s21::stack<std::vector<int>> a;
  std::stack<std::vector<int>> b;
  ASSERT_EQ(true, checkEq(a, b));
}
TEST(Init, SeriasI) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});
  ASSERT_EQ(true, checkEq(a, b));
}
TEST(Init, SeriasD) {
  s21::stack<double> a({1, 2, 3, 4});
  std::stack<double> b({4, 3, 2, 1});
  ASSERT_EQ(false, checkEq(a, b));
}
TEST(Init, stackCopy) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});

  s21::stack<int> c(a);
  std::stack<int> d(b);
  ASSERT_EQ(true, checkEq(c, d));
}
TEST(Init, stackMove) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});

  s21::stack<int> c(std::move(a));
  std::stack<int> d(std::move(b));
  ASSERT_EQ(true, checkEq(c, d));
}
TEST(Operator, move) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});

  s21::stack<int> c = a;
  std::stack<int> d = b;
  ASSERT_EQ(true, checkEq(c, d));
}
TEST(Acsseor, top_eq) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});
  ASSERT_EQ(true, a.top() == b.top());
}
TEST(Acsseor, top_not) {
  s21::stack<double> a({1, 2, 3, 4});
  std::stack<double> b({4, 3, 2, 1});
  ASSERT_EQ(false, a.top() == b.top());
}
TEST(Acsseor, areEmpty) {
  s21::stack<int> a;
  std::stack<int> b;
  ASSERT_EQ(true, a.empty() == b.empty());
}
TEST(Acsseor, notEmpty) {
  s21::stack<double> a({1, 2, 3, 4});
  std::stack<double> b({4, 3, 2});
  ASSERT_EQ(true, a.empty() == b.empty());
}

TEST(size, eq) {
  s21::stack<double> a({1, 2, 3, 4});
  std::stack<double> b({4, 3, 2, 1});
  ASSERT_EQ(true, a.size() == b.size());
}
TEST(size, empty) {
  s21::stack<int> a;
  std::stack<int> b;
  ASSERT_EQ(true, a.size() == b.size());
}
TEST(size, notEq) {
  s21::stack<double> a({1, 2, 3, 4});
  std::stack<double> b({4, 3, 2});
  ASSERT_EQ(false, a.size() == b.size());
}

TEST(push, int) {
  s21::stack<int> a;
  std::stack<int> b;

  a.push(1);
  a.push(100);
  a.push(-5);

  b.push(1);
  b.push(100);
  b.push(-5);

  ASSERT_EQ(true, checkEq(a, b));
}

TEST(pop, int) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});

  a.pop();
  a.pop();
  a.pop();

  b.pop();
  b.pop();
  b.pop();

  ASSERT_EQ(true, checkEq(a, b));
}

TEST(swap, int) {
  s21::stack<int> a({1, 2, 3, 4});
  std::stack<int> b({1, 2, 3, 4});
  s21::stack<int> c({5, 6, 7, 8, 9, 10});
  std::stack<int> d({5, 6, 7, 8, 9, 10});
  a.swap(c);
  b.swap(d);

  ASSERT_EQ(true, checkEq(a, b));
  ASSERT_EQ(true, checkEq(c, d));
}

// int main(int argc, char **argv)
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
