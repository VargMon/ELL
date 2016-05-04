////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     SortingTreeLearner.h (trainers)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// dataset
#include "RowDataset.h"
#include "DenseDataVector.h"

// predictors
#include "DecisionTree.h"

// stl
#include <queue>

namespace trainers
{
    template <typename LossFunctionType>
    class SortingTreeLearner
    {
    public:

        /// <summary> Constructs an instance of SortingTreeLearner. </summary>
        ///
        /// <param name="lossFunction"> The loss function. </param>
        SortingTreeLearner(LossFunctionType lossFunction);

        /// <summary> Trains a decision tree. </summary>
        ///
        /// <typeparam name="ExampleIteratorType"> Type of the example iterator. </typeparam>
        /// <param name="exampleIterator"> [in,out] The example iterator. </param>
        ///
        /// <returns> A decision tree. </returns>
        template <typename ExampleIteratorType>
        predictors::DecisionTree Train(ExampleIteratorType exampleIterator);

    private:

        struct SplitCandidate
        {
            predictors::DecisionTree::SplitRule splitRule;
            predictors::DecisionTree::Child child;
            double gain;
            uint64_t fromIndex;
            uint64_t size;
        };


        template <typename ExampleIteratorType>
        void LoadData(ExampleIteratorType exampleIterator);

        void Cleanup();

        LossFunctionType _lossFunction;
        dataset::RowDataset<dataset::DoubleDataVector> _dataset;
        std::priority_queue<double> _queue;
    };
}

#include "../tcc/SortingTreeLearner.tcc"
