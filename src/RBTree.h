#ifndef CONTAINERS_SRC_RBTREE_H_
#define CONTAINERS_SRC_RBTREE_H_

#include <iostream>
#include <limits>
#include <memory>
#include <vector>

namespace S21 {

template <typename T>
class RBTree {
  // strongly typed enum
  enum class Color { RED, BLACK };
  // Часть приватных методов необходимых в начале класса
 private:
  struct Node {
    T data;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(T val, Color c)
        : data(val), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
  };
  Node *root_;

 public:
  class iterator;
  class const_iterator;
  // Конструктор
  RBTree() : root_(nullptr) {}

  // Деструктор
  ~RBTree() { clearTree(root_); }

  // Конструктор копирования
  RBTree(const RBTree &other) : root_(nullptr) {
    root_ = copyTree(other.root_);
  }

  // Оператор присваивания копированием
  RBTree &operator=(const RBTree &other) {
    if (this != &other) {
      clearTree(root_);
      root_ = copyTree(other.root_);
    }
    return *this;
  }

  // Конструктор перемещения
  RBTree(RBTree &&other) noexcept : root_(other.root_) {
    other.root_ = nullptr;
  }

  // Оператор присваивания перемещением
  RBTree &operator=(RBTree &&other) noexcept {
    if (this != &other) {
      clearTree(root_);
      root_ = other.root_;
      other.root_ = nullptr;
    }
    return *this;
  }

  /* Оператор равенства проверяет, равны ли два дерева. Два дерева считаются
    равными, если они имеют одинаковую структуру и одинаковые значения в каждом
    узле. */
  bool operator==(const RBTree &other) const {
    iterator it1 = begin();
    iterator it2 = other.begin();

    // Продолжаем итерацию, пока оба итератора не достигнут конца.
    while (it1 != end() && it2 != other.end()) {
      // Если какое-либо значение не совпадает, деревья не равны.
      if (*it1 != *it2) return false;
      ++it1;
      ++it2;
    }

    // Если один из итераторов не достиг конца, одно дерево больше, чем другое.
    return it1 == end() && it2 == other.end();
  }

  /* Оператор неравенства проверяет, не равны ли два дерева. Он просто
    возвращает отрицание результата оператора равенства. */
  bool operator!=(const RBTree &other) const { return !(*this == other); }

  /* Оператор "меньше". Он проверяет, является ли одно дерево "меньше" другого.
    Дерево считается "меньше", если его первый отличающийся элемент меньше
    соответствующего элемента другого дерева, или если все совпадающие элементы
    одинаковы, но дерево короче. */
  bool operator<(const RBTree &other) const {
    iterator it1 = begin();
    iterator it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
      if (*it1 != *it2) return *it1 < *it2;
      ++it1;
      ++it2;
    }

    /* Если оба дерева равны до тех пор, пока одно из них не закончится, то
      более короткое дерево меньше. */
    return (it1 == end() && it2 != other.end());
  }

  /* Оператор "меньше или равно". Он возвращает true, если одно дерево меньше
    другого или если они равны. */
  bool operator<=(const RBTree &other) const {
    return *this == other || *this < other;
  }

  /* Оператор "больше". Он возвращает true, если одно дерево не меньше и не
    равно другому. */
  bool operator>(const RBTree &other) const { return !(*this <= other); }

  /* Оператор "больше или равно". Он возвращает true, если одно дерево больше
    другого или если они равны. */
  bool operator>=(const RBTree &other) const {
    return *this == other || *this > other;
  }

  /* Методы для основных операций с деревом
    Метод для вставки нового значения в дерево */
  iterator insert(T value) {
    // Выделение памяти без вызова исключенй
    Node *newNode = new (std::nothrow) Node(value, Color::RED);
    // В случае не выделения проверяем на nullptr
    if (!newNode) {
      std::cerr << "Memory allocation failed." << std::endl;
      return end();
    }

    // Вставка как в обычное бинарное дерево
    Node *parent = nullptr;
    Node *current = root_;
    while (current != nullptr) {
      parent = current;
      if (value < current->data)
        current = current->left;
      else
        current = current->right;
    }
    newNode->parent = parent;
    if (parent == nullptr)
      root_ = newNode;
    else if (value < parent->data)
      parent->left = newNode;
    else
      parent->right = newNode;
    // Вызываем метод для восстановления свойств красно-черного дерева
    fixInsertion(newNode);
    return iterator(newNode);
  }

  /* Вставляет несколько элементов в дерево. Аргументы передаются в виде
    перечисления. Метод возвращает вектор пар, где первый элемент каждой пары
    является итератором к вставленному элементу, а второй элемент обозначает
    успешность вставки (true, если элемент был вставлен успешно). */
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    std::vector<std::pair<iterator, bool>> results;
    results.reserve(sizeof...(args));  // Выделяем память для результатов
    auto insertSingleElement = [this](const auto &element) {
      Node *newNode = new (std::nothrow) Node(element, Color::RED);
      if (!newNode) {
        std::cerr << "Memory allocation failed." << std::endl;
        return std::make_pair(end(), false);
      }

      // Вставляем как в обычное бинарное дерево
      Node *parent = nullptr;
      Node *current = root_;
      while (current != nullptr) {
        parent = current;
        if (element < current->data)
          current = current->left;
        else
          current = current->right;
      }
      newNode->parent = parent;
      if (parent == nullptr)
        root_ = newNode;
      else if (element < parent->data)
        parent->left = newNode;
      else
        parent->right = newNode;

      // Вызываем метод для восстановления свойств красно-черного дерева
      fixInsertion(newNode);

      return std::make_pair(
          iterator(newNode),
          true);  // Возвращаем пару с итератором и флагом успеха
    };

    // Вставляем каждый элемент из списка аргументов
    for (auto element : {std::forward<Args>(args)...}) {
      results.push_back(insertSingleElement(element));
    }

    return results;
  }

  // возвращает итератор к первому элементу, который не меньше заданного ключа
  iterator lower_bound(const T &key) const {
    Node *current = root_;
    Node *result = nullptr;
    while (current) {
      if (key < current->data) {
        result = current;
        current = current->left;
      } else if (key == current->data) {
        return iterator(current);  // Нашли точное совпадение
      } else {
        current = current->right;
      }
    }
    return iterator(result);  // Возвращаем ближайший больший элемент
  }

  // возвращает итератор к первому элементу, который строго больше заданного
  iterator upper_bound(const T &key) const {
    Node *current = root_;
    Node *result = nullptr;
    while (current) {
      if (key < current->data) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result);  // Возвращаем ближайший больший элемент
  }

  // ключа Метод для удаления значения из дерева
  void remove(T value) {
    // Находим узел, содержащий значение для удаления
    Node *node = findNode(root_, value);
    if (!node) return;  // Узел не найден, ничего не делаем

    // Находим узел, который фактически будет удален из дерева
    Node *nodeToRemove = (node->left && node->right) ? successor(node) : node;

    // Получаем ссылку на дочерний узел (не нулевой), если он есть
    Node *child = nodeToRemove->left ? nodeToRemove->left : nodeToRemove->right;

    // Устанавливаем родителю дочернего узла ссылку на правильное поддерево
    if (child) child->parent = nodeToRemove->parent;

    // Удаляем узел из дерева
    if (!nodeToRemove->parent)
      root_ = child;
    else if (nodeToRemove == nodeToRemove->parent->left)
      nodeToRemove->parent->left = child;
    else
      nodeToRemove->parent->right = child;

    // Если удаляемый узел был черного цвета, может быть нарушено свойство 4
    if (nodeToRemove->color == Color::BLACK && child) fixDeletion(child);

    /* Если удаляемый узел не совпадает с тем, что нужно было удалить, копируем
      данные
      if (nodeToRemove != node) node->data = nodeToRemove->data; */

    delete nodeToRemove;
  }

  // Метод для поиска значения в дереве
  iterator find(T value) const {
    Node *node = findNode(root_, value);
    if (node) {
      return iterator(node);
    } else {
      return end();
    }
  }

  // Метод для печати значений дерева в порядке возрастания
  void print() const {
    inOrderTraversal(root_);
    std::cout << std::endl;
  }

  // Метод для получения размера дерева
  int size() const { return size(root_); }

  // Метод проверки на пустое дерево
  bool isEmpty() const { return root_ == nullptr; };

  // Метод для очистки дерева. Он удаляет все узлы в дереве.
  void clear() {
    clearTree(root_);
    root_ = nullptr;
  }

  /* Возвращает итератор, указывающий на узел с минимальным значением в дереве.
    Этот узел является первым узлом в порядке возрастания. */
  iterator begin() {
    Node *current = root_;
    while (current && current->left) current = current->left;
    return iterator(current);
  }

  /* Возвращает итератор, указывающий на следующий узел после последнего узла
    в дереве. */
  iterator end() const { return iterator(nullptr); }

  // Метод для подсчета количества элементов с заданным ключом
  size_t count(const T &key) const { return countKeys(root_, key); }

  // обменивает корневые узлы двух деревьев
  void swap(RBTree &other) { std::swap(root_, other.root_); }

  // возвращающает максимально возможный размер для этого дерева
  size_t max_size() const { return std::numeric_limits<size_t>::max(); }

  // Объединение
  void merge(RBTree<T> &otherTree) {
    // Перебираем все элементы в другом дереве и вставляем их в текущее дерево
    for (auto it = otherTree.begin(); it != otherTree.end(); ++it)
      this->insert(*it);
    otherTree.clear();
  }

  /* поиск с корневого узла и движется влево или вправо в зависимости от
    значения ключа */
  bool contains(const T &key) const {
    Node *current = root_;
    while (current)
      if (key < current->data)
        current = current->left;
      else if (key == current->data)
        return true;
      else
        current = current->right;
    return false;
  }

  std::pair<iterator, iterator> equal_range(const T &key) const {
    Node *current = root_;
    iterator first = end();
    iterator second = end();

    while (current)
      if (key < current->data)
        current = current->left;
      else if (key == current->data) {
        first = iterator(current);
        current = current->left;
      } else
        current = current->right;

    current = root_;
    while (current)
      if (key >= current->data)
        current = current->right;
      else {
        second = iterator(current);
        current = current->left;
      }

    return {first, second};
  }

  class iterator {
    friend class RBTree;

   private:
    Node *node;

   public:
    iterator(Node *node = nullptr) : node(node) {}

    // Перегрузка оператора разыменования
    T &operator*() { return node->data; }

    // Перегрузка оператора '->'
    T *operator->() { return &(node->data); }

    // Перегрузка оператора '=='
    bool operator==(const iterator &other) const { return node == other.node; }

    // Перегрузка оператора '!='
    bool operator!=(const iterator &other) const { return node != other.node; }

    /* Перегрузка оператора '++' для префиксного инкремента
      Итерация по возрастанию */
    iterator &operator++() {
      if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
      } else {
        Node *parent = node->parent;
        while (parent && node == parent->right) {
          node = parent;
          parent = node->parent;
        }
        node = parent;
      }
      return *this;
    }

    /* Перегрузка оператора '++' для постфиксного инкремента
      Итерация по возрастанию */
    iterator operator++(int) {
      iterator old = *this;
      ++(*this);
      return old;
    }
  };
  class const_iterator {
    friend class RBTree;

   private:
    const Node *node;

   public:
    const_iterator(const Node *node = nullptr) : node(node) {}

    // Перегрузка оператора разыменования
    const T &operator*() const { return node->data; }

    // Перегрузка оператора '->'
    const T *operator->() const { return &(node->data); }

    // Перегрузка оператора '=='
    bool operator==(const const_iterator &other) const {
      return node == other.node;
    }

    // Перегрузка оператора '!='
    bool operator!=(const const_iterator &other) const {
      return node != other.node;
    }

    /* Перегрузка оператора '++' для префиксного инкремента
      Итерация по возрастанию */
    const_iterator &operator++() {
      if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
      } else {
        const Node *parent = node->parent;
        while (parent && node == parent->right) {
          node = parent;
          parent = node->parent;
        }
        node = parent;
      }
      return *this;
    }

    /* Перегрузка оператора '++' для постфиксного инкремента
      Итерация по возрастанию */
    const_iterator operator++(int) {
      const_iterator old = *this;
      ++(*this);
      return old;
    }
  };

 private:
  // Вспомогательная рекурсивная функция для подсчета ключей
  size_t countKeys(const Node *node, const T &key) const {
    if (node == nullptr) return 0;

    size_t cnt = 0;

    // Если текущий узел совпадает с ключом, увеличиваем счетчик
    if (node->data == key) ++cnt;

    // Рекурсивно обходим левое и правое поддерево
    cnt += countKeys(node->left, key);
    cnt += countKeys(node->right, key);

    return cnt;
  }

  // Рекурсивный метод для обхода дерева в порядке возрастания
  void inOrderTraversal(Node *node, std::vector<T> &result) const {
    if (node == nullptr) return;

    inOrderTraversal(node->left, result);
    result.push_back(node->data);
    inOrderTraversal(node->right, result);
  }

  /* Метод для поиска узла по значению в поддереве с корнем node.
    Возвращает найденный узел или nullptr, если узел с таким значением не
    найден. */
  Node *findNode(Node *node, T value) const {
    if (node == nullptr || node->data == value) return node;
    if (value < node->data)
      return findNode(node->left, value);
    else
      return findNode(node->right, value);
  }

  // Метод для рекурсивного обхода дерева и печати значений
  void inOrderTraversal(Node *node) const {
    if (node) {
      inOrderTraversal(node->left);
      std::cout << node->data << " ";
      inOrderTraversal(node->right);
    }
  }

  // Метод для освобождения памяти и удаления всех узлов в дереве
  void clearTree(Node *node) {
    if (node) {
      clearTree(node->left);
      clearTree(node->right);
      delete node;
    }
  }

  // Левое малое вращение
  void rotateLeft(Node *node) {
    if (!node || !node->right) return;  // Невозможно выполнить левый поворот
    Node *pivot = node->right;
    node->right = pivot->left;
    if (pivot->left) pivot->left->parent = node;
    pivot->parent = node->parent;
    if (!node->parent)
      root_ = pivot;
    else if (node == node->parent->left)
      node->parent->left = pivot;
    else
      node->parent->right = pivot;
    pivot->left = node;
    node->parent = pivot;
  }

  // Правое малое вращение
  void rotateRight(Node *node) {
    if (!node || !node->left) return;  // Невозможно выполнить правый поворот
    Node *pivot = node->left;
    node->left = pivot->right;
    if (pivot->right) pivot->right->parent = node;
    pivot->parent = node->parent;
    if (!node->parent)
      root_ = pivot;
    else if (node == node->parent->left)
      node->parent->left = pivot;
    else
      node->parent->right = pivot;
    pivot->right = node;
    node->parent = pivot;
  }

  // Восстановление свойств красно-черного дерева
  void fixInsertion(Node *node) {
    while (node != root_ && node->parent->color == Color::RED)
      if (node->parent == node->parent->parent->left) {
        Node *uncle = node->parent->parent->right;
        if (uncle && uncle->color == Color::RED) {
          // Случай 1: Дядя тоже красный
          node->parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          node->parent->parent->color = Color::RED;
          node = node->parent->parent;
        } else {
          if (node == node->parent->right) {
            // Случай 2: Дядя черный, и текущий узел - правый ребенок
            node = node->parent;
            rotateLeft(node);
          }
          // Случай 3: Дядя черный, и текущий узел - левый ребенок
          node->parent->color = Color::BLACK;
          node->parent->parent->color = Color::RED;
          rotateRight(node->parent->parent);
        }
      } else {
        // Аналогичные случаи, но симметрично для правой стороны
        Node *uncle = node->parent->parent->left;
        if (uncle && uncle->color == Color::RED) {
          node->parent->color = Color::BLACK;
          uncle->color = Color::BLACK;
          node->parent->parent->color = Color::RED;
          node = node->parent->parent;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            rotateRight(node);
          }
          node->parent->color = Color::BLACK;
          node->parent->parent->color = Color::RED;
          rotateLeft(node->parent->parent);
        }
      }
    root_->color = Color::BLACK;
  }

  /* Преемник - это узел, содержащий наименьшее значение, которое больше
    значения узла node. */
  Node *successor(Node *node) const {
    Node *current = node->right;
    while (current->left) current = current->left;

    return current;
  }

  /* Метод для исправления дерева после удаления узла.
    Входной узел должен быть красным или nullptr (черным). */
  void fixDeletion(Node *node) {
    while (node != root_ && getColor(node) == Color::BLACK) {
      if (node == node->parent->left) {
        Node *sibling = node->parent->right;
        if (getColor(sibling) == Color::RED) {
          sibling->color = Color::BLACK;
          node->parent->color = Color::RED;
          rotateLeft(node->parent);
          sibling = node->parent->right;
        }
        if (getColor(sibling->left) == Color::BLACK &&
            getColor(sibling->right) == Color::BLACK) {
          sibling->color = Color::RED;
          node = node->parent;
        } else {
          if (getColor(sibling->right) == Color::BLACK) {
            sibling->left->color = Color::BLACK;
            sibling->color = Color::RED;
            rotateRight(sibling);
            sibling = node->parent->right;
          }
          sibling->color = node->parent->color;
          node->parent->color = Color::BLACK;
          sibling->right->color = Color::BLACK;
          rotateLeft(node->parent);
          node = root_;
        }
      } else {
        Node *sibling = node->parent->left;  // Правый ребенок своего родителя
        if (getColor(sibling) == Color::RED) {
          sibling->color = Color::BLACK;
          node->parent->color = Color::RED;
          rotateRight(node->parent);
          sibling = node->parent->left;
        }
        if (getColor(sibling->left) == Color::BLACK &&
            getColor(sibling->right) == Color::BLACK) {
          sibling->color = Color::RED;
          node = node->parent;
        } else {
          if (getColor(sibling->left) == Color::BLACK) {
            sibling->right->color = Color::BLACK;
            sibling->color = Color::RED;
            rotateLeft(sibling);
            sibling = node->parent->left;
          }
          sibling->color = node->parent->color;
          node->parent->color = Color::BLACK;
          sibling->left->color = Color::BLACK;
          rotateRight(node->parent);
          node = root_;
        }
      }
    }
    node->color = Color::BLACK;
  }

  // Копирование всего дерева
  Node *copyTree(Node *node) {
    if (!node) return nullptr;

    Node *newNode = new (std::nothrow) Node(node->data, node->color);
    if (!newNode) {
      std::cerr << "Memory allocation failed." << std::endl;
      return nullptr;
    }

    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    if (newNode->left) newNode->left->parent = newNode;
    if (newNode->right) newNode->right->parent = newNode;

    return newNode;
  }

  // Получения цвета узла
  Color getColor(Node *node) {
    if (node == nullptr)
      return Color::BLACK;  // возвращаем черный цвет для пустых (null) узлов

    return node->color;  // возвращаем цвет данного узла
  }

  // Количество узлов в поддереве
  int size(Node *node) const {
    if (node == nullptr)
      return 0;
    else
      return size(node->left) + 1 + size(node->right);
  }
};

}  // namespace S21
#endif  // CONTAINERS_SRC_RBTREE_H_
