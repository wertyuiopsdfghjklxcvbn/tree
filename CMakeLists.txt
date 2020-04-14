cmake_minimum_required (VERSION 3.14.3)

project(tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

IF (NOT WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF ()


set (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -Wno-unused-command-line-argument -IF:/projects/llvm-project/llvm/include -IF:/projects/llvm-project/build_deb/include -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS -D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -LF:/projects/llvm-project/build_deb/lib -lLLVMXRay.lib -lLLVMWindowsManifest.lib -lLLVMTextAPI.lib -lLLVMTestingSupport.lib -lLLVMTableGen.lib -lLLVMSymbolize.lib -lLLVMDebugInfoPDB.lib -lLLVMOrcJIT.lib -lLLVMJITLink.lib -lLLVMObjectYAML.lib -lLLVMMCA.lib -lLLVMLTO.lib -lLLVMPasses.lib -lLLVMObjCARCOpts.lib -lLLVMLineEditor.lib -lLLVMLibDriver.lib -lLLVMInterpreter.lib -lgtest_main.lib -lgtest.lib -lLLVMFuzzMutate.lib -lLLVMMCJIT.lib -lLLVMExecutionEngine.lib -lLLVMRuntimeDyld.lib -lLLVMDlltoolDriver.lib -lLLVMOption.lib -lLLVMDebugInfoGSYM.lib -lLLVMCoverage.lib -lLLVMCoroutines.lib -lLLVMXCoreDisassembler.lib -lLLVMXCoreCodeGen.lib -lLLVMXCoreDesc.lib -lLLVMXCoreInfo.lib -lLLVMX86Disassembler.lib -lLLVMX86AsmParser.lib -lLLVMX86CodeGen.lib -lLLVMX86Desc.lib -lLLVMX86Utils.lib -lLLVMX86Info.lib -lLLVMWebAssemblyDisassembler.lib -lLLVMWebAssemblyCodeGen.lib -lLLVMWebAssemblyDesc.lib -lLLVMWebAssemblyAsmParser.lib -lLLVMWebAssemblyInfo.lib -lLLVMSystemZDisassembler.lib -lLLVMSystemZCodeGen.lib -lLLVMSystemZAsmParser.lib -lLLVMSystemZDesc.lib -lLLVMSystemZInfo.lib -lLLVMSparcDisassembler.lib -lLLVMSparcCodeGen.lib -lLLVMSparcAsmParser.lib -lLLVMSparcDesc.lib -lLLVMSparcInfo.lib -lLLVMRISCVDisassembler.lib -lLLVMRISCVCodeGen.lib -lLLVMRISCVAsmParser.lib -lLLVMRISCVDesc.lib -lLLVMRISCVUtils.lib -lLLVMRISCVInfo.lib -lLLVMPowerPCDisassembler.lib -lLLVMPowerPCCodeGen.lib -lLLVMPowerPCAsmParser.lib -lLLVMPowerPCDesc.lib -lLLVMPowerPCInfo.lib -lLLVMNVPTXCodeGen.lib -lLLVMNVPTXDesc.lib -lLLVMNVPTXInfo.lib -lLLVMMSP430Disassembler.lib -lLLVMMSP430CodeGen.lib -lLLVMMSP430AsmParser.lib -lLLVMMSP430Desc.lib -lLLVMMSP430Info.lib -lLLVMMipsDisassembler.lib -lLLVMMipsCodeGen.lib -lLLVMMipsAsmParser.lib -lLLVMMipsDesc.lib -lLLVMMipsInfo.lib -lLLVMLanaiDisassembler.lib -lLLVMLanaiCodeGen.lib -lLLVMLanaiAsmParser.lib -lLLVMLanaiDesc.lib -lLLVMLanaiInfo.lib -lLLVMHexagonDisassembler.lib -lLLVMHexagonCodeGen.lib -lLLVMHexagonAsmParser.lib -lLLVMHexagonDesc.lib -lLLVMHexagonInfo.lib -lLLVMBPFDisassembler.lib -lLLVMBPFCodeGen.lib -lLLVMBPFAsmParser.lib -lLLVMBPFDesc.lib -lLLVMBPFInfo.lib -lLLVMARMDisassembler.lib -lLLVMARMCodeGen.lib -lLLVMARMAsmParser.lib -lLLVMARMDesc.lib -lLLVMARMUtils.lib -lLLVMARMInfo.lib -lLLVMAMDGPUDisassembler.lib -lLLVMAMDGPUCodeGen.lib -lLLVMMIRParser.lib -lLLVMipo.lib -lLLVMInstrumentation.lib -lLLVMVectorize.lib -lLLVMLinker.lib -lLLVMIRReader.lib -lLLVMAsmParser.lib -lLLVMAMDGPUAsmParser.lib -lLLVMAMDGPUDesc.lib -lLLVMAMDGPUUtils.lib -lLLVMAMDGPUInfo.lib -lLLVMAArch64Disassembler.lib -lLLVMMCDisassembler.lib -lLLVMAArch64CodeGen.lib -lLLVMGlobalISel.lib -lLLVMSelectionDAG.lib -lLLVMAsmPrinter.lib -lLLVMDebugInfoDWARF.lib -lLLVMCodeGen.lib -lLLVMTarget.lib -lLLVMScalarOpts.lib -lLLVMInstCombine.lib -lLLVMAggressiveInstCombine.lib -lLLVMTransformUtils.lib -lLLVMBitWriter.lib -lLLVMAnalysis.lib -lLLVMProfileData.lib -lLLVMObject.lib -lLLVMBitReader.lib -lLLVMBitstreamReader.lib -lLLVMCore.lib -lLLVMRemarks.lib -lLLVMAArch64AsmParser.lib -lLLVMMCParser.lib -lLLVMAArch64Desc.lib -lLLVMMC.lib -lLLVMDebugInfoCodeView.lib -lLLVMDebugInfoMSF.lib -lLLVMBinaryFormat.lib -lLLVMAArch64Utils.lib -lLLVMAArch64Info.lib -lLLVMSupport.lib -lLLVMDemangle.lib")
#-Wno-deprecated-declarations 

add_subdirectory(src)


