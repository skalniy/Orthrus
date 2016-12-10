#include "router_controller.hpp"
#include <chrono>
#include <thread>



int main(int argc, char const *argv[])
{
    RouterController s;
    s.start();
    std::this_thread::sleep_for(std::chrono::seconds(300));
    s.stop();
    return 0;
}
