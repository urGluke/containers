#include <gtest/gtest.h>

#include <set>

#include "../s21_containersplus.h"

TEST(MultisetTest, ConstructorAndInsert) {
  S21::multiset<int> ms;
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(int(ms.size()), 0);

  ms.insert(5);
  ms.insert(3);
  EXPECT_FALSE(ms.empty());
  EXPECT_EQ(int(ms.size()), 2);
  EXPECT_EQ(int(ms.count(5)), 1);
}

TEST(MultisetTest, CopyConstructor_Empty) {
  S21::multiset<int> original;
  S21::multiset<int> copy(original);
  EXPECT_TRUE(copy.empty());
}

TEST(MultisetTest, CopyConstructor_WithElements) {
  S21::multiset<int> original;
  original.insert(5);
  original.insert(5);
  original.insert(3);
  original.insert(7);

  S21::multiset<int> copy(original);
  EXPECT_EQ(copy.size(), size_t(4));
  EXPECT_EQ(*copy.find(5), 5);
  EXPECT_EQ(copy.count(5), size_t(2));
  EXPECT_EQ(copy.count(3), size_t(1));
  EXPECT_EQ(copy.count(7), size_t(1));
}

TEST(MultisetTest, InsertDuplicates) {
  S21::multiset<int> ms;
  auto it1 = ms.insert(3);
  auto it2 = ms.insert(3);

  // Проверяем размер multiset после вставки двух одинаковых элементов
  EXPECT_EQ(int(ms.size()), 2);

  // Проверяем, что вставленные элементы действительно присутствуют в multiset
  EXPECT_EQ(*it1, 3);
  EXPECT_EQ(*it2, 3);

  // Проверяем, что элементы не указывают на один и тот же узел
  EXPECT_NE(it1, it2);

  // Проверяем количество элементов со значением 3
  EXPECT_EQ(int(ms.count(3)), 2);
}

TEST(MultisetTest, InsertMany) {
  S21::multiset<int> ms;
  auto results = ms.insert_many(3, 5, 3, 7);
  EXPECT_EQ(int(results.size()), 4);
  EXPECT_EQ(int(ms.size()), 4);
  EXPECT_EQ(int(ms.count(3)), 2);
}

TEST(MultisetTest, FindAndErase) {
  S21::multiset<int> ms;
  ms.insert(5);
  ms.insert(3);

  auto it = ms.find(3);
  EXPECT_NE(it, ms.end());
  ms.erase(it);
  EXPECT_EQ(int(ms.size()), 1);
  EXPECT_EQ(int(ms.count(3)), 0);
}

TEST(MultisetTest, LowerAndUpperBound) {
  S21::multiset<int> ms;
  ms.insert(5);
  ms.insert(3);
  ms.insert(3);

  auto lower = ms.lower_bound(3);
  auto upper = ms.upper_bound(3);
  EXPECT_EQ(*lower, 3);
  EXPECT_EQ(*upper, 5);
}

TEST(MultisetTest, ClearAndSwap) {
  S21::multiset<int> ms1;
  S21::multiset<int> ms2;

  ms1.insert(3);
  ms2.insert(5);
  ms2.insert(7);

  ms1.swap(ms2);
  EXPECT_EQ(int(ms1.size()), 2);
  EXPECT_EQ(int(ms2.size()), 1);

  ms1.clear();
  EXPECT_TRUE(ms1.empty());
  EXPECT_EQ(int(ms1.size()), 0);
}

TEST(MultisetTest, Iterators) {
  S21::multiset<int> ms;
  ms.insert(5);
  ms.insert(3);

  auto it = ms.begin();
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 5);
  EXPECT_EQ(++it, ms.end());
}

TEST(MultisetTest, InitializerListConstructor_Empty) {
  S21::multiset<int> ms{};
  EXPECT_EQ(int(ms.size()), 0);
}

TEST(MultisetTest, InitializerListConstructor_MultipleElements) {
  S21::multiset<int> ms{5, 3, 7, 3};
  EXPECT_EQ(int(ms.size()), 4);
  EXPECT_EQ(int(ms.count(3)), 2);
  EXPECT_EQ(int(ms.count(5)), 1);
  EXPECT_EQ(int(ms.count(7)), 1);
}

TEST(MultisetTest, MoveConstructor_Empty) {
  S21::multiset<int> ms1{};
  S21::multiset<int> ms2{std::move(ms1)};
  EXPECT_EQ(int(ms2.size()), 0);
  EXPECT_EQ(int(ms1.size()), 0);  // После перемещения состояние ms1
                                  // неопределено, но size() должен быть 0
}

TEST(MultisetTest, MoveConstructor_MultipleElements) {
  S21::multiset<int> ms1{5, 3, 7, 3};
  S21::multiset<int> ms2{std::move(ms1)};
  EXPECT_EQ(int(ms2.size()), 4);
  EXPECT_EQ(int(ms2.count(3)), 2);
  EXPECT_EQ(int(ms2.count(5)), 1);
  EXPECT_EQ(int(ms2.count(7)), 1);
  EXPECT_EQ(int(ms1.size()), 0);  // После перемещения состояние ms1
                                  // неопределено, но size() должен быть 0
}

TEST(MultisetTest, MoveAssignmentOperator_Empty) {
  S21::multiset<int> ms1{};
  S21::multiset<int> ms2;
  ms2 = std::move(ms1);
  EXPECT_EQ(int(ms2.size()), 0);
  EXPECT_EQ(int(ms1.size()), 0);  // После перемещения состояние ms1
                                  // неопределено, но size() должен быть 0
}

TEST(MultisetTest, MoveAssignmentOperator_MultipleElements) {
  S21::multiset<int> ms1{5, 3, 7, 3};
  S21::multiset<int> ms2;
  ms2 = std::move(ms1);
  EXPECT_EQ(int(ms2.size()), 4);
  EXPECT_EQ(int(ms2.count(3)), 2);
  EXPECT_EQ(int(ms2.count(5)), 1);
  EXPECT_EQ(int(ms2.count(7)), 1);
  EXPECT_EQ(int(ms1.size()), 0);  // После перемещения состояние ms1
                                  // неопределено, но size() должен быть 0
}

TEST(MultisetTest, MaxSize_Empty) {
  S21::multiset<int> ms;
  // max_size() должен возвращать максимально допустимое количество элементов в
  // контейнере, это значение может зависеть от системы и компилятора
  EXPECT_GT(ms.max_size(), size_t(0));
}

TEST(MultisetTest, MaxSize_WithElements) {
  S21::multiset<int> ms{5, 3, 7, 3};
  // max_size() должен возвращать то же значение, независимо от количества
  // элементов в контейнере
  EXPECT_GT(ms.max_size(), size_t(4));
}

TEST(MultisetTest, Merge_EmptySets) {
  S21::multiset<int> ms1;
  S21::multiset<int> ms2;
  ms1.merge(ms2);
  EXPECT_EQ(ms1.size(), size_t(0));
  EXPECT_EQ(ms2.size(), size_t(0));
}

TEST(MultisetTest, Merge_WithElements) {
  S21::multiset<int> ms1{1, 2, 3};
  S21::multiset<int> ms2{3, 4, 5};
  ms1.merge(ms2);
  EXPECT_EQ(ms1.size(), size_t(6));
  EXPECT_EQ(ms1.count(3), size_t(2));
  EXPECT_EQ(ms1.count(4), size_t(1));
  EXPECT_EQ(ms1.count(5), size_t(1));
}

TEST(MultisetTest, Contains_ExistingElement) {
  S21::multiset<int> ms;
  ms.insert(5);
  ms.insert(3);
  EXPECT_TRUE(ms.contains(5));
  EXPECT_TRUE(ms.contains(3));
}

TEST(MultisetTest, Contains_NonExistingElement) {
  S21::multiset<int> ms;
  ms.insert(5);
  EXPECT_FALSE(ms.contains(7));
}

TEST(MultisetTest, EqualRange_Empty) {
  S21::multiset<int> ms;
  auto range = ms.equal_range(5);
  EXPECT_EQ(range.first, ms.end());
  EXPECT_EQ(range.second, ms.end());
}

TEST(MultisetTest, EqualRange_WithElements) {
  S21::multiset<int> ms;
  ms.insert(5);
  ms.insert(5);
  ms.insert(3);
  ms.insert(7);

  auto range = ms.equal_range(5);
  int count = 0;
  for (auto it = range.first; it != range.second; ++it) {
    EXPECT_EQ(*it, 5);
    ++count;
  }
  EXPECT_EQ(count, 2);
}
