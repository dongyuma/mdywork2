set(DIR_NAME ScopeDev)

set(CHECK_SCOPE_SRC ${CHECK_SCOPE_SRC}
${DIR_NAME}/ScopePointerSubChecker.cpp
${DIR_NAME}/ScopeStackAddrEscapeAwayChecker.cpp
PARENT_SCOPE) 