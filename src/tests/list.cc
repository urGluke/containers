#include <gtest/gtest.h>

#include <list>

#include "../s21_containers.h"

TEST(ListTest, DefaultConstructor) {
  S21::List<int> l1;
  std::list<int> l2;

  EXPECT_EQ(l1.size(), l2.size());
  EXPECT_EQ(l1.begin(), l1.end());
  EXPECT_EQ(l1.empty(), l2.empty());
}

TEST(ListTest, DefaultConstructedFrontBack) {
  S21::List<int> l1;
  std::list<int> l2;

  // Добавить элементы в оба списка
  l1.push_back(1);
  l2.push_back(1);

  EXPECT_EQ(l1.front(), l2.front());
  EXPECT_EQ(l1.back(), l2.back());
}

TEST(ListTest, DefaultConstructedIterator) {
  S21::List<int> l1;
  std::list<int> l2;

  EXPECT_EQ(l1.begin(), l1.end());
}

TEST(ListTest, DefaultConstructedbeginend) {
  const S21::List<int> l1;
  const std::list<int> l2;

  EXPECT_EQ(l1.begin(), l1.end());
}

TEST(ListTest, SizeConstructor) {
  const std::size_t count = 5;
  S21::List<int> l1(count);
  std::list<int> l2(count);

  EXPECT_EQ(l1.size(), l2.size());

  // Проверяем, что каждый элемент добавлен в конец списка
  auto it1 = l1.begin();
  auto it2 = l2.begin();
  for (; it1 != l1.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(ListTest, InitializerListConstructor) {
  std::initializer_list<int> items = {1, 2, 3, 4, 5};
  S21::List<int> l1(items);
  std::list<int> l2(items);

  EXPECT_EQ(l1.size(), l2.size());

  // Проверяем, что каждый элемент добавлен в конец списка
  auto it1 = l1.begin();
  auto it2 = l2.begin();
  for (; it1 != l1.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(ListTest, CopyConstructor) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  S21::List<int> l2(l1);

  EXPECT_EQ(l2.size(), l1.size());

  auto it1 = l2.begin();
  auto it2 = l1.begin();
  for (; it1 != l2.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(ListTest, MoveConstructor) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  S21::List<int> l2(std::move(l1));

  // Проверка, что l1 не содержит элементов
  EXPECT_TRUE(l1.empty());
  // Проверка, что l2 содержит все элементы из l1
  EXPECT_EQ(l2.size(), 3u);

  auto it = l2.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

// TEST(ListTest, Destructor) {
//   // Создаем объект списка
//   S21::List<int>* l1 = new S21::List<int>;

//   // Добавляем элементы в список
//   l1->push_back(1);
//   l1->push_back(2);
//   l1->push_back(3);

//   // Проверяем, что список не пуст перед вызовом деструктора
//   EXPECT_FALSE(l1->empty());

//   // Удаляем объект списка
//   delete l1;

//   // Проверяем, что память была корректно освобождена
//   // и список стал пустым
//   EXPECT_TRUE(l1->empty());
// }

TEST(ListTest, MoveAssignmentOperator) {
  // Создаем и заполняем список l1
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Создаем и заполняем список l2
  S21::List<int> l2;
  l2.push_back(4);
  l2.push_back(5);

  // Присваиваем списки
  l2 = std::move(l1);

  // Проверяем, что l2 содержит элементы из l1
  EXPECT_EQ(l2.size(), 3u);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 3);

  // Проверяем, что l1 стал пустым
  EXPECT_TRUE(l1.empty());
}
TEST(ListTest, CopyAssignmentOperator) {
  // Создаем и заполняем список l1
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Создаем и заполняем список l2
  S21::List<int> l2;
  l2.push_back(4);
  l2.push_back(5);

  // Присваиваем списки
  l2 = l1;

  // Проверяем, что l2 содержит элементы из l1
  EXPECT_EQ(l2.size(), 3u);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 3);

  // Изменяем l1
  l1.push_back(6);

  // Проверяем, что l2 остался без изменений
  EXPECT_EQ(l2.size(), 3u);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 3);
}
TEST(ListTest, Front) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что у пустого списка front() возвращает default-значение
  EXPECT_EQ(l1.front(), 0);

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что front() возвращает первый элемент списка
  EXPECT_EQ(l1.front(), 1);

  // Создаем список с одним элементом равным 5
  S21::List<int> l2{5};

  // Проверяем, что у списка l2 front() возвращает 5
  EXPECT_EQ(l2.front(), 5);
}
TEST(ListTest, Back) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что у пустого списка back() возвращает default-значение
  EXPECT_EQ(l1.back(), 0);

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что back() возвращает последний элемент списка
  EXPECT_EQ(l1.back(), 3);

  // Создаем список с одним элементом равным 5
  S21::List<int> l2{5};

  // Проверяем, что у списка l2 back() возвращает 5
  EXPECT_EQ(l2.back(), 5);
}
TEST(ListTest, Begin) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что begin() возвращает итератор на конец списка
  EXPECT_EQ(l1.begin(), l1.end());

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что begin() возвращает итератор на первый элемент списка
  EXPECT_EQ(*l1.begin(), 1);
}
TEST(ListTest, End) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что begin() и end() возвращают одинаковые итераторы
  EXPECT_EQ(l1.begin(), l1.end());

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Получаем итератор на конец списка
  auto it = l1.end();

  // Проверяем, что итератор равен end()
  EXPECT_TRUE(it == l1.end());

  // Проверяем, что итератор не равен begin()
  EXPECT_FALSE(it == l1.begin());

  // Проверяем, что можно декрементировать итератор
  it--;
  EXPECT_EQ(*it, 3);
}
TEST(ListTest, begin) {
  // Создаем пустой список
  S21::List<int> l1;

  // Получаем константный итератор на начало списка
  auto it = l1.begin();

  // Проверяем, что итератор равен end()
  EXPECT_TRUE(it == l1.end());

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Получаем константный итератор на начало списка
  it = l1.begin();

  // Проверяем, что итератор не равен end()
  EXPECT_FALSE(it == l1.end());

  // Проверяем, что можно разыменовать итератор и получить значение
  EXPECT_EQ(*it, 1);
}
TEST(ListTest, end) {
  // Создаем пустой список
  S21::List<int> l1;

  // Получаем константный итератор на конец списка
  auto it = l1.end();

  // Проверяем, что итератор равен begin()
  EXPECT_TRUE(it == l1.begin());

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Получаем константный итератор на конец списка
  it = l1.end();

  // Проверяем, что итератор не равен begin()
  EXPECT_FALSE(it == l1.begin());

  // Пытаемся разыменовать итератор, должно вызвать ошибку компиляции
  // Поскольку итератор указывает на конец списка, он не может быть разыменован
  // и попытка сравнения его значения приведет к ошибке компиляции
  // EXPECT_EQ(*it, ??);
}
TEST(ListTest, Empty) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что список пустой
  EXPECT_TRUE(l1.empty());

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что список больше не пустой
  EXPECT_FALSE(l1.empty());

  // Удаляем все элементы из списка
  l1.clear();

  // Проверяем, что список снова пустой
  EXPECT_TRUE(l1.empty());
}
TEST(ListTest, Size) {
  // Создаем пустой список
  S21::List<int> l1;

  // Проверяем, что размер списка равен 0
  EXPECT_EQ(l1.size(), 0u);

  // Добавляем элементы в список
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что размер списка равен 3
  EXPECT_EQ(l1.size(), 3u);

  // Удаляем элемент из списка
  l1.pop_back();

  // Проверяем, что размер списка уменьшился до 2
  EXPECT_EQ(l1.size(), 2u);
}
TEST(ListTest, MaxSize) {
  // Получаем максимальный размер списка для std::list
  const std::size_t expected_std_max_size = std::list<int>().max_size();

  // Получаем максимальный размер списка для S21::List
  const std::size_t expected_S21_max_size = S21::List<int>().max_size();

  // Проверяем, что оба значения получены
  EXPECT_EQ(expected_S21_max_size ? 1 : 0, expected_std_max_size ? 1 : 0);
}
TEST(ListTest, Clear) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что список не пуст до вызова clear()
  EXPECT_FALSE(l1.empty());

  // Вызываем метод clear()
  l1.clear();

  // Проверяем, что список стал пустым
  EXPECT_TRUE(l1.empty());
}
TEST(ListTest, Insert) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Вставляем новый элемент перед вторым элементом
  S21::List<int>::iterator it = l1.insert(++l1.begin(), 5);

  // Проверяем, что второй элемент теперь равен 5
  EXPECT_EQ(*++l1.begin(), 5);

  // Проверяем, что итератор указывает на вставленный элемент
  EXPECT_EQ(*it, 5);
}
TEST(ListTest, Erase) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Удаляем второй элемент
  l1.erase(++l1.begin());

  // Проверяем, что второй элемент был удален
  EXPECT_EQ(*++l1.begin(), 3);

  // Проверяем, что размер списка уменьшился
  EXPECT_EQ(l1.size(), 2u);
  l1.erase(l1.begin());
  l1.erase(l1.begin());
  EXPECT_EQ(*l1.begin(), 0);
  EXPECT_EQ(l1.size(), 0u);
}

TEST(ListTest, PushBack) {
  S21::List<int> l1;

  // Добавляем элементы в конец списка
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Проверяем, что элементы были добавлены в правильном порядке
  EXPECT_EQ(*l1.begin(), 1);
  EXPECT_EQ(*++l1.begin(), 2);
  EXPECT_EQ(*--l1.end(), 3);

  // Проверяем, что размер списка увеличился
  EXPECT_EQ(l1.size(), 3u);
}
TEST(ListTest, PopBack) {
  S21::List<int> l1;

  // Добавляем элементы в конец списка
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Удаляем последний элемент
  l1.pop_back();

  // Проверяем, что последний элемент был удален
  EXPECT_EQ(*--l1.end(), 2);

  // Проверяем, что размер списка уменьшился
  EXPECT_EQ(l1.size(), 2u);
}
TEST(ListTest, PushFront) {
  S21::List<int> l1;

  // Добавляем элементы в начало списка
  l1.push_front(3);
  l1.push_front(2);
  l1.push_front(1);

  // Проверяем, что первый элемент равен 1
  EXPECT_EQ(l1.front(), 1);

  // Проверяем, что размер списка увеличился
  EXPECT_EQ(l1.size(), 3u);
}
TEST(ListTest, PopFront) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Удаляем первый элемент
  l1.pop_front();

  // Проверяем, что первый элемент теперь равен 2
  EXPECT_EQ(l1.front(), 2);

  // Проверяем, что размер списка уменьшился
  EXPECT_EQ(l1.size(), 2u);
  l1.pop_front();
  l1.pop_front();
  EXPECT_EQ(l1.front(), 0);
  EXPECT_EQ(l1.size(), 0u);
}
TEST(ListTest, Swap) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  S21::List<int> l2;
  l2.push_back(4);
  l2.push_back(5);

  // Обменять списки
  l1.swap(l2);

  // Проверить, что элементы первого списка стали элементами второго списка
  EXPECT_EQ(l1.size(), 2u);
  EXPECT_EQ(l1.front(), 4);
  EXPECT_EQ(l1.back(), 5);

  // Проверить, что элементы второго списка стали элементами первого списка
  EXPECT_EQ(l2.size(), 3u);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 3);
}
TEST(ListTest, Sort) {
  S21::List<int> l1;
  l1.push_back(4);
  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(1);

  // Отсортировать список
  l1.sort();

  // Проверить, что список отсортирован
  EXPECT_EQ(l1.size(), 4u);
  EXPECT_EQ(l1.front(), 1);
  EXPECT_EQ(l1.back(), 4);

  // Проверить порядок элементов
  S21::List<int>::iterator it = l1.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  it++;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 4);
}
TEST(ListTest, Splice) {
  S21::List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);

  // Создаем второй список l2
  S21::List<int> l2;
  l2.push_back(4);
  l2.push_back(5);
  l2.push_back(6);

  // Удаляем элементы из l2 и перемещаем их в l1
  auto it = ++l1.begin();  // Неконстантный итератор
  // const auto const_it =
  //     S21::List<int>::const_iterator(it);  // Константный итератор

  l1.splice(it, l2);

  // Проверяем, что элементы были перемещены из l2 в l1
  std::vector<int> expected_values_l1 = {1, 4, 5, 6, 2, 3};
  auto it1 = l1.begin();
  auto it2 = expected_values_l1.begin();
  for (; it1 != l1.end() && it2 != expected_values_l1.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
  EXPECT_TRUE(it1 == l1.end() && it2 == expected_values_l1.end());

  // Проверяем, что l2 стал пустым
  EXPECT_TRUE(l2.empty());

  // Создаем третий список l3
  S21::List<int> l3;
  l3.push_back(7);
  l3.push_back(8);
  l3.push_back(9);

  // Удаляем элементы из l3 и перемещаем их в начало l1
  l1.splice(l1.begin(), l3);

  // Проверяем, что элементы были перемещены из l3 в начало l1
  std::vector<int> expected_values_l1_after_splice = {7, 8, 9, 1, 4,
                                                      5, 6, 2, 3};
  it1 = l1.begin();
  it2 = expected_values_l1_after_splice.begin();
  for (; it1 != l1.end() && it2 != expected_values_l1_after_splice.end();
       ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
  EXPECT_TRUE(it1 == l1.end() && it2 == expected_values_l1_after_splice.end());

  // Проверяем, что l3 стал пустым
  EXPECT_TRUE(l3.empty());

  // Создаем четвертый список l4
  S21::List<int> l4;

  // Удаляем элементы из l1 и перемещаем их в l4
  l4.splice(l4.begin(), l1);

  // Проверяем, что элементы были перемещены из l1 в l4
  std::vector<int> expected_values_l4 = {7, 8, 9, 1, 4, 5, 6, 2, 3};
  it1 = l4.begin();
  it2 = expected_values_l4.begin();
  for (; it1 != l4.end() && it2 != expected_values_l4.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
  EXPECT_TRUE(it1 == l4.end() && it2 == expected_values_l4.end());

  // Проверяем, что l1 стал пустым
  EXPECT_TRUE(l1.empty());
  // Создаем пустой список l1
  S21::List<int> l11;

  // Создаем пустой список l2
  S21::List<int> l22;

  // Удаляем элементы из l2 и перемещаем их в l1 (для пустых списков)
  auto it11 = l11.begin();
  l11.splice(it11, l22);

  // Проверяем, что l1 остался пустым
  EXPECT_TRUE(l11.empty());
  // Проверяем, что l2 остался пустым
  EXPECT_TRUE(l22.empty());
}
// Тестовый случай для слияния двух пустых списков
TEST(ListMergeTest, EmptyLists) {
  S21::List<int> list1;
  S21::List<int> list2;
  list1.merge(list2);
  ASSERT_TRUE(list1.empty());
  ASSERT_TRUE(list2.empty());
}

// Тестовый случай для слияния пустого списка с непустым
TEST(ListMergeTest, EmptyListWithNonEmptyList) {
  S21::List<int> list1;
  S21::List<int> list2;
  list2.push_back(1);
  list2.push_back(3);
  list2.push_back(5);
  list1.merge(list2);
  ASSERT_TRUE(list1.size() == 3u);
  ASSERT_TRUE(list2.empty());
  // Проверка, что list1 содержит элементы в правильном порядке
  ASSERT_EQ(*list1.begin(), 1);
  ASSERT_EQ(*++list1.begin(), 3);
  ASSERT_EQ(*++(++list1.begin()), 5);
}

// Тестовый случай для слияния непустых списков
TEST(ListMergeTest, NonEmptyLists) {
  S21::List<int> list1;
  list1.push_back(2);
  list1.push_back(4);
  list1.push_back(6);

  S21::List<int> list2;
  list2.push_back(1);
  list2.push_back(3);
  list2.push_back(5);

  list1.merge(list2);
  ASSERT_TRUE(list1.size() == 6u);
  ASSERT_TRUE(list2.empty());
  // Проверка, что list1 содержит элементы в правильном порядке
  ASSERT_EQ(*list1.begin(), 1);
  ASSERT_EQ(*++list1.begin(), 2);
  ASSERT_EQ(*++(++list1.begin()), 3);
  ASSERT_EQ(*++(++(++list1.begin())), 4);
  ASSERT_EQ(*++(++(++(++list1.begin()))), 5);
  ASSERT_EQ(*++(++(++(++(++list1.begin())))), 6);
}

TEST(ListReverseTest, EmptyList) {
  S21::List<int> myList;
  myList.reverse();
  ASSERT_TRUE(myList.empty());
}

TEST(ListReverseTest, SingleElementList) {
  S21::List<int> myList;
  myList.push_back(42);
  myList.reverse();
  ASSERT_EQ(*myList.begin(), 42);
}

TEST(ListReverseTest, MultipleElements) {
  S21::List<int> myList;
  myList.push_back(1);
  myList.push_back(2);
  myList.push_back(3);
  myList.push_back(4);
  myList.push_back(5);

  myList.reverse();
  ASSERT_EQ(*myList.begin(), 5);

  auto it = myList.begin();
  ++it;
  ASSERT_EQ(*it, 4);

  ++it;
  ASSERT_EQ(*it, 3);

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 1);
}

TEST(ListReverseTest, OddNumberOfElements) {
  S21::List<int> myList;
  myList.push_back(1);
  myList.push_back(2);
  myList.push_back(3);
  myList.push_back(4);
  myList.push_back(5);
  myList.push_back(6);

  myList.reverse();
  ASSERT_EQ(*myList.begin(), 6);

  auto it = myList.begin();
  ++it;
  ASSERT_EQ(*it, 5);

  ++it;
  ASSERT_EQ(*it, 4);

  ++it;
  ASSERT_EQ(*it, 3);

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 1);
}

TEST(ListUniqueTest, EmptyList) {
  S21::List<int> myList;
  myList.unique();
  ASSERT_TRUE(myList.empty());
}

TEST(ListUniqueTest, NoDuplicates) {
  S21::List<int> myList;
  myList.push_back(1);
  myList.push_back(2);
  myList.push_back(3);
  myList.push_back(4);
  myList.push_back(5);

  myList.unique();
  ASSERT_EQ(myList.size(), 5u);

  auto it = myList.begin();
  ASSERT_EQ(*it, 1);

  it++;
  ASSERT_EQ(*it, 2);

  it++;
  ASSERT_EQ(*it, 3);

  it++;
  ASSERT_EQ(*it, 4);

  it++;
  ASSERT_EQ(*it, 5);
}

TEST(ListUniqueTest, WithDuplicates) {
  S21::List<int> myList;
  myList.push_back(1);
  myList.push_back(2);
  myList.push_back(2);
  myList.push_back(3);
  myList.push_back(4);
  myList.push_back(4);
  myList.push_back(4);
  myList.push_back(5);
  myList.push_back(5);

  myList.unique();
  ASSERT_EQ(myList.size(), 5u);

  auto it = myList.begin();
  ASSERT_EQ(*it, 1);

  it++;
  ASSERT_EQ(*it, 2);

  it++;
  ASSERT_EQ(*it, 3);

  it++;
  ASSERT_EQ(*it, 4);

  it++;
  ASSERT_EQ(*it, 5);
}
TEST(ListTest, insert_manyBackTest) {
  // Создание списка
  S21::List<int> myList;
  // Добавление элементов с помощью insert_manyBack
  myList.insert_many_back(1, 2, 3, 4);

  // Проверка, что список содержит добавленные элементы
  ASSERT_EQ(myList.size(), 4u);
  ASSERT_EQ(myList.front(), 1);
  ASSERT_EQ(myList.back(), 4);
}

TEST(ListTest, insert_manyFrontTest) {
  // Создание списка
  S21::List<int> myList;

  // Добавление элементов с помощью insert_manyFront
  myList.insert_many_front(4, 3, 2, 1);

  // Проверка, что список содержит добавленные элементы
  ASSERT_EQ(myList.size(), 4u);
  ASSERT_EQ(myList.front(), 4);
  ASSERT_EQ(myList.back(), 1);
}

TEST(ListTest, insert_manyTest) {
  // Создание списка
  S21::List<int> myList;

  // Добавление элементов
  myList.push_back(1);
  myList.push_back(2);
  myList.push_back(3);

  // Получение итератора на элемент со значением 2
  auto it = myList.begin();
  while (*it != 2) {
    ++it;
  }

  // Вставка элементов с помощью insert_many перед элементом со значением 2
  myList.insert_many(it, 4, 5);

  // Проверка, что список содержит добавленные элементы
  ASSERT_EQ(myList.size(), 5u);
  ASSERT_EQ(*it, 2);

  // std::cout << "\nРезультат объединения мой" << std::endl;
  // Проверка элементов до и после вставленных элементов
  --it;
  --it;
  --it;
  ASSERT_EQ(*it, 1);
  ++it;
  ASSERT_EQ(*it, 4);
  ++it;
  ASSERT_EQ(*it, 5);
  ++it;
  ++it;
  ASSERT_EQ(*it, 3);
}

TEST(ListTest, Iterator_Increment_Operator) {
  S21::List<int> list{1, 2, 3, 4, 5};
  auto it = list.begin();

  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);

  it++;
  EXPECT_EQ(*it, 3);
}

TEST(ListTest, Iterator_Decrement_Operator) {
  S21::List<int> list{1, 2, 3, 4, 5};
  auto it = list.end();

  --it;
  EXPECT_EQ(*it, 5);

  it--;
  EXPECT_EQ(*it, 4);
}

TEST(ListTest, Iterator_Postfix_Increment) {
  S21::List<int> list{1, 2, 3};
  auto it = list.begin();

  auto tempIt = it++;
  EXPECT_EQ(*tempIt, 1);
  EXPECT_EQ(*it, 2);
}

TEST(ListTest, Iterator_Postfix_Decrement) {
  S21::List<int> list{1, 2, 3};
  auto it = list.end();
  --it;
  auto tempIt = it--;
  EXPECT_EQ(*tempIt, 3);
  EXPECT_EQ(*it, 2);
}
