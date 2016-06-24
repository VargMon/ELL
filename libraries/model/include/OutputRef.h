////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     OutputRef.h (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Port.h"
#include "OutputPort.h"
#include "Node.h"

#include <vector>
#include <cassert>

/// <summary> model namespace </summary>
namespace model
{
    /// <summary> Represents a reference to a single element in another node's output </summary>
    class ElementRef
    {
    public:
        /// <summary> Creates an ElementRef representing a single value from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="index"> The index of the value </param>
        ElementRef(const Port* port, size_t index);

        /// <summary> Returns the type of the values referenced </summary>
        ///
        /// <returns> The type of the values referenced </returns>
        Port::PortType GetType() const { return _referencedPort->GetType(); }

        /// <summary> The index of the element this range refers to </summary>
        ///
        /// <returns> The index of the element this range refers to </returns>
        size_t GetIndex() const { return _index; }

        /// <summary> The port this range refers to </summary>
        ///
        /// <returns> The port this range refers to </returns>
        const Port* ReferencedPort() const { return _referencedPort; }

    private:
        const Port* _referencedPort;
        size_t _index;
    };
    
    /// <summary> Represents a contiguous set of values from an output port </summary>
    class OutputRange
    {
    public:
        /// <summary> Creates an OutputRange representing all the values from a given port </summary>
        ///
        /// <param name="port"> The port to take values from </param>
        OutputRange(const Port& port);

        /// <summary> Creates an OutputRange representing a single value from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="index"> The index of the value </param>
        OutputRange(const Port& port, size_t index);

        /// <summary> Creates an OutputRange representing a range of values from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="startIndex"> The index of the first value to take </param>
        /// <param name="numValues"> The number of values to take </param>
        OutputRange(const Port& port, size_t startIndex, size_t numValues);

        /// <summary> Returns the type of the values referenced </summary>
        ///
        /// <returns> The type of the values referenced </returns>
        Port::PortType GetType() const { return _referencedPort->GetType(); }

        /// <summary> The dimensionality of the output </summary>
        ///
        /// <returns> The dimensionality of the output </returns>
        size_t Size() const;

        /// <summary> The index of the first element this range refers to </summary>
        ///
        /// <returns> The index of the first element this range refers to </returns>
        size_t GetStartIndex() const;

        /// <summary> The port this range refers to </summary>
        ///
        /// <returns> The port this range refers to </returns>
        const Port* ReferencedPort() const { return _referencedPort; }

    private:
        const Port* _referencedPort;
        size_t _startIndex;
        size_t _numValues;
        bool _isFixedSize;
    };

    /// <summary> Represents a set of values from one or more output ports </summary>
    class UntypedOutputRef
    {
    public:
        /// <summary> Creates an UntypedOutputRef representing all the values from a given port </summary>
        ///
        /// <param name="port"> The port to take values from </param>
        UntypedOutputRef(const Port& port);

        /// <summary> Creates an UntypedOutputRef representing a single value from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="index"> The index of the value </param>
        UntypedOutputRef(const Port& port, size_t startIndex);

        /// <summary> Creates an UntypedOutputRef representing a range of values from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="startIndex"> The index of the first value to take </param>
        /// <param name="numValues"> The number of values to take </param>
        UntypedOutputRef(const Port& port, size_t startIndex, size_t numValues);

        /// <summary> Creates an UntypedOutputRef from an OutputRange </summary>
        ///
        /// <param name="range"> The range to get values from </param>
        UntypedOutputRef(const OutputRange& range);

        /// <summary> Creates an UntypedOutputRef from a set of OutputRange </summary>
        ///
        /// <param name="range"> The ranges to get values from </param>
        UntypedOutputRef(const std::vector<OutputRange>& ranges);

        /// <summary> Returns a single-element range for the element at the given index </summary>
        ///
        /// <param name="index"> The index of the element </param>
        /// <returns> A single-element range for the element </returns>
        OutputRange GetElementRef(size_t index) const;

        /// <summary> The dimensionality of the output </summary>
        ///
        /// <returns> The dimensionality of the output </returns>
        size_t Size() const { return _size; }

        /// <summary> An STL-type iterator pointing to the beginning of the list of ranges </summary>
        std::vector<OutputRange>::const_iterator begin() const { return _ranges.cbegin(); }

        /// <summary> An STL-type iterator pointing to the end of the list of ranges </summary>
        std::vector<OutputRange>::const_iterator end() const { return _ranges.cend(); }

    protected:
        UntypedOutputRef(){};
        void ComputeSize();
        void AddRange(const OutputRange& range);
    
    private:
        std::vector<OutputRange> _ranges;
        size_t _size = 0;
    };

    /// <summary> Represents a statically-typed set of values from one or more output ports </summary>
    template <typename ValueType>
    class OutputRef : public UntypedOutputRef
    {
    public:
        /// <summary> Creates a OutputRef representing all the values from a given port </summary>
        ///
        /// <param name="port"> The port to take values from </param>
        OutputRef(const OutputPort<ValueType>& port);

        /// <summary> Creates a OutputRef representing a single value from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="index"> The index of the value </param>
        OutputRef(const OutputPort<ValueType>& port, size_t startIndex);

        /// <summary> Creates a OutputRef representing a range of values from a given port </summary>
        ///
        /// <param name="port"> The port to take a value from </param>
        /// <param name="startIndex"> The index of the first value to take </param>
        /// <param name="numValues"> The number of values to take </param>
        OutputRef(const OutputPort<ValueType>& port, size_t startIndex, size_t numValues);

        /// <summary> Creates a OutputRef by concatenating a set of them together </summary>
        ///
        /// <param name="groups"> The list of groups to concantenate together </param>
        OutputRef(const std::initializer_list<OutputRef<ValueType>>& groups);

        /// <summary> Creates a OutputRef by concatenating a set of them together </summary>
        ///
        /// <param name="groups"> The list of groups to concantenate together </param>
        OutputRef(const std::vector<OutputRef<ValueType>>& groups);
    };

    //
    // Helper functions
    //

    /// <summary> Creates a OutputRef representing all the values from a given port </summary>
    ///
    /// <param name="port"> The port to take values from </param>
    /// <returns> The composite OutputRef </returns>
    template <typename ValueType>
    OutputRef<ValueType> MakeRef(const OutputPort<ValueType>& port);

    /// <summary> Creates a OutputRef representing a single value from a given port </summary>
    ///
    /// <param name="port"> The port to take a value from </param>
    /// <param name="index"> The index of the value </param>
    /// <returns> The composite OutputRef </returns>
    template <typename ValueType>
    OutputRef<ValueType> MakeRef(const OutputPort<ValueType>& port, size_t startIndex);

    /// <summary> Creates a OutputRef representing a range of values from a given port </summary>
    ///
    /// <param name="port"> The port to take a value from </param>
    /// <param name="startIndex"> The index of the first value to take </param>
    /// <param name="numValues"> The number of values to take </param>
    /// <returns> The composite OutputRef </returns>
    template <typename ValueType>
    OutputRef<ValueType> MakeRef(const OutputPort<ValueType>& port, size_t startIndex, size_t numValues);

    /// <summary> Creates a OutputRef by concatenating together one or more OutputRefs
    ///
    /// <param name="ref"> The OutputRefs to concatenate together </param>
    /// <returns> The composite OutputRef </returns>
    template <typename RefType, typename... Refs>
    RefType Concat(const RefType& ref1, Refs&&... refs);
}

#include "../tcc/OutputRef.tcc"
