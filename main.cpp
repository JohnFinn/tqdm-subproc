#include <boost/iterator/transform_iterator.hpp>
#include <boost/process.hpp>
#include <chrono>
#include <cstddef>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <thread>

namespace bp = boost::process;

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

void baz() {
  tqdm_proc foo(100);
  for (int i = 0; i < 100; ++i) {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    foo.tick();
  }
}

int main() {
  bp::opstream in;
  baz();
}
