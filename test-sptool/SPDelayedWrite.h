#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "test-defintions.h"
#include <string>

namespace test_sptool
{
    class SPDelayedWrite
    {
    public:
        SPDelayedWrite(std::string portName, int delay) : delay(delay), portName(portName) {}

        void operator() ()
        {
            boost::thread(boost::bind(&SPDelayedWrite::WriteData, this));
        }

        void WriteData()
        {
            boost::asio::io_service service;
            boost::asio::serial_port port { service, portName };
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            port.write_some(boost::asio::buffer("some data\n"));
        }

    private:
        int delay;
        std::string portName;

    };
}