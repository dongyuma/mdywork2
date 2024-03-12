//==- CheckSizeofPointer.cpp - Check for sizeof on pointers ------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines a check for unintended use of sizeof() on pointer
//  expressions.
//
//===----------------------------------------------------------------------===//

#include <iostream>
// #include "ClangSACheckers.h"
// #include "clang/AST/StmtVisitor.h"
// #include "clang/AST/DataCollection.h"
// #include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
// #include "clang/StaticAnalyzer/Core/Checker.h"
// #include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"


#include "Taint.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class WalkAST : public StmtVisitor<WalkAST> {
  BugReporter &BR;
  const CheckerBase *Checker;
  AnalysisDeclContext* AC;
  std::set<unsigned> setSL;
  std::map<unsigned, SourceLocation> mapSL;

public:
  WalkAST(BugReporter &br, const CheckerBase *checker, AnalysisDeclContext *ac)
      : BR(br), Checker(checker), AC(ac) {}
  void VisitStmt(Stmt *S) { VisitChildren(S); }
  void VisitChildren(Stmt *S);
  void ProcessLocation(unsigned lineNumber);
  void GetLine(const char *charBuffer, char *charLine);
  bool CheckMacro(char *charLine);
};
}

void WalkAST::GetLine(const char *charBuffer, char *charLine) {
  const char *p = charBuffer;
  while(p) {
    *charLine = *p;
    p++;
    charLine++;
    if((*p == '\r' && *(p+1) == '\n') || *p == '\n') {
      break;
    }
  }
  *charLine = 0;
}

bool WalkAST::CheckMacro(char *charLine) {
  if(!strncmp(charLine, "#define", 7)) {
    charLine += 7;
    while(*charLine == ' ' || *charLine == '\t') {
      charLine++;
    }

    char *p = charLine;
    int sign = 0, change = 0;

    std::vector<std::string> vectorString;
    std::string tempString = "";

    while(true) {
      if(*p=='(') {
        sign += 1;
        change = 1;
        tempString = "";
      } else if(*p==')') {
        sign -= 1;
        vectorString.push_back(tempString);
        tempString = "";
      } else if(*p==',') {
        vectorString.push_back(tempString);
        tempString = "";
      } else if(*p==' ' || *p=='\t') {
        if(sign){
          ;
        }
        else {
          break;
        }
      } else {
        if(sign) {
          tempString += *p;
        }
      }
      p++;
    }
    if(!change) {
      return false;
    }

    while(*p == ' ' || *p == '\t') {
      p++;
    }

    std::string pString = p;

    for(int i=0; i < (int)vectorString.size(); i++) {
      tempString = vectorString[i];
      std::string::size_type pos1 = 0;
      // break;
      while(true) {
        pos1 = pString.find(tempString, pos1);
        if(pos1 == std::string::npos) {
          break;
        }
        else {
          if(pos1 < 1 || pos1 + tempString.size() >= pString.size())
            return true;
          if(pString[pos1-1] != '(' || pString[pos1 + tempString.size()] != ')')
            return true;
        }
        pos1 += 1;
      }
    }
  }
  return false;
}

void WalkAST::ProcessLocation(unsigned lineNumber) {
  FullSourceLoc fullLoc = FullSourceLoc(mapSL[lineNumber], BR.getSourceManager());
  // std::cout << fullLoc.getSpellingLineNumber() << std::endl;
  // std::cout << fullLoc.getSpellingColumnNumber() << std::endl;
  char LineBuffer[2000];
  GetLine(fullLoc.getCharacterLine(), LineBuffer);
  // std::cout << "LineBuffer: " << LineBuffer << std::endl;
  bool re = CheckMacro(LineBuffer);
  if(re)
  {
    SourceRange Sr = SourceRange(fullLoc.getSpellingLoc());
    PathDiagnosticLocation Loc(fullLoc.getSpellingLoc(), BR.getSourceManager());
    BR.EmitBasicReport(
        AC->getDecl(), Checker, "Check for Macro function parameters",
        categories::LogicError, "Macro function parameters fromat Error",
        Loc, Sr);
  }
}

void WalkAST::VisitChildren(Stmt *S) {
  // std::cout << "In the MacroFunctionChecker VisitChildren" << std::endl;
  SourceLocation Loc = S->getBeginLoc();
  const SourceManager &SM = BR.getSourceManager();
  if (Loc.isMacroID())
  { 
      Loc = SM.getTopMacroCallerLoc(Loc);
      if (Loc.isMacroID())
      {
        unsigned lineNumber = BR.getSourceManager().getSpellingLineNumber(Loc);
        if(!setSL.count(lineNumber)){
          setSL.insert(lineNumber);
          mapSL[lineNumber] = Loc;
          ProcessLocation(lineNumber);
        }
      }
  }

  for (Stmt *Child : S->children())
    if (Child)
      Visit(Child);
}

//===----------------------------------------------------------------------===//
// SizeofPointerChecker
//===----------------------------------------------------------------------===//

namespace {
class MacroFunctionChecker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    WalkAST walker(BR, this, mgr.getAnalysisDeclContext(D));
    walker.Visit(D->getBody());
  }
};
}

// void ento::registerMacroFunctionChecker(CheckerManager &mgr) {
//   mgr.registerChecker<MacroFunctionChecker>();
// }

// bool ento::shouldRegisterMacroFunctionChecker(const CheckerManager &mgr) {
//   return true;
// }