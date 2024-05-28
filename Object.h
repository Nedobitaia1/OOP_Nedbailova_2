#include "Container.h"
#include <map>
#include <sstream>
/*------------------------------------Abstract class
 * Object----------------------------------*/

class Object {
private:
  static uint64_t objectCount;

public:
  Object() { objectCount++; }

  virtual ~Object() { objectCount--; }

  static uint64_t getObjectCount() { return objectCount; }

  virtual std::string toString() const = 0;
};
uint64_t Object::objectCount = 0;
// uint64_t Object::objectCount = 0;
/*------------------------------------Abstract classs
 * Task------------------------------------*/
class Task : virtual public Object {
public:
  Task(){};
  virtual bool hasResult() const = 0;
  virtual void execute() = 0;
  virtual std::string toString() const = 0;
};

/*------------------------------------Abstract class
 * Named-----------------------------------*/
class Named : virtual public Object {
protected:
  std::string name;

public:
  Named(const std::string &name) : name{name} {};
  virtual std::string toString() const override { return name; }
};

class ResultTask : public Task {
public:
  ResultTask(){};
  bool hasResult() const override { return true; }
};

class NoResultTask : public Task {
public:
  NoResultTask(){};
  bool hasResult() const override { return false; }
};

enum BinaryOperator { ADD, SUB, MUL, DIV };

/*------------------------------------Class
 * BinaryOperation----------------------------------*/
class BinaryOperation : public ResultTask, public Named {
private:
  double left;
  double right;
  BinaryOperator operation;
  double result;
  void add() { result = left + right; };
  void subtruct() { result = left - right; };
  void mult() { result = left * right; };
  void divide() {
    if (right == 0) {
      throw std::invalid_argument("Cant devide on zero");
    }
    result = left / right;
  };

public:
  BinaryOperation(BinaryOperator op, const double &l, const double &r,
                  const std::string &name)
      : Named{name}, left{l}, right{r}, operation{op}, result{0} {}

  ~BinaryOperation() {}

  void execute() override {
    switch (this->operation) {
    case ADD:
      this->add();
      break;
    case SUB:
      this->subtruct();
      break;
    case MUL:
      this->mult();
      break;
    case DIV:
      this->divide();
      break;
    }
  }

  std::string toString() const override {
    std::string op;
    switch (this->operation) {
    case ADD:
      op = "+";
      break;
    case SUB:
      op = "-";
      break;
    case MUL:
      op = "*";
      break;
    case DIV:
      op = "/";
      break;
    }
    std::stringstream ss;
    ss << "name: " << this->name << "\n"
       << "must culculate " << this->left << op << this->right << "\n"
       << "result: " << this->result;
    return ss.str();
  }
};

/*------------------------------------Class
 * СountObjectsInContainerTask----------------------------------*/
class CountObjectsInContainerTask : public ResultTask {
private:
  int result;
  const Container<Task *> *container;

public:
  CountObjectsInContainerTask(const Container<Task *> *container)
      : container{container} {}
  void execute() override { result = container->getLength(); }

  std::string toString() const override {
    return "Number of objects in container: " + std::to_string(result);
  }
};

/*------------------------------------Class
 * AddToContainerTask--------------------------------*/
class AddToContainerTask : public NoResultTask {
private:
  Container<Task *> *container;
  Task *task;

public:
  AddToContainerTask(Container<Task *> *container, Task *task)
      : container(container), task(task) {}

  void execute() override { container->pushBack(task); }

  std::string toString() const override { return "Task added to container"; }
};

/*------------------------------------Class
 * CountTasksWithResultTask-------------------------*/
class CountTasksWithResultTask : public ResultTask {
private:
  const Container<Task *> *container;
  uint64_t result;

public:
  CountTasksWithResultTask(const Container<Task *> *container)
      : container(container) {}

  void execute() override {
    result = 0;
    for (Container<Task *>::Iterator it = container->first();
         it != container->last(); it++) {
      if ((*it)->hasResult()) {
        ++result;
      }
    }
  }

  std::string toString() const override {
    return "Number of tasks with result: " + std::to_string(result);
  }
};

/*----------------------------------Class
 * DeleteContainerTask--------------------------------*/
class DeleteContainerTask : public NoResultTask {
private:
  Container<Task *> *container;

public:
  DeleteContainerTask(Container<Task *> *cont) : container{cont} {};
  void execute() override {
    while (!container->isEmpty()) {
      Task *t = container->peekFront();
      container->removeFront();
      delete t;
    }
  }

  std::string toString() const override { return "Container has been cleaned"; }
};

/*-----------------------------------Class
 * CountObjectsTask----------------------------------*/
class CountObjectsTask : public ResultTask {
private:
  uint64_t result;

public:
  CountObjectsTask() { result = 0; };
  bool hasResult() const override { return true; };

  void execute() override { result = Object::getObjectCount(); };

  std::string toString() const override {
    if (result == 0) {
      throw MyError("Task was not executed");
    }
    return ("Amount of Objects: " + std::to_string(result));
  }
};
