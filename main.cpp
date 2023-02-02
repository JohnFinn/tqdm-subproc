#include <boost/iterator/transform_iterator.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <cstddef>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <thread>

namespace bp = boost::process;

class WaitingChild : public bp::child {
public:
  using bp::child::child;

  ~WaitingChild() { this->wait(); }
};

class tqdm_proc {
public:
  tqdm_proc(std::size_t total)
      : _in(),
        _child(fmt::format("tqdm --null --total {}", total), bp::std_in < _in) {

  }

  void tick() { _in << std::endl; }

  ~tqdm_proc() {
    _in.pipe().close();
    _child.wait();
  }

private:
  bp::opstream _in;
  bp::child _child;
};

// TODO call _get_func only in operator++
// TODO make work with unknown std::distance
// TODO make it only forward iterable
// TODO make it only once iterable
template <typename tIter> class tqdm {
public:
  tqdm(tIter begin, tIter end)
      : _begin(std::move(begin)), _end(std::move(end)),
        _tqdm_proc(std::distance(_begin, _end)) {}

  auto begin() {
    return boost::make_transform_iterator(_begin, this->_get_func());
  }

  auto end() { return boost::make_transform_iterator(_end, this->_get_func()); }

private:
  tIter _begin;
  tIter _end;

  tqdm_proc _tqdm_proc;

  auto _get_func() {
    return [this](const auto &value) {
      this->_tqdm_proc.tick();
      return value;
    };
  }
};

void baz() {
  std::vector<int> foo(100, 100);
  for (auto i : tqdm(foo.begin(), foo.end())) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  bp::opstream in;
  baz();
}
