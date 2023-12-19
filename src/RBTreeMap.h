#ifndef ___RBTREEMAP_H___
#define ___RBTREEMAP_H___

#include <iostream>
#include <limits>
#include <memory>

namespace s21 {

template <typename K, typename V>
class RBTreeMap {
  typedef K key_type;
  typedef V mapped_type;
  typedef std::pair<const key_type, mapped_type> value_type;

  // strongly typed enum
  enum class Color { RED, BLACK };
  // Часть приватных методов необходимых в начале класса
 private:
  struct Node {
    value_type data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(value_type val, Color c)
        : data(val), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
  };
  Node* root;

 public:
  class iterator;
  class const_iterator;

  // Конструктор
  RBTreeMap() : root(nullptr) {}

  // Деструктор
  ~RBTreeMap() { clearTree(root); }

  // Конструктор копирования
  RBTreeMap(const RBTreeMap& other) : root(nullptr) {
    root = copyTree(other.root);
  }

  // Оператор присваивания копированием
  RBTreeMap& operator=(const RBTreeMap& other) {
    if (this != &other) {
      clearTree(root);
      root = copyTree(other.root);
    }
    return *this;
  }

  // Конструктор перемещения
  RBTreeMap(RBTreeMap&& other) noexcept : root(other.root) {
    other.root = nullptr;
  }

  void setData(const value_type& value) {
    Node* nodeset = findNode(root, value.first);
    nodeset->data.second = value.second;
  }

  // Оператор присваивания перемещением
  RBTreeMap& operator=(RBTreeMap&& other) noexcept {
    if (this != &other) {
      clearTree(root);
      root = other.root;
      other.root = nullptr;
    }
    return *this;
  }

  // Оператор равенства проверяет, равны ли два дерева. Два дерева считаются
  // равными, если они имеют одинаковую структуру и одинаковые значения в каждом
  // узле.
  bool operator==(const RBTreeMap& other) const {
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

  // Оператор неравенства проверяет, не равны ли два дерева. Он просто
  // возвращает отрицание результата оператора равенства.
  bool operator!=(const RBTreeMap& other) const { return !(*this == other); }

  // Оператор "меньше". Он проверяет, является ли одно дерево "меньше" другого.
  // Дерево считается "меньше", если его первый отличающийся элемент меньше
  // соответствующего элемента другого дерева, или если все совпадающие элементы
  // одинаковы, но дерево короче.
  bool operator<(const RBTreeMap& other) const {
    iterator it1 = begin();
    iterator it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
      if (*it1 != *it2) return *it1 < *it2;
      ++it1;
      ++it2;
    }

    // Если оба дерева равны до тех пор, пока одно из них не закончится, то
    // более короткое дерево меньше.
    return (it1 == end() && it2 != other.end());
  }

  // Оператор "меньше или равно". Он возвращает true, если одно дерево меньше
  // другого или если они равны.
  bool operator<=(const RBTreeMap& other) const {
    return *this == other || *this < other;
  }

  // Оператор "больше". Он возвращает true, если одно дерево не меньше и не
  // равно другому.
  bool operator>(const RBTreeMap& other) const { return !(*this <= other); }

  // Оператор "больше или равно". Он возвращает true, если одно дерево больше
  // другого или если они равны.
  bool operator>=(const RBTreeMap& other) const {
    return *this == other || *this > other;
  }

  // Методы для основных операций с деревом
  // Метод для вставки нового значения в дерево
  // iterator insert(key_type key, value_type val) {
  //  return insert(std::make_pair(key, val));
  //}

  iterator insert(value_type value) {
    // Выделение памяти без вызова исключенй
    Node* newNode = new (std::nothrow) Node(value, Color::RED);
    // В случае не выделения проверяем на nullptr
    if (!newNode) {
      std::cerr << "Memory allocation failed." << std::endl;
      return end();  // или другое особое значение
    }

    // Вставка как в обычное бинарное дерево
    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr) {
      parent = current;
      if (value.first < current->data.first)
        current = current->left;
      else
        current = current->right;
    }
    newNode->parent = parent;
    if (parent == nullptr)
      root = newNode;
    else if (value.first < parent->data.first)
      parent->left = newNode;
    else
      parent->right = newNode;
    // Вызываем метод для восстановления свойств красно-черного дерева
    fixInsertion(newNode);
    return iterator(newNode);
  }

  // ключа Метод для удаления значения из дерева
  void remove(key_type value) {
    // Находим узел, содержащий значение для удаления
    Node* node = findNode(root, value);
    if (!node) return;  // Узел не найден, ничего не делаем

    // Находим узел, который фактически будет удален из дерева
    Node* nodeToRemove = (node->left && node->right) ? successor(node) : node;

    // Получаем ссылку на дочерний узел (не нулевой), если он есть
    Node* child = nodeToRemove->left ? nodeToRemove->left : nodeToRemove->right;

    // Устанавливаем родителю дочернего узла ссылку на правильное поддерево
    if (child) child->parent = nodeToRemove->parent;

    // Удаляем узел из дерева
    if (!nodeToRemove->parent)
      root = child;
    else if (nodeToRemove == nodeToRemove->parent->left)
      nodeToRemove->parent->left = child;
    else
      nodeToRemove->parent->right = child;

    // Если удаляемый узел был черного цвета, может быть нарушено свойство 4
    if (nodeToRemove->color == Color::BLACK && child) fixDeletion(child);

    delete nodeToRemove;
  }

  // Метод для поиска значения в дереве
  iterator find(key_type value) const {
    Node* node = findNode(root, value);
    if (node) {
      return iterator(node);
    } else {
      return end();
    }
  }

  // Метод для печати значений дерева в порядке возрастания
  void print() const {
    inOrderTraversal(root);
    std::cout << std::endl;
  }

  // Метод для получения размера дерева
  int size() const { return size(root); }

  // Метод проверки на пустое дерево
  bool isEmpty() const { return root == nullptr; };

  // Метод для очистки дерева. Он удаляет все узлы в дереве.
  void clear() {
    clearTree(root);
    root = nullptr;
  }

  // Возвращает итератор, указывающий на узел с минимальным значением в дереве.
  // Этот узел является первым узлом в порядке возрастания.
  iterator begin() {
    Node* current = root;
    while (current && current->left) current = current->left;
    return iterator(current);
  }

  // Возвращает итератор, указывающий на следующий узел после последнего узла
  // в дереве.
  iterator end() const { return iterator(nullptr); }

  // Метод для подсчета количества элементов с заданным ключом
  size_t count(const key_type& key) const { return countKeys(root, key); }

  // обменивает корневые узлы двух деревьев
  void swap(RBTreeMap& other) { std::swap(root, other.root); }

  // возвращающает максимально возможный размер для этого дерева
  size_t max_size() const { return std::numeric_limits<size_t>::max(); }

  // Объединение
  void merge(RBTreeMap<key_type, mapped_type>& otherTree) {
    // Перебираем все элементы в другом дереве и вставляем их в текущее дерево
    for (auto it = otherTree.begin(); it != otherTree.end(); ++it)
      this->insert(*it);
    otherTree.clear();
  }

  // поиск с корневого узла и движется влево или вправо в зависимости от
  // значения ключа
  bool contains(const key_type& key) const {
    Node* current = root;
    while (current) {
      if (key < current->data.first) {
        current = current->left;
      } else if (key == current->data.first) {
        return true;
      } else {
        current = current->right;
      }
    }
    return false;
  }

  std::pair<iterator, iterator> equal_range(const key_type& key) const {
    Node* current = root;
    iterator first = end();
    iterator second = end();

    while (current)
      if (key < current->data.first)
        current = current->left;
      else if (key == current->data.first) {
        first = iterator(current);
        current = current->left;
      } else
        current = current->right;

    current = root;
    while (current)
      if (key >= current->data.first)
        current = current->right;
      else {
        second = iterator(current);
        current = current->left;
      }

    return {first, second};
  }

  class iterator {
    friend class RBTreeMap;

   private:
    Node* node;

   public:
    iterator(Node* node = nullptr) : node(node) {}

    // Перегрузка оператора разыменования
    value_type& operator*() { return node->data; }

    // Перегрузка оператора '->'
    value_type* operator->() { return &(node->data); }

    // Перегрузка оператора '=='
    bool operator==(const iterator& other) const { return node == other.node; }

    // Перегрузка оператора '!='
    bool operator!=(const iterator& other) const { return node != other.node; }

    // Перегрузка оператора '++' для префиксного инкремента
    // Итерация по возрастанию
    iterator& operator++() {
      if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
      } else {
        Node* parent = node->parent;
        while (parent && node == parent->right) {
          node = parent;
          parent = node->parent;
        }
        node = parent;
      }
      return *this;
    }
    // Перегрузка оператора '++' для постфиксного инкремента
    // Итерация по возрастанию
    iterator operator++(int) {
      iterator old = *this;
      ++(*this);
      return old;
    }
  };
  class const_iterator {
    friend class RBTreeMap;

   private:
    const Node* node;

   public:
    const_iterator(const Node* node = nullptr) : node(node) {}

    // Перегрузка оператора разыменования
    const value_type& operator*() const { return node->data; }

    // Перегрузка оператора '->'
    const value_type* operator->() const { return &(node->data); }

    // Перегрузка оператора '=='
    bool operator==(const const_iterator& other) const {
      return node == other.node;
    }

    // Перегрузка оператора '!='
    bool operator!=(const const_iterator& other) const {
      return node != other.node;
    }

    // Перегрузка оператора '++' для префиксного инкремента
    // Итерация по возрастанию
    const_iterator& operator++() {
      if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
      } else {
        const Node* parent = node->parent;
        while (parent && node == parent->right) {
          node = parent;
          parent = node->parent;
        }
        node = parent;
      }
      return *this;
    }

    // Перегрузка оператора '++' для постфиксного инкремента
    // Итерация по возрастанию
    const_iterator operator++(int) {
      const_iterator old = *this;
      ++(*this);
      return old;
    }
  };

 private:
  // Вспомогательная рекурсивная функция для подсчета ключей
  size_t countKeys(const Node* node, const key_type& key) const {
    if (node == nullptr) return 0;

    size_t cnt = 0;

    // Если текущий узел совпадает с ключом, увеличиваем счетчик
    if (node->data.first == key) ++cnt;

    // Рекурсивно обходим левое и правое поддерево
    cnt += countKeys(node->left, key);
    cnt += countKeys(node->right, key);

    return cnt;
  }
  // Рекурсивный метод для обхода дерева в порядке возрастания
  void inOrderTraversal(Node* node, std::vector<value_type>& result) const {
    if (node == nullptr) return;

    inOrderTraversal(node->left, result);
    result.push_back(node->data);
    inOrderTraversal(node->right, result);
  }

  // Метод для поиска узла по значению в поддереве с корнем node.
  // Возвращает найденный узел или nullptr, если узел с таким значением не
  // найден.
  Node* findNode(Node* node, key_type key) const {
    if (node == nullptr || node->data.first == key) return node;
    if (key < node->data.first)
      return findNode(node->left, key);
    else
      return findNode(node->right, key);
  }

  // Метод для рекурсивного обхода дерева и печати значений
  void inOrderTraversal(Node* node) const {
    if (node) {
      inOrderTraversal(node->left);
      std::cout << node->data.first << " " << node->data.second;
      inOrderTraversal(node->right);
    }
  }

  // Метод для освобождения памяти и удаления всех узлов в дереве
  void clearTree(Node* node) {
    if (node) {
      clearTree(node->left);
      clearTree(node->right);
      delete node;
    }
  }

  // Левое малое вращение
  void rotateLeft(Node* node) {
    if (!node || !node->right) return;  // Невозможно выполнить левый поворот
    Node* pivot = node->right;
    node->right = pivot->left;
    if (pivot->left) pivot->left->parent = node;
    pivot->parent = node->parent;
    if (!node->parent)
      root = pivot;
    else if (node == node->parent->left)
      node->parent->left = pivot;
    else
      node->parent->right = pivot;
    pivot->left = node;
    node->parent = pivot;
  }

  // Правое малое вращение
  void rotateRight(Node* node) {
    if (!node || !node->left) return;  // Невозможно выполнить правый поворот
    Node* pivot = node->left;
    node->left = pivot->right;
    if (pivot->right) pivot->right->parent = node;
    pivot->parent = node->parent;
    if (!node->parent)
      root = pivot;
    else if (node == node->parent->left)
      node->parent->left = pivot;
    else
      node->parent->right = pivot;
    pivot->right = node;
    node->parent = pivot;
  }

  // Восстановление свойств красно-черного дерева
  void fixInsertion(Node* node) {
    while (node != root && node->parent->color == Color::RED)
      if (node->parent == node->parent->parent->left) {
        Node* uncle = node->parent->parent->right;
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
        Node* uncle = node->parent->parent->left;
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
    root->color = Color::BLACK;
  }

  // Преемник - это узел, содержащий наименьшее значение, которое больше
  // значения узла node.
  Node* successor(Node* node) const {
    Node* current = node->right;
    while (current->left) {
      current = current->left;
    }
    return current;
  }

  // Метод для исправления дерева после удаления узла.
  // Входной узел должен быть красным или nullptr (черным).
  void fixDeletion(Node* node) {
    while (node != root && getColor(node) == Color::BLACK) {
      if (node == node->parent->left) {
        Node* sibling = node->parent->right;
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
          node = root;
        }
      } else {
        Node* sibling = node->parent->left;  // Правый ребенок своего родителя
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
          node = root;
        }
      }
    }
    node->color = Color::BLACK;
  }

  // Копирование всего дерева
  Node* copyTree(Node* node) {
    if (!node) return nullptr;

    Node* newNode = new (std::nothrow) Node(node->data, node->color);
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
  Color getColor(Node* node) {
    if (node == nullptr) {
      return Color::BLACK;  // возвращаем черный цвет для пустых (null) узлов
    }
    return node->color;  // возвращаем цвет данного узла
  }

  // Количество узлов в поддереве
  int size(Node* node) const {
    if (node == nullptr) {
      return 0;
    } else {
      return size(node->left) + 1 + size(node->right);
    }
  }
};

}  // namespace s21
#endif  // ___RBTREEMAP_H___
