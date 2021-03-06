////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     SequentialLineIterator.h (data)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// stl
#include <fstream>
#include <memory>
#include <string>

namespace ell
{
namespace data
{
    /// <summary> An iterator that reads a long text line by line. </summary>
    class SequentialLineIterator
    {
    public:
        /// <summary> Constructs a sequential line iterator. </summary>
        ///
        /// <param name="filepath"> The filepath. </param>
        /// <param name="delim"> The delimiter. </param>
        SequentialLineIterator(const std::string& filepath, char delim = '\n');

        SequentialLineIterator(SequentialLineIterator&&) = default;

        SequentialLineIterator(const SequentialLineIterator&) = delete; // this ctor is deleted because a private member of this class cannot be copied

        /// <summary> Returns true if the iterator is currently pointing to a valid iterate. </summary>
        ///
        /// <returns> true if it succeeds, false if it fails. </returns>
        bool IsValid() const { return (_spCurrentLine != nullptr); }

        /// <summary> Proceeds to the Next row. </summary>
        void Next();

        /// <summary> Returns a const reference to the row. </summary>
        ///
        /// <returns> A std::shared_ptr&lt;const std::string&gt; </returns>
        std::shared_ptr<const std::string> Get() const { return _spCurrentLine; }

    private:
        std::shared_ptr<std::string> _spCurrentLine = nullptr;
        std::ifstream _iFStream;
        char _delim;
    };
}
}
