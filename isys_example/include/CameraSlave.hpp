#include <lely/io2/sys/io.hpp>
#include <lely/io2/sys/sigset.hpp>
#include <lely/io2/sys/timer.hpp>
#include <lely/coapp/slave.hpp>
#include <iostream>
#include <bits/stdc++.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using namespace lely;


class CameraSlave : public canopen::BasicSlave
{
public:
    using BasicSlave::BasicSlave;
    void initialize(){
        std::cout << "registering functions!\n";
        _camera_thread = std::thread([this](){this->worker();});
    }

protected:


private:
    int _x_accel_address;
    std::thread _camera_thread;
    void worker(){
        while(true)
        {
            using namespace std::chrono_literals;
            uint32_t x = std::rand();
            Write(0x4001, 0, x);
            std::cout << x << '\n' << std::flush;
            std::this_thread::sleep_for(100ms);
        }
    }
};