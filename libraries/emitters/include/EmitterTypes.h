////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     EmitterTypes.h (emitter)
//  Authors:  Umesh Madan, Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

// model
#include "DynamicArray.h"

// stl
#include <cstddef>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace ell
{
namespace emitters
{
    ///<summary> An enumeration of primitive types our compilers support </summary>
    enum class VariableType
    {
        Void = 0,
        ///<summary> 8 bit unsigned integer </summary>
        Byte,
        ///<summary> 16 bit signed integer </summary>
        Short,
        ///<summary> 32 bit signed integer </summary>
        Int32,
        ///<summary> 64 bit signed integer </summary>
        Int64,
        ///<summary> 8 byte floating point </summary>
        Double,
        ///<summary> 8 bit character </summary>
        Char8,
        //
        // Pointers
        //
        VoidPointer,
        ///<summary> Pointer to a byte </summary>
        BytePointer,
        ///<summary> Pointer to a short </summary>
        ShortPointer,
        ///<summary> Pointer to an Int32 </summary>
        Int32Pointer,
        ///<summary> Pointer to an Int64 </summary>
        Int64Pointer,
        ///<summary> Pointer to a Double </summary>
        DoublePointer,
        ///<summary> Pointer to a character array </summary>
        Char8Pointer
    };

    /// <summary> Types of coordinatewise operations supported by this node type. </summary>
    enum class BinaryOperationType
    {
        none,
        add,
        subtract,
        coordinatewiseMultiply, // coordinatewise multiplication
        coordinatewiseDivide, // coordinatewise division
        logicalAnd,
        logicalOr,
        logicalXor
    };

    /// <summary> Types of coordinatewise operations supported by this node type. </summary>
    enum class BinaryPredicateType
    {
        none,
        equal,
        less,
        greater,
        notEqual,
        lessOrEqual,
        greaterOrEqual
    };

    /// <summary> Types of unary operations supported by this node type. </summary>
    enum class UnaryOperationType
    {
        none,
        sqrt, // real only
        logicalNot // bool only
    };

    ///<summary> An enumeration of strongly TYPED operations on numbers </summary>
    enum class TypedOperator
    {
        none = 0,
        ///<summary> Integer addition </summary>
        add,
        ///<summary> Integer subtraction </summary>
        subtract,
        ///<summary> Integer multiplication </summary>
        multiply,
        ///<summary> Signed division - returns an integer </summary>
        divideSigned,
        ///<summary> Floating point addition </summary>
        addFloat,
        ///<summary> Floating point subtraction </summary>
        subtractFloat,
        ///<summary> Floating point multiplication </summary>
        multiplyFloat,
        ///<summary> Floating point division </summary>
        divideFloat,
        ///<summary> Binary and </summary>
        logicalAnd,
        ///<summary> Binary or </summary>
        logicalOr,
        ///<summary> Xor </summary>
        logicalXor
    };

    ///<summary> An enumeration of strongly TYPED comparisons on numbers </summary>
    enum class TypedComparison
    {
        none = 0,
        ///<summary> Integer Equal </summary>
        equals,
        ///<summary> Integer Less than </summary>
        lessThan,
        ///<summary> Integer Less than equals </summary>
        lessThanOrEquals,
        ///<summary> Integer Greater than </summary>
        greaterThan,
        ///<summary> Integer Greater than equals </summary>
        greaterThanOrEquals,
        ///<summary> Integer Not Equals </summary>
        notEquals,
        ///<summary> Floating point Equal </summary>
        equalsFloat,
        ///<summary> Floating point less than  </summary>
        lessThanFloat,
        ///<summary> Floating point less than equals </summary>
        lessThanOrEqualsFloat,
        ///<summary> Floating point greater than </summary>
        greaterThanFloat,
        ///<summary> Floating point less than equals </summary>
        greaterThanOrEqualsFloat,
        ///<summary> Floating point Not equals </summary>
        notEqualsFloat
    };

    /// <summary> Translate the unary operation operator into a strongly typed operator for LLVM </summary>
    template <typename T>
    TypedOperator GetOperator(emitters::UnaryOperationType operation);

    /// <summary> Translate the binary operation operator into a strongly typed operator for LLVM </summary>
    template <typename T>
    TypedOperator GetOperator(emitters::BinaryOperationType operation);

    /// <summary> Translate the binary predicate operator into a more strongly typed operator for LLVM </summary>
    template <typename T>
    TypedComparison GetComparison(emitters::BinaryPredicateType predicate);

    ///<summary> Commonly used to create named fields, arguments, variables </summary>
    using NamedVariableType = std::pair<std::string, VariableType>;

    ///<summary> Collections of variable types </summary>
    using ValueTypeList = utilities::DynamicArray<VariableType>;

    ///<summary> Collections of named variable types </summary>
    using NamedVariableTypeList = utilities::DynamicArray<NamedVariableType>;

    /// <summary> Gets the value form the VariableType enum that corresponds to a given native c++ type. </summary>
    ///
    /// <typeparam name="ValueType"> The native c++ type being mapped to a value of VariableType. </typeparam>
    ///
    /// <returns> A VariableType that corresponds to a given native c++ type. </returns>
    template <typename ValueType>
    VariableType GetVariableType();

    /// <summary> Gets the value form the VariableType enum that corresponds to a pointer to a given nonpointer type. </summary>
    ///
    /// <param name="type"> The nonpointer type, such as Short or Double. </typeparam>
    ///
    /// <returns> A VariableType that corresponds to the pointer to a given type. </returns>
    VariableType GetPointerType(VariableType type);

    /// <summary> Gets the default value for a certain type. </summary>
    ///
    /// <typeparam name="ValueType"> The type. </typeparam>
    ///
    /// <returns> The default value for the given type. </returns>
    template <typename ValueType>
    ValueType GetDefaultValue();

    /// <summary> Gets the type-specific add element of the TypedOperator enum. </summary>
    ///
    /// <typeparam name="ValueType"> The type. </typeparam>
    ///
    /// <returns> The add operation that corresponds to the given type. </returns>
    template <typename ValueType>
    TypedOperator GetAddForValueType();

    /// <summary> Gets the type-specific subtract element of the TypedOperator enum. </summary>
    ///
    /// <typeparam name="ValueType"> The type. </typeparam>
    ///
    /// <returns> The subtract operation that corresponds to the given type. </returns>
    template <typename ValueType>
    TypedOperator GetSubtractForValueType();

    /// <summary> Gets the type-specific multiply element of the TypedOperator enum. </summary>
    ///
    /// <typeparam name="ValueType"> The type. </typeparam>
    ///
    /// <returns> The multiply operation that corresponds to the given type. </returns>
    template <typename ValueType>
    TypedOperator GetMultiplyForValueType();

    /// <summary> Gets the type-specific divide element of the TypedOperator enum. </summary>
    ///
    /// <typeparam name="ValueType"> The type. </typeparam>
    ///
    /// <returns> The divide operation that corresponds to the given type. </returns>
    template <typename ValueType>
    TypedOperator GetDivideForValueType();

    /// <summary> Does the given primitive type have a sign? </summary>
    ///
    /// <param name="type"> The type. </param>
    ///
    /// <returns> true if signed, false if not. </returns>
    bool IsSigned(VariableType type);

    /// <summary> Helper struct for getting the backing value type for a variable </summary>
    template <typename T>
    struct VariableValueType
    {
        using type = T;
        using DestType = type;

        static std::vector<DestType> ToVariableVector(const std::vector<T>& src);
        static std::vector<T> FromVariableVector(const std::vector<DestType>& src);
    };

    template <>
    struct VariableValueType<bool>
    {
        using type = int;
        using DestType = type;

        static std::vector<DestType> ToVariableVector(const std::vector<bool>& src);
        static std::vector<bool> FromVariableVector(const std::vector<DestType>& src);
    };
}
}

#include "../tcc/EmitterTypes.tcc"