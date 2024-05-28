#include "Object.h"
#include <experimental/random>
#include <iostream>

Task *generateTask(int n, Container<Task *> *con) {
  Task *result;
  std::uniform_real_distribution<double> unif(-10000, 10000);
  std::default_random_engine re;

  int k = std::experimental::randint(0, 3);
  BinaryOperator op = (k == 0) ? ADD : (k == 1) ? SUB : (k == 2) ? MUL : DIV;
  switch (n) {
  case 0:
    result = new BinaryOperation(op, unif(re), unif(re), "BinaryTask_" + std::to_string(k));
    break;
  case 1:
    result = new AddToContainerTask(
        con, generateTask(std::experimental::randint(0, 4), con));
    break;
  case 2:
    result = new CountObjectsInContainerTask(con);
    break;
  case 3:
    result = new CountTasksWithResultTask(con);
    break;
  default:
    result = new CountObjectsTask();
    break;
  }

  return result;
}

int main() {
  Container<Task *> *container = new Container<Task *>();
  int n = std::experimental::randint(15, 20);
  for (int i = 0; i < n; ++i) {
    container->pushBack(
        generateTask(std::experimental::randint(0, 4), container));
  }
  Container<std::string> *results = new Container<std::string>();

  std::cout << "Amount of objects in program: " << Object::getObjectCount() << '\n';
  Task *t;
  while (!container->isEmpty()) {
    t = container->peekFront();
    t->execute();
    results->pushBack(t->toString());
    container->removeFront();
    delete t;
  }
  uint64_t l = results->getLength();
  for (uint64_t i = 0; i < l; ++i) {
    std::cout << results->peekFront() << '\n';
    results->removeFront();
  }
  results->clear();

  std::cout << "Amount of objects in program: " << Object::getObjectCount() << '\n';

  return 0;
}