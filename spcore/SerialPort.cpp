#include "SerialPort.h"
#include <boost\asio\buffer.hpp>
#include <boost\array.hpp>


namespace sptcore
{
    void SerialPort::open(const std::string &portName) const
    {
        port.open(portName);
    }

    void SerialPort::write(const std::string_view data) const
    {
        port.write_some(boost::asio::buffer(data));
    }

    std::string SerialPort::read() const
    {
        boost::array<char, 256> buffer;
        auto dataSize = port.read_some(boost::asio::buffer(buffer));
        return std::string(buffer.data(), dataSize).c_str();
    }

    void SerialPort::close() const
    {
        port.close();
    }
}