#include <gtest/gtest.h>

#include <vector>

#include "../s21_vector.h"

class VectorTest : public testing::Test {
 protected:
  s21::Vector<int> s21_vector, s21_vector_dest, s21_vector_src;
  std::vector<int> std_vector, std_vector_dest, std_vector_src;

  void initWithSize(s21::Vector<int> &s21_vector,
                    std::vector<int> &std_vector) {
    s21_vector = s21::Vector<int>(5);
    std_vector = std::vector<int>(5);
  }
  void initInitializerList(s21::Vector<int> &s21_vector,
                           std::vector<int> &std_vector) {
    s21_vector = s21::Vector<int>({1, 2, 3, 4});
    std_vector = std::vector<int>({1, 2, 3, 4});
  }
  void checkEqVector(const s21::Vector<int> &s21_vector,
                     const std::vector<int> &std_vector) const {
    EXPECT_EQ(s21_vector.size(), std_vector.size());
    EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
    int dataSize = s21_vector.size() * sizeof(int);
    EXPECT_EQ(memcmp(s21_vector.data(), std_vector.data(), dataSize), 0);
  }
  void printSizeCapacity() const {
    std::cout << "size: " << s21_vector.size() << " " << std_vector.size()
              << std::endl;
    std::cout << "capacity: " << s21_vector.capacity() << " "
              << std_vector.capacity() << std::endl;
  }
  void printData() const {
    if (s21_vector.data() != nullptr) {
      std::cout << "s21_vector data: ";
      for (std::size_t i = 0; i < s21_vector.size(); ++i) {
        std::cout << s21_vector.data()[i] << " ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "s21_vector data: nullptr" << std::endl;
    }
    if (s21_vector.data() != nullptr) {
      std::cout << "std_vector data: ";
      for (std::size_t i = 0; i < std_vector.size(); ++i) {
        std::cout << std_vector.data()[i] << " ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "std_vector data: nullptr" << std::endl;
    }
  }
};

TEST_F(VectorTest, DefaultConstructor) {
  checkEqVector(s21_vector, std_vector);
}
TEST_F(VectorTest, ConstructorWithSize) {
  initWithSize(s21_vector, std_vector);
  checkEqVector(s21_vector, std_vector);
}
TEST_F(VectorTest, ConstructorWithInitializerList) {
  initInitializerList(s21_vector, std_vector);
  checkEqVector(s21_vector, std_vector);
}
TEST_F(VectorTest, CopyConstructor) {
  initInitializerList(s21_vector_src, std_vector_src);
  s21::Vector<int> s21_vector_dest(s21_vector_src);
  std::vector<int> std_vector_dest(std_vector_src);
  checkEqVector(s21_vector_dest, std_vector_dest);
}
TEST_F(VectorTest, MoveConstructor) {
  initInitializerList(s21_vector_src, std_vector_src);
  s21::Vector<int> s21_vector_dest(std::move(s21_vector_src));
  std::vector<int> std_vector_dest(std::move(std_vector_src));
  checkEqVector(s21_vector_dest, std_vector_dest);
}
TEST_F(VectorTest, MoveOperator) {
  initInitializerList(s21_vector_src, std_vector_src);

  s21_vector_dest = std::move(s21_vector_src);
  std_vector_dest = std::move(std_vector_src);

  checkEqVector(s21_vector_dest, std_vector_dest);
  checkEqVector(s21_vector_src, std_vector_src);
}
TEST_F(VectorTest, Destructor) {
  s21_vector = s21::Vector<int>({1, 2, 3, 4});
  s21_vector.~Vector();
  checkEqVector(s21_vector, std_vector);
}

TEST_F(VectorTest, ReferenceAt) {
  initInitializerList(s21_vector, std_vector);
  for (std::size_t i = 0; i < s21_vector.size(); i++) {
    EXPECT_EQ(s21_vector.at(i), std_vector.at(i));
  }
  EXPECT_THROW(s21_vector.at(-1), std::out_of_range);
  EXPECT_THROW(s21_vector.at(4), std::out_of_range);
  EXPECT_THROW(std_vector.at(-1), std::out_of_range);
  EXPECT_THROW(std_vector.at(4), std::out_of_range);
}

TEST_F(VectorTest, OperatorBracket) {
  initInitializerList(s21_vector, std_vector);
  s21_vector[0] = 5;
  std_vector[0] = 5;
  checkEqVector(s21_vector, std_vector);
}

TEST_F(VectorTest, Front) {
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.front(), std_vector.front());
}

TEST_F(VectorTest, FrontThrow) {
  EXPECT_THROW(s21_vector.front(), std::out_of_range);
}

TEST_F(VectorTest, Back) {
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.back(), std_vector.back());
}

TEST_F(VectorTest, BackThrow) {
  EXPECT_THROW(s21_vector.back(), std::out_of_range);
}

TEST_F(VectorTest, Data) {
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(&s21_vector[0], s21_vector.data());
  EXPECT_EQ(&std_vector[0], std_vector.data());
}

TEST_F(VectorTest, Empty) {
  EXPECT_EQ(s21_vector.empty(), std_vector.empty());
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.empty(), std_vector.empty());
}

TEST_F(VectorTest, Size) {
  EXPECT_EQ(s21_vector.size(), std_vector.size());
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.size(), std_vector.size());
}

TEST_F(VectorTest, MaxSize) {
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.max_size() ? 1 : 0, std_vector.max_size() ? 1 : 0);
}

TEST_F(VectorTest, Reserve) {
  s21_vector.reserve(10);
  EXPECT_EQ(s21_vector.capacity(), 10);
  s21_vector.reserve(5);
  EXPECT_EQ(s21_vector.capacity(), 10);
}

TEST_F(VectorTest, Capacity) {
  EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
  s21_vector.reserve(10);
  std_vector.reserve(10);
  EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
}

TEST_F(VectorTest, ShrinkToFit) {
  initInitializerList(s21_vector, std_vector);
  EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
  s21_vector.reserve(10);
  std_vector.reserve(10);
  EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
  s21_vector.shrink_to_fit();
  std_vector.shrink_to_fit();
  EXPECT_EQ(s21_vector.capacity(), std_vector.capacity());
}
TEST_F(VectorTest, Clear) {
  initInitializerList(s21_vector, std_vector);
  checkEqVector(s21_vector, std_vector);
  s21_vector.clear();
  std_vector.clear();
  checkEqVector(s21_vector, std_vector);
}

TEST_F(VectorTest, Insert) {
  checkEqVector(s21_vector_src, std_vector_src);
  s21::Vector<int>::VectorIterator s21_iter_src = s21_vector.begin();
  std::vector<int>::iterator std_iter_src = std_vector.begin();
  s21_vector_src.insert(s21_iter_src, 100);
  std_vector_src.insert(std_iter_src, 100);
  checkEqVector(s21_vector_src, std_vector_src);

  initInitializerList(s21_vector, std_vector);
  checkEqVector(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin();
  std::vector<int>::iterator std_iter = std_vector.begin();
  s21_vector.insert(s21_iter, 100);
  std_vector.insert(std_iter, 100);
  checkEqVector(s21_vector, std_vector);

  s21_iter = s21_vector.begin() - 1;
  EXPECT_THROW(s21_vector.insert(s21_iter, 100), std::out_of_range);

  s21_iter = s21_vector.end() + 1;
  EXPECT_THROW(s21_vector.insert(s21_iter, 100), std::out_of_range);
}

TEST_F(VectorTest, Erase) {
  initInitializerList(s21_vector, std_vector);
  checkEqVector(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin() + 2;
  std::vector<int>::iterator std_iter = std_vector.begin() + 2;
  s21_vector.erase(s21_iter);
  std_vector.erase(std_iter);
  checkEqVector(s21_vector, std_vector);

  s21_iter = s21_vector.begin() - 1;
  std_iter = std_vector.begin() - 1;

  EXPECT_THROW(s21_vector.erase(s21_iter), std::out_of_range);
}

TEST_F(VectorTest, PushBack) {
  s21_vector.push_back(5);
  std_vector.push_back(5);
  checkEqVector(s21_vector, std_vector);

  s21_vector.push_back(1);
  s21_vector.push_back(2);

  std_vector.push_back(1);
  std_vector.push_back(2);
  checkEqVector(s21_vector, std_vector);
}
TEST_F(VectorTest, PopBack) {
  initInitializerList(s21_vector, std_vector);

  s21_vector.pop_back();
  s21_vector.pop_back();
  s21_vector.pop_back();
  std_vector.pop_back();
  std_vector.pop_back();
  std_vector.pop_back();
  checkEqVector(s21_vector, std_vector);

  s21_vector.pop_back();
  std_vector.pop_back();
  checkEqVector(s21_vector, std_vector);
}

TEST_F(VectorTest, Swap) {
  initInitializerList(s21_vector_src, std_vector_src);
  s21_vector_dest.push_back(5);
  std_vector_dest.push_back(5);

  s21_vector_dest.swap(s21_vector_src);
  std_vector_dest.swap(std_vector_src);

  checkEqVector(s21_vector_dest, std_vector_dest);
  checkEqVector(s21_vector_src, std_vector_src);
}

TEST_F(VectorTest, DefaultConstructorIterator) {
  s21::Vector<int>::VectorIterator iterator;
  ASSERT_TRUE(iterator == nullptr);
}

TEST_F(VectorTest, ConstructorIterator) {
  s21::Vector<int>::VectorIterator iterator(s21_vector.data());
  ASSERT_TRUE(iterator == nullptr);
}

TEST_F(VectorTest, EqIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator iter1(s21_vector.data());
  s21::Vector<int>::VectorIterator iter2(s21_vector.data());
  s21::Vector<int>::VectorIterator iter3(std_vector.data());
  ASSERT_TRUE(iter1 == iter2);
  ASSERT_FALSE(iter1 == iter3);
}

TEST_F(VectorTest, NoEqIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator iter1(s21_vector.data());
  s21::Vector<int>::VectorIterator iter2(std_vector.data());
  ASSERT_TRUE(iter1 != iter2);
}

TEST_F(VectorTest, PreOperPlusPlusIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin();
  std::vector<int>::iterator std_iter = std_vector.begin();
  ++s21_iter;
  ++std_iter;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PreOperMinMinIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin();
  std::vector<int>::iterator std_iter = std_vector.begin();
  ++s21_iter;
  ++std_iter;
  --s21_iter;
  --std_iter;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PostOperPlusPlusIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin();
  std::vector<int>::iterator std_iter = std_vector.begin();
  s21_iter++;
  std_iter++;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PostOperMinMinIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.end();
  std::vector<int>::iterator std_iter = std_vector.end();
  s21_iter--;
  std_iter--;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, OperPlusPlusIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.begin();
  std::vector<int>::iterator std_iter = std_vector.begin();
  ASSERT_EQ(*(s21_iter + 2), *(std_iter + 2));
}

TEST_F(VectorTest, OperMinMinIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorIterator s21_iter = s21_vector.end();
  std::vector<int>::iterator std_iter = std_vector.end();
  ASSERT_EQ(*(s21_iter - 2), *(std_iter - 2));
}

TEST_F(VectorTest, DefaultConstructorConstIterator) {
  s21::Vector<int>::VectorConstIterator iterator;
  ASSERT_TRUE(iterator == nullptr);
}

TEST_F(VectorTest, ConstructorConstIterator) {
  s21::Vector<int>::VectorConstIterator iterator(s21_vector.data());
  ASSERT_TRUE(iterator == nullptr);
}

TEST_F(VectorTest, EqConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator iter1(s21_vector.data());
  s21::Vector<int>::VectorConstIterator iter2(s21_vector.data());
  s21::Vector<int>::VectorConstIterator iter3(std_vector.data());
  ASSERT_TRUE(iter1 == iter2);
  ASSERT_FALSE(iter1 == iter3);
}

TEST_F(VectorTest, NoEqConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator iter1(s21_vector.data());
  s21::Vector<int>::VectorConstIterator iter2(std_vector.data());
  ASSERT_TRUE(iter1 != iter2);
}

TEST_F(VectorTest, PreOperPlusPlusConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cbegin();
  std::vector<int>::const_iterator std_iter = std_vector.cbegin();
  ++s21_iter;
  ++std_iter;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PreOperMinMinConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cbegin();
  std::vector<int>::const_iterator std_iter = std_vector.cbegin();
  ++s21_iter;
  ++std_iter;
  --s21_iter;
  --std_iter;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PostOperPlusPlusConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cbegin();
  std::vector<int>::const_iterator std_iter = std_vector.cbegin();
  s21_iter++;
  std_iter++;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, PostOperMinMinConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cend();
  std::vector<int>::const_iterator std_iter = std_vector.cend();
  s21_iter--;
  std_iter--;
  ASSERT_EQ(*s21_iter, *std_iter);
}

TEST_F(VectorTest, OperPlusPlusConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cbegin();
  std::vector<int>::const_iterator std_iter = std_vector.cbegin();
  ASSERT_EQ(*(s21_iter + 2), *(std_iter + 2));
  s21::Vector<int>::VectorConstIterator s21_iter2 = s21_vector.cend();
  std::vector<int>::const_iterator std_iter2 = std_vector.cend();
  ASSERT_EQ(*(s21_iter2 + -2), *(std_iter2 + -2));
}

TEST_F(VectorTest, OperMinMinConstIterator) {
  initInitializerList(s21_vector, std_vector);
  s21::Vector<int>::VectorConstIterator s21_iter = s21_vector.cbegin();
  std::vector<int>::const_iterator std_iter = std_vector.cbegin();
  ASSERT_EQ(*(s21_iter - -2), *(std_iter - -2));
  s21::Vector<int>::VectorConstIterator s21_iter2 = s21_vector.cend();
  std::vector<int>::const_iterator std_iter2 = std_vector.cend();
  ASSERT_EQ(*(s21_iter2 - 2), *(std_iter2 - 2));
}

TEST_F(VectorTest, InsertMany) {
  initInitializerList(s21_vector, std_vector);
  s21_vector.insert_many(s21_vector.cbegin(), 6, 7, 8);
  std_vector.insert(std_vector.cbegin(), {6, 7, 8});
  checkEqVector(s21_vector, std_vector);
}

TEST_F(VectorTest, InsertBack) {
  initInitializerList(s21_vector, std_vector);
  s21_vector.insert_many_back(6, 7, 8);
  std_vector.insert(std_vector.end(), {6, 7, 8});
  checkEqVector(s21_vector, std_vector);
}

// int main(int argc, char **argv) {
//   testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }