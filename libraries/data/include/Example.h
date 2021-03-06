////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     Example.h (data)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AutoDataVector.h"
#include "DataVector.h"
#include "DenseDataVector.h"

// utilities
#include "TypeTraits.h"

// stl
#include <algorithm> // for std::swap
#include <cstddef>
#include <memory>
#include <type_traits>

namespace ell
{
namespace data
{
    /// <summary> A supervised example. </summary>
    template <typename DataVectorT, typename MetadataT>
    class Example
    {
    public:
        using DataVectorType = DataVectorT;
        using MetadataType = MetadataT;

        Example() = default;

        Example(const Example<DataVectorType, MetadataType>& other) = default;

        Example(Example<DataVectorType, MetadataType>&& other) = default;

        /// <summary> Constructs a supervised example. </summary>
        ///
        /// <param name="dataVector"> The data vector. </param>
        /// <param name="metadataType"> The metadata. </param>
        Example(DataVectorType dataVector, const MetadataType& metadata);

        template <typename InputDataVectorType, typename InputMetadataType, utilities::IsDifferent<InputDataVectorType, DataVectorType> IsDifferentConcept = true, IsDataVector<InputDataVectorType> IsDataConcept = true>
        Example(const InputDataVectorType& dataVector, const InputMetadataType& metadata);

        /// <summary> Constructs a supervised example. </summary>
        ///
        /// <param name="dataVector"> The data vector. </param>
        /// <param name="metadataType"> The metadata. </param>
        Example(const std::shared_ptr<const DataVectorType>& dataVector, const MetadataType& metadata);

        /// <summary> Assignment operator. </summary>
        ///
        /// <param name="other"> The other. </param>
        ///
        /// <returns> A reference to this Example. </returns>
        Example& operator=(const Example<DataVectorType, MetadataType>& other) = delete;

        /// <summary> Move assignment operator. </summary>
        ///
        /// <param name="other"> [in,out] The other. </param>
        ///
        /// <returns> A reference to this Example. </returns>
        Example& operator=(Example<DataVectorType, MetadataType>&& other) = default;

        /// <summary> Gets the data vector. </summary>
        ///
        /// <returns> The data vector. </returns>
        const DataVectorType& GetDataVector() const { return *_dataVector.get(); }

        /// <summary> Gets the metadata. </summary>
        ///
        /// <returns> The metadata. </returns>
        MetadataType& GetMetadata() { return _metadata; }

        /// <summary> Gets the metadata. </summary>
        ///
        /// <returns> The metadata. </returns>
        const MetadataType& GetMetadata() const { return _metadata; }

        /// <summary> Gets the value of the reference counter of the shared pointer that holds the data vector. </summary>
        ///
        /// <returns> The data vector reference count. </returns>
        size_t GetDataVectorReferenceCount() const { return _dataVector.use_count(); }

        /// <summary>
        /// Creates a new example that contains the same data as this example, in a specified data vector
        /// type and meta data type. This overload creates a shallow copy of the data vector, because the
        /// requested data vector type matches the one already stored in this example.
        /// </summary>
        ///
        /// <typeparam name="TargetExampleType"> Requested target example type (metadata ctor must take old
        /// MetadataType). </typeparam>
        /// <returns> An example of the desired type. </returns>
        template <typename TargetExampleType, utilities::IsSame<typename TargetExampleType::DataVectorType, DataVectorType> Concept = true>
        TargetExampleType CopyAs() const;

        /// <summary>
        /// Creates a new example that contains the same data as this example, in a specified data vector
        /// type and meta data type. This overload creates a deep copy of the data vector, because the
        /// requested data vector is different from the one stored in this example.
        /// </summary>
        ///
        /// <typeparam name="TargetExampleType"> Requested target example type (metadata ctor must take old
        /// MetadataType). </typeparam>
        /// <returns> An example of the desired type. </returns>
        template <typename TargetExampleType, utilities::IsDifferent<typename TargetExampleType::DataVectorType, DataVectorType> Concept = true>
        TargetExampleType CopyAs() const;

        /// <summary> Prints the datavector to an output stream. </summary>
        ///
        /// <param name="os"> [in,out] Stream to write data to. </param>
        void Print(std::ostream& os) const;

    private:
        std::shared_ptr<const DataVectorType> _dataVector;
        MetadataType _metadata;
    };

    /// <summary> A metadata class that contains a weight and a real valued label. </summary>
    struct WeightLabel
    {
        /// <summary> Prints the weight label pair. </summary>
        ///
        /// <param name="os"> [in,out] The output stream. </param>
        void Print(std::ostream& os) const;

        double weight;
        double label;
    };

    typedef Example<AutoDataVector, WeightLabel> AutoSupervisedExample;
    typedef Example<FloatDataVector, WeightLabel> DenseSupervisedExample;

    /// <summary> Stream insertion operator. </summary>
    ///
    /// <param name="ostream"> [in,out] Stream to write data to. </param>
    /// <param name="example"> The example. </param>
    ///
    /// <returns> The shifted ostream. </returns>
    template <typename DataVectorType, typename MetadataType>
    std::ostream& operator<<(std::ostream& ostream, const Example<DataVectorType, MetadataType>& example);
}
}

#include "../tcc/Example.tcc"
