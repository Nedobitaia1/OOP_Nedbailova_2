#include <string>
class MyError {
private:
  std::string messege;

public:
  MyError(const std::string &message) : messege{message} {}
  std::string what() const { return messege; }
};

template <typename T> class Container {

  struct node {
    struct node *prev;
    struct node *next;
    T val;
  };

public:
  Container();
  ~Container();
  bool isEmpty() const;
  uint64_t getLength() const;
  void pushFront(const T &value);
  void pushBack(const T &value);
  const T &peekFront() const;
  const T &peekBack() const;
  void removeFront();
  void removeBack();
  void swap(Container<T> &c);
  void reverce();
  void clear();

  class Iterator {
  private:
    node *head;

  public:
    Iterator(node *head) { this->head = head; }

    Iterator operator++(int) {
      Iterator itr = *this;
      head = head->next;
      return itr;
    }

    T &operator*() { return head->val; }

    bool operator==(const Iterator &itr) { return head == itr.head; }

    bool operator!=(const Iterator &itr) { return (head != itr.head); }
  };

  Iterator first() const { return Iterator(head); };
  Iterator last() const { return Iterator(nullptr); };

private:
  node *head;
  node *end;
  uint64_t length;
};

template <typename T> Container<T>::Container() {
  this->length = 0;
  this->head = nullptr;
  this->end = nullptr;
};

template <typename T> Container<T>::~Container() { clear(); }

template <typename T> void Container<T>::pushBack(const T &value) {
  ++this->length;
  if (this->head == nullptr) {
    this->head = new node{nullptr, nullptr, value};
    this->end = head;
  } else {
    this->end->next = new node{end, nullptr, value};
    this->end = this->end->next;
  }
};

template <typename T> void Container<T>::pushFront(const T &value) {
  ++this->length;
  if (this->head == nullptr) {
    this->head = new node{nullptr, nullptr, value};
    this->end = head;
  } else {
    this->head = new node{nullptr, this->head, value};
  }
}

template <typename T> const T &Container<T>::peekFront() const {
  if (head == nullptr) {
    throw MyError("Container is empty, nothing to peek");
  }
  return this->head->val;
};

template <typename T> const T &Container<T>::peekBack() const {
  if (head == nullptr) {
    throw MyError("Container is empty, nothing to peek");
  }
  return this->end->val;
};

template <typename T> void Container<T>::removeBack() {
  if (head == nullptr) {
    throw MyError("Container is empty, nothing to remove");
  }
  if (!this->isEmpty()) {
    struct node *tmp = this->end->prev;
    delete this->end;
    this->end = tmp;
    this->end->next = nullptr;
    --this->length;
    if (this->end == nullptr)
      this->head = nullptr;
  }
};

template <typename T> void Container<T>::removeFront() {
  if (head == nullptr) {
    throw MyError("Container is empty, nothing to remove");
  }
  if (!this->isEmpty()) {
    struct node *tmp = this->head->next;
    delete this->head;
    this->head = tmp;
    if (this->head == nullptr)
      this->end = head;
    else {
      this->head->prev = nullptr;
    }
    --this->length;
  }
};

template <typename T> void Container<T>::swap(Container<T> &c) {
  struct node *tmpHead = this->head;
  struct node *tmpEnd = this->end;
  this->head = c.head;
  this->end = c.end;
  c.head = tmpHead;
  c.end = tmpEnd;
};

template <typename T> void Container<T>::reverce() {
  struct node *cursor = head;
  struct node *nextOld;
  struct node *prevOld;
  while (cursor != nullptr) {
    nextOld = cursor->next;
    prevOld = cursor->prev;
    cursor->head = prevOld;
    cursor->prev = nextOld;
    cursor = nextOld;
  }
  cursor = end;
  end = head;
  head = cursor;
};

template <typename T> bool Container<T>::isEmpty() const {
  return this->length == 0;
};

template <typename T> uint64_t Container<T>::getLength() const {
  return this->length;
};

template <typename T> void Container<T>::clear() {
  while (!isEmpty()) {
    this->removeFront();
  }
};
