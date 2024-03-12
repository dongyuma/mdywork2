====================================================
Extra Clang Tools 12.0.0 (In-Progress) Release Notes
==================================================== 

.. contents::
   :local:
   :depth: 3

Written by the `LLVM Team <https://llvm.org/>`_

.. warning::

   These are in-progress notes for the upcoming Extra Clang Tools 12 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.

Introduction
============ 

This document contains the release notes for the Extra Clang Tools, part of the
Clang release 12.0.0. Here we describe the status of the Extra Clang Tools in
some detail, including major improvements from the previous release and new
feature work. All LLVM releases may be downloaded from the `LLVM releases web
site <https://llvm.org/releases/>`_.

For more information about Clang or LLVM, including information about
the latest release, please see the `Clang Web Site <https://clang.llvm.org>`_ or
the `LLVM Web Site <https://llvm.org>`_.

Note that if you are reading this file from a Git checkout or the
main Clang web page, this document applies to the *next* release, not
the current one. To see the release notes for a specific release, please
see the `releases page <https://llvm.org/releases/>`_.

What's New in Extra Clang Tools 12.0.0?
=======================================

Some of the major new features and improvements to Extra Clang Tools are listed
here. Generic improvements to Extra Clang Tools as a whole or to its underlying
infrastructure are described first, followed by tool-specific sections.

Major New Features
------------------

...

Improvements to clangd
----------------------

- clangd's memory usage is significantly reduced on most Linux systems.
  In particular, memory usage should not increase dramatically over time.

  The standard allocator on most systems is glibc's ptmalloc2, and it creates
  disproportionately large heaps when handling clangd's allocation patterns.
  By default, clangd will now periodically call ``malloc_trim`` to release free
  pages on glibc systems.

  Users of other allocators (such as ``jemalloc`` or ``tcmalloc``) on glibc
  systems can disable this using ``--malloc_trim=0`` or the CMake flag
  ``-DCLANGD_MALLOC_TRIM=0``.

Improvements to clang-doc
-------------------------

The improvements are...

Improvements to clang-query
---------------------------

- The IgnoreImplicitCastsAndParentheses traversal mode has been removed.

Improvements to clang-rename
----------------------------

The improvements are...

Improvements to clang-tidy
--------------------------

- Checks that allow configuring names of headers to include now support wrapping 
 
- Checks that allow configuring names of headers to include now support wrapping
  the include in angle brackets to create a system include. For example,
  :doc:`cppcoreguidelines-init-variables
  <clang-tidy/checks/cppcoreguidelines-init-variables>` and
  :doc:`modernize-make-unique <clang-tidy/checks/modernize-make-unique>`.

- CheckOptions that take boolean values now support all spellings supported in 
  the `YAML format <https://yaml.org/type/bool.html>`_.
 
New modules
^^^^^^^^^^^

- New ``altera`` module.

  Includes checks related to OpenCL for FPGA coding guidelines, based on the
  `Altera SDK for OpenCL: Best Practices Guide
  <https://www.altera.com/en_US/pdfs/literature/hb/opencl-sdk/aocl_optimization_guide.pdf>`_.

- New ``concurrency`` module.

  Includes checks related to concurrent programming (e.g. threads, fibers,
  coroutines, etc.).

New checks
^^^^^^^^^^

- New :doc:`altera-kernel-name-restriction
  <clang-tidy/checks/altera-kernel-name-restriction>` check.

  Finds kernel files and include directives whose filename is `kernel.cl`,
  `Verilog.cl`, or `VHDL.cl`.

- New :doc:`altera-single-work-item-barrier
  <clang-tidy/checks/altera-single-work-item-barrier>` check.

  Finds OpenCL kernel functions that call a barrier function but do not call
  an ID function.

- New :doc:`altera-struct-pack-align
  <clang-tidy/checks/altera-struct-pack-align>` check.

  Finds structs that are inefficiently packed or aligned, and recommends
  packing and/or aligning of said structs as needed.

- New :doc:`bugprone-interrupt-function-storage
  <clang-tidy/checks/bugprone-interrupt-function-storage>` check.

  FIXME: add release notes.

- New :doc:`bz12-basic-block-unused-or-repeatused
  <clang-tidy/checks/bz12-basic-block-unused-or-repeatused>` check.

  FIXME: add release notes.

- New :doc:`bz12-branch-assignment-difference
  <clang-tidy/checks/bz12-branch-assignment-difference>` check.

  FIXME: add release notes.

- New :doc:`bz12-branch-variable-statistics
  <clang-tidy/checks/bz12-branch-variable-statistics>` check.

  FIXME: add release notes.

- New :doc:`bz12-conditional-statement-equality
  <clang-tidy/checks/bz12-conditional-statement-equality>` check.

  FIXME: add release notes.

- New :doc:`bz12-extern-defination-declare-type
  <clang-tidy/checks/bz12-extern-defination-declare-type>` check.

  FIXME: add release notes.

- New :doc:`bz12-infinitel-loop
  <clang-tidy/checks/bz12-infinitel-loop>` check.

  FIXME: add release notes.

- New :doc:`bz12-initial-value
  <clang-tidy/checks/bz12-initial-value>` check.

  FIXME: add release notes.

- New :doc:`bz12-memset-multiple-rank-pointer
  <clang-tidy/checks/bz12-memset-multiple-rank-pointer>` check.

  FIXME: add release notes.

- New :doc:`bz12-struct-default-alignment
  <clang-tidy/checks/bz12-struct-default-alignment>` check.

  FIXME: add release notes.

- New :doc:`bz12-struct-defination-declare-consistence
  <clang-tidy/checks/bz12-struct-defination-declare-consistence>` check.

  FIXME: add release notes.

- New :doc:`bz12-test-sss-demo
  <clang-tidy/checks/bz12-test-sss-demo>` check.

  FIXME: add release notes.

- New :doc:`bz12-type-cast
  <clang-tidy/checks/bz12-type-cast>` check.

  FIXME: add release notes.

- New :doc:`bz12-uninitialized-struct-element
  <clang-tidy/checks/bz12-uninitialized-struct-element>` check.

  FIXME: add release notes.

- New :doc:`bz12-unused-struct-element
  <clang-tidy/checks/bz12-unused-struct-element>` check.

  FIXME: add release notes.

- New :doc:`bz12-variable-name-analysis
  <clang-tidy/checks/bz12-variable-name-analysis>` check.

  FIXME: add release notes.

- New :doc:`bz12-variable-name-in-if-analysis
  <clang-tidy/checks/bz12-variable-name-in-if-analysis>` check.

  FIXME: add release notes.

- New :doc:`counter-function-info
  <clang-tidy/checks/counter-function-info>` check.

  FIXME: add release notes.

- New :doc:`cppcoreguidelines-prefer-member-initializer
  <clang-tidy/checks/cppcoreguidelines-prefer-member-initializer>` check.

  Finds member initializations in the constructor body which can be placed into
  the initialization list instead.

- New :doc:`bugprone-misplaced-pointer-arithmetic-in-alloc
  <clang-tidy/checks/bugprone-misplaced-pointer-arithmetic-in-alloc>` check.

- New :doc:`bugprone-redundant-branch-condition
  <clang-tidy/checks/bugprone-redundant-branch-condition>` check.

  Finds condition variables in nested ``if`` statements that were also checked
  in the outer ``if`` statement and were not changed.

- New :doc:`concurrency-mt-unsafe <clang-tidy/checks/concurrency-mt-unsafe>`
  check.

  Finds thread-unsafe functions usage. Currently knows about POSIX and
  Glibc function sets.

- New :doc:`bugprone-signal-handler
  <clang-tidy/checks/bugprone-signal-handler>` check.

  Finds functions registered as signal handlers that call non asynchronous-safe
  functions.

- New :doc:`cert-sig30-c
  <clang-tidy/checks/cert-sig30-c>` check.

  Alias to the :doc:`bugprone-signal-handler
  <clang-tidy/checks/bugprone-signal-handler>` check.

- New :doc:`custom-GlobalUninitial
  <clang-tidy/checks/custom-GlobalUninitial>` check.

  FIXME: add release notes.

- New :doc:`gb5369-ArraySubscriptPP
  <clang-tidy/checks/gb5369-ArraySubscriptPP>` check.

  FIXME: add release notes.

- New :doc:`gb5369-BanExeCodeBeforeInclude
  <clang-tidy/checks/gb5369-BanExeCodeBeforeInclude>` check.

  FIXME: add release notes.

- New :doc:`gb5369-ConditionalBodyBrace
  <clang-tidy/checks/gb5369-ConditionalBodyBrace>` check.

  FIXME: add release notes.

- New :doc:`gb5369-EnumInit
  <clang-tidy/checks/gb5369-EnumInit>` check.

  FIXME: add release notes.

- New :doc:`gb5369-Function-Parameters
  <clang-tidy/checks/gb5369-Function-Parameters>` check.

  FIXME: add release notes.

- New :doc:`gb5369-FunctionVoid
  <clang-tidy/checks/gb5369-FunctionVoid>` check.

  FIXME: add release notes.

- New :doc:`gb5369-GotoRelated
  <clang-tidy/checks/gb5369-GotoRelated>` check.

  FIXME: add release notes.

- New :doc:`gb5369-HeaderUseDirectPath
  <clang-tidy/checks/gb5369-HeaderUseDirectPath>` check.

  FIXME: add release notes.

- New :doc:`gb5369-Invalid-Varibale-Name
  <clang-tidy/checks/gb5369-Invalid-Varibale-Name>` check.

  FIXME: add release notes.

- New :doc:`gb5369-Loop-Control
  <clang-tidy/checks/gb5369-Loop-Control>` check.

  FIXME: add release notes.

- New :doc:`gb5369-MacroRelated
  <clang-tidy/checks/gb5369-MacroRelated>` check.

  FIXME: add release notes.

- New :doc:`gb5369-NameForbiddenReuse
  <clang-tidy/checks/gb5369-NameForbiddenReuse>` check.

  FIXME: add release notes.

- New :doc:`gb5369-Nested-Comment
  <clang-tidy/checks/gb5369-Nested-Comment>` check.

  FIXME: add release notes.

- New :doc:`gb5369-PreprocessIfEndif
  <clang-tidy/checks/gb5369-PreprocessIfEndif>` check.

  FIXME: add release notes.

- New :doc:`gb5369-StructDeclIncomplete
  <clang-tidy/checks/gb5369-StructDeclIncomplete>` check.

  FIXME: add release notes.

- New :doc:`gb5369-StructHaveEmptySpace
  <clang-tidy/checks/gb5369-StructHaveEmptySpace>` check.

  FIXME: add release notes.

- New :doc:`gb5369-StructInit
  <clang-tidy/checks/gb5369-StructInit>` check.

  FIXME: add release notes.

- New :doc:`gb5369-TypedefRedefine
  <clang-tidy/checks/gb5369-TypedefRedefine>` check.

  FIXME: add release notes.

- New :doc:`gb5369-Variablename-Duplication
  <clang-tidy/checks/gb5369-Variablename-Duplication>` check.

  FIXME: add release notes.

- New :doc:`gb5369-test_5369
  <clang-tidy/checks/gb5369-test_5369>` check.

  FIXME: add release notes.

- New :doc:`gb8114-EmptyIf
  <clang-tidy/checks/gb8114-EmptyIf>` check.

  FIXME: add release notes.

- New :doc:`gb8114-ExternDeclName
  <clang-tidy/checks/gb8114-ExternDeclName>` check.

  FIXME: add release notes.

- New :doc:`gb8114-ExternInit
  <clang-tidy/checks/gb8114-ExternInit>` check.

  FIXME: add release notes.

- New :doc:`gb8114-Functionvoid
  <clang-tidy/checks/gb8114-Functionvoid>` check.

- New :doc:`gb8114-FunctionVoid
  <clang-tidy/checks/gb8114-FunctionVoid>` check.

  FIXME: add release notes.

- New :doc:`gb8114-HeaderUseDirectPath
  <clang-tidy/checks/gb8114-HeaderUseDirectPath>` check.

  FIXME: add release notes.

- New :doc:`gb8114-Mustdefinename
  <clang-tidy/checks/gb8114-Mustdefinename>` check.

- New :doc:`gb8114-Structhaveemptyspace
  <clang-tidy/checks/gb8114-Structhaveemptyspace>` check.

  FIXME: add release notes.

- New :doc:`gb8114-array-init
  <clang-tidy/checks/gb8114-array-init>` check.

  FIXME: add release notes.

- New :doc:`gb8114-assign-in-sizeof
  <clang-tidy/checks/gb8114-assign-in-sizeof>` check.

  FIXME: add release notes.

- New :doc:`gb8114-ban-bit-logical
  <clang-tidy/checks/gb8114-ban-bit-logical>` check.
 
- New :doc:`gb8114-call-void-function
  <clang-tidy/checks/gb8114-call-void-function>` check.

  FIXME: add release notes.

- New :doc:`gb8114-case-in-different-layer
  <clang-tidy/checks/gb8114-case-in-different-layer>` check.

  FIXME: add release notes.

- New :doc:`gb8114-condition-with-brace
  <clang-tidy/checks/gb8114-condition-with-brace>` check.

  FIXME: add release notes.

- New :doc:`gb8114-char-symbol
  <clang-tidy/checks/gb8114-char-symbol>` check.

  FIXME: add release notes.

- New :doc:`gb8114-conditional-expression-without-assignment-statement
  <clang-tidy/checks/gb8114-conditional-expression-without-assignment-statement>` check.

  FIXME: add release notes.

- New :doc:`gb8114-conditional-goto
  <clang-tidy/checks/gb8114-conditional-goto>` check.

  FIXME: add release notes.

- New :doc:`gb8114-constant-logical
  <clang-tidy/checks/gb8114-constant-logical>` check.

- New :doc:`gb8114-bool-switch
  <clang-tidy/checks/gb8114-bool-switch>` check.

- New :doc:`gb8114-anti-null-switch
  <clang-tidy/checks/gb8114-anti-null-switch>` check.
  FIXME: add release notes.

- New :doc:`gb8114-bit-define
  <clang-tidy/checks/gb8114-bit-define>` check.

  FIXME: add release notes.

- New :doc:`gb8114-cxxrecord-definition-identifier
  <clang-tidy/checks/gb8114-cxxrecord-definition-identifier>` check.

  FIXME: add release notes.

- New :doc:`gb8114-default-in-switch
  <clang-tidy/checks/gb8114-default-in-switch>` check.

  FIXME: add release notes.

- New :doc:`gb8114-end-with-break
  <clang-tidy/checks/gb8114-end-with-break>` check.

  FIXME: add release notes.

- New :doc:`gb8114-enum-error-initial
  <clang-tidy/checks/gb8114-enum-error-initial>` check.
- New :doc:`gb8114-enum-variable-same-with-other-variable
  <clang-tidy/checks/gb8114-enum-variable-same-with-other-variable>` check.

  <clang-tidy/checks/gb8114-PointerAllocV2>` check.

  FIXME: add release notes.

- New :doc:`gb8114-Structhaveemptyspace
  <clang-tidy/checks/gb8114-Structhaveemptyspace>` check.
  FIXME: add release notes.

- New :doc:`gb8114-error-use-of-loop-variable
  <clang-tidy/checks/gb8114-error-use-of-loop-variable>` check.
- New :doc:`gb8114-extern-variable-init
  <clang-tidy/checks/gb8114-extern-variable-init>` check.
- New :doc:`gb8114-extern-decl-in-function-body
  <clang-tidy/checks/gb8114-extern-decl-in-function-body>` check.

  FIXME: add release notes.

- New :doc:`gb8114-floatliteral-to-int
  <clang-tidy/checks/gb8114-floatliteral-to-int>` check.

  FIXME: add release notes.

- New :doc:`gb8114-float-equal
  <clang-tidy/checks/gb8114-float-equal>` check.

- New :doc:`gb8114-function-call-itself
  <clang-tidy/checks/gb8114-function-call-itself>` check.

  FIXME: add release notes.

- New :doc:`gb8114-function-declare-different-with-definition
  <clang-tidy/checks/gb8114-function-declare-different-with-definition>` check.

  FIXME: add release notes.

- New :doc:`gb8114-function-declare-same
  <clang-tidy/checks/gb8114-function-declare-same>` check.

  FIXME: add release notes.

- New :doc:`gb8114-function-parameter-explicit-void
  <clang-tidy/checks/gb8114-function-parameter-explicit-void>` check.

  FIXME: add release notes.

- New :doc:`gb8114-function-parameter-same-with-global-variable
  <clang-tidy/checks/gb8114-function-parameter-same-with-global-variable>` check.

  FIXME: add release notes.

- New :doc:`gb8114-function-void
  <clang-tidy/checks/gb8114-function-void>` check.
- New :doc:`gb8114-if-elseif-with-else
  <clang-tidy/checks/gb8114-if-elseif-with-else>` check.

  FIXME: add release notes.

- New :doc:`gb8114-if-or-else-null
  <clang-tidy/checks/gb8114-if-or-else-null>` check.

  FIXME: add release notes.

- New :doc:`gb8114-infinite-loop-statement-without-null-for-condition
  <clang-tidy/checks/gb8114-infinite-loop-statement-without-null-for-condition>` check.

- New :doc:`gb8114-include-header-use-direct-path
  <clang-tidy/checks/gb8114-include-header-use-direct-path>` check.

  FIXME: add release notes.

- New :doc:`gb8114-invalid-variable-name
  <clang-tidy/checks/gb8114-invalid-variable-name>` check.

  FIXME: add release notes.

- New :doc:`gb8114-label-reuse-prohibit
  <clang-tidy/checks/gb8114-label-reuse-prohibit>` check.

  FIXME: add release notes.

- New :doc:`gb8114-local-variable-name-same-as-global
  <clang-tidy/checks/gb8114-local-variable-name-same-as-global>` check.

  FIXME: add release notes.

- New :doc:`gb8114-logical-expr-operation-parenthese
  <clang-tidy/checks/gb8114-logical-expr-operation-parenthese>` check.
- New :doc:`gb8114-logical-variable-compare
  <clang-tidy/checks/gb8114-logical-variable-compare>` check.
- New :doc:`gb8114-logical-expr-explicit-parenthses
  <clang-tidy/checks/gb8114-logical-expr-explicit-parenthses>` check.

  FIXME: add release notes.

- New :doc:`gb8114-nested-annotation
  <clang-tidy/checks/gb8114-nested-annotation>` check.

  FIXME: add release notes.

- New :doc:`gb8114-only-default-in-switch
  <clang-tidy/checks/gb8114-only-default-in-switch>` check.

  FIXME: add release notes.

- New :doc:`gb8114-pointer-beyond-two-levels
  <clang-tidy/checks/gb8114-pointer-beyond-two-levels>` check.

  FIXME: add release notes.

- New :doc:`gb8114-pointer-initial
  <clang-tidy/checks/gb8114-pointer-initial>` check.

  FIXME: add release notes.

- New :doc:`gb8114-pointer-logical-compare
  <clang-tidy/checks/gb8114-pointer-logical-compare>` check.

  FIXME: add release notes.

- New :doc:`gb8114-real-parameter-same-with-formal-parameter
  <clang-tidy/checks/gb8114-real-parameter-same-with-formal-parameter>` check.

  FIXME: add release notes.

- New :doc:`gb8114-redefine-typedef
  <clang-tidy/checks/gb8114-redefine-typedef>` check.

  FIXME: add release notes.

- New :doc:`gb8114-return-local-variable-address
  <clang-tidy/checks/gb8114-return-local-variable-address>` check.

  FIXME: add release notes.

- New :doc:`gb8114-struct-anonymous-field
  <clang-tidy/checks/gb8114-struct-anonymous-field>` check.

  FIXME: add release notes.

- New :doc:`gb8114-type-cast
  <clang-tidy/checks/gb8114-type-cast>` check.

  FIXME: add release notes.

- New :doc:`gb8114-unsigned-compare-with-signed
  <clang-tidy/checks/gb8114-unsigned-compare-with-signed>` check.

  FIXME: add release notes.

- New :doc:`gb8114-PointerMalloc
  <clang-tidy/checks/gb8114-PointerMalloc>` check.

  FIXME: add release notes.

- New :doc:`gb8114-PointerMalloc
  <clang-tidy/checks/gb8114-PointerMalloc>` check.

  FIXME: add release notes.

- New :doc:`gb8114-unsigned-compare-with-zero
  <clang-tidy/checks/gb8114-unsigned-compare-with-zero>` check.

  FIXME: add release notes.

- New :doc:`gb8114-usage-of-pointer
  <clang-tidy/checks/gb8114-usage-of-pointer>` check.

  FIXME: add release notes.

- New :doc:`gb8114-use-function-as-pointer
  <clang-tidy/checks/gb8114-use-function-as-pointer>` check.
- New :doc:`gb8114-use-enum-out-of-bound
  <clang-tidy/checks/gb8114-use-enum-out-of-bound>` check.

  FIXME: add release notes.

- New :doc:`gb8114-use-of-gets
  <clang-tidy/checks/gb8114-use-of-gets>` check.

  FIXME: add release notes.

- New :doc:`gb8114-use-unaryoperator-in-callexpr-or-binaryoperator
  <clang-tidy/checks/gb8114-use-unaryoperator-in-callexpr-or-binaryoperator>` check.
- New :doc:`gb8114-variable-declare-same-name-with-function
  <clang-tidy/checks/gb8114-variable-declare-same-name-with-function>` check.

  FIXME: add release notes.

- New :doc:`gb8114-variable-name-same-with-typedef
  <clang-tidy/checks/gb8114-variable-name-same-with-typedef>` check.

  FIXME: add release notes.

- New :doc:`gb8114-variable-use
  <clang-tidy/checks/gb8114-variable-use>` check.
  
- New :doc:`gb8114-use-of-setjmp-or-longjmp
  <clang-tidy/checks/gb8114-use-of-setjmp-or-longjmp>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-define-match
  <clang-tidy/checks/gjb8114-define-match>` check.
- New :doc:`gjb8114-code-before-include
  <clang-tidy/checks/gjb8114-code-before-include>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-error-declare-of-struct
  <clang-tidy/checks/gjb8114-error-declare-of-struct>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-function-declare-different-with-prototype
  <clang-tidy/checks/gjb8114-function-declare-different-with-prototype>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-header-prohibited-from-include-repeatedly
  <clang-tidy/checks/gjb8114-header-prohibited-from-include-repeatedly>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-keyword-related
  <clang-tidy/checks/gjb8114-keyword-related>` check.

- New :doc:`gjb8114-macro-define-repeated-prohibit
  <clang-tidy/checks/gjb8114-macro-define-repeated-prohibit>` check.
  
- New :doc:`gjb8114-global-or-static-variable-depend-on-system
  <clang-tidy/checks/gjb8114-global-or-static-variable-depend-on-system>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-macro-related
  <clang-tidy/checks/gjb8114-macro-related>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-no-use-return-value-without-void
  <clang-tidy/checks/gjb8114-no-use-return-value-without-void>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-no-use-transform
  <clang-tidy/checks/gjb8114-no-use-transform>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-number-suffix-use
  <clang-tidy/checks/gjb8114-number-suffix-use>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-pointer-without-check-null
  <clang-tidy/checks/gjb8114-pointer-without-check-null>` check.
- New :doc:`gjb8114-octonary-number-use
  <clang-tidy/checks/gjb8114-octonary-number-use>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-pointer-assign-null-after-free
  <clang-tidy/checks/gjb8114-pointer-assign-null-after-free>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-return-type-same-with-define
  <clang-tidy/checks/gjb8114-return-type-same-with-define>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-sequential-call-same-variable
  <clang-tidy/checks/gjb8114-sequential-call-same-variable>` check.
- New :doc:`gjb8114-shift-signed-number
  <clang-tidy/checks/gjb8114-shift-signed-number>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-standard-library-function-rename-prohibit
  <clang-tidy/checks/gjb8114-standard-library-function-rename-prohibit>` check.
- New :doc:`gjb8114-string-without-end-symbol
  <clang-tidy/checks/gjb8114-string-without-end-symbol>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-unreachable-code
  <clang-tidy/checks/gjb8114-unreachable-code>` check.

  FIXME: add release notes.

- New :doc:`gjb8114-volatile-variable-repeat-in-binaryoperator
  <clang-tidy/checks/gjb8114-volatile-variable-repeat-in-binaryoperator>` check.
- New :doc:`gjb8114-unuse-of-static-function
  <clang-tidy/checks/gjb8114-unuse-of-static-function>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-catch-exception-throw-in-destructor
  <clang-tidy/checks/gjb8114cpp-catch-exception-throw-in-destructor>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-const-cast-pointer-reference
  <clang-tidy/checks/gjb8114cpp-const-cast-pointer-reference>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-const-member-function-return
  <clang-tidy/checks/gjb8114cpp-const-member-function-return>` check.

- New :doc:`gjb8114cpp-default-constructor-must-exist
  <clang-tidy/checks/gjb8114cpp-default-constructor-must-exist>` check.

  类中必须明确定义缺省构造函数。

- New :doc:`gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class
  <clang-tidy/checks/gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class>` check.
- New :doc:`gjb8114cpp-diamond-virtual-inherit
  <clang-tidy/checks/gjb8114cpp-diamond-virtual-inherit>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer
  <clang-tidy/checks/gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private
  <clang-tidy/checks/gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-each-specific-throw-has-catch
  <clang-tidy/checks/gjb8114cpp-each-specific-throw-has-catch>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-explicitly-throw-null
  <clang-tidy/checks/gjb8114cpp-explicitly-throw-null>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-fixed-array-param-passing
  <clang-tidy/checks/gjb8114cpp-fixed-array-param-passing>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-global-variables-prohibited-in-constructors
  <clang-tidy/checks/gjb8114cpp-global-variables-prohibited-in-constructors>` check.
  
- New :doc:`gjb8114cpp-member-function-mutable-return
  <clang-tidy/checks/gjb8114cpp-member-function-mutable-return>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-member-variables-initialized-in-the-constructor
  <clang-tidy/checks/gjb8114cpp-member-variables-initialized-in-the-constructor>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference
  <clang-tidy/checks/gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference>` check.

  FIXME: add release notes.
  
- New :doc:`gjb8114cpp-reinterpret-cast-class-pointer
  <clang-tidy/checks/gjb8114cpp-reinterpret-cast-class-pointer>` check.
  
  FIXME: add release notes.

- New :doc:`gjb8114cpp-single-parameter-constructor-set-explicit
  <clang-tidy/checks/gjb8114cpp-single-parameter-constructor-set-explicit>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class
  <clang-tidy/checks/gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class>` check.

  FIXME: add release notes.

- New :doc:`gjb8114cpp-virtual-destructors-in-class-with-virtual-functions
  <clang-tidy/checks/gjb8114cpp-virtual-destructors-in-class-with-virtual-functions>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-Bangoto
  <clang-tidy/checks/misrac2012-Bangoto>` check.
- New :doc:`misrac2012-ArrayStatic
  <clang-tidy/checks/misrac2012-ArrayStatic>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-Banselfcallfunc
  <clang-tidy/checks/misrac2012-Banselfcallfunc>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-BitField
  <clang-tidy/checks/misrac2012-BitField>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-BoolSwitch
  <clang-tidy/checks/misrac2012-BoolSwitch>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-CaseBreak
  <clang-tidy/checks/misrac2012-CaseBreak>` check.

  FIXME: add release notes.


- New :doc:`misrac2012-CondBodyBrace
  <clang-tidy/checks/misrac2012-CondBodyBrace>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-CondExpShouldBeChange
  <clang-tidy/checks/misrac2012-CondExpShouldBeChange>` check.

- New :doc:`misrac2012-CastConst
  <clang-tidy/checks/misrac2012-CastConst>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-CommentEnd
  <clang-tidy/checks/misrac2012-CommentEnd>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-DeclImplic
  <clang-tidy/checks/misrac2012-DeclImplic>` check.


  FIXME: add release notes.

- New :doc:`misrac2012-DefaultLabel
  <clang-tidy/checks/misrac2012-DefaultLabel>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-DefaultPosition
  <clang-tidy/checks/misrac2012-DefaultPosition>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-FloatLoopCounter
  <clang-tidy/checks/misrac2012-FloatLoopCounter>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-IfCondShouldBeBool
  <clang-tidy/checks/misrac2012-IfCondShouldBeBool>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-Ifelseif
  <clang-tidy/checks/misrac2012-Ifelseif>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-DifferCast
  <clang-tidy/checks/misrac2012-DifferCast>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-ExplicitReturn
  <clang-tidy/checks/misrac2012-ExplicitReturn>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-FunctionPtr
  <clang-tidy/checks/misrac2012-FunctionPtr>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-IncompleteType
  <clang-tidy/checks/misrac2012-IncompleteType>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-NonvoidFunc
  <clang-tidy/checks/misrac2012-NonvoidFunc>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-NullPtr
  <clang-tidy/checks/misrac2012-NullPtr>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-OctalNum
  <clang-tidy/checks/misrac2012-OctalNum>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-OctalTerm
  <clang-tidy/checks/misrac2012-OctalTerm>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-SingleByteField
  <clang-tidy/checks/misrac2012-SingleByteField>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-StdargFeature
  <clang-tidy/checks/misrac2012-StdargFeature>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-StringAssign
  <clang-tidy/checks/misrac2012-StringAssign>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-SwitchBranch
  <clang-tidy/checks/misrac2012-SwitchBranch>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-SwitchLabel
  <clang-tidy/checks/misrac2012-SwitchLabel>` check.

- New :doc:`misrac2012-BanDefineInReservedMacro
  <clang-tidy/checks/misrac2012-BanDefineInReservedMacro>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-BanatoinStdlib
  <clang-tidy/checks/misrac2012-BanatoinStdlib>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-Bansomeheadfile
  <clang-tidy/checks/misrac2012-Bansomeheadfile>` check.

  FIXME: add release notes.


- New :doc:`misrac2012-UnsignedTypeSuffix
  <clang-tidy/checks/misrac2012-UnsignedTypeSuffix>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-VarName
  <clang-tidy/checks/misrac2012-VarName>` check.

- New :doc:`misrac2012-GlobalDiffLocal
  <clang-tidy/checks/misrac2012-GlobalDiffLocal>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-InterNotOverrideExter
  <clang-tidy/checks/misrac2012-InterNotOverrideExter>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-UniqueExternal
  <clang-tidy/checks/misrac2012-UniqueExternal>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-UniqueMacro
  <clang-tidy/checks/misrac2012-UniqueMacro>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-UniqueTag
  <clang-tidy/checks/misrac2012-UniqueTag>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-UniqueTypedef
  <clang-tidy/checks/misrac2012-UniqueTypedef>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-assigned-inappropriate-type
  <clang-tidy/checks/misrac2012-assigned-inappropriate-type>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-compatible-declarations
  <clang-tidy/checks/misrac2012-compatible-declarations>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword
  <clang-tidy/checks/misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-enum-implicitly-constant
  <clang-tidy/checks/misrac2012-enum-implicitly-constant>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-escape-sequence-termination
  <clang-tidy/checks/misrac2012-escape-sequence-termination>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-external-identifiers-shall-be-distinct
  <clang-tidy/checks/misrac2012-external-identifiers-shall-be-distinct>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-file-stream-pointer-dereference
  <clang-tidy/checks/misrac2012-file-stream-pointer-dereference>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-function-parameters-name
  <clang-tidy/checks/misrac2012-function-parameters-name>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-goto-label-different-block
  <clang-tidy/checks/misrac2012-goto-label-different-block>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-header-use-direct-path
  <clang-tidy/checks/misrac2012-header-use-direct-path>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-include-ban-header
  <clang-tidy/checks/misrac2012-include-ban-header>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-initializer-lists-shall-not-contain-persistent-side-effects
  <clang-tidy/checks/misrac2012-initializer-lists-shall-not-contain-persistent-side-effects>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-inline-function-static-storage
  <clang-tidy/checks/misrac2012-inline-function-static-storage>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-line-comments-used
  <clang-tidy/checks/misrac2012-line-comments-used>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-list-designated-initialized
  <clang-tidy/checks/misrac2012-list-designated-initialized>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-macro-identifiers-shall-be-distinct
  <clang-tidy/checks/misrac2012-macro-identifiers-shall-be-distinct>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-macro-if-elif-controlling-expression-shall-evaluate
  <clang-tidy/checks/misrac2012-macro-if-elif-controlling-expression-shall-evaluate>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-operator-inappropriate-type
  <clang-tidy/checks/misrac2012-operator-inappropriate-type>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-persistent-side-effects-not-same
  <clang-tidy/checks/misrac2012-persistent-side-effects-not-same>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-restrict-qualifier-pointer
  <clang-tidy/checks/misrac2012-restrict-qualifier-pointer>` check.
  
- New :doc:`misrac2012-convert-between-pointer-and-arithmetic
  <clang-tidy/checks/misrac2012-convert-between-pointer-and-arithmetic>` check.


  FIXME: add release notes.

- New :doc:`misrac2012-testmisra
  <clang-tidy/checks/misrac2012-testmisra>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-unused-return-value
  <clang-tidy/checks/misrac2012-unused-return-value>` check.

  FIXME: add release notes.

- New :doc:`misrac2012-void-pointer-convert-figure
  <clang-tidy/checks/misrac2012-void-pointer-convert-figure>` check.

  FIXME: add release notes.

- New :doc:`performance-no-int-to-ptr
  <clang-tidy/checks/performance-no-int-to-ptr>` check.

  Diagnoses every integer to pointer cast.

- New :doc:`readability-Function-Parameters
  <clang-tidy/checks/readability-Function-Parameters>` check.

  FIXME: add release notes.

- New :doc:`readability-Invalid-Varibale-Name
  <clang-tidy/checks/readability-Invalid-Varibale-Name>` check.

  FIXME: add release notes.

- New :doc:`readability-Nested-Comment
  <clang-tidy/checks/readability-Nested-Comment>` check.

  FIXME: add release notes.

- New :doc:`readability-Variablename-Duplication
  <clang-tidy/checks/readability-Variablename-Duplication>` check.

  FIXME: add release notes.

- New :doc:`readability-function-cognitive-complexity
  <clang-tidy/checks/readability-function-cognitive-complexity>` check.

  Flags functions with Cognitive Complexity metric exceeding the configured limit.

Changes in existing checks
^^^^^^^^^^^^^^^^^^^^^^^^^^

- Improved :doc:`modernize-loop-convert
  <clang-tidy/checks/modernize-loop-convert>` check.

  Now able to transform iterator loops using ``rbegin`` and ``rend`` methods.

- Improved :doc:`readability-identifier-naming
  <clang-tidy/checks/readability-identifier-naming>` check.

  Added an option `GetConfigPerFile` to support including files which use
  different naming styles.

  Now renames overridden virtual methods if the method they override has a
  style violation.
  
  Added support for specifying the style of scoped ``enum`` constants. If 
  unspecified, will fall back to the style for regular ``enum`` constants.

  Added an option `IgnoredRegexp` per identifier type to suppress identifier
  naming checks for names matching a regular expression.

- Removed `google-runtime-references` check because the rule it checks does
  not exist in the Google Style Guide anymore.

- Improved :doc:`readability-redundant-string-init
  <clang-tidy/checks/readability-redundant-string-init>` check.

  Added `std::basic_string_view` to default list of ``string``-like types.

Improvements to include-fixer
-----------------------------

The improvements are...

Improvements to clang-include-fixer
-----------------------------------

The improvements are...

Improvements to modularize
--------------------------

The improvements are...

Improvements to pp-trace
------------------------

The improvements are...

Clang-tidy visual studio plugin
-------------------------------
 