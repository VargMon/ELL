////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     DelayNode.tcc (nodes)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ell
{
namespace nodes
{
    template <typename ValueType>
    DelayNode<ValueType>::DelayNode(const model::PortElements<ValueType>& input, size_t windowSize)
        : CompilableNode({ &_input }, { &_output }), _input(this, input, inputPortName), _output(this, outputPortName, _input.Size()), _windowSize(windowSize)
    {
        auto dimension = input.Size();
        for (size_t index = 0; index < windowSize; ++index)
        {
            _samples.push_back(std::vector<ValueType>(dimension));
        }
    }

    template <typename ValueType>
    DelayNode<ValueType>::DelayNode()
        : CompilableNode({ &_input }, { &_output }), _input(this, {}, inputPortName), _output(this, outputPortName, 0), _windowSize(0)
    {
    }

    template <typename ValueType>
    void DelayNode<ValueType>::Compute() const
    {
        auto lastBufferedSample = _samples[0];
        _samples.push_back(_input.GetValue());
        _samples.erase(_samples.begin());
        _output.SetOutput(lastBufferedSample);
    };

    template <typename ValueType>
    void DelayNode<ValueType>::Copy(model::ModelTransformer& transformer) const
    {
        auto newPortElements = transformer.TransformPortElements(_input.GetPortElements());
        auto newNode = transformer.AddNode<DelayNode<ValueType>>(newPortElements, _windowSize);
        transformer.MapNodeOutput(output, newNode->output);
    }

    template <typename ValueType>
    void DelayNode<ValueType>::Compile(model::IRMapCompiler& compiler)
    {
        compiler.NewBlockRegion(*this);

        auto inputPort = this->GetInputPorts()[0];
        auto outputPort = this->GetOutputPorts()[0];
        llvm::Value* result = compiler.EnsureEmitted(outputPort);

        size_t sampleSize = outputPort->Size();
        size_t windowSize = this->GetWindowSize();
        size_t bufferSize = sampleSize * windowSize;

        auto& function = compiler.GetCurrentFunction();
        //
        // Delay nodes are always long lived - either globals or heap. Currently, we use globals
        // Each sample chunk is of size == sampleSize. The number of chunks we hold onto == windowSize
        // We need two buffers - one for the entire lot, one for the "last" chunk forwarded to the next operator
        //
        emitters::Variable* delayLineVar = compiler.Variables().AddVariable<emitters::InitializedVectorVariable<ValueType>>(emitters::VariableScope::global, bufferSize);
        llvm::Value* delayLine = compiler.EnsureEmitted(*delayLineVar);

        //
        // We implement a delay as a Shift Register
        //
        llvm::Value* inputBuffer = compiler.EnsureEmitted(inputPort);
        function.ShiftAndUpdate<ValueType>(delayLine, bufferSize, sampleSize, inputBuffer, result);

        compiler.TryMergeRegion(*this);
    }

    template <typename ValueType>
    void DelayNode<ValueType>::WriteToArchive(utilities::Archiver& archiver) const
    {
        Node::WriteToArchive(archiver);
        archiver[inputPortName] << _input;
        archiver[outputPortName] << _output;
        archiver["windowSize"] << _windowSize;
    }

    template <typename ValueType>
    void DelayNode<ValueType>::ReadFromArchive(utilities::Unarchiver& archiver)
    {
        Node::ReadFromArchive(archiver);
        archiver[inputPortName] >> _input;
        archiver[outputPortName] >> _output;
        archiver["windowSize"] >> _windowSize;

        auto dimension = _input.Size();
        _samples.clear();
        _samples.reserve(_windowSize);
        for (size_t index = 0; index < _windowSize; ++index)
        {
            _samples.push_back(std::vector<ValueType>(dimension));
        }
    }
}
}
