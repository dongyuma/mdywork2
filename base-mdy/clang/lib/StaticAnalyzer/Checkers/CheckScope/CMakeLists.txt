set(CHECK_SCOPE_SRC common/CheckScope.cpp
	common/ScopeExprCmp.cpp
	common/ScopeDynamicExtent.cpp
	common/ScopeSValCmp.cpp
	common/ScopeOverRunRegionRawOffset.cpp
	common/ScopeTaint.cpp
	common/ScopeBuiltinCheckerRegister.cpp
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Scope/CMakeLists.txt")
	add_subdirectory(Scope)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ScopeAlpha/CMakeLists.txt")
	add_subdirectory(ScopeAlpha)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ScopeDev/CMakeLists.txt")
	add_subdirectory(ScopeDev)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/common)

if(LLVM_ENABLE_PLUGINS)
	#插件可用时使用外置编译以提高编译速度
	set(LLVM_LINK_COMPONENTS
	Support
	)

	set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CheckScopePlugin.exports)
	add_llvm_library(CheckScopePlugin MODULE BUILDTREE_ONLY 
	${CHECK_SCOPE_SRC}

	PLUGIN_TOOL clang
	)

else()
	#插件不可用时使用内置编译

	#扫描源码文件生成中间头文件以令静态库中的全局变量得到初始化进而注册Checker
	execute_process(COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/CheckScopeBuiltinInclude.py" ${CMAKE_CURRENT_SOURCE_DIR} "${CHECK_SCOPE_SRC}" "${CMAKE_CURRENT_BINARY_DIR}")

	add_definitions(-DBUILIT_IN_SCOPE_CHECKERS=1)
	set(LLVM_LINK_COMPONENTS FrontendOpenMP Support)
	add_clang_library(clangCheckScopeCheckers
	${CHECK_SCOPE_SRC}
	
	LINK_LIBS
	clangAST
	clangASTMatchers
	clangAnalysis
	clangBasic
	clangStaticAnalyzerCore

	DEPENDS
	omp_gen
	)
endif()

