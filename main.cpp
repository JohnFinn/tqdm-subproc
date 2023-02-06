#include <thread>
#include "tqdm_proc.hpp"

void baz() {
  tqdm_proc foo(100);
  for (int i = 0; i < 100; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    foo.tick();
  }
}

int main() {
  baz();
}
