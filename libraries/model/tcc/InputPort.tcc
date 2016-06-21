////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     InputPort.tcc (model)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary> model namespace </summary>
namespace model
{    
    //
    // InputRange
    //


    //template <typename ValueType>
    //InputRange::InputRange(const OutputRange<ValueType>& input) : referencedPort(input.referencedPort), startIndex(input.startIndex), numValues(input.numValues), isFixedSize(input.isFixedSize)
    //{

    //}


    //template <typename ValueType>
    //InputRange::InputRange(const OutputPort<ValueType>& port) : referencedPort(static_cast<const Port*>(&port)), startIndex(0), numValues(port.Size()), isFixedSize(false)
    //{
    //}

    //
    // InputPort
    //

    //template <typename ValueType>
    //InputPort::InputPort(const class Node* owningNode, size_t portIndex, const OutputPort<ValueType>* output) : Port(owningNode, portIndex, Port::GetTypeCode<ValueType>(), output->Size())
    //{
    //    _inputRanges.emplace_back(InputRange(*output));
 
    //     if (Port::GetTypeCode<ValueType>() != output->Type())
    //    {
    //        throw std::runtime_error("InputPort type doesn't match output");
    //    }
    //}

    template <typename ValueType>
    InputPort::InputPort(const class Node* owningNode, size_t portIndex, const TypedRange<ValueType>& input) : Port(owningNode, portIndex, Port::GetTypeCode<ValueType>(), input.Size())
    {
        _inputRanges.emplace_back(InputRange(input));
    }

    template <typename ValueType>
    InputPort::InputPort(const class Node* owningNode, size_t portIndex, const TypedRanges<ValueType>& input) : Port(owningNode, portIndex, Port::GetTypeCode<ValueType>(), input.Size())
    {
        _inputRanges.insert(_inputRanges.begin(), input.begin(), input.end());
    }

    template <typename ValueType>
    std::vector<ValueType> InputPort::GetValue() const
    {
        size_t size = 0;
        std::vector<ValueType> result;
        for (const auto& range : _inputRanges)
        {
            auto temp = range.referencedPort->Node()->GetOutputValue<ValueType>(range.referencedPort->Index());
            if (range.isFixedSize)
            {
                size += range.referencedPort->Size();
                result.insert(result.end(), temp.begin()+range.startIndex, temp.begin()+range.startIndex+range.numValues);

            }
            else
            {
                size += range.referencedPort->Size();
                result.insert(result.end(), temp.begin(), temp.end());
            }
        }

        return result;
    }
}
