#pragma once
#include <chrono>
#include <thread>

namespace coppa
{
namespace ow
{
template<typename DeviceToCheck>
bool atomic_connect_wrapper(
        const DeviceToCheck& dev,
        bool& b) // b is for connection failure
{
    std::chrono::milliseconds to(3000);
    static const std::chrono::milliseconds frame(10);

    while(!dev.query_is_connected())
    {
        if(to < std::chrono::milliseconds(0) || !b)
        {
            return false;
        }
        else
        {
            std::this_thread::sleep_for(frame);
            to -= frame;
        }
    }

    return true;
}

template<typename Fun, typename CallbackToCheck>
void atomic_update_wrapper(
        Fun f,
        CallbackToCheck& c)
{
    bool b = false;
    c = [&] () {
        b = true;
    };

    f();

    std::chrono::milliseconds to(3000);
    static const std::chrono::milliseconds frame(10);
    while(!b && to > std::chrono::milliseconds(0))
    {
        std::this_thread::sleep_for(frame);
        to -= frame;
    }
}

}
}
