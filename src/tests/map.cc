#include <gtest/gtest.h>

#include <map>

#include "../s21_containers.h"

TEST(MapTest, DefaltConstructor) {
  s21::map<int, int> s21_map;
  std::map<int, int> std_map;
  EXPECT_TRUE(s21_map.empty());
  EXPECT_EQ(s21_map.size(), std_map.size());
}

TEST(MapTest, InitializerListConstructor) {
  s21::map<int, int> s21_map = {{1, 0}, {2, 1}, {3, 2}};
  std::map<int, int> std_map = {{1, 0}, {2, 1}, {3, 2}};
  EXPECT_EQ(s21_map.size(), std_map.size());
}

TEST(MapTest, CopyConsrtuctor) {
  s21::map<int, int> original = {{-1, 0}, {2, 1}, {3, 2}};
  s21::map<int, int> copy(original);
  EXPECT_EQ(copy.size(), original.size());
}

TEST(MapTest, MoveConstructor) {
  s21::map<int, int> original = {{7, 0}, {2, 1}, {-2, 2}};
  s21::map<int, int> moved(std::move(original));
  EXPECT_EQ(int(moved.size()), 3);
  EXPECT_TRUE(original.empty());
}

TEST(MapTest, EraseMethod) {
  s21::map<int, int> s21_map = {{1, 0}, {2, 1}, {3, 2}};
  auto it = s21_map.begin();
  s21_map.erase(it);
  EXPECT_EQ(int(s21_map.size()), 2);
}

TEST(MapTest, SwapMethod) {
  s21::map<int, int> map1 = {{1, 0}, {2, 1}, {3, 2}};
  s21::map<int, int> map2 = {{1, 20}, {21, 1}};
  map1.swap(map2);
  EXPECT_EQ(int(map1.size()), 2);
  EXPECT_EQ(int(map2.size()), 3);
}

TEST(MapTest, FindMethod) {
  s21::map<int, int> s21_map = {{1, 0}, {2, 1}, {3, 2}};
  auto it = s21_map.find(3);
  EXPECT_EQ(it->second, 2);
}

TEST(MapTest, InsertionFailurePair) {
  std::pair<int, int> p(1, 1);
  s21::map<int, int> map1;
  map1.insert(p);
  auto result = map1.insert(1, 1);
  EXPECT_FALSE(result.second);
}

TEST(MapTest, InsertionFailureKeyValue) {
  s21::map<int, int> map1;
  map1.insert(1, 1);
  auto result = map1.insert(1, 1);
  EXPECT_FALSE(result.second);
}

TEST(MapTest, MoveAssignmentOperator) {
  s21::map<int, int> map1 = {{1, 0}, {2, 1}, {3, 2}};
  s21::map<int, int> map2 = map1;
  EXPECT_EQ(int(map2.size()), map1.size());
}

TEST(MapTest, MergeMethod) {
  s21::map<int, int> s21_map1 = {{1, 0}, {2, 1}, {3, 2}};
  s21::map<int, int> s21_map2 = {{4, 0}, {26, 100}};
  s21_map1.merge(s21_map2);
  auto it = s21_map1.find(26);
  EXPECT_EQ(it->second, 100);
  EXPECT_EQ(s21_map1.size(), 5);
}

TEST(MapTest, InsertionSuccessPair) {
  std::pair<int, int> p(1, 1);
  s21::map<int, int> map1;
  auto result = map1.insert(p);
  EXPECT_TRUE(result.second);
}

TEST(MapTest, InsertionSuccessKeyValue) {
  s21::map<int, int> map1;
  auto result = map1.insert(1, 1);
  EXPECT_TRUE(result.second);
}

TEST(MapTest, MaxSizeEmpty) {
  s21::map<int, int> s21_map;
  std::map<int, int> std_map;
  EXPECT_GT(s21_map.max_size(), std_map.max_size());
}

TEST(MapTest, MaxSize) {
  s21::map<int, int> s21_map = {{1, 2}, {2, 3}};
  std::map<int, int> std_map = {{1, 2}, {2, 3}};
  EXPECT_GT(s21_map.max_size(), std_map.max_size());
}

TEST(MapTest, Iterators) {
  s21::map<int, int> mp = {{1, 2}, {2, 3}};
  auto it = mp.begin();
  EXPECT_EQ(it->second, 2);
  ++it;
  EXPECT_EQ(it->second, 3);
  EXPECT_EQ(++it, mp.end());
}

TEST(MapTest, InitializerListConstructor_Empty) {
  s21::map<int, int> mp{};
  EXPECT_EQ(int(mp.size()), 0);
}

TEST(MapTest, MoveConstructor_Empty) {
  s21::map<int, int> ms1{};
  s21::map<int, int> ms2{std::move(ms1)};
  EXPECT_EQ(int(ms2.size()), 0);
  EXPECT_EQ(int(ms1.size()), 0);
}

TEST(MapTest, Contains_ExistingElement) {
  s21::map<int, int> mp;
  mp.insert(5, 1);
  mp.insert(3, 2);
  EXPECT_TRUE(mp.contains(5));
  EXPECT_TRUE(mp.contains(3));
}

TEST(MapTest, Contains_NonExistingElement) {
  s21::map<int, int> mp;
  mp.insert(5, 3);
  EXPECT_FALSE(mp.contains(7));
}

TEST(MapTest, InsertOrAssign) {
  s21::map<int, int> s21_mp = {{1, 2}, {2, 3}, {4, 5}};
  auto s21_i1 = s21_mp.insert_or_assign(1, 1);
  auto s21_i2 = s21_mp.insert_or_assign(3, 1);
  EXPECT_FALSE(s21_i1.second);
  EXPECT_TRUE(s21_i2.second);
}

TEST(MapTest, InsertMany) {
  s21::map<int, int> mp;
  auto results = mp.insert_many(3, 5, 4, 7);
  EXPECT_EQ(int(results.size()), 4);
  EXPECT_EQ(int(mp.size()), 2);
  EXPECT_EQ(int(mp[4]), 7);
}