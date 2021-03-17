#pragma once

#include <boost/asio.hpp>
#include <string>
#include <string_view>

namespace sptcore
{
    class BasicSerialPort
    {
    public:
        virtual void open(const std::string &portName) const = 0;
        virtual void write(const std::string_view data) const = 0;
        virtual std::string read() const = 0;
        virtual void close() const = 0;
    };

    class SerialPort : public BasicSerialPort
    {
    public:
        SerialPort() :
            service(),
            port(service)
        {};
        virtual ~SerialPort() = default;
        void open(const std::string &portName) const override;
        void write(const std::string_view data) const override;
        std::string read() const override;
        void close() const override;
    private:
        boost::asio::io_service service;
        mutable boost::asio::serial_port port;
    };
} // namespace sptcore