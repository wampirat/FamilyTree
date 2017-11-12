#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_TDM-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Family.o \
	${OBJECTDIR}/PeopleCollection.o \
	${OBJECTDIR}/Person.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/FamilyTest.o \
	${TESTDIR}/FamilyTreeFrameworkTest.o \
	${TESTDIR}/PeopleCollectionTest.o \
	${TESTDIR}/PersonTest.o \
	${TESTDIR}/ScenarioTests.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++14
CXXFLAGS=-std=c++14

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/familytreeengine.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/familytreeengine.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/familytreeengine ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Family.o: Family.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Family.o Family.cpp

${OBJECTDIR}/PeopleCollection.o: PeopleCollection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PeopleCollection.o PeopleCollection.cpp

${OBJECTDIR}/Person.o: Person.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Person.o Person.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:
	cd ../GoogleTest && ${MAKE}  -f Makefile CONF=Release

${TESTDIR}/TestFiles/f1: ${TESTDIR}/FamilyTest.o ${TESTDIR}/FamilyTreeFrameworkTest.o ${TESTDIR}/PeopleCollectionTest.o ${TESTDIR}/PersonTest.o ${TESTDIR}/ScenarioTests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   ../GoogleTest/dist/Release/MinGW_TDM-Windows/libgoogletest.a 


${TESTDIR}/FamilyTest.o: FamilyTest.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -I../../googletest/googletest -I../../googletest/googletest/include -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/FamilyTest.o FamilyTest.cpp


${TESTDIR}/FamilyTreeFrameworkTest.o: FamilyTreeFrameworkTest.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -I../../googletest/googletest -I../../googletest/googletest/include -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/FamilyTreeFrameworkTest.o FamilyTreeFrameworkTest.cpp


${TESTDIR}/PeopleCollectionTest.o: PeopleCollectionTest.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -I../../googletest/googletest -I../../googletest/googletest/include -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/PeopleCollectionTest.o PeopleCollectionTest.cpp


${TESTDIR}/PersonTest.o: PersonTest.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -I../../googletest/googletest -I../../googletest/googletest/include -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/PersonTest.o PersonTest.cpp


${TESTDIR}/ScenarioTests.o: ScenarioTests.cpp 
	${MKDIR} -p ${TESTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -I../../googletest/googletest -I../../googletest/googletest/include -I. -I. -std=c++14 -MMD -MP -MF "$@.d" -o ${TESTDIR}/ScenarioTests.o ScenarioTests.cpp


${OBJECTDIR}/Family_nomain.o: ${OBJECTDIR}/Family.o Family.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Family.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Family_nomain.o Family.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Family.o ${OBJECTDIR}/Family_nomain.o;\
	fi

${OBJECTDIR}/PeopleCollection_nomain.o: ${OBJECTDIR}/PeopleCollection.o PeopleCollection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/PeopleCollection.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PeopleCollection_nomain.o PeopleCollection.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/PeopleCollection.o ${OBJECTDIR}/PeopleCollection_nomain.o;\
	fi

${OBJECTDIR}/Person_nomain.o: ${OBJECTDIR}/Person.o Person.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Person.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Person_nomain.o Person.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Person.o ${OBJECTDIR}/Person_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I/D/MinGW/include -I/C/wxWidgets-3.0.2/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
