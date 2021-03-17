#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <exception>
#include "SerialPort.h"

namespace sptcore
{
    struct CMDOutput {};

    template <class Output, class Error = Output>
    struct Context
    {
    public:
        const Output output {};
        const Error error {};
        virtual const BasicSerialPort* port() const = 0;
    };

    template<class TPort, class Output, class Error = Output>
    struct SPTContext : public Context<Output, Error>
    {
    public:
        SPTContext() : serialPort() {}
        virtual const BasicSerialPort* port() const override { return &serialPort; }
    private:
        TPort serialPort;
    };

    template <class T>
    const CMDOutput& operator<<(const CMDOutput& output, const T* text)
    {
        std::cout << text;
        return output;
    }

    template <class T>
    const CMDOutput& operator<<(const CMDOutput& output, const T& text)
    {
        std::cout << text;
        return output;
    }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
    typedef CoutType& (*StandardManipulator)(CoutType&);

    const CMDOutput& operator<<(const CMDOutput& output, StandardManipulator manipulator)
    {
        std::cout << manipulator;
        return output;
    }
}