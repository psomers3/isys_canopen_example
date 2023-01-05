#include "CameraSlave.hpp"
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

void CameraSlave::initialize(){
    std::cout << "\nStarting Camera!\n";
    _last_value_read = false;
    _read_value_idx = 0x4000;
    _write_value_idx = 0x4001;
    _value_subidx = 0;
    _camera_thread = std::thread([this](){this->worker();});
    std::function<OnReadSignature<uint32_t>> f = [this](auto idx, auto subidx, auto val) -> std::error_code {return this->on_read();};
    // register the lambda function above to be called when the value is read
    OnRead(_write_value_idx, _value_subidx, f);  
}

std::error_code CameraSlave::on_read() {
    std::cout << "read value\n";
    std::lock_guard<std::mutex> lock(_value_mutex);
    _last_value_read = true;
    return std::error_code{};
}

void CameraSlave::worker(){
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);  // wait a sec before starting (not necessary)

    // set up camera stuff ... Should eventually be done elsewhere
    // for cleaner management (shutdown, startup, etc...) 
    cv::Mat frame;
    cv::VideoCapture cap;


    while(true)
    {
        
        uint32_t x = std::rand();
        {
            // do your locking with a lock_guard inside a scope
            // instead of manually "lock" and "unlock"
            // so that if an exception or something is thrown,
            // the mutex is automatically release.
            std::lock_guard<std::mutex> lock(_value_mutex);
            _last_value_read = false;
        }
        Write(_write_value_idx, 0, x);
        std::cout << "waiting\n"; 

        // wait until the master has read the value
        while (!_last_value_read)
        {
            // slight pause to give the cpu a break.
            std::this_thread::sleep_for(50ms);
        }
    }
}