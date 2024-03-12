//===--- FunctionInfoCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// 使用clang-tidy工具 调用本Checker时 必须向环境变量导入 CHECKSCOPE_ANALYZE_RESULT_OUT_DIR 并制定为.StaticAnalyzerTool/result的绝对路径 
// 同时result/sc_out/ 目录下由对应的源文件统计信息json文件
//===----------------------------------------------------------------------===//

#include "FunctionInfoCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Analysis/CFG.h"
#include "llvm/Support/jsoncpp.h"
#include "clang/CrossTU/CrossTranslationUnit.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Preprocessor.h"

#include <set>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace counter {

namespace 
{
	//控制流图边对象
	class CFGBlockEdge
	{
	public:
		CFGBlockEdge(unsigned int start_block_id, unsigned int end_block_id)
		{
			start_id = start_block_id;
			end_id = end_block_id;
		}

		bool operator<(const CFGBlockEdge &r) const
		{
			if (r.start_id != start_id)
			{
				return start_id < r.start_id;
			}
			else
			{
				return end_id < r.end_id;
			}
		}
		unsigned int start_id;
		unsigned int end_id;
	};

	class FunctionCallExprCountVisitor :public RecursiveASTVisitor<FunctionCallExprCountVisitor>
	{
	public:
		FunctionCallExprCountVisitor()
		{
			
		}

		bool VisitCallExpr(CallExpr *CE)
		{
			if (CE && CE->getCalleeDecl())
			{
				if (const FunctionDecl* MatchedFunctionDecl = dyn_cast<FunctionDecl>(CE->getCalleeDecl()))
				{
					MatchedFunctionDeclSet.insert(MatchedFunctionDecl);
				}
			}
			return true;
		}

		std::set<const clang::FunctionDecl*> MatchedFunctionDeclSet;

	};
}

class FunctionInfoCheck::CommentsLinesCountHandler : public CommentHandler
{
public:
	CommentsLinesCountHandler(FunctionInfoCheck &_Check) :Check(_Check) {}

	bool HandleComment(Preprocessor &PP, SourceRange Range) override
	{
		if (!PP.isInPrimaryFile()) //仅分析主文件中的注释函数
		{
			return false;
		}

		uint64_t comments_lines = 1;
		StringRef Text = Lexer::getSourceText(CharSourceRange::getCharRange(Range), PP.getSourceManager(), PP.getLangOpts());

		std::size_t pre_pos = 0;
		std::size_t pos = 0;
		while (pos < Text.size() && (pos = Text.find_first_of("\r\n", pos)) != std::string::npos)
		{
			StringRef LineStrRef = Text.substr(pre_pos, pos - pre_pos);
			if (LineStrRef.trim().empty())
			{
				Check.commentBlanks++;
			}
			++comments_lines;
			if ((pos + 1) < Text.size() && Text[pos] == '\r' && Text[pos + 1] == '\n')
			{
				pos += 2;
			}
			else
			{
				++pos;
			}
			pre_pos = pos;
		}
		Check.commentLines += comments_lines;
		return false;
	}

private:
	FunctionInfoCheck &Check;
};

FunctionInfoCheck::FunctionInfoCheck(StringRef Name, ClangTidyContext *Context)
	: ClangTidyCheck(Name, Context),Handler(std::make_unique<CommentsLinesCountHandler>(*this))
{
	countInfoFilePath = getSourceCountInfoFilePath(Context->getCurrentFile());
	if (countInfoFilePath.empty())
	{
		skipCheck = true;
	}
	else
	{
		skipCheck = false;
	}

	functionsNumber = 0;
	maxLines = 0;
	minLines = UINT_MAX;
	commentLines = 0;
	commentBlanks = 0;
	blanks = 0;
}

FunctionInfoCheck::~FunctionInfoCheck() = default;

void FunctionInfoCheck::registerMatchers(MatchFinder *Finder) {
  // 匹配器
  Finder->addMatcher(
      functionDecl(
          unless(isExpansionInSystemHeader()),
          isDefinition(),
		  unless(isImplicit()),
		  hasBody(compoundStmt())
      ).bind("functionDecl"), this);
}

void FunctionInfoCheck::check(const MatchFinder::MatchResult &Result) 
{
    if (skipCheck) return;

    const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl");
	if (FD)
    {
		std::string funSginature = getFunctionSginatureString(FD); //获取函数唯一签名
		if (funSginature.empty())
		{
			return;
		}

        functionsNumber++;
        checkFunctionInfo(FD, Result);
		
		//基于每个单独的函数进行统计
		checkFunctionCyclomaticComplexity(funSginature, FD, Result); //圈复杂度
		checkFunctionFanOut(funSginature, FD, Result);
    }
}


void FunctionInfoCheck::checkFunctionInfo(const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result)
{
	if (FD->getBeginLoc().isInvalid() || FD->getEndLoc().isInvalid())
	{
		return;
	}

	auto SourceMgr = Result.SourceManager;
	bool StmtBeginLineInvalid = true;
	unsigned StmtBeginLine = SourceMgr->getSpellingLineNumber(FD->getBeginLoc(), &StmtBeginLineInvalid);
	bool StmtEndLineInvalid = true;
	unsigned StmtEndLine = SourceMgr->getSpellingLineNumber(FD->getEndLoc(), &StmtEndLineInvalid);

	if (StmtEndLineInvalid || StmtEndLineInvalid || (StmtBeginLine > StmtEndLine))
	{
		//行号非法 或者 起始大于结束
		return;
	}

	if (((StmtEndLine - StmtBeginLine) == 0) && (FD->getBeginLoc() == FD->getEndLoc()))
	{
		//0行 0列
		return;
	}

	unsigned currentLines = StmtEndLine - StmtBeginLine + 1;

	if (maxLines < currentLines)
	{
		maxLines = currentLines;
		maxLinesFunName = getFunctionSginatureString(FD);
	}
	if (minLines > currentLines)
	{
		minLines = currentLines;
		minLinesFunName = getFunctionSginatureString(FD);
	}
}


void FunctionInfoCheck::checkFunctionCyclomaticComplexity(const std::string funSginature, const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result)
{
	Stmt *FB = FD->getBody();
	if (!FB)
	{
		return;
	}
	ASTContext *AC = Result.Context;

	std::unique_ptr<CFG> TheCFG = CFG::buildCFG(nullptr, FB, AC, CFG::BuildOptions());
	if (!TheCFG)
	{
		return;
	}

	unsigned int cyclomaticComplexity = 0;
	unsigned int blocksNum = TheCFG->getNumBlockIDs();

	//统计CFG中边的数量
	std::set<CFGBlockEdge> cfgBlockEdgeSet;
	for (const CFGBlock * BB : *TheCFG)
	{
		if (!BB)
		{
			continue;
		}
		unsigned int start_id = BB->getBlockID();
		for (const CFGBlock *SB : BB->succs())
		{
			if (!SB)
			{
				continue;
			}
			unsigned int end_id = SB->getBlockID();
			if (start_id != end_id)
			{
				CFGBlockEdge edge(start_id, end_id);
				cfgBlockEdgeSet.insert(edge);
			}
		}
	}

	//std::cerr << cfgBlockEdgeSet.size() << " + 2 -" << blocksNum;
	if (cfgBlockEdgeSet.size() + 2 > blocksNum)
	{
		cyclomaticComplexity = cfgBlockEdgeSet.size() + 2 - blocksNum; //圈复杂度 = 边数 + 2 - 节点数
	}

	std::map<std::string, FunctionCountInfo>::iterator iter = FunCountMap.find(funSginature);
	if (iter != FunCountMap.end())
	{
		iter->second.cyclomatic_complexity = cyclomaticComplexity;
	}
	else
	{
		FunctionCountInfo FCI;
		FCI.cyclomatic_complexity = cyclomaticComplexity;
		FunCountMap[funSginature] = FCI;
	}
}


void FunctionInfoCheck::checkFunctionFanOut(const std::string funSginature, const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result)
{
	Stmt *FB = FD->getBody();
	if (!FB)
	{
		return;
	}

	//遍历并统计函数中的CallExpr数量作为扇出度
	FunctionCallExprCountVisitor v;
	v.TraverseFunctionDecl(const_cast<FunctionDecl *>(FD));
	
	unsigned int fan_out = v.MatchedFunctionDeclSet.size();

	std::map<std::string, FunctionCountInfo>::iterator iter = FunCountMap.find(funSginature);
	if (iter != FunCountMap.end())
	{
		iter->second.fan_out = fan_out;
	}
	else
	{
		FunctionCountInfo FCI;
		FCI.fan_out = fan_out;
		FunCountMap[funSginature] = FCI;
	}
}


void FunctionInfoCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP)
{
	PP->addCommentHandler(Handler.get());
}

std::string FunctionInfoCheck::getFunctionSginatureString(const FunctionDecl *FD)
{
	std::string res;
	std::string funQualifiedName = FD->getQualifiedNameAsString();
	if (funQualifiedName.empty())
	{
		return res;
	}
	else
	{
		res = funQualifiedName;
	}
	std::string funTypeStr = FD->getType().getAsString();
	std::string funRetTypeStr = FD->getReturnType().getAsString();
	if (!funTypeStr.empty() && !funRetTypeStr.empty() && funTypeStr.find(funRetTypeStr + " ") == 0)
	{
		res = funTypeStr.insert(funRetTypeStr.length() + 1, funQualifiedName);
	}
	return res;
}

void FunctionInfoCheck::onEndOfTranslationUnit()
{
	if (skipCheck) return;
	llvm::Json::Value jcountInfo = llvm::Json::LoadToJson(countInfoFilePath);
	if (jcountInfo.isNull() || !jcountInfo.isMember("Location"))
	{
		return;
	}

	if (minLines == UINT_MAX)
	{
		minLines = 0;
	}

	jcountInfo["MaxFunctionLines"] = maxLines;
	jcountInfo["MaxFunctionName"] = maxLinesFunName;
	jcountInfo["MinFunctionLines"] = minLines;
	jcountInfo["MinFunctionName"] = minLinesFunName;
	jcountInfo["FunctionsNumber"] = functionsNumber;
	jcountInfo["CommentLines"] = commentLines - commentBlanks; //注释行数不包含块注释中的空行数  20220726 会议决定
	if (jcountInfo.isMember("Blanks"))
	{
		blanks = jcountInfo["Blanks"].asUInt();
	}
	blanks = blanks + commentBlanks; //空行数需要加上块注释中的空行数 20220726 会议决定
	jcountInfo["Blanks"] = blanks;

	llvm::Json::Value functionCountJarray;
	functionCountJarray.resize(0);
	for (auto FI : FunCountMap)
	{
		llvm::Json::Value funJInfo;
		funJInfo["Function"] = FI.first;
		funJInfo["Fanout"] = FI.second.fan_out;
		funJInfo["CyclomaticComplexity"] = FI.second.cyclomatic_complexity;
		functionCountJarray.append(funJInfo);
	}
	jcountInfo["Functions"] = functionCountJarray;

	llvm::Json::JsonToSave(jcountInfo, countInfoFilePath);
}


} // namespace counter
} // namespace tidy
} // namespace clang
