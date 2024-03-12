#include "HTMLGenerate.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace clang {
namespace tidy {

HTMLGenerate::HTMLGenerate(SourceLocation Loc, StringRef Message, ClangTidyContext *Context)
    : Loc(Loc), Message(Message), Context(Context){
}

StringRef HTMLGenerate::showHelpJavascript() {
  return R"<<<(
<script type='text/javascript'>

var toggleHelp = function() {
    var hint = document.querySelector("#tooltiphint");
    var attributeName = "hidden";
    if (hint.hasAttribute(attributeName)) {
      hint.removeAttribute(attributeName);
    } else {
      hint.setAttribute("hidden", "true");
    }
};
window.addEventListener("keydown", function (event) {
  if (event.defaultPrevented) {
    return;
  }
  if (event.key == "?") {
    toggleHelp();
  } else {
    return;
  }
  event.preventDefault();
});
</script>
)<<<";
}

StringRef HTMLGenerate::generateKeyboardNavigationJavascript() {
  return R"<<<(
<script type='text/javascript'>
var digitMatcher = new RegExp("[0-9]+");

var querySelectorAllArray = function(selector) {
  return Array.prototype.slice.call(
    document.querySelectorAll(selector));
}

document.addEventListener("DOMContentLoaded", function() {
    querySelectorAllArray(".PathNav > a").forEach(
        function(currentValue, currentIndex) {
            var hrefValue = currentValue.getAttribute("href");
            currentValue.onclick = function() {
                scrollTo(document.querySelector(hrefValue));
                return false;
            };
        });
});

var findNum = function() {
    var s = document.querySelector(".selected");
    if (!s || s.id == "EndPath") {
        return 0;
    }
    var out = parseInt(digitMatcher.exec(s.id)[0]);
    return out;
};

var scrollTo = function(el) {
    querySelectorAllArray(".selected").forEach(function(s) {
        s.classList.remove("selected");
    });
    el.classList.add("selected");
    window.scrollBy(0, el.getBoundingClientRect().top -
        (window.innerHeight / 2));
}

var move = function(num, up, numItems) {
  if (num == 1 && up || num == numItems - 1 && !up) {
    return 0;
  } else if (num == 0 && up) {
    return numItems - 1;
  } else if (num == 0 && !up) {
    return 1 % numItems;
  }
  return up ? num - 1 : num + 1;
}

var numToId = function(num) {
  if (num == 0) {
    return document.getElementById("EndPath")
  }
  return document.getElementById("Path" + num);
};

var navigateTo = function(up) {
  var numItems = document.querySelectorAll(
      ".line > .msgEvent, .line > .msgControl").length;
  var currentSelected = findNum();
  var newSelected = move(currentSelected, up, numItems);
  var newEl = numToId(newSelected, numItems);

  // Scroll element into center.
  scrollTo(newEl);
};

window.addEventListener("keydown", function (event) {
  if (event.defaultPrevented) {
    return;
  }
  if (event.key == "j") {
    navigateTo(/*up=*/false);
  } else if (event.key == "k") {
    navigateTo(/*up=*/true);
  } else {
    return;
  }
  event.preventDefault();
}, true);
</script>
  )<<<";
}

// void HTMLGenerate::dumpCoverageData(
//     const PathDiagnostic &D,
//     const PathPieces &path,
//     llvm::raw_string_ostream &os) {

//   const FilesToLineNumsMap &ExecutedLines = D.getExecutedLines();

//   os << "var relevant_lines = {";
//   for (auto I = ExecutedLines.begin(),
//             E = ExecutedLines.end(); I != E; ++I) {
//     if (I != ExecutedLines.begin())
//       os << ", ";

//     os << "\"" << I->first.getHashValue() << "\": {";
//     for (unsigned LineNo : I->second) {
//       if (LineNo != *(I->second.begin()))
//         os << ", ";

//       os << "\"" << LineNo << "\": 1";
//     }
//     os << "}";
//   }

//   os << "};";
// }

std::string HTMLGenerate::showRelevantLinesJavascript() {
  std::string s;
  llvm::raw_string_ostream os(s);
  os << "<script type='text/javascript'>\n";
  SourceManager &SM = Context->getDiagEngineDiagEngineSourceManager();
  unsigned lineNumber = SM.getSpellingLineNumber(Loc);
  std::string re_line = "var relevant_lines = {\"1\": {\"" + std::to_string(lineNumber) + "\": 1}};\n";
  os << re_line;
  os << R"<<<(

var filterCounterexample = function (hide) {
  var tables = document.getElementsByClassName("code");
  for (var t=0; t<tables.length; t++) {
    var table = tables[t];
    var file_id = table.getAttribute("data-fileid");
    var lines_in_fid = relevant_lines[file_id];
    if (!lines_in_fid) {
      lines_in_fid = {};
    }
    var lines = table.getElementsByClassName("codeline");
    for (var i=0; i<lines.length; i++) {
        var el = lines[i];
        var lineNo = el.getAttribute("data-linenumber");
        if (!lines_in_fid[lineNo]) {
          if (hide) {
            el.setAttribute("hidden", "");
          } else {
            el.removeAttribute("hidden");
          }
        }
    }
  }
}

window.addEventListener("keydown", function (event) {
  if (event.defaultPrevented) {
    return;
  }
  if (event.key == "S") {
    var checked = document.getElementsByName("showCounterexample")[0].checked;
    filterCounterexample(!checked);
    document.getElementsByName("showCounterexample")[0].checked = !checked;
  } else {
    return;
  }
  event.preventDefault();
}, true);

document.addEventListener("DOMContentLoaded", function() {
    document.querySelector('input[name="showCounterexample"]').onchange=
        function (event) {
      filterCounterexample(this.checked);
    };
});
</script>

<form>
    <input type="checkbox" name="showCounterexample" id="showCounterexample" />
    <label for="showCounterexample">
       只展示相关的行
    </label>
</form>
)<<<";

  return os.str();
}

bool HTMLGenerate::GenerateHtml(){
    std::ifstream myfile("/tmp/result-path");
    std::string temp;
    getline(myfile,temp);
      
    std::string seed = "0123456789abcdef";
    std::string rand_str = "";
    for(unsigned i=0; i<6; i++)
    {
      rand_str += seed[rand()%16];
    }
    std::string outPath = temp + "/report-" + rand_str + ".html";
    std::ofstream outfile(outPath, std::ios::ate);

    SourceManager &SM = Context->getDiagEngineDiagEngineSourceManager();
    const LangOptions& LO = Context->getLangOpts();
    Preprocessor* PP = Context->getPreprocessor();
    Rewriter *rewriter = new Rewriter(SM, LO);
    FileID FID = SM.getFileID(Loc);
    const FileEntry* Entry = SM.getFileEntryForID(FID);

    llvm::SmallString<0> DirName;

    if (llvm::sys::path::is_relative(Entry->getName())) {
      llvm::sys::fs::current_path(DirName);
      DirName += '/';
    }

    unsigned LineNumber = SM.getSpellingLineNumber(Loc);
    unsigned ColumnNumber = SM.getSpellingColumnNumber(Loc);


    {
      FullSourceLoc Pos = FullSourceLoc(Loc, SM);

      if (!Pos.isValid())
        return false;

      std::pair<FileID, unsigned> LPosInfo = SM.getDecomposedExpansionLoc(Pos);

      if (LPosInfo.first != FID)
        return false;

      llvm::MemoryBufferRef Buf = SM.getBufferOrFake(LPosInfo.first);
      const char *FileStart = Buf.getBufferStart();

      // Compute the column number.  Rewind from the current position to the start
      // of the line.
      unsigned ColNo = SM.getColumnNumber(LPosInfo.first, LPosInfo.second);
      const char *TokInstantiationPtr =Pos.getExpansionLoc().getCharacterData();
      const char *LineStart = TokInstantiationPtr-ColNo;

      // Compute LineEnd.
      const char *LineEnd = TokInstantiationPtr;
      const char *FileEnd = Buf.getBufferEnd();

      while (*LineEnd != '\n' && LineEnd != FileEnd)
        ++LineEnd;

      // Compute the margin offset by counting tabs and non-tabs.
      unsigned PosNo = 0;
      for (const char* c = LineStart; c != TokInstantiationPtr; ++c)
        PosNo += *c == '\t' ? 8 : 1;

      // Create the html for the message.

      const char *Kind = "Standard";
      bool IsNote = false;
      // bool SuppressIndex = (max == 1);
      bool SuppressIndex = true;
      unsigned num = 1;
      unsigned max = 1;
      // switch (P.getKind()) {
      // case PathDiagnosticPiece::Event: Kind = "Event"; break;
      // case PathDiagnosticPiece::ControlFlow: Kind = "Control"; break;
      //   // Setting Kind to "Control" is intentional.
      // case PathDiagnosticPiece::Macro: Kind = "Control"; break;
      // case PathDiagnosticPiece::Note:
      //   Kind = "Note";
      //   IsNote = true;
      //   SuppressIndex = true;
      //   break;
      // case PathDiagnosticPiece::Call:
      // case PathDiagnosticPiece::PopUp:
      //   llvm_unreachable("Calls and extra notes should already be handled");
      // }
      std::string sbuf;
      llvm::raw_string_ostream os(sbuf);

      os << "\n<tr><td class=\"num\"></td><td class=\"line\"><div id=\"";

      if (IsNote)
        os << "Note" << num;
      else if (num == max)
        os << "EndPath";
      else
        os << "Path" << num;

      os << "\" class=\"msg";
      if (Kind)
        os << " msg" << Kind;
      os << "\" style=\"margin-left:" << PosNo << "ex";

      os << "; max-width:100em";

      os << "\">";

      if (!SuppressIndex) {
        os << "<table class=\"msgT\"><tr><td valign=\"top\">";
        os << "<div class=\"PathIndex";
        if (Kind) os << " PathIndex" << Kind;
        os << "\"></div>";

        if (num > 1) {
          os << "</td><td><div class=\"PathNav\"><a href=\"#Path"
             << (num - 1)
             << "\" title=\"Previous event ("
             << (num - 1)
             << ")\">&#x2190;</a></div>";
        }

        os << "</td><td>";
      }

      {
        os << "<span class='variable'>" << html::EscapeText(Message);

        if (!SuppressIndex) {
          os << "</td>";
          if (num < max) {
            os << "<td><div class=\"PathNav\"><a href=\"#";
            if (num == max - 1)
              os << "EndPath";
            else
              os << "Path" << (num + 1);
            os << "\" title=\"Next event ("
               << (num + 1)
               << ")\">&#x2192;</a></div></td>";
          }

          os << "</tr></table>";
        }
      }

      os << "</div></td></tr>";

      // Insert the new html.
      unsigned DisplayPos = LineEnd - FileStart;
      SourceLocation tLoc =
        SM.getLocForStartOfFile(LPosInfo.first).getLocWithOffset(DisplayPos);

      rewriter->InsertTextBefore(tLoc, os.str());
      // SourceLocation eLoc = tLoc.getLocWithOffset(6);
      // html::HighlightRange(*rewriter, Loc, tLoc, "<span class='variable'>", "");
    }


    {
      SmallString<256> Buf;
      llvm::raw_svector_ostream Out(Buf);

      // Write out the path indices with a right arrow and the message as a row.
      Out << "<tr><td valign='top'><div class='PathIndex PathIndexPopUp'>"
          << 1;

      // Also annotate the state transition with extra indices.
      Out << '.' << 1;

      Out << "</div></td><td>" << Message<< "</td></tr>";

      // If no report made at this range mark the variable and add the end tags.
      {

        Out << "</tbody></table></span>";
        html::HighlightRange(*rewriter, Loc, Loc,
                             "<span class='variable'>", Buf.c_str(),
                             /*IsTokenRange=*/true);
        html::HighlightRange(*rewriter, Loc, Loc, "",
                     "<table class='variable_popup'><tbody>",
                     /*IsTokenRange=*/true);
      }
    }

    html::EscapeText(*rewriter, FID);
    html::AddLineNumbers(*rewriter, FID);

    html::SyntaxHighlight(*rewriter, FID, *PP);
    html::HighlightMacros(*rewriter, FID, *PP);

    const RewriteBuffer *Buf = rewriter->getRewriteBufferFor(FID);

    rewriter->InsertTextBefore(SM.getLocForStartOfFile(FID), showHelpJavascript());
    rewriter->InsertTextBefore(SM.getLocForStartOfFile(FID), generateKeyboardNavigationJavascript());
    rewriter->InsertTextBefore(SM.getLocForStartOfFile(FID), showRelevantLinesJavascript());


    {
      std::string s;
      llvm::raw_string_ostream os(s);

      os << "<!-- REPORTHEADER -->\n"
         << "<h3>Bug详情</h3>\n<table class=\"simpletable\">\n"
            "<tr><td class=\"rowname\">文件:</td><td>"
         << html::EscapeText(DirName)
         << html::EscapeText(Entry->getName())
         << "</td></tr>\n<tr><td class=\"rowname\">警告信息:</td><td>"
            "<a href=\"#EndPath\">行数:"
         << LineNumber
         << ", 列数:"
         << ColumnNumber
         << "</a><br />"
         << Message << "</td></tr>\n";

      os << R"<<<(
</table>
<!-- REPORTSUMMARYEXTRA -->
<h3>源代码信息</h3>
<p>按 <a href="#" onclick="toggleHelp(); return false;">'?'</a>
键查看快捷方式</p>
<input type="checkbox" class="spoilerhider" id="showinvocation" />
<label for="showinvocation" >查看调用指令</label>
<div class="spoiler">clang -cc1 )<<<";
// os << html::EscapeText(DiagOpts.ToolInvocation);
os << R"<<<(
</div>
<div id='tooltiphint' hidden="true">
<p>快捷键: </p>
<ul>
<li>使用 'j/k' 键用于浏览页面</li>
<li>使用 'Shift+S' 只展示相关的行</li>
<li>使用 '?' 切换当前窗口</li>
</ul>
<a href="#" onclick="toggleHelp(); return false;">关闭</a>
</div>
)<<<";
      rewriter->InsertTextBefore(SM.getLocForStartOfFile(FID), os.str());
    }

  {
    std::string s;
    llvm::raw_string_ostream os(s);

    StringRef BugDesc = Message;
    if (!BugDesc.empty())
      os << "\n<!-- BUGDESC " << BugDesc << " -->\n";

    StringRef BugType = "Standard Check";
    if (!BugType.empty())
      os << "\n<!-- BUGTYPE " << BugType << " -->\n";

    os << "\n<!-- BUGCATEGORY " << "Standard" << " -->\n";

    os << "\n<!-- BUGFILE " << DirName << Entry->getName() << " -->\n";

    os << "\n<!-- FILENAME " << llvm::sys::path::filename(Entry->getName()) << " -->\n";

    os  << "\n<!-- FUNCTIONNAME " <<  "" << " -->\n";

    os << "\n<!-- ISSUEHASHCONTENTOFLINEINCONTEXT "
       << "39b1419c63ce2a2a0099039c575ab748"
       << " -->\n";

    os << "\n<!-- BUGLINE "
       << LineNumber
       << " -->\n";

    os << "\n<!-- BUGCOLUMN "
      << ColumnNumber
      << " -->\n";

    os << "\n<!-- BUGPATHLENGTH " << 1 << " -->\n";

    // Mark the end of the tags.
    os << "\n<!-- BUGMETAEND -->\n";

    // Insert the text.
    rewriter->InsertTextBefore(SM.getLocForStartOfFile(FID), os.str());
  }


    html::AddHeaderFooterInternalBuiltinCSS(*rewriter, FID, Entry->getName());

    std::string file;
    llvm::raw_string_ostream fs(file);

    if(Buf) {
        for (auto BI : *Buf)
            fs << BI;
    }

    std::string report = fs.str();
    // std::cout << report << std::endl;
    outfile << report;
    outfile.close();
    return true;
}

} // namespace tidy
} // namespace clang
