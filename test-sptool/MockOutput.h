#pragma once

#include "Context.h"
#include <string>
#include <gmock/gmock.h>
#include <sstream>

namespace test_sptool
{
    class MockOutput
    {
    public:
        MOCK_METHOD(const MockOutput&, OutputOperator, (const std::string& message), (const));
    };

    template <class T>
    const MockOutput& operator<<(const MockOutput& output, const T* message)
    {
        std::stringstream ss;
        ss << message;
        return output.OutputOperator(ss.str());
    }

    template <class T>
    const MockOutput& operator<<(const MockOutput& output, const T& message)
    {
        std::stringstream ss;
        ss << message;
        return output.OutputOperator(ss.str());
    }

    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
    typedef CoutType& (*StandardManipulator)(CoutType&);

    const MockOutput& operator<<(const MockOutput& output, StandardManipulator message)
    {
        std::stringstream ss;
        ss << message;
        return output.OutputOperator(ss.str());
    }

}