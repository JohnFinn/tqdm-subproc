#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

namespace bp = boost::process;

class WaitingChild : public bp::child {
public:
    using bp::child::child;

    ~WaitingChild() {
        this->wait();
    }
};

int main() {
    bp::opstream in;
    WaitingChild foo("tqdm --null --total 100", bp::std_in < in);
    //bp::child foo("wc", bp::std_in < in);
    for (int i = 0; i < 100; ++i) {
        in << "\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    in.pipe().close();
}
