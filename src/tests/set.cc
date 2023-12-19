#include <gtest/gtest.h>

#include <set>

#include "../s21_containers.h"

TEST(SetTest, DefaultConstructor) {
  S21::Set<int> S21_set;
  std::set<int> std_set;
  EXPECT_TRUE(S21_set.empty());
  EXPECT_EQ(S21_set.size(), std_set.size());
}

TEST(SetTest, InitializerListConstructor) {
  S21::Set<int> S21_set = {1, 2, 3};
  std::set<int> std_set = {1, 2, 3};
  EXPECT_EQ(S21_set.size(), std_set.size());
}

TEST(SetTest, CopyConstructor) {
  S21::Set<int> original = {4, 5, 6};
  S21::Set<int> copy(original);
  EXPECT_EQ(copy.size(), original.size());
}

TEST(SetTest, MoveConstructor) {
  S21::Set<int> original = {7, 8, 9};
  S21::Set<int> moved(std::move(original));
  EXPECT_EQ(int(moved.size()), 3);
  EXPECT_TRUE(original.empty());
}

TEST(SetTest, InsertMethod) {
  S21::Set<int> S21_set;
  std::set<int> std_set;
  auto res1 = S21_set.insert(10);
  auto res2 = std_set.insert(10);
  EXPECT_EQ(res1.second, res2.second);
  EXPECT_EQ(S21_set.size(), std_set.size());
}

TEST(SetTest, EraseMethod) {
  S21::Set<int> S21_set = {11, 12, 13};
  auto it = S21_set.begin();
  S21_set.erase(it);
  EXPECT_EQ(int(S21_set.size()), 2);
}

TEST(SetTest, SwapMethod) {
  S21::Set<int> set1 = {14, 15};
  S21::Set<int> set2 = {16, 17, 18};
  set1.swap(set2);
  EXPECT_EQ(int(set1.size()), 3);
  EXPECT_EQ(int(set2.size()), 2);
}

TEST(SetTest, MergeMethod) {
  S21::Set<int> set1 = {19, 20};
  S21::Set<int> set2 = {21, 22};
  set1.merge(set2);
  EXPECT_EQ(int(set1.size()), 4);
  EXPECT_TRUE(set2.empty());
}

TEST(SetTest, FindMethod) {
  S21::Set<int> S21_set = {23, 24, 25};
  auto it = S21_set.find(24);
  EXPECT_EQ(*it, 24);
}

TEST(SetTest, ContainsMethod) {
  S21::Set<int> S21_set = {26, 27, 28};
  EXPECT_TRUE(S21_set.contains(27));
  EXPECT_FALSE(S21_set.contains(29));
}

TEST(SetTest, InsertionFailure) {
  S21::Set<int> my_set;
  my_set.insert(5);
  auto result = my_set.insert(5);
  EXPECT_FALSE(result.second);
}

TEST(SetTest, MoveAssignmentOperator_Empty) {
  S21::Set<int> original;
  S21::Set<int> toMove;
  original = std::move(toMove);
  EXPECT_TRUE(original.empty());
  EXPECT_TRUE(toMove.empty());
}

TEST(SetTest, MoveAssignmentOperator) {
  S21::Set<int> set1;
  set1.insert(1);
  set1.insert(2);
  set1.insert(3);

  S21::Set<int> set2;
  set2.insert(4);
  set2.insert(5);

  set2 = std::move(set1);

  EXPECT_EQ(int(set2.size()), 3);
  EXPECT_NE(set2.find(1), set2.end());
  EXPECT_NE(set2.find(2), set2.end());
  EXPECT_NE(set2.find(3), set2.end());

  EXPECT_TRUE(set1.empty() || set1.size() == 0);
}
