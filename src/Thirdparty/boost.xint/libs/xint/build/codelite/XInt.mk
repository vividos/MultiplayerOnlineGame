##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release,_Header_Only
ProjectName            :=XInt
ConfigurationName      :=Release,_Header_Only
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/chad/projects/xint/libs/xint/build/codelite"
ProjectPath            := "/home/chad/projects/xint/libs/xint/build/codelite"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Chad Nelson
Date                   :=03/09/2011
CodeLitePath           :="/home/chad/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)BOOST_XINT_SINGLE_TEST_PROGRAM $(PreprocessorSwitch)BOOST_TEST_DYN_LINK 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -O2 -Wall $(Preprocessors)
C_CmpOptions           := -O2 -Wall $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)/home/chad/projects/xint" "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)boost_unit_test_framework 
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/test_test_add$(ObjectSuffix) $(IntermediateDirectory)/test_test_bit_manipulations$(ObjectSuffix) $(IntermediateDirectory)/test_test_compare$(ObjectSuffix) $(IntermediateDirectory)/test_test_convert$(ObjectSuffix) $(IntermediateDirectory)/test_test_divide$(ObjectSuffix) $(IntermediateDirectory)/test_test_fixed$(ObjectSuffix) $(IntermediateDirectory)/test_test_main$(ObjectSuffix) $(IntermediateDirectory)/test_test_misc$(ObjectSuffix) $(IntermediateDirectory)/test_test_modular$(ObjectSuffix) $(IntermediateDirectory)/test_test_monty$(ObjectSuffix) \
	$(IntermediateDirectory)/test_test_multiply$(ObjectSuffix) $(IntermediateDirectory)/test_test_nothrow$(ObjectSuffix) $(IntermediateDirectory)/test_test_powers$(ObjectSuffix) $(IntermediateDirectory)/test_test_streams$(ObjectSuffix) $(IntermediateDirectory)/test_test_invalid_overload$(ObjectSuffix) $(IntermediateDirectory)/test_test_arithmetic_conversion$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/test_test_add$(ObjectSuffix): ../../test/test_add.cpp $(IntermediateDirectory)/test_test_add$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_add.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_add$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_add$(DependSuffix): ../../test/test_add.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_add$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_add$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_add.cpp"

$(IntermediateDirectory)/test_test_add$(PreprocessSuffix): ../../test/test_add.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_add$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_add.cpp"

$(IntermediateDirectory)/test_test_bit_manipulations$(ObjectSuffix): ../../test/test_bit_manipulations.cpp $(IntermediateDirectory)/test_test_bit_manipulations$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_bit_manipulations.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_bit_manipulations$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_bit_manipulations$(DependSuffix): ../../test/test_bit_manipulations.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_bit_manipulations$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_bit_manipulations$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_bit_manipulations.cpp"

$(IntermediateDirectory)/test_test_bit_manipulations$(PreprocessSuffix): ../../test/test_bit_manipulations.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_bit_manipulations$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_bit_manipulations.cpp"

$(IntermediateDirectory)/test_test_compare$(ObjectSuffix): ../../test/test_compare.cpp $(IntermediateDirectory)/test_test_compare$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_compare.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_compare$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_compare$(DependSuffix): ../../test/test_compare.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_compare$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_compare$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_compare.cpp"

$(IntermediateDirectory)/test_test_compare$(PreprocessSuffix): ../../test/test_compare.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_compare$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_compare.cpp"

$(IntermediateDirectory)/test_test_convert$(ObjectSuffix): ../../test/test_convert.cpp $(IntermediateDirectory)/test_test_convert$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_convert.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_convert$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_convert$(DependSuffix): ../../test/test_convert.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_convert$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_convert$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_convert.cpp"

$(IntermediateDirectory)/test_test_convert$(PreprocessSuffix): ../../test/test_convert.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_convert$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_convert.cpp"

$(IntermediateDirectory)/test_test_divide$(ObjectSuffix): ../../test/test_divide.cpp $(IntermediateDirectory)/test_test_divide$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_divide.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_divide$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_divide$(DependSuffix): ../../test/test_divide.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_divide$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_divide$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_divide.cpp"

$(IntermediateDirectory)/test_test_divide$(PreprocessSuffix): ../../test/test_divide.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_divide$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_divide.cpp"

$(IntermediateDirectory)/test_test_fixed$(ObjectSuffix): ../../test/test_fixed.cpp $(IntermediateDirectory)/test_test_fixed$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_fixed.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_fixed$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_fixed$(DependSuffix): ../../test/test_fixed.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_fixed$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_fixed$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_fixed.cpp"

$(IntermediateDirectory)/test_test_fixed$(PreprocessSuffix): ../../test/test_fixed.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_fixed$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_fixed.cpp"

$(IntermediateDirectory)/test_test_main$(ObjectSuffix): ../../test/test_main.cpp $(IntermediateDirectory)/test_test_main$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_main$(DependSuffix): ../../test/test_main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_main$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_main$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_main.cpp"

$(IntermediateDirectory)/test_test_main$(PreprocessSuffix): ../../test/test_main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_main$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_main.cpp"

$(IntermediateDirectory)/test_test_misc$(ObjectSuffix): ../../test/test_misc.cpp $(IntermediateDirectory)/test_test_misc$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_misc.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_misc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_misc$(DependSuffix): ../../test/test_misc.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_misc$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_misc$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_misc.cpp"

$(IntermediateDirectory)/test_test_misc$(PreprocessSuffix): ../../test/test_misc.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_misc$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_misc.cpp"

$(IntermediateDirectory)/test_test_modular$(ObjectSuffix): ../../test/test_modular.cpp $(IntermediateDirectory)/test_test_modular$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_modular.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_modular$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_modular$(DependSuffix): ../../test/test_modular.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_modular$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_modular$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_modular.cpp"

$(IntermediateDirectory)/test_test_modular$(PreprocessSuffix): ../../test/test_modular.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_modular$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_modular.cpp"

$(IntermediateDirectory)/test_test_monty$(ObjectSuffix): ../../test/test_monty.cpp $(IntermediateDirectory)/test_test_monty$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_monty.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_monty$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_monty$(DependSuffix): ../../test/test_monty.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_monty$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_monty$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_monty.cpp"

$(IntermediateDirectory)/test_test_monty$(PreprocessSuffix): ../../test/test_monty.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_monty$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_monty.cpp"

$(IntermediateDirectory)/test_test_multiply$(ObjectSuffix): ../../test/test_multiply.cpp $(IntermediateDirectory)/test_test_multiply$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_multiply.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_multiply$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_multiply$(DependSuffix): ../../test/test_multiply.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_multiply$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_multiply$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_multiply.cpp"

$(IntermediateDirectory)/test_test_multiply$(PreprocessSuffix): ../../test/test_multiply.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_multiply$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_multiply.cpp"

$(IntermediateDirectory)/test_test_nothrow$(ObjectSuffix): ../../test/test_nothrow.cpp $(IntermediateDirectory)/test_test_nothrow$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_nothrow.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_nothrow$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_nothrow$(DependSuffix): ../../test/test_nothrow.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_nothrow$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_nothrow$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_nothrow.cpp"

$(IntermediateDirectory)/test_test_nothrow$(PreprocessSuffix): ../../test/test_nothrow.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_nothrow$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_nothrow.cpp"

$(IntermediateDirectory)/test_test_powers$(ObjectSuffix): ../../test/test_powers.cpp $(IntermediateDirectory)/test_test_powers$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_powers.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_powers$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_powers$(DependSuffix): ../../test/test_powers.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_powers$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_powers$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_powers.cpp"

$(IntermediateDirectory)/test_test_powers$(PreprocessSuffix): ../../test/test_powers.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_powers$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_powers.cpp"

$(IntermediateDirectory)/test_test_streams$(ObjectSuffix): ../../test/test_streams.cpp $(IntermediateDirectory)/test_test_streams$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_streams.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_streams$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_streams$(DependSuffix): ../../test/test_streams.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_streams$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_streams$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_streams.cpp"

$(IntermediateDirectory)/test_test_streams$(PreprocessSuffix): ../../test/test_streams.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_streams$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_streams.cpp"

$(IntermediateDirectory)/test_test_invalid_overload$(ObjectSuffix): ../../test/test_invalid_overload.cpp $(IntermediateDirectory)/test_test_invalid_overload$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_invalid_overload.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_invalid_overload$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_invalid_overload$(DependSuffix): ../../test/test_invalid_overload.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_invalid_overload$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_invalid_overload$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_invalid_overload.cpp"

$(IntermediateDirectory)/test_test_invalid_overload$(PreprocessSuffix): ../../test/test_invalid_overload.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_invalid_overload$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_invalid_overload.cpp"

$(IntermediateDirectory)/test_test_arithmetic_conversion$(ObjectSuffix): ../../test/test_arithmetic_conversion.cpp $(IntermediateDirectory)/test_test_arithmetic_conversion$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/chad/projects/xint/libs/xint/test/test_arithmetic_conversion.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/test_test_arithmetic_conversion$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_test_arithmetic_conversion$(DependSuffix): ../../test/test_arithmetic_conversion.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_test_arithmetic_conversion$(ObjectSuffix) -MF$(IntermediateDirectory)/test_test_arithmetic_conversion$(DependSuffix) -MM "/home/chad/projects/xint/libs/xint/test/test_arithmetic_conversion.cpp"

$(IntermediateDirectory)/test_test_arithmetic_conversion$(PreprocessSuffix): ../../test/test_arithmetic_conversion.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_test_arithmetic_conversion$(PreprocessSuffix) "/home/chad/projects/xint/libs/xint/test/test_arithmetic_conversion.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/test_test_add$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_add$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_add$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_bit_manipulations$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_bit_manipulations$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_bit_manipulations$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_compare$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_compare$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_compare$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_convert$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_convert$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_convert$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_divide$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_divide$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_divide$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_fixed$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_fixed$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_fixed$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_misc$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_misc$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_misc$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_modular$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_modular$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_modular$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_monty$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_monty$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_monty$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_multiply$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_multiply$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_multiply$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_nothrow$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_nothrow$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_nothrow$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_powers$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_powers$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_powers$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_streams$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_streams$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_streams$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_invalid_overload$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_invalid_overload$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_invalid_overload$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/test_test_arithmetic_conversion$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/test_test_arithmetic_conversion$(DependSuffix)
	$(RM) $(IntermediateDirectory)/test_test_arithmetic_conversion$(PreprocessSuffix)
	$(RM) $(OutputFile)


