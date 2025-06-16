add_circt_conversion_library(CIRCTHWToPexcs
  HWToPexcs.cpp

  LINK_LIBS PUBLIC
  CIRCTHW
  CIRCTPexcs
  CIRCTComb
  CIRCTSV
  MLIRIR
  MLIRPass
  MLIRFuncDialect
  MLIRSupport
  MLIRTransforms
  ) 