add_circt_unittest(PexcsTests
  DataFlowTest.cpp
)

target_link_libraries(PexcsTests
  PRIVATE
  CIRCTPexcs
  MLIRFuncDialect
  MLIRDialect
) 