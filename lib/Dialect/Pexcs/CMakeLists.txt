add_circt_dialect_library(CIRCTPexcs
  PexcsDialect.cpp
  PexcsOps.cpp
  PexcsTypes.cpp

  ADDITIONAL_HEADER_DIRS
  ${CIRCT_MAIN_INCLUDE_DIR}/circt/Dialect/Pexcs

  DEPENDS
  PexcsEnumsIncGen
  PexcsAttributesIncGen
  PexcsOpsIncGen

  LINK_COMPONENTS
  Support

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRInferTypeOpInterface
  MLIRSideEffectInterfaces
) 