set(DIR_NAME ScopeAlpha)

set(CHECK_SCOPE_SRC ${CHECK_SCOPE_SRC}
	${DIR_NAME}/ScopeSampleChecker.cpp
	${DIR_NAME}/ScopeSinEvaluateChecker.cpp
	${DIR_NAME}/ScopeFileDescriptorChecker.cpp
	${DIR_NAME}/ScopeMemoryAllocationChecker.cpp
PARENT_SCOPE)