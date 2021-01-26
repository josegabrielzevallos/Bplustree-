// Copyright 2021 Roger Peralta Aranibar
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "b-link.hpp"

std::size_t NUMBER_THREADS = 8;

const std::size_t NUMBER_OPERATIONS = 10;

int MIN_VALUE = 1;
int MAX_VALUE = 10;

std::random_device rd;

class TreeSearcher {
 public:
  TreeSearcher(unsigned int id, EDA::Concurrent::BLinkTree<3, int> *b_link)
      : id_(id), b_link_(b_link) {}

  void operator()() {
    std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
    for (std::size_t i = 0; i < NUMBER_OPERATIONS; ++i) {
      std::cout << distribution(rd) << "\n";//comentar
       b_link_->search(distribution(rd));
    }
  }

 private:
  unsigned int id_;
  EDA::Concurrent::BLinkTree<3, int> *b_link_;
};

void sequential_insert(EDA::Concurrent::BLinkTree<3, int> *b_link) {
  std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
  for (std::size_t i = 0; i < NUMBER_OPERATIONS; ++i) {
    std::cout << distribution(rd) << "\n";//comentar
    b_link->insert(distribution(rd));
  }
}

void run_test() {
  EDA::Concurrent::BLinkTree<3, int> b_link;

  sequential_insert(&b_link);

  std::thread *threads = new std::thread[NUMBER_THREADS];

  for (std::size_t i = 0; i < NUMBER_THREADS; ++i) {
    threads[i] = std::thread(TreeSearcher(i, &b_link));
  }
  for (std::size_t i = 0; i < NUMBER_THREADS; ++i) {
    threads[i].join();
  }
    /*b_link.insert(5);
    b_link.insert(6);
    b_link.insert(69);
    b_link.insert(9);
    b_link.insert(11);
    b_link.insert(19);
    b_link.insert(1);
    b_link.insert(4986513);
    b_link.insert(65);
    b_link.search(16);*/
}

int main() {
  run_test();
  return 0;
}
