.. title:: clang-tidy - Clang-Tidy Checks

Clang-Tidy Checks
=================

.. toctree::
   :glob:
   :hidden:

   *

.. csv-table::
   :header: "Name", "Offers fixes"

   `abseil-duration-addition <abseil-duration-addition.html>`_, "Yes"
   `abseil-duration-comparison <abseil-duration-comparison.html>`_, "Yes"
   `abseil-duration-conversion-cast <abseil-duration-conversion-cast.html>`_, "Yes"
   `abseil-duration-division <abseil-duration-division.html>`_, "Yes"
   `abseil-duration-factory-float <abseil-duration-factory-float.html>`_, "Yes"
   `abseil-duration-factory-scale <abseil-duration-factory-scale.html>`_, "Yes"
   `abseil-duration-subtraction <abseil-duration-subtraction.html>`_, "Yes"
   `abseil-duration-unnecessary-conversion <abseil-duration-unnecessary-conversion.html>`_, "Yes"
   `abseil-faster-strsplit-delimiter <abseil-faster-strsplit-delimiter.html>`_, "Yes"
   `abseil-no-internal-dependencies <abseil-no-internal-dependencies.html>`_,
   `abseil-no-namespace <abseil-no-namespace.html>`_,
   `abseil-redundant-strcat-calls <abseil-redundant-strcat-calls.html>`_, "Yes"
   `abseil-str-cat-append <abseil-str-cat-append.html>`_, "Yes"
   `abseil-string-find-startswith <abseil-string-find-startswith.html>`_, "Yes"
   `abseil-string-find-str-contains <abseil-string-find-str-contains.html>`_, "Yes"
   `abseil-time-comparison <abseil-time-comparison.html>`_, "Yes"
   `abseil-time-subtraction <abseil-time-subtraction.html>`_, "Yes"
   `abseil-upgrade-duration-conversions <abseil-upgrade-duration-conversions.html>`_, "Yes"
   `altera-kernel-name-restriction <altera-kernel-name-restriction.html>`_,
   `altera-single-work-item-barrier <altera-single-work-item-barrier.html>`_,
   `altera-struct-pack-align <altera-struct-pack-align.html>`_, "Yes"
   `android-cloexec-accept <android-cloexec-accept.html>`_, "Yes"
   `android-cloexec-accept4 <android-cloexec-accept4.html>`_,
   `android-cloexec-creat <android-cloexec-creat.html>`_, "Yes"
   `android-cloexec-dup <android-cloexec-dup.html>`_, "Yes"
   `android-cloexec-epoll-create <android-cloexec-epoll-create.html>`_,
   `android-cloexec-epoll-create1 <android-cloexec-epoll-create1.html>`_,
   `android-cloexec-fopen <android-cloexec-fopen.html>`_,
   `android-cloexec-inotify-init <android-cloexec-inotify-init.html>`_,
   `android-cloexec-inotify-init1 <android-cloexec-inotify-init1.html>`_,
   `android-cloexec-memfd-create <android-cloexec-memfd-create.html>`_,
   `android-cloexec-open <android-cloexec-open.html>`_,
   `android-cloexec-pipe <android-cloexec-pipe.html>`_, "Yes"
   `android-cloexec-pipe2 <android-cloexec-pipe2.html>`_,
   `android-cloexec-socket <android-cloexec-socket.html>`_,
   `android-comparison-in-temp-failure-retry <android-comparison-in-temp-failure-retry.html>`_,
   `boost-use-to-string <boost-use-to-string.html>`_, "Yes"
   `bugprone-argument-comment <bugprone-argument-comment.html>`_, "Yes"
   `bugprone-assert-side-effect <bugprone-assert-side-effect.html>`_,
   `bugprone-bad-signal-to-kill-thread <bugprone-bad-signal-to-kill-thread.html>`_,
   `bugprone-bool-pointer-implicit-conversion <bugprone-bool-pointer-implicit-conversion.html>`_, "Yes"
   `bugprone-branch-clone <bugprone-branch-clone.html>`_,
   `bugprone-copy-constructor-init <bugprone-copy-constructor-init.html>`_, "Yes"
   `bugprone-dangling-handle <bugprone-dangling-handle.html>`_,
   `bugprone-dynamic-static-initializers <bugprone-dynamic-static-initializers.html>`_,
   `bugprone-exception-escape <bugprone-exception-escape.html>`_,
   `bugprone-fold-init-type <bugprone-fold-init-type.html>`_,
   `bugprone-forward-declaration-namespace <bugprone-forward-declaration-namespace.html>`_,
   `bugprone-forwarding-reference-overload <bugprone-forwarding-reference-overload.html>`_,
   `bugprone-inaccurate-erase <bugprone-inaccurate-erase.html>`_, "Yes"
   `bugprone-incorrect-roundings <bugprone-incorrect-roundings.html>`_,
   `bugprone-infinite-loop <bugprone-infinite-loop.html>`_,
   `bugprone-integer-division <bugprone-integer-division.html>`_,
   `bugprone-interrupt-function-storage <bugprone-interrupt-function-storage.html>`_, "Yes"
   `bugprone-lambda-function-name <bugprone-lambda-function-name.html>`_,
   `bugprone-macro-parentheses <bugprone-macro-parentheses.html>`_, "Yes"
   `bugprone-macro-repeated-side-effects <bugprone-macro-repeated-side-effects.html>`_,
   `bugprone-misplaced-operator-in-strlen-in-alloc <bugprone-misplaced-operator-in-strlen-in-alloc.html>`_, "Yes"
   `bugprone-misplaced-pointer-arithmetic-in-alloc <bugprone-misplaced-pointer-arithmetic-in-alloc.html>`_, "Yes"
   `bugprone-misplaced-widening-cast <bugprone-misplaced-widening-cast.html>`_,
   `bugprone-move-forwarding-reference <bugprone-move-forwarding-reference.html>`_, "Yes"
   `bugprone-multiple-statement-macro <bugprone-multiple-statement-macro.html>`_,
   `bugprone-no-escape <bugprone-no-escape.html>`_,
   `bugprone-not-null-terminated-result <bugprone-not-null-terminated-result.html>`_, "Yes"
   `bugprone-parent-virtual-call <bugprone-parent-virtual-call.html>`_, "Yes"
   `bugprone-posix-return <bugprone-posix-return.html>`_, "Yes"
   `bugprone-redundant-branch-condition <bugprone-redundant-branch-condition.html>`_, "Yes"
   `bugprone-reserved-identifier <bugprone-reserved-identifier.html>`_, "Yes"
   `bugprone-signal-handler <bugprone-signal-handler.html>`_,
   `bugprone-signed-char-misuse <bugprone-signed-char-misuse.html>`_,
   `bugprone-sizeof-container <bugprone-sizeof-container.html>`_,
   `bugprone-sizeof-expression <bugprone-sizeof-expression.html>`_,
   `bugprone-spuriously-wake-up-functions <bugprone-spuriously-wake-up-functions.html>`_,
   `bugprone-string-constructor <bugprone-string-constructor.html>`_, "Yes"
   `bugprone-string-integer-assignment <bugprone-string-integer-assignment.html>`_, "Yes"
   `bugprone-string-literal-with-embedded-nul <bugprone-string-literal-with-embedded-nul.html>`_,
   `bugprone-suspicious-enum-usage <bugprone-suspicious-enum-usage.html>`_,
   `bugprone-suspicious-include <bugprone-suspicious-include.html>`_,
   `bugprone-suspicious-memset-usage <bugprone-suspicious-memset-usage.html>`_, "Yes"
   `bugprone-suspicious-missing-comma <bugprone-suspicious-missing-comma.html>`_,
   `bugprone-suspicious-semicolon <bugprone-suspicious-semicolon.html>`_, "Yes"
   `bugprone-suspicious-string-compare <bugprone-suspicious-string-compare.html>`_, "Yes"
   `bugprone-swapped-arguments <bugprone-swapped-arguments.html>`_, "Yes"
   `bugprone-terminating-continue <bugprone-terminating-continue.html>`_, "Yes"
   `bugprone-test_check <bugprone-test_check.html>`_,
   `bugprone-throw-keyword-missing <bugprone-throw-keyword-missing.html>`_,
   `bugprone-too-small-loop-variable <bugprone-too-small-loop-variable.html>`_,
   `bugprone-undefined-memory-manipulation <bugprone-undefined-memory-manipulation.html>`_,
   `bugprone-undelegated-constructor <bugprone-undelegated-constructor.html>`_,
   `bugprone-unhandled-self-assignment <bugprone-unhandled-self-assignment.html>`_,
   `bugprone-unused-raii <bugprone-unused-raii.html>`_, "Yes"
   `bugprone-unused-return-value <bugprone-unused-return-value.html>`_,
   `bugprone-use-after-move <bugprone-use-after-move.html>`_,
   `bugprone-virtual-near-miss <bugprone-virtual-near-miss.html>`_, "Yes"
   `bz12-AssignConsistency <bz12-AssignConsistency.html>`_,
   `bz12-abs-parameter-check <bz12-abs-parameter-check.html>`_,
   `bz12-abs-parameter <bz12-abs-parameter.html>`_,
   `bz12-basic-block-double-assign <bz12-basic-block-double-assign.html>`_,
   `bz12-basic-block-unused-or-repeatused <bz12-basic-block-unused-or-repeatused.html>`_,
   `bz12-block-consistency <bz12-block-consistency.html>`_,
   `bz12-branch-assignment-difference <bz12-branch-assignment-difference.html>`_,
   `bz12-branch-variable-statistics <bz12-branch-variable-statistics.html>`_,
   `bz12-conditional-statement-equality <bz12-conditional-statement-equality.html>`_, "Yes"
   `bz12-extern-defination-declare-type <bz12-extern-defination-declare-type.html>`_,
   `bz12-infinitel-loop <bz12-infinitel-loop.html>`_,
   `bz12-initial-value <bz12-initial-value.html>`_,
   `bz12-matrix-calc-parameters <bz12-matrix-calc-parameters.html>`_,
   `bz12-memset-multiple-rank-pointer <bz12-memset-multiple-rank-pointer.html>`_,
   `bz12-struct-default-alignment <bz12-struct-default-alignment.html>`_,
   `bz12-struct-defination-declare-consistence <bz12-struct-defination-declare-consistence.html>`_,
   `bz12-type-cast <bz12-type-cast.html>`_,
   `bz12-uninitialized-struct-element <bz12-uninitialized-struct-element.html>`_,
   `bz12-unused-struct-element <bz12-unused-struct-element.html>`_,
   `bz12-variable-name-analysis <bz12-variable-name-analysis.html>`_,
   `bz12-variable-name-in-if-analysis <bz12-variable-name-in-if-analysis.html>`_,
   `cert-dcl21-cpp <cert-dcl21-cpp.html>`_,
   `cert-dcl50-cpp <cert-dcl50-cpp.html>`_,
   `cert-dcl58-cpp <cert-dcl58-cpp.html>`_,
   `cert-env33-c <cert-env33-c.html>`_,
   `cert-err34-c <cert-err34-c.html>`_,
   `cert-err52-cpp <cert-err52-cpp.html>`_,
   `cert-err58-cpp <cert-err58-cpp.html>`_,
   `cert-err60-cpp <cert-err60-cpp.html>`_,
   `cert-flp30-c <cert-flp30-c.html>`_,
   `cert-mem57-cpp <cert-mem57-cpp.html>`_,
   `cert-msc50-cpp <cert-msc50-cpp.html>`_,
   `cert-msc51-cpp <cert-msc51-cpp.html>`_,
   `cert-oop57-cpp <cert-oop57-cpp.html>`_,
   `cert-oop58-cpp <cert-oop58-cpp.html>`_,
   `clang-analyzer-core.DynamicTypePropagation <clang-analyzer-core.DynamicTypePropagation.html>`_,
   `clang-analyzer-core.uninitialized.CapturedBlockVariable <clang-analyzer-core.uninitialized.CapturedBlockVariable.html>`_,
   `clang-analyzer-cplusplus.InnerPointer <clang-analyzer-cplusplus.InnerPointer.html>`_,
   `clang-analyzer-nullability.NullableReturnedFromNonnull <clang-analyzer-nullability.NullableReturnedFromNonnull.html>`_,
   `clang-analyzer-optin.osx.OSObjectCStyleCast <clang-analyzer-optin.osx.OSObjectCStyleCast.html>`_,
   `clang-analyzer-optin.performance.GCDAntipattern <clang-analyzer-optin.performance.GCDAntipattern.html>`_,
   `clang-analyzer-optin.performance.Padding <clang-analyzer-optin.performance.Padding.html>`_,
   `clang-analyzer-optin.portability.UnixAPI <clang-analyzer-optin.portability.UnixAPI.html>`_,
   `clang-analyzer-osx.MIG <clang-analyzer-osx.MIG.html>`_,
   `clang-analyzer-osx.NumberObjectConversion <clang-analyzer-osx.NumberObjectConversion.html>`_,
   `clang-analyzer-osx.OSObjectRetainCount <clang-analyzer-osx.OSObjectRetainCount.html>`_,
   `clang-analyzer-osx.ObjCProperty <clang-analyzer-osx.ObjCProperty.html>`_,
   `clang-analyzer-osx.cocoa.AutoreleaseWrite <clang-analyzer-osx.cocoa.AutoreleaseWrite.html>`_,
   `clang-analyzer-osx.cocoa.Loops <clang-analyzer-osx.cocoa.Loops.html>`_,
   `clang-analyzer-osx.cocoa.MissingSuperCall <clang-analyzer-osx.cocoa.MissingSuperCall.html>`_,
   `clang-analyzer-osx.cocoa.NonNilReturnValue <clang-analyzer-osx.cocoa.NonNilReturnValue.html>`_,
   `clang-analyzer-osx.cocoa.RunLoopAutoreleaseLeak <clang-analyzer-osx.cocoa.RunLoopAutoreleaseLeak.html>`_,
   `clang-analyzer-valist.CopyToSelf <clang-analyzer-valist.CopyToSelf.html>`_,
   `clang-analyzer-valist.Uninitialized <clang-analyzer-valist.Uninitialized.html>`_,
   `clang-analyzer-valist.Unterminated <clang-analyzer-valist.Unterminated.html>`_,
   `concurrency-mt-unsafe <concurrency-mt-unsafe.html>`_,
   `counter-function-info <counter-function-info.html>`_,
   `cppcoreguidelines-avoid-goto <cppcoreguidelines-avoid-goto.html>`_,
   `cppcoreguidelines-avoid-non-const-global-variables <cppcoreguidelines-avoid-non-const-global-variables.html>`_,
   `cppcoreguidelines-init-variables <cppcoreguidelines-init-variables.html>`_, "Yes"
   `cppcoreguidelines-interfaces-global-init <cppcoreguidelines-interfaces-global-init.html>`_,
   `cppcoreguidelines-macro-usage <cppcoreguidelines-macro-usage.html>`_,
   `cppcoreguidelines-narrowing-conversions <cppcoreguidelines-narrowing-conversions.html>`_,
   `cppcoreguidelines-no-malloc <cppcoreguidelines-no-malloc.html>`_,
   `cppcoreguidelines-owning-memory <cppcoreguidelines-owning-memory.html>`_,
   `cppcoreguidelines-prefer-member-initializer <cppcoreguidelines-prefer-member-initializer.html>`_, "Yes"
   `cppcoreguidelines-pro-bounds-array-to-pointer-decay <cppcoreguidelines-pro-bounds-array-to-pointer-decay.html>`_,
   `cppcoreguidelines-pro-bounds-constant-array-index <cppcoreguidelines-pro-bounds-constant-array-index.html>`_, "Yes"
   `cppcoreguidelines-pro-bounds-pointer-arithmetic <cppcoreguidelines-pro-bounds-pointer-arithmetic.html>`_,
   `cppcoreguidelines-pro-type-const-cast <cppcoreguidelines-pro-type-const-cast.html>`_,
   `cppcoreguidelines-pro-type-cstyle-cast <cppcoreguidelines-pro-type-cstyle-cast.html>`_, "Yes"
   `cppcoreguidelines-pro-type-member-init <cppcoreguidelines-pro-type-member-init.html>`_, "Yes"
   `cppcoreguidelines-pro-type-reinterpret-cast <cppcoreguidelines-pro-type-reinterpret-cast.html>`_,
   `cppcoreguidelines-pro-type-static-cast-downcast <cppcoreguidelines-pro-type-static-cast-downcast.html>`_, "Yes"
   `cppcoreguidelines-pro-type-union-access <cppcoreguidelines-pro-type-union-access.html>`_,
   `cppcoreguidelines-pro-type-vararg <cppcoreguidelines-pro-type-vararg.html>`_,
   `cppcoreguidelines-slicing <cppcoreguidelines-slicing.html>`_,
   `cppcoreguidelines-special-member-functions <cppcoreguidelines-special-member-functions.html>`_,
   `custom-GlobalUninitial <custom-GlobalUninitial.html>`_,
   `darwin-avoid-spinlock <darwin-avoid-spinlock.html>`_,
   `darwin-dispatch-once-nonstatic <darwin-dispatch-once-nonstatic.html>`_, "Yes"
   `fuchsia-default-arguments-calls <fuchsia-default-arguments-calls.html>`_,
   `fuchsia-default-arguments-declarations <fuchsia-default-arguments-declarations.html>`_, "Yes"
   `fuchsia-multiple-inheritance <fuchsia-multiple-inheritance.html>`_,
   `fuchsia-overloaded-operator <fuchsia-overloaded-operator.html>`_,
   `fuchsia-statically-constructed-objects <fuchsia-statically-constructed-objects.html>`_,
   `fuchsia-trailing-return <fuchsia-trailing-return.html>`_,
   `fuchsia-virtual-inheritance <fuchsia-virtual-inheritance.html>`_,
   `gb5369-test_5369 <gb5369-test_5369.html>`_,
   `gb8114-BanUnary <gb8114-BanUnary.html>`_,
   `gb8114-Baninclude <gb8114-Baninclude.html>`_,
   `gb8114-Integerforloop <gb8114-Integerforloop.html>`_,
   `gb8114-Localdeclareloop <gb8114-Localdeclareloop.html>`_,
   `gb8114-Macrodefinition <gb8114-Macrodefinition.html>`_,
   `gb8114-SwitchTest <gb8114-SwitchTest.html>`_,
   `gjb5369-ArraySubscriptPP <gjb5369-ArraySubscriptPP.html>`_,
   `gjb5369-Arrayinit <gjb5369-Arrayinit.html>`_,
   `gjb5369-BanExeCodeBeforeInclude <gjb5369-BanExeCodeBeforeInclude.html>`_,
   `gjb5369-ConditionalBodyBrace <gjb5369-ConditionalBodyBrace.html>`_,
   `gjb5369-EnumInit <gjb5369-EnumInit.html>`_,
   `gjb5369-Function-Parameters <gjb5369-Function-Parameters.html>`_,
   `gjb5369-FunctionVoid <gjb5369-FunctionVoid.html>`_,
   `gjb5369-GotoRelated <gjb5369-GotoRelated.html>`_,
   `gjb5369-HeaderUseDirectPath <gjb5369-HeaderUseDirectPath.html>`_,
   `gjb5369-Invalid-Varibale-Name <gjb5369-Invalid-Varibale-Name.html>`_,
   `gjb5369-Loop-Control <gjb5369-Loop-Control.html>`_,
   `gjb5369-MacroRelated <gjb5369-MacroRelated.html>`_,
   `gjb5369-NameForbiddenReuse <gjb5369-NameForbiddenReuse.html>`_,
   `gjb5369-Nested-Comment <gjb5369-Nested-Comment.html>`_,
   `gjb5369-PreprocessIfEndif <gjb5369-PreprocessIfEndif.html>`_,
   `gjb5369-StructDeclIncomplete <gjb5369-StructDeclIncomplete.html>`_,
   `gjb5369-StructHaveEmptySpace <gjb5369-StructHaveEmptySpace.html>`_,
   `gjb5369-StructInit <gjb5369-StructInit.html>`_,
   `gjb5369-TypedefRedefine <gjb5369-TypedefRedefine.html>`_,
   `gjb5369-Variablename-Duplication <gjb5369-Variablename-Duplication.html>`_,
   `gjb8114-anti-null-switch <gjb8114-anti-null-switch.html>`_,
   `gjb8114-array-init <gjb8114-array-init.html>`_,
   `gjb8114-assign-in-sizeof <gjb8114-assign-in-sizeof.html>`_,
   `gjb8114-ban-bit-logical <gjb8114-ban-bit-logical.html>`_,
   `gjb8114-bit-define <gjb8114-bit-define.html>`_,
   `gjb8114-bool-switch <gjb8114-bool-switch.html>`_,
   `gjb8114-call-void-function <gjb8114-call-void-function.html>`_,
   `gjb8114-case-in-different-layer <gjb8114-case-in-different-layer.html>`_,
   `gjb8114-char-symbol <gjb8114-char-symbol.html>`_,
   `gjb8114-code-before-include <gjb8114-code-before-include.html>`_,
   `gjb8114-condition-with-brace <gjb8114-condition-with-brace.html>`_,
   `gjb8114-conditional-expression-without-assignment-statement <gjb8114-conditional-expression-without-assignment-statement.html>`_,
   `gjb8114-conditional-goto <gjb8114-conditional-goto.html>`_,
   `gjb8114-constant-logical <gjb8114-constant-logical.html>`_,
   `gjb8114-cxxrecord-definition-identifier <gjb8114-cxxrecord-definition-identifier.html>`_,
   `gjb8114-default-in-switch <gjb8114-default-in-switch.html>`_,
   `gjb8114-define-match <gjb8114-define-match.html>`_,
   `gjb8114-end-with-break <gjb8114-end-with-break.html>`_,
   `gjb8114-enum-error-initial <gjb8114-enum-error-initial.html>`_,
   `gjb8114-enum-variable-same-with-other-variable <gjb8114-enum-variable-same-with-other-variable.html>`_,
   `gjb8114-error-declare-of-struct <gjb8114-error-declare-of-struct.html>`_,
   `gjb8114-error-use-of-loop-variable <gjb8114-error-use-of-loop-variable.html>`_,
   `gjb8114-extern-decl-in-function-body <gjb8114-extern-decl-in-function-body.html>`_,
   `gjb8114-extern-variable-init <gjb8114-extern-variable-init.html>`_,
   `gjb8114-float-equal <gjb8114-float-equal.html>`_,
   `gjb8114-floatliteral-to-int <gjb8114-floatliteral-to-int.html>`_,
   `gjb8114-function-call-itself <gjb8114-function-call-itself.html>`_,
   `gjb8114-function-declare-different-with-definition <gjb8114-function-declare-different-with-definition.html>`_,
   `gjb8114-function-declare-different-with-prototype <gjb8114-function-declare-different-with-prototype.html>`_,
   `gjb8114-function-declare-same <gjb8114-function-declare-same.html>`_,
   `gjb8114-function-parameter-explicit-void <gjb8114-function-parameter-explicit-void.html>`_,
   `gjb8114-function-parameter-same-with-global-variable <gjb8114-function-parameter-same-with-global-variable.html>`_,
   `gjb8114-global-or-static-variable-depend-on-system <gjb8114-global-or-static-variable-depend-on-system.html>`_,
   `gjb8114-header-prohibited-from-include-repeatedly <gjb8114-header-prohibited-from-include-repeatedly.html>`_,
   `gjb8114-if-elseif-with-else <gjb8114-if-elseif-with-else.html>`_,
   `gjb8114-if-or-else-null <gjb8114-if-or-else-null.html>`_,
   `gjb8114-include-header-use-direct-path <gjb8114-include-header-use-direct-path.html>`_,
   `gjb8114-infinite-loop-statement-without-null-for-condition <gjb8114-infinite-loop-statement-without-null-for-condition.html>`_,
   `gjb8114-invalid-variable-name <gjb8114-invalid-variable-name.html>`_,
   `gjb8114-keyword-related <gjb8114-keyword-related.html>`_,
   `gjb8114-label-reuse-prohibit <gjb8114-label-reuse-prohibit.html>`_,
   `gjb8114-local-variable-name-same-as-global <gjb8114-local-variable-name-same-as-global.html>`_,
   `gjb8114-logical-expr-explicit-parenthses <gjb8114-logical-expr-explicit-parenthses.html>`_,
   `gjb8114-logical-expr-operation-parenthese <gjb8114-logical-expr-operation-parenthese.html>`_,
   `gjb8114-logical-variable-compare <gjb8114-logical-variable-compare.html>`_,
   `gjb8114-macro-define-repeated-prohibit <gjb8114-macro-define-repeated-prohibit.html>`_,
   `gjb8114-macro-related <gjb8114-macro-related.html>`_,
   `gjb8114-nested-annotation <gjb8114-nested-annotation.html>`_,
   `gjb8114-no-use-return-value-without-void <gjb8114-no-use-return-value-without-void.html>`_,
   `gjb8114-no-use-transform <gjb8114-no-use-transform.html>`_,
   `gjb8114-number-suffix-use <gjb8114-number-suffix-use.html>`_,
   `gjb8114-octonary-number-use <gjb8114-octonary-number-use.html>`_,
   `gjb8114-only-default-in-switch <gjb8114-only-default-in-switch.html>`_,
   `gjb8114-pointer-assign-null-after-free <gjb8114-pointer-assign-null-after-free.html>`_,
   `gjb8114-pointer-beyond-two-levels <gjb8114-pointer-beyond-two-levels.html>`_,
   `gjb8114-pointer-initial <gjb8114-pointer-initial.html>`_,
   `gjb8114-pointer-logical-compare <gjb8114-pointer-logical-compare.html>`_,
   `gjb8114-pointer-without-check-null <gjb8114-pointer-without-check-null.html>`_,
   `gjb8114-real-parameter-same-with-formal-parameter <gjb8114-real-parameter-same-with-formal-parameter.html>`_,
   `gjb8114-redefine-typedef <gjb8114-redefine-typedef.html>`_,
   `gjb8114-return-local-variable-address <gjb8114-return-local-variable-address.html>`_,
   `gjb8114-return-type-same-with-define <gjb8114-return-type-same-with-define.html>`_,
   `gjb8114-sequential-call-same-variable <gjb8114-sequential-call-same-variable.html>`_,
   `gjb8114-shift-signed-number <gjb8114-shift-signed-number.html>`_,
   `gjb8114-standard-library-function-rename-prohibit <gjb8114-standard-library-function-rename-prohibit.html>`_,
   `gjb8114-string-without-end-symbol <gjb8114-string-without-end-symbol.html>`_,
   `gjb8114-struct-anonymous-field <gjb8114-struct-anonymous-field.html>`_,
   `gjb8114-type-cast <gjb8114-type-cast.html>`_,
   `gjb8114-unreachable-code <gjb8114-unreachable-code.html>`_,
   `gjb8114-unsigned-compare-with-signed <gjb8114-unsigned-compare-with-signed.html>`_,
   `gjb8114-unsigned-compare-with-zero <gjb8114-unsigned-compare-with-zero.html>`_,
   `gjb8114-unuse-of-static-function <gjb8114-unuse-of-static-function.html>`_,
   `gjb8114-usage-of-pointer <gjb8114-usage-of-pointer.html>`_,
   `gjb8114-use-enum-out-of-bound <gjb8114-use-enum-out-of-bound.html>`_,
   `gjb8114-use-function-as-pointer <gjb8114-use-function-as-pointer.html>`_,
   `gjb8114-use-of-gets <gjb8114-use-of-gets.html>`_,
   `gjb8114-use-of-setjmp-or-longjmp <gjb8114-use-of-setjmp-or-longjmp.html>`_,
   `gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator <gjb8114-use-unaryoperator-in-callexpr-or-binaryoperator.html>`_,
   `gjb8114-variable-declare-same-name-with-function <gjb8114-variable-declare-same-name-with-function.html>`_,
   `gjb8114-variable-name-same-with-typedef <gjb8114-variable-name-same-with-typedef.html>`_,
   `gjb8114-variable-use <gjb8114-variable-use.html>`_,
   `gjb8114-variadic-function-ellipsis <gjb8114-variadic-function-ellipsis.html>`_,
   `gjb8114-volatile-variable-repeat-in-binaryoperator <gjb8114-volatile-variable-repeat-in-binaryoperator.html>`_,
   `gjb8114cpp-base-constructor-must-exist-in-initializer <gjb8114cpp-base-constructor-must-exist-in-initializer.html>`_,
   `gjb8114cpp-catch-exception-throw-in-destructor <gjb8114cpp-catch-exception-throw-in-destructor.html>`_,
   `gjb8114cpp-const-cast-pointer-reference <gjb8114cpp-const-cast-pointer-reference.html>`_,
   `gjb8114cpp-const-member-function-return <gjb8114cpp-const-member-function-return.html>`_,
   `gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private <gjb8114cpp-copy-operator-overloads-in-abstract-classes-must-be-protected-or-private.html>`_,
   `gjb8114cpp-default-constructor-must-exist <gjb8114cpp-default-constructor-must-exist.html>`_,
   `gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class <gjb8114cpp-default-paramter-value-in-virtual-func-prohibit-change-in-derived-class.html>`_,
   `gjb8114cpp-diamond-virtual-inherit <gjb8114cpp-diamond-virtual-inherit.html>`_,
   `gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer <gjb8114cpp-dynamic-cast-from-virtual-base-pointer-to-derived-pointer.html>`_,
   `gjb8114cpp-each-specific-throw-has-catch <gjb8114cpp-each-specific-throw-has-catch.html>`_,
   `gjb8114cpp-explicitly-throw-null <gjb8114cpp-explicitly-throw-null.html>`_,
   `gjb8114cpp-explicitly_throw_null <gjb8114cpp-explicitly_throw_null.html>`_,
   `gjb8114cpp-fixed-array-param-passing <gjb8114cpp-fixed-array-param-passing.html>`_,
   `gjb8114cpp-global-variables-prohibited-in-constructors <gjb8114cpp-global-variables-prohibited-in-constructors.html>`_,
   `gjb8114cpp-member-function-mutable-return <gjb8114cpp-member-function-mutable-return.html>`_,
   `gjb8114cpp-member-variables-initialized-in-the-constructor <gjb8114cpp-member-variables-initialized-in-the-constructor.html>`_,
   `gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference <gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference.html>`_,
   `gjb8114cpp-reinterpret-cast-class-pointer <gjb8114cpp-reinterpret-cast-class-pointer.html>`_,
   `gjb8114cpp-single-parameter-constructor-set-explicit <gjb8114cpp-single-parameter-constructor-set-explicit.html>`_,
   `gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class <gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class.html>`_,
   `gjb8114cpp-virtual-destructors-in-class-with-virtual-functions <gjb8114cpp-virtual-destructors-in-class-with-virtual-functions.html>`_,
   `google-build-explicit-make-pair <google-build-explicit-make-pair.html>`_,
   `google-build-namespaces <google-build-namespaces.html>`_,
   `google-build-using-namespace <google-build-using-namespace.html>`_,
   `google-default-arguments <google-default-arguments.html>`_,
   `google-explicit-constructor <google-explicit-constructor.html>`_, "Yes"
   `google-global-names-in-headers <google-global-names-in-headers.html>`_,
   `google-objc-avoid-nsobject-new <google-objc-avoid-nsobject-new.html>`_,
   `google-objc-avoid-throwing-exception <google-objc-avoid-throwing-exception.html>`_,
   `google-objc-function-naming <google-objc-function-naming.html>`_,
   `google-objc-global-variable-declaration <google-objc-global-variable-declaration.html>`_,
   `google-readability-avoid-underscore-in-googletest-name <google-readability-avoid-underscore-in-googletest-name.html>`_,
   `google-readability-casting <google-readability-casting.html>`_,
   `google-readability-todo <google-readability-todo.html>`_,
   `google-runtime-int <google-runtime-int.html>`_,
   `google-runtime-operator <google-runtime-operator.html>`_,
   `google-upgrade-googletest-case <google-upgrade-googletest-case.html>`_, "Yes"
   `hicpp-avoid-goto <hicpp-avoid-goto.html>`_,
   `hicpp-exception-baseclass <hicpp-exception-baseclass.html>`_,
   `hicpp-multiway-paths-covered <hicpp-multiway-paths-covered.html>`_,
   `hicpp-no-assembler <hicpp-no-assembler.html>`_,
   `hicpp-signed-bitwise <hicpp-signed-bitwise.html>`_,
   `linuxkernel-must-use-errs <linuxkernel-must-use-errs.html>`_,
   `llvm-header-guard <llvm-header-guard.html>`_,
   `llvm-include-order <llvm-include-order.html>`_, "Yes"
   `llvm-namespace-comment <llvm-namespace-comment.html>`_,
   `llvm-prefer-isa-or-dyn-cast-in-conditionals <llvm-prefer-isa-or-dyn-cast-in-conditionals.html>`_, "Yes"
   `llvm-prefer-register-over-unsigned <llvm-prefer-register-over-unsigned.html>`_, "Yes"
   `llvm-twine-local <llvm-twine-local.html>`_, "Yes"
   `llvmlibc-callee-namespace <llvmlibc-callee-namespace.html>`_,
   `llvmlibc-implementation-in-namespace <llvmlibc-implementation-in-namespace.html>`_,
   `llvmlibc-restrict-system-libc-headers <llvmlibc-restrict-system-libc-headers.html>`_, "Yes"
   `misc-definitions-in-headers <misc-definitions-in-headers.html>`_, "Yes"
   `misc-misplaced-const <misc-misplaced-const.html>`_,
   `misc-new-delete-overloads <misc-new-delete-overloads.html>`_,
   `misc-no-recursion <misc-no-recursion.html>`_,
   `misc-non-copyable-objects <misc-non-copyable-objects.html>`_,
   `misc-non-private-member-variables-in-classes <misc-non-private-member-variables-in-classes.html>`_,
   `misc-redundant-expression <misc-redundant-expression.html>`_, "Yes"
   `misc-static-assert <misc-static-assert.html>`_, "Yes"
   `misc-throw-by-value-catch-by-reference <misc-throw-by-value-catch-by-reference.html>`_,
   `misc-unconventional-assign-operator <misc-unconventional-assign-operator.html>`_,
   `misc-uniqueptr-reset-release <misc-uniqueptr-reset-release.html>`_, "Yes"
   `misc-unused-alias-decls <misc-unused-alias-decls.html>`_, "Yes"
   `misc-unused-parameters <misc-unused-parameters.html>`_, "Yes"
   `misc-unused-using-decls <misc-unused-using-decls.html>`_, "Yes"
   `misrac2012-ArrayStatic <misrac2012-ArrayStatic.html>`_,
   `misrac2012-BanDefineInReservedMacro <misrac2012-BanDefineInReservedMacro.html>`_,
   `misrac2012-BanatoinStdlib <misrac2012-BanatoinStdlib.html>`_,
   `misrac2012-Bangoto <misrac2012-Bangoto.html>`_,
   `misrac2012-Banselfcallfunc <misrac2012-Banselfcallfunc.html>`_,
   `misrac2012-Bansomeheadfile <misrac2012-Bansomeheadfile.html>`_,
   `misrac2012-BitField <misrac2012-BitField.html>`_,
   `misrac2012-BoolSwitch <misrac2012-BoolSwitch.html>`_,
   `misrac2012-CaseBreak <misrac2012-CaseBreak.html>`_,
   `misrac2012-CastConst <misrac2012-CastConst.html>`_,
   `misrac2012-CondBodyBrace <misrac2012-CondBodyBrace.html>`_,
   `misrac2012-CondExpShouldBeChange <misrac2012-CondExpShouldBeChange.html>`_,
   `misrac2012-DeclImplic <misrac2012-DeclImplic.html>`_,
   `misrac2012-DefaultLabel <misrac2012-DefaultLabel.html>`_,
   `misrac2012-DefaultPosition <misrac2012-DefaultPosition.html>`_,
   `misrac2012-DifferCast <misrac2012-DifferCast.html>`_,
   `misrac2012-ExplicitReturn <misrac2012-ExplicitReturn.html>`_,
   `misrac2012-FloatLoopCounter <misrac2012-FloatLoopCounter.html>`_,
   `misrac2012-FunctionPtr <misrac2012-FunctionPtr.html>`_,
   `misrac2012-GlobalDiffLocal <misrac2012-GlobalDiffLocal.html>`_,
   `misrac2012-IfCondShouldBeBool <misrac2012-IfCondShouldBeBool.html>`_,
   `misrac2012-Ifelseif <misrac2012-Ifelseif.html>`_,
   `misrac2012-IncompleteType <misrac2012-IncompleteType.html>`_,
   `misrac2012-InterNotOverrideExter <misrac2012-InterNotOverrideExter.html>`_,
   `misrac2012-NonvoidFunc <misrac2012-NonvoidFunc.html>`_,
   `misrac2012-NullPtr <misrac2012-NullPtr.html>`_,
   `misrac2012-OctalNum <misrac2012-OctalNum.html>`_,
   `misrac2012-OctalTerm <misrac2012-OctalTerm.html>`_,
   `misrac2012-SingleByteField <misrac2012-SingleByteField.html>`_,
   `misrac2012-StdargFeature <misrac2012-StdargFeature.html>`_,
   `misrac2012-StringAssign <misrac2012-StringAssign.html>`_,
   `misrac2012-SwitchBranch <misrac2012-SwitchBranch.html>`_,
   `misrac2012-SwitchLabel <misrac2012-SwitchLabel.html>`_,
   `misrac2012-UniqueExternal <misrac2012-UniqueExternal.html>`_,
   `misrac2012-UniqueMacro <misrac2012-UniqueMacro.html>`_,
   `misrac2012-UniqueTag <misrac2012-UniqueTag.html>`_,
   `misrac2012-UniqueTypedef <misrac2012-UniqueTypedef.html>`_,
   `misrac2012-UnsignedTypeSuffix <misrac2012-UnsignedTypeSuffix.html>`_,
   `misrac2012-VarName <misrac2012-VarName.html>`_,
   `misrac2012-assigned-inappropriate-type <misrac2012-assigned-inappropriate-type.html>`_,
   `misrac2012-compatible-declarations <misrac2012-compatible-declarations.html>`_,
   `misrac2012-convert-between-pointer-and-arithmetic <misrac2012-convert-between-pointer-and-arithmetic.html>`_,
   `misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword <misrac2012-declaration-of-array-parameter-shall-not-contain-static-keyword.html>`_,
   `misrac2012-enum-implicitly-constant <misrac2012-enum-implicitly-constant.html>`_,
   `misrac2012-escape-sequence-termination <misrac2012-escape-sequence-termination.html>`_,
   `misrac2012-external-identifiers-shall-be-distinct <misrac2012-external-identifiers-shall-be-distinct.html>`_,
   `misrac2012-file-stream-pointer-dereference <misrac2012-file-stream-pointer-dereference.html>`_,
   `misrac2012-function-parameters-name <misrac2012-function-parameters-name.html>`_,
   `misrac2012-goto-label-different-block <misrac2012-goto-label-different-block.html>`_,
   `misrac2012-header-use-direct-path <misrac2012-header-use-direct-path.html>`_,
   `misrac2012-include-ban-header <misrac2012-include-ban-header.html>`_,
   `misrac2012-initializer-lists-shall-not-contain-persistent-side-effects <misrac2012-initializer-lists-shall-not-contain-persistent-side-effects.html>`_,
   `misrac2012-inline-function-static-storage <misrac2012-inline-function-static-storage.html>`_,
   `misrac2012-line-comments-used <misrac2012-line-comments-used.html>`_,
   `misrac2012-list-designated-initialized <misrac2012-list-designated-initialized.html>`_,
   `misrac2012-macro-identifiers-shall-be-distinct <misrac2012-macro-identifiers-shall-be-distinct.html>`_,
   `misrac2012-macro-if-elif-controlling-expression-shall-be-defined <misrac2012-macro-if-elif-controlling-expression-shall-be-defined.html>`_,
   `misrac2012-macro-if-elif-controlling-expression-shall-evaluate <misrac2012-macro-if-elif-controlling-expression-shall-evaluate.html>`_,
   `misrac2012-operator-inappropriate-type <misrac2012-operator-inappropriate-type.html>`_,
   `misrac2012-persistent-side-effects-not-same <misrac2012-persistent-side-effects-not-same.html>`_,
   `misrac2012-restrict-qualifier-pointer <misrac2012-restrict-qualifier-pointer.html>`_,
   `misrac2012-testmisra <misrac2012-testmisra.html>`_,
   `misrac2012-unused-return-value <misrac2012-unused-return-value.html>`_,
   `misrac2012-void-pointer-convert-figure <misrac2012-void-pointer-convert-figure.html>`_,
   `modernize-avoid-bind <modernize-avoid-bind.html>`_, "Yes"
   `modernize-avoid-c-arrays <modernize-avoid-c-arrays.html>`_,
   `modernize-concat-nested-namespaces <modernize-concat-nested-namespaces.html>`_, "Yes"
   `modernize-deprecated-headers <modernize-deprecated-headers.html>`_, "Yes"
   `modernize-deprecated-ios-base-aliases <modernize-deprecated-ios-base-aliases.html>`_, "Yes"
   `modernize-loop-convert <modernize-loop-convert.html>`_, "Yes"
   `modernize-make-shared <modernize-make-shared.html>`_, "Yes"
   `modernize-make-unique <modernize-make-unique.html>`_, "Yes"
   `modernize-pass-by-value <modernize-pass-by-value.html>`_, "Yes"
   `modernize-raw-string-literal <modernize-raw-string-literal.html>`_, "Yes"
   `modernize-redundant-void-arg <modernize-redundant-void-arg.html>`_, "Yes"
   `modernize-replace-auto-ptr <modernize-replace-auto-ptr.html>`_, "Yes"
   `modernize-replace-disallow-copy-and-assign-macro <modernize-replace-disallow-copy-and-assign-macro.html>`_, "Yes"
   `modernize-replace-random-shuffle <modernize-replace-random-shuffle.html>`_, "Yes"
   `modernize-return-braced-init-list <modernize-return-braced-init-list.html>`_, "Yes"
   `modernize-shrink-to-fit <modernize-shrink-to-fit.html>`_, "Yes"
   `modernize-unary-static-assert <modernize-unary-static-assert.html>`_, "Yes"
   `modernize-use-auto <modernize-use-auto.html>`_, "Yes"
   `modernize-use-bool-literals <modernize-use-bool-literals.html>`_, "Yes"
   `modernize-use-default-member-init <modernize-use-default-member-init.html>`_, "Yes"
   `modernize-use-emplace <modernize-use-emplace.html>`_, "Yes"
   `modernize-use-equals-default <modernize-use-equals-default.html>`_, "Yes"
   `modernize-use-equals-delete <modernize-use-equals-delete.html>`_, "Yes"
   `modernize-use-nodiscard <modernize-use-nodiscard.html>`_, "Yes"
   `modernize-use-noexcept <modernize-use-noexcept.html>`_, "Yes"
   `modernize-use-nullptr <modernize-use-nullptr.html>`_, "Yes"
   `modernize-use-override <modernize-use-override.html>`_, "Yes"
   `modernize-use-trailing-return-type <modernize-use-trailing-return-type.html>`_, "Yes"
   `modernize-use-transparent-functors <modernize-use-transparent-functors.html>`_, "Yes"
   `modernize-use-uncaught-exceptions <modernize-use-uncaught-exceptions.html>`_, "Yes"
   `modernize-use-using <modernize-use-using.html>`_, "Yes"
   `mpi-buffer-deref <mpi-buffer-deref.html>`_, "Yes"
   `mpi-type-mismatch <mpi-type-mismatch.html>`_, "Yes"
   `objc-avoid-nserror-init <objc-avoid-nserror-init.html>`_,
   `objc-dealloc-in-category <objc-dealloc-in-category.html>`_,
   `objc-forbidden-subclassing <objc-forbidden-subclassing.html>`_,
   `objc-missing-hash <objc-missing-hash.html>`_,
   `objc-nsinvocation-argument-lifetime <objc-nsinvocation-argument-lifetime.html>`_, "Yes"
   `objc-property-declaration <objc-property-declaration.html>`_, "Yes"
   `objc-super-self <objc-super-self.html>`_, "Yes"
   `openmp-exception-escape <openmp-exception-escape.html>`_,
   `openmp-use-default-none <openmp-use-default-none.html>`_,
   `performance-faster-string-find <performance-faster-string-find.html>`_, "Yes"
   `performance-for-range-copy <performance-for-range-copy.html>`_, "Yes"
   `performance-implicit-conversion-in-loop <performance-implicit-conversion-in-loop.html>`_,
   `performance-inefficient-algorithm <performance-inefficient-algorithm.html>`_, "Yes"
   `performance-inefficient-string-concatenation <performance-inefficient-string-concatenation.html>`_,
   `performance-inefficient-vector-operation <performance-inefficient-vector-operation.html>`_, "Yes"
   `performance-move-const-arg <performance-move-const-arg.html>`_, "Yes"
   `performance-move-constructor-init <performance-move-constructor-init.html>`_, "Yes"
   `performance-no-automatic-move <performance-no-automatic-move.html>`_,
   `performance-no-int-to-ptr <performance-no-int-to-ptr.html>`_,
   `performance-noexcept-move-constructor <performance-noexcept-move-constructor.html>`_, "Yes"
   `performance-trivially-destructible <performance-trivially-destructible.html>`_, "Yes"
   `performance-type-promotion-in-math-fn <performance-type-promotion-in-math-fn.html>`_, "Yes"
   `performance-unnecessary-copy-initialization <performance-unnecessary-copy-initialization.html>`_,
   `performance-unnecessary-value-param <performance-unnecessary-value-param.html>`_, "Yes"
   `portability-restrict-system-includes <portability-restrict-system-includes.html>`_, "Yes"
   `portability-simd-intrinsics <portability-simd-intrinsics.html>`_,
   `readability-Function-Parameters <readability-Function-Parameters.html>`_,
   `readability-Invalid-Varibale-Name <readability-Invalid-Varibale-Name.html>`_,
   `readability-Loop-Control <readability-Loop-Control.html>`_,
   `readability-Nested-Comment <readability-Nested-Comment.html>`_,
   `readability-Variablename-Duplication <readability-Variablename-Duplication.html>`_,
   `readability-avoid-const-params-in-decls <readability-avoid-const-params-in-decls.html>`_,
   `readability-braces-around-statements <readability-braces-around-statements.html>`_, "Yes"
   `readability-const-return-type <readability-const-return-type.html>`_, "Yes"
   `readability-container-size-empty <readability-container-size-empty.html>`_, "Yes"
   `readability-convert-member-functions-to-static <readability-convert-member-functions-to-static.html>`_,
   `readability-delete-null-pointer <readability-delete-null-pointer.html>`_, "Yes"
   `readability-deleted-default <readability-deleted-default.html>`_,
   `readability-else-after-return <readability-else-after-return.html>`_, "Yes"
   `readability-function-cognitive-complexity <readability-function-cognitive-complexity.html>`_,
   `readability-function-size <readability-function-size.html>`_,
   `readability-identifier-naming <readability-identifier-naming.html>`_, "Yes"
   `readability-implicit-bool-conversion <readability-implicit-bool-conversion.html>`_, "Yes"
   `readability-inconsistent-declaration-parameter-name <readability-inconsistent-declaration-parameter-name.html>`_, "Yes"
   `readability-isolate-declaration <readability-isolate-declaration.html>`_, "Yes"
   `readability-magic-numbers <readability-magic-numbers.html>`_,
   `readability-make-member-function-const <readability-make-member-function-const.html>`_, "Yes"
   `readability-misleading-indentation <readability-misleading-indentation.html>`_,
   `readability-misplaced-array-index <readability-misplaced-array-index.html>`_, "Yes"
   `readability-named-parameter <readability-named-parameter.html>`_, "Yes"
   `readability-non-const-parameter <readability-non-const-parameter.html>`_, "Yes"
   `readability-qualified-auto <readability-qualified-auto.html>`_, "Yes"
   `readability-redundant-access-specifiers <readability-redundant-access-specifiers.html>`_, "Yes"
   `readability-redundant-control-flow <readability-redundant-control-flow.html>`_, "Yes"
   `readability-redundant-declaration <readability-redundant-declaration.html>`_, "Yes"
   `readability-redundant-function-ptr-dereference <readability-redundant-function-ptr-dereference.html>`_, "Yes"
   `readability-redundant-member-init <readability-redundant-member-init.html>`_, "Yes"
   `readability-redundant-preprocessor <readability-redundant-preprocessor.html>`_,
   `readability-redundant-smartptr-get <readability-redundant-smartptr-get.html>`_, "Yes"
   `readability-redundant-string-cstr <readability-redundant-string-cstr.html>`_, "Yes"
   `readability-redundant-string-init <readability-redundant-string-init.html>`_, "Yes"
   `readability-simplify-boolean-expr <readability-simplify-boolean-expr.html>`_, "Yes"
   `readability-simplify-subscript-expr <readability-simplify-subscript-expr.html>`_, "Yes"
   `readability-static-accessed-through-instance <readability-static-accessed-through-instance.html>`_, "Yes"
   `readability-static-definition-in-anonymous-namespace <readability-static-definition-in-anonymous-namespace.html>`_, "Yes"
   `readability-string-compare <readability-string-compare.html>`_, "Yes"
   `readability-uniqueptr-delete-release <readability-uniqueptr-delete-release.html>`_, "Yes"
   `readability-uppercase-literal-suffix <readability-uppercase-literal-suffix.html>`_, "Yes"
   `readability-use-anyofallof <readability-use-anyofallof.html>`_,
   `scope-EmptyCondition <scope-EmptyCondition.html>`_,
   `scope-IntegerDivLossAccuracy <scope-IntegerDivLossAccuracy.html>`_,
   `zircon-temporary-objects <zircon-temporary-objects.html>`_,


.. csv-table:: Aliases..
   :header: "Name", "Redirect", "Offers fixes"

   `cert-con36-c <cert-con36-c.html>`_, `bugprone-spuriously-wake-up-functions <bugprone-spuriously-wake-up-functions.html>`_,
   `cert-con54-cpp <cert-con54-cpp.html>`_, `bugprone-spuriously-wake-up-functions <bugprone-spuriously-wake-up-functions.html>`_,
   `cert-dcl03-c <cert-dcl03-c.html>`_, `misc-static-assert <misc-static-assert.html>`_, "Yes"
   `cert-dcl16-c <cert-dcl16-c.html>`_, `readability-uppercase-literal-suffix <readability-uppercase-literal-suffix.html>`_, "Yes"
   `cert-dcl37-c <cert-dcl37-c.html>`_, `bugprone-reserved-identifier <bugprone-reserved-identifier.html>`_, "Yes"
   `cert-dcl51-cpp <cert-dcl51-cpp.html>`_, `bugprone-reserved-identifier <bugprone-reserved-identifier.html>`_, "Yes"
   `cert-dcl54-cpp <cert-dcl54-cpp.html>`_, `misc-new-delete-overloads <misc-new-delete-overloads.html>`_,
   `cert-dcl59-cpp <cert-dcl59-cpp.html>`_, `google-build-namespaces <google-build-namespaces.html>`_,
   `cert-err09-cpp <cert-err09-cpp.html>`_, `misc-throw-by-value-catch-by-reference <misc-throw-by-value-catch-by-reference.html>`_,
   `cert-err61-cpp <cert-err61-cpp.html>`_, `misc-throw-by-value-catch-by-reference <misc-throw-by-value-catch-by-reference.html>`_,
   `cert-fio38-c <cert-fio38-c.html>`_, `misc-non-copyable-objects <misc-non-copyable-objects.html>`_,
   `cert-msc30-c <cert-msc30-c.html>`_, `cert-msc50-cpp <cert-msc50-cpp.html>`_,
   `cert-msc32-c <cert-msc32-c.html>`_, `cert-msc51-cpp <cert-msc51-cpp.html>`_,
   `cert-oop11-cpp <cert-oop11-cpp.html>`_, `performance-move-constructor-init <performance-move-constructor-init.html>`_, "Yes"
   `cert-oop54-cpp <cert-oop54-cpp.html>`_, `bugprone-unhandled-self-assignment <bugprone-unhandled-self-assignment.html>`_,
   `cert-pos44-c <cert-pos44-c.html>`_, `bugprone-bad-signal-to-kill-thread <bugprone-bad-signal-to-kill-thread.html>`_,
   `cert-sig30-c <cert-sig30-c.html>`_, `bugprone-signal-handler <bugprone-signal-handler.html>`_,
   `cert-str34-c <cert-str34-c.html>`_, `bugprone-signed-char-misuse <bugprone-signed-char-misuse.html>`_,
   `clang-analyzer-core.CallAndMessage <clang-analyzer-core.CallAndMessage.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.DivideZero <clang-analyzer-core.DivideZero.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.NonNullParamChecker <clang-analyzer-core.NonNullParamChecker.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.NullDereference <clang-analyzer-core.NullDereference.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.StackAddressEscape <clang-analyzer-core.StackAddressEscape.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.UndefinedBinaryOperatorResult <clang-analyzer-core.UndefinedBinaryOperatorResult.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.VLASize <clang-analyzer-core.VLASize.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.uninitialized.ArraySubscript <clang-analyzer-core.uninitialized.ArraySubscript.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.uninitialized.Assign <clang-analyzer-core.uninitialized.Assign.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.uninitialized.Branch <clang-analyzer-core.uninitialized.Branch.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-core.uninitialized.UndefReturn <clang-analyzer-core.uninitialized.UndefReturn.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-cplusplus.Move <clang-analyzer-cplusplus.Move.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-cplusplus.NewDelete <clang-analyzer-cplusplus.NewDelete.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-cplusplus.NewDeleteLeaks <clang-analyzer-cplusplus.NewDeleteLeaks.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-deadcode.DeadStores <clang-analyzer-deadcode.DeadStores.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-nullability.NullPassedToNonnull <clang-analyzer-nullability.NullPassedToNonnull.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-nullability.NullReturnedFromNonnull <clang-analyzer-nullability.NullReturnedFromNonnull.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-nullability.NullableDereferenced <clang-analyzer-nullability.NullableDereferenced.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-nullability.NullablePassedToNonnull <clang-analyzer-nullability.NullablePassedToNonnull.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-optin.cplusplus.UninitializedObject <clang-analyzer-optin.cplusplus.UninitializedObject.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-optin.cplusplus.VirtualCall <clang-analyzer-optin.cplusplus.VirtualCall.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-optin.mpi.MPI-Checker <clang-analyzer-optin.mpi.MPI-Checker.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-optin.osx.cocoa.localizability.EmptyLocalizationContextChecker <clang-analyzer-optin.osx.cocoa.localizability.EmptyLocalizationContextChecker.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-optin.osx.cocoa.localizability.NonLocalizedStringChecker <clang-analyzer-optin.osx.cocoa.localizability.NonLocalizedStringChecker.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.API <clang-analyzer-osx.API.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.SecKeychainAPI <clang-analyzer-osx.SecKeychainAPI.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.AtSync <clang-analyzer-osx.cocoa.AtSync.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.ClassRelease <clang-analyzer-osx.cocoa.ClassRelease.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.Dealloc <clang-analyzer-osx.cocoa.Dealloc.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.IncompatibleMethodTypes <clang-analyzer-osx.cocoa.IncompatibleMethodTypes.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.NSAutoreleasePool <clang-analyzer-osx.cocoa.NSAutoreleasePool.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.NSError <clang-analyzer-osx.cocoa.NSError.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.NilArg <clang-analyzer-osx.cocoa.NilArg.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.ObjCGenerics <clang-analyzer-osx.cocoa.ObjCGenerics.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.RetainCount <clang-analyzer-osx.cocoa.RetainCount.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.SelfInit <clang-analyzer-osx.cocoa.SelfInit.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.SuperDealloc <clang-analyzer-osx.cocoa.SuperDealloc.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.UnusedIvars <clang-analyzer-osx.cocoa.UnusedIvars.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.cocoa.VariadicMethodTypes <clang-analyzer-osx.cocoa.VariadicMethodTypes.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.coreFoundation.CFError <clang-analyzer-osx.coreFoundation.CFError.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.coreFoundation.CFNumber <clang-analyzer-osx.coreFoundation.CFNumber.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.coreFoundation.CFRetainRelease <clang-analyzer-osx.coreFoundation.CFRetainRelease.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.coreFoundation.containers.OutOfBounds <clang-analyzer-osx.coreFoundation.containers.OutOfBounds.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-osx.coreFoundation.containers.PointerSizedValues <clang-analyzer-osx.coreFoundation.containers.PointerSizedValues.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.FloatLoopCounter <clang-analyzer-security.FloatLoopCounter.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling <clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.UncheckedReturn <clang-analyzer-security.insecureAPI.UncheckedReturn.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.bcmp <clang-analyzer-security.insecureAPI.bcmp.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.bcopy <clang-analyzer-security.insecureAPI.bcopy.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.bzero <clang-analyzer-security.insecureAPI.bzero.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.getpw <clang-analyzer-security.insecureAPI.getpw.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.gets <clang-analyzer-security.insecureAPI.gets.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.mkstemp <clang-analyzer-security.insecureAPI.mkstemp.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.mktemp <clang-analyzer-security.insecureAPI.mktemp.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.rand <clang-analyzer-security.insecureAPI.rand.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.strcpy <clang-analyzer-security.insecureAPI.strcpy.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-security.insecureAPI.vfork <clang-analyzer-security.insecureAPI.vfork.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.API <clang-analyzer-unix.API.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.Malloc <clang-analyzer-unix.Malloc.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.MallocSizeof <clang-analyzer-unix.MallocSizeof.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.MismatchedDeallocator <clang-analyzer-unix.MismatchedDeallocator.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.Vfork <clang-analyzer-unix.Vfork.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.cstring.BadSizeArg <clang-analyzer-unix.cstring.BadSizeArg.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `clang-analyzer-unix.cstring.NullArg <clang-analyzer-unix.cstring.NullArg.html>`_, `Clang Static Analyzer <https://clang.llvm.org/docs/analyzer/checkers.html>`_,
   `cppcoreguidelines-avoid-c-arrays <cppcoreguidelines-avoid-c-arrays.html>`_, `modernize-avoid-c-arrays <modernize-avoid-c-arrays.html>`_,
   `cppcoreguidelines-avoid-magic-numbers <cppcoreguidelines-avoid-magic-numbers.html>`_, `readability-magic-numbers <readability-magic-numbers.html>`_,
   `cppcoreguidelines-c-copy-assignment-signature <cppcoreguidelines-c-copy-assignment-signature.html>`_, `misc-unconventional-assign-operator <misc-unconventional-assign-operator.html>`_,
   `cppcoreguidelines-explicit-virtual-functions <cppcoreguidelines-explicit-virtual-functions.html>`_, `modernize-use-override <modernize-use-override.html>`_, "Yes"
   `cppcoreguidelines-non-private-member-variables-in-classes <cppcoreguidelines-non-private-member-variables-in-classes.html>`_, `misc-non-private-member-variables-in-classes <misc-non-private-member-variables-in-classes.html>`_,
   `fuchsia-header-anon-namespaces <fuchsia-header-anon-namespaces.html>`_, `google-build-namespaces <google-build-namespaces.html>`_,
   `google-readability-braces-around-statements <google-readability-braces-around-statements.html>`_, `readability-braces-around-statements <readability-braces-around-statements.html>`_, "Yes"
   `google-readability-function-size <google-readability-function-size.html>`_, `readability-function-size <readability-function-size.html>`_,
   `google-readability-namespace-comments <google-readability-namespace-comments.html>`_, `llvm-namespace-comment <llvm-namespace-comment.html>`_,
   `hicpp-avoid-c-arrays <hicpp-avoid-c-arrays.html>`_, `modernize-avoid-c-arrays <modernize-avoid-c-arrays.html>`_,
   `hicpp-braces-around-statements <hicpp-braces-around-statements.html>`_, `readability-braces-around-statements <readability-braces-around-statements.html>`_, "Yes"
   `hicpp-deprecated-headers <hicpp-deprecated-headers.html>`_, `modernize-deprecated-headers <modernize-deprecated-headers.html>`_, "Yes"
   `hicpp-explicit-conversions <hicpp-explicit-conversions.html>`_, `google-explicit-constructor <google-explicit-constructor.html>`_, "Yes"
   `hicpp-function-size <hicpp-function-size.html>`_, `readability-function-size <readability-function-size.html>`_,
   `hicpp-invalid-access-moved <hicpp-invalid-access-moved.html>`_, `bugprone-use-after-move <bugprone-use-after-move.html>`_,
   `hicpp-member-init <hicpp-member-init.html>`_, `cppcoreguidelines-pro-type-member-init <cppcoreguidelines-pro-type-member-init.html>`_, "Yes"
   `hicpp-move-const-arg <hicpp-move-const-arg.html>`_, `performance-move-const-arg <performance-move-const-arg.html>`_, "Yes"
   `hicpp-named-parameter <hicpp-named-parameter.html>`_, `readability-named-parameter <readability-named-parameter.html>`_, "Yes"
   `hicpp-new-delete-operators <hicpp-new-delete-operators.html>`_, `misc-new-delete-overloads <misc-new-delete-overloads.html>`_,
   `hicpp-no-array-decay <hicpp-no-array-decay.html>`_, `cppcoreguidelines-pro-bounds-array-to-pointer-decay <cppcoreguidelines-pro-bounds-array-to-pointer-decay.html>`_,
   `hicpp-no-malloc <hicpp-no-malloc.html>`_, `cppcoreguidelines-no-malloc <cppcoreguidelines-no-malloc.html>`_,
   `hicpp-noexcept-move <hicpp-noexcept-move.html>`_, `performance-noexcept-move-constructor <performance-noexcept-move-constructor.html>`_, "Yes"
   `hicpp-special-member-functions <hicpp-special-member-functions.html>`_, `cppcoreguidelines-special-member-functions <cppcoreguidelines-special-member-functions.html>`_,
   `hicpp-static-assert <hicpp-static-assert.html>`_, `misc-static-assert <misc-static-assert.html>`_, "Yes"
   `hicpp-undelegated-constructor <hicpp-undelegated-constructor.html>`_, `bugprone-undelegated-constructor <bugprone-undelegated-constructor.html>`_,
   `hicpp-uppercase-literal-suffix <hicpp-uppercase-literal-suffix.html>`_, `readability-uppercase-literal-suffix <readability-uppercase-literal-suffix.html>`_, "Yes"
   `hicpp-use-auto <hicpp-use-auto.html>`_, `modernize-use-auto <modernize-use-auto.html>`_, "Yes"
   `hicpp-use-emplace <hicpp-use-emplace.html>`_, `modernize-use-emplace <modernize-use-emplace.html>`_, "Yes"
   `hicpp-use-equals-default <hicpp-use-equals-default.html>`_, `modernize-use-equals-default <modernize-use-equals-default.html>`_, "Yes"
   `hicpp-use-equals-delete <hicpp-use-equals-delete.html>`_, `modernize-use-equals-delete <modernize-use-equals-delete.html>`_, "Yes"
   `hicpp-use-noexcept <hicpp-use-noexcept.html>`_, `modernize-use-noexcept <modernize-use-noexcept.html>`_, "Yes"
   `hicpp-use-nullptr <hicpp-use-nullptr.html>`_, `modernize-use-nullptr <modernize-use-nullptr.html>`_, "Yes"
   `hicpp-use-override <hicpp-use-override.html>`_, `modernize-use-override <modernize-use-override.html>`_, "Yes"
   `hicpp-vararg <hicpp-vararg.html>`_, `cppcoreguidelines-pro-type-vararg <cppcoreguidelines-pro-type-vararg.html>`_,
   `llvm-else-after-return <llvm-else-after-return.html>`_, `readability-else-after-return <readability-else-after-return.html>`_, "Yes"
   `llvm-qualified-auto <llvm-qualified-auto.html>`_, `readability-qualified-auto <readability-qualified-auto.html>`_, "Yes"
