#
# cmake file
#

set (library_name nodes)

set (include include/AccumulatorNode.h
             include/BinaryOperationNode.h
             include/BinaryPredicateNode.h
             include/ConstantNode.h
             include/DelayNode.h
             include/DotProductNode.h
             include/DTWDistanceNode.h
             include/ExtremalValueNode.h
             include/MultiplexerNode.h
             include/ForestPredictorNode.h
             include/LinearPredictorNode.h
             include/L2NormNode.h
             include/MovingAverageNode.h
             include/MovingVarianceNode.h
             include/DemultiplexerNode.h
             include/SingleElementThresholdNode.h
             include/SumNode.h
             include/TypeCastNode.h
             include/UnaryOperationNode.h
             include/ValueSelectorNode.h)

set (src src/ConstantNode.cpp
         src/LinearPredictorNode.cpp
         src/SingleElementThresholdNode.cpp)

set (tcc tcc/AccumulatorNode.tcc
         tcc/BinaryOperationNode.tcc
         tcc/BinaryPredicateNode.tcc
         tcc/ConstantNode.tcc
         tcc/DelayNode.tcc
         tcc/DotProductNode.tcc
         tcc/DTWDistanceNode.tcc
         tcc/ExtremalValueNode.tcc
         tcc/MultiplexerNode.tcc
         tcc/ForestPredictorNode.tcc
         tcc/L2NormNode.tcc
         tcc/MovingAverageNode.tcc
         tcc/MovingVarianceNode.tcc
         tcc/DemultiplexerNode.tcc
         tcc/SumNode.tcc
         tcc/TypeCastNode.tcc
         tcc/UnaryOperationNode.tcc
         tcc/ValueSelectorNode.tcc)

source_group("include" FILES ${include})
source_group("src" FILES ${src})
source_group("tcc" FILES ${tcc})

add_library(${library_name} ${src} ${include} ${tcc})
target_include_directories(${library_name} PUBLIC include)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${library_name} PRIVATE -fPIC)
endif()
target_link_libraries(${library_name} model predictors utilities)

set_property(TARGET ${library_name} PROPERTY FOLDER "libraries")

#
# test project
#

set (test_name ${library_name}_test)

set (test_src test/src/main.cpp 
              test/src/Nodes_test.cpp)
set (test_include test/include/Nodes_test.h)

source_group("src" FILES ${test_src})
source_group("include" FILES ${test_include})

add_executable(${test_name} ${test_src} ${test_include} ${include})
target_include_directories(${test_name} PRIVATE test/include)
target_link_libraries(${test_name} nodes testing common model predictors utilities)
copy_shared_libraries(${test_name} $<TARGET_FILE_DIR:${test_name}>)

set_property(TARGET ${test_name} PROPERTY FOLDER "tests")

add_test(NAME ${test_name} COMMAND ${test_name})
