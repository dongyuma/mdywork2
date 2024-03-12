//==--- DiagnosticSemaKinds.td - libsema diagnostics ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Semantic Analysis
//===----------------------------------------------------------------------===//

let Component = "Sema" in {
let CategoryName = "Semantic Issue" in {
def note_previous_decl : Note<"%0 declared here">;
def note_entity_declared_at : Note<"%0 declared here">;
def note_callee_decl : Note<"%0 declared here">;
def note_defined_here : Note<"%0 defined here">;

// For loop analysis
def warn_variables_not_in_loop_body : Warning<
  "变量%select{s| %1|s %1 and %2|s %1, %2, and %3|s %1, %2, %3, and %4}0 "
  "在循环体中未修改的循环条件中使用">,
  InGroup<ForLoopAnalysis>, DefaultIgnore;
def warn_redundant_loop_iteration : Warning<
  "变量 %0 是 %select{decremented|incremented}1 在循环头 "
  "和循环体中">,
  InGroup<ForLoopAnalysis>, DefaultIgnore;
def note_loop_iteration_here : Note<"%select{decremented|incremented}0 here">;

def warn_duplicate_enum_values : Warning<
  "元素 %0 已经被隐式分配 %1 另一个元素已经 "
  "被分配给该元素">, InGroup<DiagGroup<"duplicate-enum">>, DefaultIgnore;
def note_duplicate_element : Note<"element %0 also has value %1">;

// Absolute value functions
def warn_unsigned_abs : Warning<
  "获取无符号类型 %0 的绝对值无效">,
  InGroup<AbsoluteValue>;
def note_remove_abs : Note<
  "remove the call to '%0' since unsigned values cannot be negative">;
def warn_abs_too_small : Warning<
  "绝对值函数%0给定了类型为%1的参数 "
  "但具有类型为%2的参数，这可能会导致值被截断">, InGroup<AbsoluteValue>;
def warn_wrong_absolute_value_type : Warning<
  "使用 %select{integer|floating point|complex}1 绝对值函数 %0 "
  "当参数是 %select{integer|floating point|complex}2 类型时">,
  InGroup<AbsoluteValue>;
def note_replace_abs_function : Note<"use function '%0' instead">;
def warn_pointer_abs : Warning<
  "获取绝对值 %%select{pointer|function|array}0 类型 %1 是可疑的">,
  InGroup<AbsoluteValue>;

def warn_max_unsigned_zero : Warning<
  "取最大值 "
  "%select{a value and unsigned zero|unsigned zero and a value}0 "
  "始终等于另一个值">,
  InGroup<MaxUnsignedZero>;
def note_remove_max_call : Note<
  "remove call to max function and unsigned zero argument">;

def warn_infinite_recursive_function : Warning<
  "通过此函数的所有路径都将调用自身">,
  InGroup<InfiniteRecursion>, DefaultIgnore;

def warn_comma_operator : Warning<"此处可能误用逗号运算符">,
  InGroup<DiagGroup<"comma">>, DefaultIgnore;
def note_cast_to_void : Note<"cast expression to void to silence warning">;

// Constant expressions
def err_expr_not_ice : Error<
  "expression is not an %select{integer|integral}0 constant expression">;
def ext_expr_not_ice : Extension<
  "expression is not an %select{integer|integral}0 constant expression; "
  "folding it to a constant is a GNU extension">, InGroup<GNUFoldingConstant>;
def err_typecheck_converted_constant_expression : Error<
  "value of type %0 is not implicitly convertible to %1">;
def err_typecheck_converted_constant_expression_disallowed : Error<
  "conversion from %0 to %1 is not allowed in a converted constant expression">;
def err_typecheck_converted_constant_expression_indirect : Error<
  "conversion from %0 to %1 in converted constant expression would "
  "bind reference to a temporary">;
def err_expr_not_cce : Error<
  "%select{case value|enumerator value|non-type template argument|"
  "array size|constexpr if condition|explicit specifier argument}0 "
  "is not a constant expression">;
def ext_cce_narrowing : ExtWarn<
  "%select{case value|enumerator value|non-type template argument|"
  "array size|constexpr if condition|explicit specifier argument}0 "
  "%select{cannot be narrowed from type %2 to %3|"
  "evaluates to %2, which cannot be narrowed to type %3}1">,
  InGroup<CXX11Narrowing>, DefaultError, SFINAEFailure;
def err_ice_not_integral : Error<
  "%select{integer|integral}1 constant expression must have "
  "%select{integer|integral or unscoped enumeration}1 type, not %0">;
def err_ice_incomplete_type : Error<
  "integral constant expression has incomplete class type %0">;
def err_ice_explicit_conversion : Error<
  "integral constant expression requires explicit conversion from %0 to %1">;
def note_ice_conversion_here : Note<
  "conversion to %select{integral|enumeration}0 type %1 declared here">;
def err_ice_ambiguous_conversion : Error<
  "ambiguous conversion from type %0 to an integral or unscoped "
  "enumeration type">;
def err_ice_too_large : Error<
  "integer constant expression evaluates to value %0 that cannot be "
  "represented in a %1-bit %select{signed|unsigned}2 integer type">;
def err_expr_not_string_literal : Error<"expression is not a string literal">;

// Semantic analysis of constant literals.
def ext_predef_outside_function : Warning<
  "预定义标识符仅在函数内部有效">,
  InGroup<DiagGroup<"predefined-identifier-outside-function">>;
def warn_float_overflow : Warning<
  "对于类型%0，浮点常量的大小太大；最大值为%1">,
   InGroup<LiteralRange>;
def warn_float_underflow : Warning<
  "对于类型%0，浮点常量的大小太小；最小值为%1">,
  InGroup<LiteralRange>;
def warn_double_const_requires_fp64 : Warning<
  "双精度常量需要 %select{cl_khr_fp64|cl_khr_fp64 and __opencl_c_fp64}0, "
  "转换为单精度">;
def err_half_const_requires_fp16 : Error<
  "half precision constant requires cl_khr_fp16">;

// C99 variable-length arrays
def ext_vla : Extension<"variable length arrays are a C99 feature">,
  InGroup<VLAExtension>;
def warn_vla_used : Warning<"使用可变长度数组">,
  InGroup<VLA>, DefaultIgnore;
def err_vla_in_sfinae : Error<
  "variable length array cannot be formed during template argument deduction">;
def err_array_star_in_function_definition : Error<
  "variable length array must be bound in function definition">;
def err_vla_decl_in_file_scope : Error<
  "variable length array declaration not allowed at file scope">;
def err_vla_decl_has_static_storage : Error<
  "variable length array declaration cannot have 'static' storage duration">;
def err_vla_decl_has_extern_linkage : Error<
  "variable length array declaration cannot have 'extern' linkage">;
def ext_vla_folded_to_constant : ExtWarn<
  "variable length array folded to constant array as an extension">,
  InGroup<GNUFoldingConstant>;
def err_vla_unsupported : Error<
  "variable length arrays are not supported for the current target">;
def note_vla_unsupported : Note<
  "variable length arrays are not supported for the current target">;

// C99 variably modified types
def err_variably_modified_template_arg : Error<
  "variably modified type %0 cannot be used as a template argument">;
def err_variably_modified_nontype_template_param : Error<
  "non-type template parameter of variably modified type %0">;
def err_variably_modified_new_type : Error<
  "'new' cannot allocate object of variably modified type %0">;

// C99 Designated Initializers
def ext_designated_init : Extension<
  "designated initializers are a C99 feature">, InGroup<C99Designator>;
def err_array_designator_negative : Error<
  "array designator value '%0' is negative">;
def err_array_designator_empty_range : Error<
  "array designator range [%0, %1] is empty">;
def err_array_designator_non_array : Error<
  "array designator cannot initialize non-array type %0">;
def err_array_designator_too_large : Error<
  "array designator index (%0) exceeds array bounds (%1)">;
def err_field_designator_non_aggr : Error<
  "field designator cannot initialize a "
  "%select{non-struct, non-union|non-class}0 type %1">;
def err_field_designator_unknown : Error<
  "field designator %0 does not refer to any field in type %1">;
def err_field_designator_nonfield : Error<
  "field designator %0 does not refer to a non-static data member">;
def note_field_designator_found : Note<"field designator refers here">;
def err_designator_for_scalar_or_sizeless_init : Error<
  "designator in initializer for %select{scalar|indivisible sizeless}0 "
  "type %1">;
def warn_initializer_overrides : Warning<
  "初始值设定项 %select{partially |}0 覆盖此子对象的 "
  "先前初始化">, InGroup<InitializerOverrides>;
def ext_initializer_overrides : ExtWarn<warn_initializer_overrides.Text>,
  InGroup<InitializerOverrides>, SFINAEFailure;
def err_initializer_overrides_destructed : Error<
  "initializer would partially override prior initialization of object of "
  "type %1 with non-trivial destruction">;
def note_previous_initializer : Note<
  "previous initialization %select{|with side effects }0is here"
  "%select{| (side effects will not occur at run time)}0">;
def err_designator_into_flexible_array_member : Error<
  "designator into flexible array member subobject">;
def note_flexible_array_member : Note<
  "initialized flexible array member %0 is here">;
def ext_flexible_array_init : Extension<
  "flexible array initialization is a GNU extension">, InGroup<GNUFlexibleArrayInitializer>;

// C++20 designated initializers
def ext_cxx_designated_init : Extension<
  "designated initializers are a C++20 extension">, InGroup<CXX20Designator>;
def warn_cxx17_compat_designated_init : Warning<
  "指定的初始值设定项与C++20之前的C++标准不兼容">,
  InGroup<CXXPre20CompatPedantic>, DefaultIgnore;
def ext_designated_init_mixed : ExtWarn<
  "mixture of designated and non-designated initializers in the same "
  "initializer list is a C99 extension">, InGroup<C99Designator>;
def note_designated_init_mixed : Note<
  "first non-designated initializer is here">;
def ext_designated_init_array : ExtWarn<
  "array designators are a C99 extension">, InGroup<C99Designator>;
def ext_designated_init_nested : ExtWarn<
  "nested designators are a C99 extension">, InGroup<C99Designator>;
def ext_designated_init_reordered : ExtWarn<
  "ISO C++ requires field designators to be specified in declaration order; "
  "field %1 will be initialized after field %0">, InGroup<ReorderInitList>,
  SFINAEFailure;
def note_previous_field_init : Note<
  "previous initialization for field %0 is here">;

// Declarations.
def ext_plain_complex : ExtWarn<
  "plain '_Complex' requires a type specifier; assuming '_Complex double'">;
def ext_imaginary_constant : Extension<
  "imaginary constants are a GNU extension">, InGroup<GNUImaginaryConstant>;
def ext_integer_complex : Extension<
  "complex integer types are a GNU extension">, InGroup<GNUComplexInteger>;

def err_invalid_saturation_spec : Error<"'_Sat' specifier is only valid on "
  "'_Fract' or '_Accum', not '%0'">;
def err_invalid_sign_spec : Error<"'%0' cannot be signed or unsigned">;
def err_invalid_width_spec : Error<
  "'%select{|short|long|long long}0 %1' is invalid">;
def err_invalid_complex_spec : Error<"'_Complex %0' is invalid">;

def ext_auto_type_specifier : ExtWarn<
  "'auto' type specifier is a C++11 extension">, InGroup<CXX11>;
def warn_auto_storage_class : Warning<
  "'auto'存储类说明符是冗余的，与C++11不兼容">,
  InGroup<CXX11Compat>, DefaultIgnore;

def warn_deprecated_register : Warning<
  "'register'存储类说明符已弃用 "
  "并且与C++17不兼容">, InGroup<DeprecatedRegister>;
def ext_register_storage_class : ExtWarn<
  "ISO C++17 does not allow 'register' storage class specifier">,
  DefaultError, InGroup<Register>;

def err_invalid_decl_spec_combination : Error<
  "cannot combine with previous '%0' declaration specifier">;
def err_invalid_vector_decl_spec_combination : Error<
  "cannot combine with previous '%0' declaration specifier. "
  "'__vector' must be first">;
def err_invalid_pixel_decl_spec_combination : Error<
  "'__pixel' must be preceded by '__vector'.  "
  "'%0' declaration specifier not allowed here">;
def err_invalid_vector_bool_decl_spec : Error<
  "cannot use '%0' with '__vector bool'">;
def err_invalid_vector_long_decl_spec : Error<
  "cannot use 'long' with '__vector'">;
def err_invalid_vector_float_decl_spec : Error<
  "cannot use 'float' with '__vector'">;
def err_invalid_vector_double_decl_spec : Error <
  "use of 'double' with '__vector' requires VSX support to be enabled "
  "(available on POWER7 or later)">;
def err_invalid_vector_bool_int128_decl_spec : Error <
  "use of '__int128' with '__vector bool' requires VSX support enabled (on "
  "POWER10 or later)">;
def err_invalid_vector_long_long_decl_spec : Error <
  "use of 'long long' with '__vector bool' requires VSX support (available on "
  "POWER7 or later) or extended Altivec support (available on POWER8 or later) "
  "to be enabled">;
def err_invalid_vector_long_double_decl_spec : Error<
  "cannot use 'long double' with '__vector'">;
def warn_vector_long_decl_spec_combination : Warning<
  "不推荐将 'long' 与 '__vector' 一起使用">, InGroup<Deprecated>;

def err_redeclaration_different_type : Error<
  "redeclaration of %0 with a different type%diff{: $ vs $|}1,2">;
def err_bad_variable_name : Error<
  "%0 cannot be the name of a variable or data member">;
def err_bad_parameter_name : Error<
  "%0 cannot be the name of a parameter">;
def err_bad_parameter_name_template_id : Error<
  "parameter name cannot have template arguments">;
def ext_parameter_name_omitted_c2x : ExtWarn<
  "omitting the parameter name in a function definition is a C2x extension">,
  InGroup<C2x>;
def err_anyx86_interrupt_attribute : Error<
  "%select{x86|x86-64}0 'interrupt' attribute only applies to functions that "
  "have %select{a 'void' return type|"
  "only a pointer parameter optionally followed by an integer parameter|"
  "a pointer as the first parameter|a %2 type as the second parameter}1">;
def err_anyx86_interrupt_called : Error<
  "interrupt service routine cannot be called directly">;
def warn_arm_interrupt_calling_convention : Warning<
   "无中断属性的函数调用可能会破坏被中断方的VFP寄存器">,
   InGroup<Extra>;
def warn_interrupt_attribute_invalid : Warning<
   "%select{MIPS|MSP430|RISC-V}0 'interrupt' 属性仅适用于 "
   "具有%select{no parameters|a 'void' return type}1">,
   InGroup<IgnoredAttributes>;
def warn_riscv_repeated_interrupt_attribute : Warning<
  "重复的RISC-V'interrupt'属性">, InGroup<IgnoredAttributes>;
def note_riscv_repeated_interrupt_attribute : Note<
  "repeated RISC-V 'interrupt' attribute is here">;
def warn_unused_parameter : Warning<"未使用的变量 %0">,
  InGroup<UnusedParameter>, DefaultIgnore;
def warn_unused_variable : Warning<"未使用的变量 %0">,
  InGroup<UnusedVariable>, DefaultIgnore;
def warn_unused_local_typedef : Warning<
  "未使用 %select{typedef|type alias}0 %1">,
  InGroup<UnusedLocalTypedef>, DefaultIgnore;
def warn_unused_property_backing_ivar :
  Warning<"支持该属性的 ivar %0 "
  "未在此属性的访问器中引用">,
  InGroup<UnusedPropertyIvar>, DefaultIgnore;
def warn_unused_const_variable : Warning<"未使用的变量 %0">,
  InGroup<UnusedConstVariable>, DefaultIgnore;
def warn_unused_exception_param : Warning<"未使用的异常参数 %0">,
  InGroup<UnusedExceptionParameter>, DefaultIgnore;
def warn_decl_in_param_list : Warning<
  "%0 的声明在此函数之外将不可见">,
  InGroup<Visibility>;
def warn_redefinition_in_param_list : Warning<
  "%0 的重新定义在此函数之外不可见">,
  InGroup<Visibility>;
def warn_empty_parens_are_function_decl : Warning<
  "空括号解释为函数声明">,
  InGroup<VexingParse>;
def warn_parens_disambiguated_as_function_declaration : Warning<
  "括号作为函数声明被消除歧义">,
  InGroup<VexingParse>;
def warn_parens_disambiguated_as_variable_declaration : Warning<
  "在名为%0的变量的声明周围 "
  "括号被消歧为冗余括号">, InGroup<VexingParse>;
def warn_redundant_parens_around_declarator : Warning<
  "围绕声明符的多余括号">,
  InGroup<DiagGroup<"redundant-parens">>, DefaultIgnore;
def note_additional_parens_for_variable_declaration : Note<
  "add a pair of parentheses to declare a variable">;
def note_raii_guard_add_name : Note<
  "add a variable name to declare a %0 initialized with %1">;
def note_function_style_cast_add_parentheses : Note<
  "add enclosing parentheses to perform a function-style cast">;
def note_remove_parens_for_variable_declaration : Note<
  "remove parentheses to silence this warning">;
def note_empty_parens_function_call : Note<
  "change this ',' to a ';' to call %0">;
def note_empty_parens_default_ctor : Note<
  "remove parentheses to declare a variable">;
def note_empty_parens_zero_initialize : Note<
  "replace parentheses with an initializer to declare a variable">;
def warn_unused_function : Warning<"未使用的函数 %0">,
  InGroup<UnusedFunction>, DefaultIgnore;
def warn_unused_template : Warning<"未使用 %select{function|variable}0 模板 %1">,
  InGroup<UnusedTemplate>, DefaultIgnore;
def warn_unused_member_function : Warning<"未使用的成员函数 %0">,
  InGroup<UnusedMemberFunction>, DefaultIgnore;
def warn_used_but_marked_unused: Warning<"%0 被标记为未使用的但是使用了">,
  InGroup<UsedButMarkedUnused>, DefaultIgnore;
def warn_unneeded_internal_decl : Warning<
  "%select{function|variable}0 %1 不需要也不会发出">,
  InGroup<UnneededInternalDecl>, DefaultIgnore;
def warn_unneeded_static_internal_decl : Warning<
  "头文件中声明的'静态'函数 %0 "
  "应声明为'静态内联'">,
  InGroup<UnneededInternalDecl>, DefaultIgnore;
def warn_unneeded_member_function : Warning<
  "不需要成员函数 %0，也不会发出">,
  InGroup<UnneededMemberFunction>, DefaultIgnore;
def warn_unused_private_field: Warning<"私有字段 %0未使用">,
  InGroup<UnusedPrivateField>, DefaultIgnore;
def warn_unused_lambda_capture: Warning<"lambda 捕获 %0 不是 "
  "%select{used|required to be captured for this use}1">,
  InGroup<UnusedLambdaCapture>, DefaultIgnore;

def warn_parameter_size: Warning<
  "%0是一个大的（%1 字节）传递值参数; "
  "改为通过引用传递 ?">, InGroup<LargeByValueCopy>;
def warn_return_value_size: Warning<
  "%0的返回值是一个较大的（%1字节）传递值对象; "
  "改为通过引用传递 ?">, InGroup<LargeByValueCopy>;
def warn_return_value_udt: Warning<
  "%0有指定的C链接，但是返回%0 "
  "与C不兼容的用户定义类型%1">, InGroup<ReturnTypeCLinkage>;
def warn_return_value_udt_incomplete: Warning<
  "%0指定了C链接，但返回的类型%1不完整"
  "可能与C不兼容 ">, InGroup<ReturnTypeCLinkage>;
def warn_implicit_function_decl : Warning<
  "函数的隐式声明%0">,
  InGroup<ImplicitFunctionDeclare>, DefaultIgnore;
def ext_implicit_function_decl : ExtWarn<
  "implicit declaration of function %0 is invalid in C99">,
  InGroup<ImplicitFunctionDeclare>;
def note_function_suggestion : Note<"did you mean %0?">;

def err_ellipsis_first_param : Error<
  "ISO C requires a named parameter before '...'">;
def err_declarator_need_ident : Error<"declarator requires an identifier">;
def err_language_linkage_spec_unknown : Error<"unknown linkage language">;
def err_language_linkage_spec_not_ascii : Error<
  "string literal in language linkage specifier cannot have an "
  "encoding-prefix">;
def ext_use_out_of_scope_declaration : ExtWarn<
  "use of out-of-scope declaration of %0%select{| whose type is not "
  "compatible with that of an implicit declaration}1">,
  InGroup<DiagGroup<"out-of-scope-function">>;
def err_inline_non_function : Error<
  "'inline' can only appear on functions%select{| and non-local variables}0">;
def err_noreturn_non_function : Error<
  "'_Noreturn' can only appear on functions">;
def warn_qual_return_type : Warning<
  "'%0'类型限定符%s1（在返回类型%plural{1:has|:have}1上）无效">,
  InGroup<IgnoredQualifiers>, DefaultIgnore;
def warn_deprecated_redundant_constexpr_static_def : Warning<
  "constexpr静态数据成员的越界定义在C++17中是多余的 "
  "不推荐使用">,
  InGroup<Deprecated>, DefaultIgnore;

def warn_decl_shadow :
  Warning<"声明遮蔽了一个 %select{{"
          "local variable|"
          "variable in %2|"
          "static data member of %2|"
          "field of %2|"
          "typedef in %2|"
          "type alias in %2}1">,
  InGroup<Shadow>, DefaultIgnore;
def warn_decl_shadow_uncaptured_local :
  Warning<warn_decl_shadow.Text>,
  InGroup<ShadowUncapturedLocal>, DefaultIgnore;
def warn_ctor_parm_shadows_field:
  Warning<"构造函数参数%0隐藏了字段%1，此字段属于%2">,
  InGroup<ShadowFieldInConstructor>, DefaultIgnore;
def warn_modifying_shadowing_decl :
  Warning<"正在修改构造函数参数%0 "
          "此参数隐藏一个%1的字段">,
  InGroup<ShadowFieldInConstructorModified>, DefaultIgnore;

// C++ decomposition declarations
def err_decomp_decl_context : Error<
  "decomposition declaration not permitted in this context">;
def warn_cxx14_compat_decomp_decl : Warning<
  "分解声明与C++17之前的C++标准 "
  "不兼容">, DefaultIgnore, InGroup<CXXPre17Compat>;
def ext_decomp_decl : ExtWarn<
  "decomposition declarations are a C++17 extension">, InGroup<CXX17>;
def ext_decomp_decl_cond : ExtWarn<
  "ISO C++17 does not permit structured binding declaration in a condition">,
  InGroup<DiagGroup<"binding-in-condition">>;
def err_decomp_decl_spec : Error<
  "decomposition declaration cannot be declared "
  "%plural{1:'%1'|:with '%1' specifiers}0">;
def ext_decomp_decl_spec : ExtWarn<
  "decomposition declaration declared "
  "%plural{1:'%1'|:with '%1' specifiers}0 is a C++20 extension">,
  InGroup<CXX20>;
def warn_cxx17_compat_decomp_decl_spec : Warning<
  "分解声明被声明为 "
  "%plural{1:'%1'|:with '%1' specifiers}0 "
  "与C++20之前的C++标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def err_decomp_decl_type : Error<
  "decomposition declaration cannot be declared with type %0; "
  "declared type must be 'auto' or reference to 'auto'">;
def err_decomp_decl_parens : Error<
  "decomposition declaration cannot be declared with parentheses">;
def err_decomp_decl_template : Error<
  "decomposition declaration template not supported">;
def err_decomp_decl_not_alone : Error<
  "decomposition declaration must be the only declaration in its group">;
def err_decomp_decl_requires_init : Error<
  "decomposition declaration %0 requires an initializer">;
def err_decomp_decl_wrong_number_bindings : Error<
  "type %0 decomposes into %3 %plural{1:element|:elements}2, but "
  "%select{%plural{0:no|:only %1}1|%1}4 "
  "%plural{1:name was|:names were}1 provided">;
def err_decomp_decl_unbindable_type : Error<
  "cannot decompose %select{union|non-class, non-array}1 type %2">;
def err_decomp_decl_multiple_bases_with_members : Error<
  "cannot decompose class type %1: "
  "%select{its base classes %2 and|both it and its base class}0 %3 "
  "have non-static data members">;
def err_decomp_decl_ambiguous_base : Error<
  "cannot decompose members of ambiguous base class %1 of %0:%2">;
def err_decomp_decl_inaccessible_base : Error<
  "cannot decompose members of inaccessible base class %1 of %0">,
  AccessControl;
def err_decomp_decl_inaccessible_field : Error<
  "cannot decompose %select{private|protected}0 member %1 of %3">,
  AccessControl;
def err_decomp_decl_lambda : Error<
  "cannot decompose lambda closure type">;
def err_decomp_decl_anon_union_member : Error<
  "cannot decompose class type %0 because it has an anonymous "
  "%select{struct|union}1 member">;
def err_decomp_decl_std_tuple_element_not_specialized : Error<
  "cannot decompose this type; 'std::tuple_element<%0>::type' "
  "does not name a type">;
def err_decomp_decl_std_tuple_size_not_constant : Error<
  "cannot decompose this type; 'std::tuple_size<%0>::value' "
  "is not a valid integral constant expression">;
def note_in_binding_decl_init : Note<
  "in implicit initialization of binding declaration %0">;

def err_std_type_trait_not_class_template : Error<
  "unsupported standard library implementation: "
  "'std::%0' is not a class template">;

// C++ using declarations
def err_using_requires_qualname : Error<
  "using declaration requires a qualified name">;
def err_using_typename_non_type : Error<
  "'typename' keyword used on a non-type">;
def err_using_dependent_value_is_type : Error<
  "dependent using declaration resolved to type without 'typename'">;
def err_using_decl_nested_name_specifier_is_not_class : Error<
  "using declaration in class refers into '%0', which is not a class">;
def err_using_decl_nested_name_specifier_is_current_class : Error<
  "using declaration refers to its own class">;
def err_using_decl_nested_name_specifier_is_not_base_class : Error<
  "using declaration refers into '%0', which is not a base class of %1">;
def err_using_decl_constructor_not_in_direct_base : Error<
  "%0 is not a direct base of %1, cannot inherit constructors">;
def err_using_decl_can_not_refer_to_class_member : Error<
  "using declaration cannot refer to class member">;
def err_ambiguous_inherited_constructor : Error<
  "constructor of %0 inherited from multiple base class subobjects">;
def note_ambiguous_inherited_constructor_using : Note<
  "inherited from base class %0 here">;
def note_using_decl_class_member_workaround : Note<
  "use %select{an alias declaration|a typedef declaration|a reference|"
  "a const variable|a constexpr variable}0 instead">;
def err_using_decl_can_not_refer_to_namespace : Error<
  "using declaration cannot refer to a namespace">;
def err_using_decl_can_not_refer_to_scoped_enum : Error<
  "using declaration cannot refer to a scoped enumerator">;
def err_using_decl_constructor : Error<
  "using declaration cannot refer to a constructor">;
def warn_cxx98_compat_using_decl_constructor : Warning<
  "继承构造函数与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_using_decl_destructor : Error<
  "using declaration cannot refer to a destructor">;
def err_using_decl_template_id : Error<
  "using declaration cannot refer to a template specialization">;
def note_using_decl_target : Note<"target of using declaration">;
def note_using_decl_conflict : Note<"conflicting declaration">;
def err_using_decl_redeclaration : Error<"redeclaration of using declaration">;
def err_using_decl_conflict : Error<
  "target of using declaration conflicts with declaration already in scope">;
def err_using_decl_conflict_reverse : Error<
  "declaration conflicts with target of using declaration already in scope">;
def note_using_decl : Note<"%select{|previous }0using declaration">;
def err_using_decl_redeclaration_expansion : Error<
  "using declaration pack expansion at block scope produces multiple values">;

def warn_access_decl_deprecated : Warning<
  "不推荐使用访问声明；改用using声明">,
  InGroup<Deprecated>;
def err_access_decl : Error<
  "ISO C++11 does not allow access declarations; "
  "use using declarations instead">;
def warn_deprecated_copy_operation : Warning<
  "隐式拷贝的定义 %select{constructor|assignment operator}1 "
  "%0的已弃用，因为它具有用户声明的副本"
  "%select{assignment operator|constructor}1">,
  InGroup<DeprecatedCopy>, DefaultIgnore;
def warn_deprecated_copy_dtor_operation : Warning<
  "隐式拷贝的定义 %select{constructor|assignment operator}1 "
  "%0的已弃用，因为它具有用户声明的副本">,
  InGroup<DeprecatedCopyDtor>, DefaultIgnore;
def warn_cxx17_compat_exception_spec_in_signature : Warning<
  "由于函数签名中未抛出异常规范 "
  "%0的损坏名称将在C++17中更改">, InGroup<CXX17CompatMangling>;

def warn_global_constructor : Warning<
  "声明需要一个全局构造函数">,
  InGroup<GlobalConstructors>, DefaultIgnore;
def warn_global_destructor : Warning<
  "声明需要一个全局析构函数">,
   InGroup<GlobalConstructors>, DefaultIgnore;
def warn_exit_time_destructor : Warning<
  "声明需要一个退出时析构函数">,
  InGroup<ExitTimeDestructors>, DefaultIgnore;

def err_invalid_thread : Error<
  "'%0' is only allowed on variable declarations">;
def err_thread_non_global : Error<
  "'%0' variables must have global storage">;
def err_thread_unsupported : Error<
  "thread-local storage is not supported for the current target">;

// FIXME: Combine fallout warnings to just one warning.
def warn_maybe_falloff_nonvoid_function : Warning<
  "非void 函数不会在所有控制路径中返回一个值">,
  InGroup<ReturnType>;
def warn_falloff_nonvoid_function : Warning<
  "非void 函数不返回一个值">,
  InGroup<ReturnType>;
def err_maybe_falloff_nonvoid_block : Error<
  "non-void block does not return a value in all control paths">;
def err_falloff_nonvoid_block : Error<
  "non-void block does not return a value">;
def warn_maybe_falloff_nonvoid_coroutine : Warning<
  "非void协同路由不会在所有控制路径中返回一个值">,
  InGroup<ReturnType>;
def warn_falloff_nonvoid_coroutine : Warning<
  "非void协同路不返回一个值 ">,
  InGroup<ReturnType>;
def warn_suggest_noreturn_function : Warning<
  "%select{function|method}0 %1 可以用属性'noreturn'声明">,
  InGroup<MissingNoreturn>, DefaultIgnore;
def warn_suggest_noreturn_block : Warning<
  "块可以用属性'noreturn'声明">,
  InGroup<MissingNoreturn>, DefaultIgnore;

// Unreachable code.
def warn_unreachable : Warning<
  "代码将永远不会被执行">,
  InGroup<UnreachableCode>, DefaultIgnore;
def warn_unreachable_break : Warning<
  "'break' 将永远不会被执行">,
  InGroup<UnreachableCodeBreak>, DefaultIgnore;
def warn_unreachable_return : Warning<
  "'return' 将永远不会被执行">,
  InGroup<UnreachableCodeReturn>, DefaultIgnore;
def warn_unreachable_loop_increment : Warning<
  "循环最多运行一次（循环增量从未执行）">,
  InGroup<UnreachableCodeLoopIncrement>, DefaultIgnore;
def note_unreachable_silence : Note<
  "silence by adding parentheses to mark code as explicitly dead">;

/// Built-in functions.
def ext_implicit_lib_function_decl : ExtWarn<
  "implicitly declaring library function '%0' with type %1">,
  InGroup<ImplicitFunctionDeclare>;
def note_include_header_or_declare : Note<
  "include the header <%0> or explicitly provide a declaration for '%1'">;
def note_previous_builtin_declaration : Note<"%0 is a builtin with type %1">;
def warn_implicit_decl_no_jmp_buf
    : Warning<"内置函数 '%0' 的声明需要声明 'jmp_buf' 类型"
    " 通常在头文件 <setjmp.h> 中提供。">,
      InGroup<DiagGroup<"incomplete-setjmp-declaration">>;
def warn_implicit_decl_requires_sysheader : Warning<
  "内置函数“%1”的声明需要包含标头 <%0>">,
  InGroup<BuiltinRequiresHeader>;
def warn_redecl_library_builtin : Warning<
  "库函数 %0 的不兼容重新声明">,
  InGroup<DiagGroup<"incompatible-library-redeclaration">>;
def err_builtin_definition : Error<"definition of builtin function %0">;
def err_builtin_redeclare : Error<"cannot redeclare builtin function %0">;
def err_arm_invalid_specialreg : Error<"invalid special register for builtin">;
def err_arm_invalid_coproc : Error<"coprocessor %0 must be configured as "
  "%select{GCP|CDE}1">;
def err_invalid_cpu_supports : Error<"invalid cpu feature string for builtin">;
def err_invalid_cpu_is : Error<"invalid cpu name for builtin">;
def err_invalid_cpu_specific_dispatch_value : Error<
"invalid option '%0' for %select{cpu_specific|cpu_dispatch}1">;
def warn_builtin_unknown : Warning<"使用未知的内置 %0">,
  InGroup<ImplicitFunctionDeclare>, DefaultError;
def warn_cstruct_memaccess : Warning<
  "%select{destination for|source of|first operand of|second operand of}0 "
  "此 %1 调用是指向记录 %2 的指针 "
  "对于%select{primitive-default-initialize|primitive-copy}3来说是微不足道的">,
  InGroup<NonTrivialMemaccess>;
def note_nontrivial_field : Note<
  "field is non-trivial to %select{copy|default-initialize}0">;
def err_non_trivial_c_union_in_invalid_context : Error<
  "cannot %select{"
  "use type %1 for a function/method parameter|"
  "use type %1 for function/method return|"
  "default-initialize an object of type %1|"
  "declare an automatic variable of type %1|"
  "copy-initialize an object of type %1|"
  "assign to a variable of type %1|"
  "construct an automatic compound literal of type %1|"
  "capture a variable of type %1|"
  "cannot use volatile type %1 where it causes an lvalue-to-rvalue conversion"
  "}3 "
  "since it %select{contains|is}2 a union that is non-trivial to "
  "%select{default-initialize|destruct|copy}0">;
def note_non_trivial_c_union : Note<
  "%select{%2 has subobjects that are|%3 has type %2 that is}0 "
  "non-trivial to %select{default-initialize|destruct|copy}1">;
def warn_dyn_class_memaccess : Warning<
  "%select{destination for|source of|first operand of|second operand of}0 "
  "此 %1 调用是一个指针指向 %select{|class containing a }2 动态类 %3; "
  "vtable pointer will be %select{overwritten|copied|moved|compared}4">,
  InGroup<DynamicClassMemaccess>;
def note_bad_memaccess_silence : Note<
  "explicitly cast the pointer to silence this warning">;
def warn_sizeof_pointer_expr_memaccess : Warning<
  "'%0'调用对类型为%1的对象进行操作 "
  "而大小基于不同的类型%2">,
  InGroup<SizeofPointerMemaccess>;
def warn_sizeof_pointer_expr_memaccess_note : Note<
  "did you mean to %select{dereference the argument to 'sizeof' (and multiply "
  "it by the number of elements)|remove the addressof in the argument to "
  "'sizeof' (and multiply it by the number of elements)|provide an explicit "
  "length}0?">;
def warn_sizeof_pointer_type_memaccess : Warning<
  "%0 调用中的'sizeof'参数，是相同的指针类型%1 "
  "和%select{destination|source}2相比; 应为%3或显式长度">,
  InGroup<SizeofPointerMemaccess>;
def warn_strlcpycat_wrong_size : Warning<
  "%0调用中的size参数似乎是源的大小; "
  "预期目标的大小">,
  InGroup<DiagGroup<"strlcpy-strlcat-size">>;
def note_strlcpycat_wrong_size : Note<
  "change size argument to be the size of the destination">;
def warn_memsize_comparison : Warning<
  "%0调用中的size参数是一个比较 ">,
  InGroup<DiagGroup<"memsize-comparison">>;
def note_memsize_comparison_paren : Note<
  "did you mean to compare the result of %0 instead?">;
def note_memsize_comparison_cast_silence : Note<
  "explicitly cast the argument to size_t to silence this warning">;
def warn_suspicious_sizeof_memset : Warning<
  "%select{'size' argument to memset is '0'|"
  "setting buffer to a 'sizeof' expression}0"
  "; 您是要转置最后两个参数吗？">,
  InGroup<MemsetTransposedArgs>;
def note_suspicious_sizeof_memset_silence : Note<
  "%select{parenthesize the third argument|"
  "cast the second argument to 'int'}0 to silence">;
def warn_suspicious_bzero_size : Warning<"bzero 的'size'参数是'0'">,
  InGroup<SuspiciousBzero>;
def note_suspicious_bzero_size_silence : Note<
  "parenthesize the second argument to silence">;

def warn_strncat_large_size : Warning<
  "'strncat' 中的 size 参数值太大 "
  "可能导致缓冲区溢出">, InGroup<StrncatSize>;
def warn_strncat_src_size : Warning<"“strncat”调用中的size参数 "
  "似乎是源的大小">, InGroup<StrncatSize>;
def warn_strncat_wrong_size : Warning<
  "'strncat' 的 size 参数的值是错误的">, InGroup<StrncatSize>;
def note_strncat_wrong_size : Note<
  "change the argument to be the free space in the destination buffer minus "
  "the terminating null byte">;

def warn_assume_side_effects : Warning<
  "%0 的参数具有将被丢弃的副作用 ">,
  InGroup<DiagGroup<"assume">>;
def warn_assume_attribute_string_unknown : Warning<
  "未知的假设字符串'%0';属性可能被潜在地忽略">,
  InGroup<UnknownAssumption>;
def warn_assume_attribute_string_unknown_suggested : Warning<
  "未知的假设字符串'%0'可能拼写错误;属性可能被忽略， "
  "您的意思是“%1”吗？">,
  InGroup<MisspelledAssumption>;

def warn_builtin_chk_overflow : Warning<
  "'%0'总是会溢出;目标缓冲区的大小为 %1, "
  " 但size参数为 %2 ">,
  InGroup<DiagGroup<"builtin-memcpy-chk-size">>;

def warn_fortify_source_overflow
  : Warning<warn_builtin_chk_overflow.Text>, InGroup<FortifySource>;
def warn_fortify_source_size_mismatch : Warning<
  "'%0' 大小参数太大；"
  "目标缓冲区的大小为%1，但大小参数为%2">, InGroup<FortifySource>;

def warn_fortify_strlen_overflow: Warning<
  "'%0'将始终溢出；目标缓冲区的大小为%1，"
  "但源字符串的长度为%2（包括NUL字节）">,
  InGroup<FortifySource>;

def warn_fortify_source_format_overflow : Warning<
  "'%0' 将始终溢出；目标缓冲区大小为%1，"
  " 但格式字符串至少扩展到%2">,
  InGroup<FortifySource>;

def warn_fortify_scanf_overflow : Warning<
  "'%0'可能溢出；参数%1中的目标缓冲区大小为 "
  "%2，但相应的说明符可能需要大小为%3">,
  InGroup<FortifySource>;

def err_function_start_invalid_type: Error<
  "argument must be a function">;
/// main()
// static main() is not an error in C, just in C++.
def warn_static_main : Warning<"'main' 不应声明为静态的">,
    InGroup<Main>;
def err_static_main : Error<"'main' is not allowed to be declared static">;
def err_inline_main : Error<"'main' is not allowed to be declared inline">;
def ext_variadic_main : ExtWarn<
  "'main' is not allowed to be declared variadic">, InGroup<Main>;
def ext_noreturn_main : ExtWarn<
  "'main' is not allowed to be declared _Noreturn">, InGroup<Main>;
def note_main_remove_noreturn : Note<"remove '_Noreturn'">;
def err_constexpr_main : Error<
  "'main' is not allowed to be declared %select{constexpr|consteval}0">;
def err_deleted_main : Error<"'main' is not allowed to be deleted">;
def err_mainlike_template_decl : Error<"%0 cannot be a template">;
def err_main_returns_nonint : Error<"'main' must return 'int'">;
def ext_main_returns_nonint : ExtWarn<"return type of 'main' is not 'int'">,
    InGroup<MainReturnType>;
def note_main_change_return_type : Note<"change return type to 'int'">;
def err_main_surplus_args : Error<"too many parameters (%0) for 'main': "
    "must be 0, 2, or 3">;
def warn_main_one_arg : Warning<"'main'声明中只有一个参数">,
    InGroup<Main>;
def err_main_arg_wrong : Error<"%select{first|second|third|fourth}0 "
    "parameter of 'main' (%select{argument count|argument array|environment|"
    "platform-specific data}0) must be of type %1">;
def warn_main_returns_bool_literal : Warning<"从'main'返回的 "
    "布尔字面量">, InGroup<Main>;
def err_main_global_variable :
    Error<"main cannot be declared as global variable">;
def warn_main_redefined : Warning<"具有外部链接的名为'main'的变量 "
    "具有未定义的行为">, InGroup<Main>;
def ext_main_used : Extension<
  "ISO C++ does not allow 'main' to be used by a program">, InGroup<Main>;

/// parser diagnostics
def ext_no_declarators : ExtWarn<"declaration does not declare anything">,
  InGroup<MissingDeclarations>;
def err_no_declarators : Error<"declaration does not declare anything">;
def ext_typedef_without_a_name : ExtWarn<"typedef requires a name">,
  InGroup<MissingDeclarations>;
def err_typedef_not_identifier : Error<"typedef name must be an identifier">;

def ext_non_c_like_anon_struct_in_typedef : ExtWarn<
  "anonymous non-C-compatible type given name for linkage purposes "
  "by %select{typedef|alias}0 declaration; "
  "add a tag name here">, InGroup<DiagGroup<"non-c-typedef-for-linkage">>;
def err_non_c_like_anon_struct_in_typedef : Error<
  "anonymous non-C-compatible type given name for linkage purposes "
  "by %select{typedef|alias}0 declaration after its linkage was computed; "
  "add a tag name here to establish linkage prior to definition">;
def err_typedef_changes_linkage : Error<
  "unsupported: anonymous type given name for linkage purposes "
  "by %select{typedef|alias}0 declaration after its linkage was computed; "
  "add a tag name here to establish linkage prior to definition">;
def note_non_c_like_anon_struct : Note<
  "type is not C-compatible due to this "
  "%select{base class|default member initializer|lambda expression|"
  "friend declaration|member declaration}0">;
def note_typedef_for_linkage_here : Note<
  "type is given name %0 for linkage purposes by this "
  "%select{typedef|alias}1 declaration">;

def err_statically_allocated_object : Error<
  "interface type cannot be statically allocated">;
def err_object_cannot_be_passed_returned_by_value : Error<
  "interface type %1 cannot be %select{returned|passed}0 by value"
  "; did you forget * in %1?">;
def err_parameters_retval_cannot_have_fp16_type : Error<
  "%select{parameters|function return value}0 cannot have __fp16 type; did you forget * ?">;
def err_opencl_half_load_store : Error<
  "%select{loading directly from|assigning directly to}0 pointer to type %1 requires "
  "cl_khr_fp16. Use vector data %select{load|store}0 builtin functions instead">;
def err_opencl_cast_to_half : Error<"casting to type %0 is not allowed">;
def err_opencl_half_declaration : Error<
  "declaring variable of type %0 is not allowed">;
def err_opencl_invalid_param : Error<
  "declaring function parameter of type %0 is not allowed%select{; did you forget * ?|}1">;
def err_opencl_invalid_return : Error<
  "declaring function return value of type %0 is not allowed %select{; did you forget * ?|}1">;
def warn_enum_value_overflow : Warning<"枚举值溢出">;
def warn_pragma_options_align_reset_failed : Warning<
  "#pragma 选项 align=reset 失败: %0">,
  InGroup<IgnoredPragmas>;
def err_pragma_options_align_mac68k_target_unsupported : Error<
  "mac68k alignment pragma is not supported on this target">;
def warn_pragma_pack_invalid_alignment : Warning<
  "预期 #pragma pack 参数为'1', '2', '4', '8', or '16'">,
  InGroup<IgnoredPragmas>;
def err_pragma_pack_invalid_alignment : Error<
  warn_pragma_pack_invalid_alignment.Text>;
def warn_pragma_pack_non_default_at_include : Warning<
  "非默认 #pragma pack 值更改包含文件中 "
  "结构体或联合体成员的对齐方式">, InGroup<PragmaPackSuspiciousInclude>,
  DefaultIgnore;
def warn_pragma_pack_modified_after_include : Warning<
  "当前#pragma pack 对齐值 "
  "在包含文件中被修改">, InGroup<PragmaPack>;
def warn_pragma_pack_no_pop_eof : Warning<"未终止的 "
  "'#pragma pack (push, ...)' 在文件末尾">, InGroup<PragmaPack>;
def note_pragma_pack_here : Note<
  "previous '#pragma pack' directive that modifies alignment is here">;
def note_pragma_pack_pop_instead_reset : Note<
  "did you intend to use '#pragma pack (pop)' instead of '#pragma pack()'?">;
// Follow the Microsoft implementation.
def warn_pragma_pack_show : Warning<"#pragma pack(show)的值 == %0">;
def warn_pragma_pack_pop_identifier_and_alignment : Warning<
  "指定名称和对齐到'pop'是未定义的">;
def warn_pragma_pop_failed : Warning<"#pragma %0(pop, ...) 失败: %1">,
  InGroup<IgnoredPragmas>;
def err_pragma_fc_pp_scope : Error<
  "'#pragma float_control push/pop' can only appear at file scope or namespace scope">;
def err_pragma_fc_noprecise_requires_nofenv : Error<
  "'#pragma float_control(precise, off)' is illegal when fenv_access is enabled">;
def err_pragma_fc_except_requires_precise : Error<
  "'#pragma float_control(except, on)' is illegal when precise is disabled">;
def err_pragma_fc_noprecise_requires_noexcept : Error<
  "'#pragma float_control(precise, off)' is illegal when except is enabled">;
def err_pragma_fenv_requires_precise : Error<
  "'#pragma STDC FENV_ACCESS ON' is illegal when precise is disabled">;
def warn_cxx_ms_struct :
  Warning<"ms_struct 可能不会为具有基类或虚函数的类 "
          "生成与 Microsoft 兼容的布局">,
  DefaultError, InGroup<IncompatibleMSStruct>;
def err_pragma_pack_identifer_not_supported : Error<
  "specifying an identifier within `#pragma pack` is not supported on this target">;
def err_section_conflict : Error<"%0 causes a section type conflict with %1">;
def err_no_base_classes : Error<"invalid use of '__super', %0 has no base classes">;
def err_invalid_super_scope : Error<"invalid use of '__super', "
  "this keyword can only be used inside class or member function scope">;
def err_super_in_lambda_unsupported : Error<
  "use of '__super' inside a lambda is unsupported">;

def warn_pragma_unused_undeclared_var : Warning<
  "未声明的变量 %0 被用作 '#pragma unused'的参数">,
  InGroup<IgnoredPragmas>;
def warn_atl_uuid_deprecated : Warning<
  "不推荐将'uuid'指定为ATL属性;改用 __declspec ">,
  InGroup<DeprecatedDeclarations>;
def warn_pragma_unused_expected_var_arg : Warning<
  "只有变量可以是 '#pragma unused'参数">,
  InGroup<IgnoredPragmas>;
def err_pragma_push_visibility_mismatch : Error<
  "#pragma visibility push with no matching #pragma visibility pop">;
def note_surrounding_namespace_ends_here : Note<
  "surrounding namespace with visibility attribute ends here">;
def err_pragma_pop_visibility_mismatch : Error<
  "#pragma visibility pop with no matching #pragma visibility push">;
def note_surrounding_namespace_starts_here : Note<
  "surrounding namespace with visibility attribute starts here">;
def err_pragma_loop_invalid_argument_type : Error<
  "invalid argument of type %0; expected an integer type">;
def err_pragma_loop_invalid_argument_value : Error<
  "%select{invalid value '%0'; must be positive|value '%0' is too large}1">;
def err_pragma_loop_compatibility : Error<
  "%select{incompatible|duplicate}0 directives '%1' and '%2'">;
def err_pragma_loop_precedes_nonloop : Error<
  "expected a for, while, or do-while loop to follow '%0'">;

def err_pragma_attribute_matcher_subrule_contradicts_rule : Error<
  "redundant attribute subject matcher sub-rule '%0'; '%1' already matches "
  "those declarations">;
def err_pragma_attribute_matcher_negated_subrule_contradicts_subrule : Error<
  "negated attribute subject matcher sub-rule '%0' contradicts sub-rule '%1'">;
def err_pragma_attribute_invalid_matchers : Error<
  "attribute %0 can't be applied to %1">;
def err_pragma_attribute_stack_mismatch : Error<
  "'#pragma clang attribute %select{%1.|}0pop' with no matching"
  " '#pragma clang attribute %select{%1.|}0push'">;
def warn_pragma_attribute_unused : Warning<
  "'#pragma clang attribute push'区域中未使用的属性%0 ">,
  InGroup<PragmaClangAttribute>;
def note_pragma_attribute_region_ends_here : Note<
  "'#pragma clang attribute push' regions ends here">;
def err_pragma_attribute_no_pop_eof : Error<"unterminated "
  "'#pragma clang attribute push' at end of file">;
def note_pragma_attribute_applied_decl_here : Note<
  "when applied to this declaration">;
def err_pragma_attr_attr_no_push : Error<
  "'#pragma clang attribute' attribute with no matching "
  "'#pragma clang attribute push'">;

/// Objective-C parser diagnostics
def err_duplicate_class_def : Error<
  "duplicate interface definition for class %0">;
def err_undef_superclass : Error<
  "cannot find interface declaration for %0, superclass of %1">;
def err_forward_superclass : Error<
  "attempting to use the forward class %0 as superclass of %1">;
def err_no_nsconstant_string_class : Error<
  "cannot find interface declaration for %0">;
def err_recursive_superclass : Error<
  "trying to recursively use %0 as superclass of %1">;
def err_conflicting_aliasing_type : Error<"conflicting types for alias %0">;
def warn_undef_interface : Warning<"找不到%0的接口声明 ">;
def warn_duplicate_protocol_def : Warning<
  "忽略了%0的重复协议定义 ">,
  InGroup<DiagGroup<"duplicate-protocol">>;
def err_protocol_has_circular_dependency : Error<
  "protocol has circular dependency">;
def err_undeclared_protocol : Error<"cannot find protocol declaration for %0">;
def warn_undef_protocolref : Warning<"找不到%0的协议定义 ">;
def err_atprotocol_protocol : Error<
  "@protocol is using a forward protocol declaration of %0">;
def warn_readonly_property : Warning<
  "属性%0的属性'readonly' "
  "限制从%1继承的属性的属性'readwrite'">,
  InGroup<PropertyAttr>;

def warn_property_attribute : Warning<
  "'%1'属性在属性%0上,与从%2继承的属性不匹配">,
  InGroup<PropertyAttr>;
def warn_property_types_are_incompatible : Warning<
  "属性类型%0与类型%1不兼容,是从%2继承的 ">,
  InGroup<DiagGroup<"incompatible-property-type">>;
def warn_protocol_property_mismatch : Warning<
  "属性 %select{of type %1|with attribute '%1'|without attribute '%1'|with "
  "getter %1|with setter %1}0 被选中进行合成">,
  InGroup<DiagGroup<"protocol-property-synthesis-ambiguity">>;
def err_protocol_property_mismatch: Error<warn_protocol_property_mismatch.Text>;
def err_undef_interface : Error<"cannot find interface declaration for %0">;
def err_category_forward_interface : Error<
  "cannot define %select{category|class extension}0 for undefined class %1">;
def err_class_extension_after_impl : Error<
  "cannot declare class extension for %0 after class implementation">;
def note_implementation_declared : Note<
  "class implementation is declared here">;
def note_while_in_implementation : Note<
  "detected while default synthesizing properties in class implementation">;
def note_class_declared : Note<
  "class is declared here">;
def note_receiver_class_declared : Note<
  "receiver is instance of class declared here">;
def note_receiver_expr_here : Note<
  "receiver expression is here">;
def note_receiver_is_id : Note<
  "receiver is treated with 'id' type for purpose of method lookup">;
def note_suppressed_class_declare : Note<
  "class with specified objc_requires_property_definitions attribute is declared here">;
def err_objc_root_class_subclass : Error<
  "objc_root_class attribute may only be specified on a root class declaration">;
def err_restricted_superclass_mismatch : Error<
  "cannot subclass a class that was declared with the "
  "'objc_subclassing_restricted' attribute">;
def err_class_stub_subclassing_mismatch : Error<
  "'objc_class_stub' attribute cannot be specified on a class that does not "
  "have the 'objc_subclassing_restricted' attribute">;
def err_implementation_of_class_stub : Error<
  "cannot declare implementation of a class declared with the "
  "'objc_class_stub' attribute">;
def warn_objc_root_class_missing : Warning<
  "类 %0 定义了,未指定基类">,
  InGroup<ObjCRootClass>;
def err_objc_runtime_visible_category : Error<
  "cannot implement a category for class %0 that is only visible via the "
  "Objective-C runtime">;
def err_objc_runtime_visible_subclass : Error<
  "cannot implement subclass %0 of a superclass %1 that is only visible via the "
  "Objective-C runtime">;
def note_objc_needs_superclass : Note<
  "add a super class to fix this problem">;
def err_conflicting_super_class : Error<"conflicting super class name %0">;
def err_dup_implementation_class : Error<"reimplementation of class %0">;
def err_dup_implementation_category : Error<
  "reimplementation of category %1 for class %0">;
def err_conflicting_ivar_type : Error<
  "instance variable %0 has conflicting type%diff{: $ vs $|}1,2">;
def err_duplicate_ivar_declaration : Error<
  "instance variable is already declared">;
def warn_on_superclass_use : Warning<
  "类实现可能没有父类">;
def err_conflicting_ivar_bitwidth : Error<
  "instance variable %0 has conflicting bit-field width">;
def err_conflicting_ivar_name : Error<
  "conflicting instance variable names: %0 vs %1">;
def err_inconsistent_ivar_count : Error<
  "inconsistent number of instance variables specified">;
def warn_undef_method_impl : Warning<"未找到 %0 的方法定义">,
  InGroup<DiagGroup<"incomplete-implementation">>;
def warn_objc_boxing_invalid_utf8_string : Warning<
  "字符串格式错误为 UTF-8，并且在装箱时将变为一个空的%0">,
  InGroup<ObjCBoxing>;

def err_objc_non_runtime_protocol_in_protocol_expr : Error<
  "cannot use a protocol declared 'objc_non_runtime_protocol' in a @protocol expression">;
def err_objc_direct_on_protocol : Error<
  "'objc_direct' attribute cannot be applied to %select{methods|properties}0 "
  "declared in an Objective-C protocol">;
def err_objc_direct_duplicate_decl : Error<
  "%select{|direct }0%select{method|property}1 declaration conflicts "
  "with previous %select{|direct }2declaration of %select{method|property}1 %3">;
def err_objc_direct_impl_decl_mismatch : Error<
  "direct method was declared in %select{the primary interface|an extension|a category}0 "
  "but is implemented in %select{the primary interface|a category|a different category}1">;
def err_objc_direct_missing_on_decl : Error<
  "direct method implementation was previously declared not direct">;
def err_objc_direct_on_override : Error<
  "methods that %select{override superclass methods|implement protocol requirements}0 cannot be direct">;
def err_objc_override_direct_method : Error<
  "cannot override a method that is declared direct by a superclass">;
def warn_objc_direct_ignored : Warning<
  "此 Objective-C 运行时未实现 %0 属性">,
  InGroup<IgnoredAttributes>;
def warn_objc_direct_property_ignored : Warning<
  "属性%0上的直接属性被忽略（此 Objective-C 运行时未实现）">,
  InGroup<IgnoredAttributes>;
def err_objc_direct_dynamic_property : Error<
  "direct property cannot be @dynamic">;
def err_objc_direct_protocol_conformance : Error<
  "%select{category %1|class extension}0 cannot conform to protocol %2 because "
  "of direct members declared in interface %3">;
def note_direct_member_here : Note<"direct member declared here">;

def warn_conflicting_overriding_ret_types : Warning<
  "返回类型冲突 "
  "在声明 %0%diff{: $ vs $|}1,2中">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_conflicting_ret_types : Warning<
  "返回类型冲突 "
  "在实现 %0%diff{: $ vs $|}1,2中">,
  InGroup<MismatchedReturnTypes>;

def warn_conflicting_overriding_ret_type_modifiers : Warning<
  "%0的声明中 "
  "返回类型的分布式对象修饰符冲突">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_conflicting_ret_type_modifiers : Warning<
  "%0的实现中 "
  "返回类型的分布式对象修饰符冲突">,
  InGroup<DistributedObjectModifiers>;

def warn_non_covariant_overriding_ret_types : Warning<
  "%0的声明中 "
  "存在冲突的返回类型: %1 vs %2">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_non_covariant_ret_types : Warning<
  "%0的实现中 "
  "存在冲突的返回类型: %1 vs %2">,
  InGroup<MethodSignatures>, DefaultIgnore;

def warn_conflicting_overriding_param_types : Warning<
  "参数类型冲突 "
  "在%0%diff{: $ vs $|}1,2的声明中">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_conflicting_param_types : Warning<
  "参数类型冲突 "
  "在%0%diff{: $ vs $|}1,2的实现中">,
  InGroup<MismatchedParameterTypes>;

def warn_conflicting_param_modifiers : Warning<
  "%0的实现中的参数类型上 "
  "存在冲突的分布式对象修饰符">,
  InGroup<DistributedObjectModifiers>;

def warn_conflicting_overriding_param_modifiers : Warning<
  "%0的声明中的参数类型上  "
  "存在冲突的分布式对象修饰符">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_non_contravariant_overriding_param_types : Warning<
  "冲突的参数类型 "
  "在%0的声明中: %1 vs %2">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_non_contravariant_param_types : Warning<
  "冲突的参数类型 "
  "在%0的实现中: %1 vs %2">,
  InGroup<MethodSignatures>, DefaultIgnore;

def warn_conflicting_overriding_variadic :Warning<
  "冲突的方法的可变声明 "
  "及其实现">,
  InGroup<OverridingMethodMismatch>, DefaultIgnore;

def warn_conflicting_variadic :Warning<
  "冲突的方法的可变声明 "
  "及其实现">;

def warn_category_method_impl_match:Warning<
  "类别正在实现一个方法，"
  "该方法也将由其主类实现">, InGroup<ObjCProtocolMethodImpl>;

def warn_implements_nscopying : Warning<
"实现 NSCopying 协议的属性 %0 上的默认分配属性 "
"不适用于 -fobjc-gc[-only]">;

def warn_multiple_method_decl : Warning<"找到多个名为 %0 的方法">,
  InGroup<ObjCMultipleMethodNames>;
def warn_strict_multiple_method_decl : Warning<
  "找到多个名为 %0 的方法">, InGroup<StrictSelector>, DefaultIgnore;
def warn_accessor_property_type_mismatch : Warning<
  "属性%0的类型与访问器 %1 的类型不匹配">;
def note_conv_function_declared_at : Note<"type conversion function declared here">;
def note_method_declared_at : Note<"method %0 declared here">;
def note_direct_method_declared_at : Note<"direct method %0 declared here">;
def note_property_attribute : Note<"property %0 is declared "
  "%select{deprecated|unavailable|partial}1 here">;
def err_setter_type_void : Error<"type of setter must be void">;
def err_duplicate_method_decl : Error<"duplicate declaration of method %0">;
def warn_duplicate_method_decl :
  Warning<"发现并忽略了方法 %0 的多个声明">,
  InGroup<MethodDuplicate>, DefaultIgnore;
def warn_objc_cdirective_format_string :
  Warning<"使用 %0 指令在 %select{NSString|CFString}1中 "
          "该指令作为格式化参数传递给格式化 "
          "%select{method|CFfunction}2">,
  InGroup<ObjCCStringFormat>, DefaultIgnore;
def err_objc_var_decl_inclass :
    Error<"cannot declare variable inside @interface or @protocol">;
def err_missing_method_context : Error<
  "missing context for method declaration">;
def err_objc_property_attr_mutually_exclusive : Error<
  "property attributes '%0' and '%1' are mutually exclusive">;
def err_objc_property_requires_object : Error<
  "property with '%0' attribute must be of object type">;
def warn_objc_property_assign_on_object : Warning<
  "对象类型的'assign'属性可能成为悬空引用;考虑使用'unsafe_unretained'">,
  InGroup<ObjCPropertyAssignOnObjectType>, DefaultIgnore;
def warn_objc_property_no_assignment_attribute : Warning<
  "没有指定'assign', 'retain', or 'copy' 属性 - "
  "假定了'assign' ">,
  InGroup<ObjCPropertyNoAttribute>;
def warn_objc_isa_use : Warning<
  "不推荐直接访问 Objective-C 的 isa，取而代之的是 "
  "object_getClass()">, InGroup<DeprecatedObjCIsaUsage>;
def warn_objc_isa_assign : Warning<
  "不推荐对 Objective-C 的 isa 赋值，取而代之的是 "
  "object_setClass()">, InGroup<DeprecatedObjCIsaUsage>;
def warn_objc_pointer_masking : Warning<
  "强烈建议不要使用位掩码来内省 "
  "Objective-C对象指针">,
  InGroup<ObjCPointerIntrospect>;
def warn_objc_pointer_masking_performSelector : Warning<warn_objc_pointer_masking.Text>,
  InGroup<ObjCPointerIntrospectPerformSelector>;
def warn_objc_property_default_assign_on_object : Warning<
  "默认属性属性'assign'不适用于对象">,
  InGroup<ObjCPropertyNoAttribute>;
def warn_property_attr_mismatch : Warning<
  "类扩展中的属性属性与主类不匹配">,
  InGroup<PropertyAttr>;
def warn_property_implicitly_mismatched : Warning <
  "主属性声明是隐式强的 "
  "而类扩展中的重新声明是弱的">,
  InGroup<DiagGroup<"objc-property-implicit-mismatch">>;
def warn_objc_property_copy_missing_on_block : Warning<
    "必须为块属性指定'copy'属性  "
    "当指定了-fobjc-gc-only时">;
def warn_objc_property_retain_of_block : Warning<
    "retain'ed block属性不复制块 "
    "-请改用copy属性">, InGroup<ObjCRetainBlockProperty>;
def warn_objc_readonly_property_has_setter : Warning<
    "无法为只读属性指定 setter">,
    InGroup<ObjCReadonlyPropertyHasSetter>;
def warn_atomic_property_rule : Warning<
  "可写原子属性 %0 无法将合成的 %select{getter|setter}1 "
  "与用户定义的 %select{getter|setter}2配对">,
  InGroup<DiagGroup<"atomic-property-with-user-defined-accessor">>;
def note_atomic_property_fixup_suggest : Note<"setter and getter must both be "
  "synthesized, or both be user defined,or the property must be nonatomic">;
def err_atomic_property_nontrivial_assign_op : Error<
  "atomic property of reference type %0 cannot have non-trivial assignment"
  " operator">;
def warn_cocoa_naming_owned_rule : Warning<
  "属性遵循 Cocoa 命名约定"
  "以返回'owned' 对象">,
  InGroup<DiagGroup<"objc-property-matches-cocoa-ownership-rule">>;
def err_cocoa_naming_owned_rule : Error<
  "property follows Cocoa naming"
  " convention for returning 'owned' objects">;
def note_cocoa_naming_declare_family : Note<
  "explicitly declare getter %objcinstance0 with '%1' to return an 'unowned' "
  "object">;
def warn_auto_synthesizing_protocol_property :Warning<
  "自动属性合成将不会合成属性 %0"
  "它是在协议 %1 中声明的">,
  InGroup<DiagGroup<"objc-protocol-property-synthesis">>;
def note_add_synthesize_directive : Note<
  "add a '@synthesize' directive">;
def warn_no_autosynthesis_shared_ivar_property : Warning <
  "自动属性合成不会合成属性%0 "
  "因为它无法与其他合成属性共享ivar">,
  InGroup<ObjCNoPropertyAutoSynthesis>;
def warn_no_autosynthesis_property : Warning<
  "自动属性合成不会合成属性%0 "
  "因为它是'readwrite'，但将通过另一个属性"
  "合成为'readonly'">,
  InGroup<ObjCNoPropertyAutoSynthesis>;
def warn_autosynthesis_property_in_superclass : Warning<
  "自动属性合成将不会合成属性%0 "
  "它将由其超类实现"
  "使用@dynamic来确认意图">,
  InGroup<ObjCNoPropertyAutoSynthesis>;
def warn_autosynthesis_property_ivar_match :Warning<
  "自动合成属性%0将使用%select{|synthesized}1 实例变量 "
  "%2, 而不是现有实例变量 %3">,
  InGroup<DiagGroup<"objc-autosynthesis-property-ivar-name-match">>;
def warn_missing_explicit_synthesis : Warning <
  "自动属性合成正在合成未显式合成的属性">,
  InGroup<DiagGroup<"objc-missing-property-synthesis">>, DefaultIgnore;
def warn_property_getter_owning_mismatch : Warning<
  "声明为返回非保留对象的属性"
  "; getter 返回保留对象">;
def warn_property_redecl_getter_mismatch : Warning<
  "属性重新声明 (%1)  "
  "与其原始声明 (%0) 之间的 getter 名称不匹配">, InGroup<PropertyAttr>;
def err_property_setter_ambiguous_use : Error<
  "合成属性 %0 和 %1 都声称 setter %2 - "
  "使用此 setter 将导致意外行为">;
def warn_default_atomic_custom_getter_setter : Warning<
  "原子默认属性 %0 有一个用户定义的 %select{getter|setter}1 "
  "（如果需要，应将属性标记为'atomic' ）">,
  InGroup<CustomAtomic>, DefaultIgnore;
def err_use_continuation_class : Error<
  "illegal redeclaration of property in class extension %0"
  " (attribute must be 'readwrite', while its primary must be 'readonly')">;
def err_type_mismatch_continuation_class : Error<
  "type of property %0 in class extension does not match "
  "property type in primary class">;
def err_use_continuation_class_redeclaration_readwrite : Error<
  "illegal redeclaration of 'readwrite' property in class extension %0"
  " (perhaps you intended this to be a 'readwrite' redeclaration of a "
  "'readonly' public property?)">;
def err_continuation_class : Error<"class extension has no primary class">;
def err_property_type : Error<"property cannot have array or function type %0">;
def err_missing_property_context : Error<
  "missing context for property implementation declaration">;
def err_bad_property_decl : Error<
  "property implementation must have its declaration in interface %0 or one of "
  "its extensions">;
def err_category_property : Error<
  "property declared in category %0 cannot be implemented in "
  "class implementation">;
def note_property_declare : Note<
  "property declared here">;
def note_protocol_property_declare : Note<
  "it could also be property "
  "%select{of type %1|without attribute '%1'|with attribute '%1'|with getter "
  "%1|with setter %1}0 declared here">;
def note_property_synthesize : Note<
  "property synthesized here">;
def err_synthesize_category_decl : Error<
  "@synthesize not allowed in a category's implementation">;
def err_synthesize_on_class_property : Error<
  "@synthesize not allowed on a class property %0">;
def err_missing_property_interface : Error<
  "property implementation in a category with no category declaration">;
def err_bad_category_property_decl : Error<
  "property implementation must have its declaration in the category %0">;
def err_bad_property_context : Error<
  "property implementation must be in a class or category implementation">;
def err_missing_property_ivar_decl : Error<
  "synthesized property %0 must either be named the same as a compatible"
  " instance variable or must explicitly name an instance variable">;
def err_arc_perform_selector_retains : Error<
  "performSelector names a selector which retains the object">;
def warn_arc_perform_selector_leaks : Warning<
  "performSelector可能导致泄漏，因为其选择器未知">,
  InGroup<DiagGroup<"arc-performSelector-leaks">>;
def warn_dealloc_in_category : Warning<
"-正在类别中重写dealloc">,
InGroup<DeallocInCategory>;
def err_gc_weak_property_strong_type : Error<
  "weak attribute declared on a __strong type property in GC mode">;
def warn_arc_repeated_use_of_weak : Warning <
  "弱 %select{variable|property|implicit property|instance variable}0 %1 "
  "被访问多次,在此 %select{function|method|block|lambda}2中 "
  "但可能会意外设置为零;指定给强变量以保持对象处于 "
  "活动状态">,
  InGroup<ARCRepeatedUseOfWeak>, DefaultIgnore;
def warn_implicitly_retains_self : Warning <
  "块隐式保留'self'; "
  "明确提及'self'以表明这是预期行为">,
  InGroup<DiagGroup<"implicit-retain-self">>, DefaultIgnore;
def warn_arc_possible_repeated_use_of_weak : Warning <
  "弱 %select{variable|property|implicit property|instance variable}0 %1 可能 "
  "被访问多次,在此 %select{function|method|block|lambda}2 "
  "并且可能会意外地设置为零;分配给一个强变量以使对象保持"
  "活动状态">,
  InGroup<ARCRepeatedUseOfWeakMaybe>, DefaultIgnore;
def note_arc_weak_also_accessed_here : Note<
  "also accessed here">;
def err_incomplete_synthesized_property : Error<
  "cannot synthesize property %0 with incomplete type %1">;

def err_property_ivar_type : Error<
  "type of property %0 (%1) does not match type of instance variable %2 (%3)">;
def err_property_accessor_type : Error<
  "type of property %0 (%1) does not match type of accessor %2 (%3)">;
def err_ivar_in_superclass_use : Error<
  "property %0 attempting to use instance variable %1 declared in super class %2">;
def err_weak_property : Error<
  "existing instance variable %1 for __weak property %0 must be __weak">;
def err_strong_property : Error<
  "existing instance variable %1 for strong property %0 may not be __weak">;
def err_dynamic_property_ivar_decl : Error<
  "dynamic property cannot have instance variable specification">;
def err_duplicate_ivar_use : Error<
  "synthesized properties %0 and %1 both claim instance variable %2">;
def err_property_implemented : Error<"property %0 is already implemented">;
def warn_objc_missing_super_call : Warning<
  "方法可能缺少一个[super %0] 调用">,
  InGroup<ObjCMissingSuperCalls>;
def err_dealloc_bad_result_type : Error<
  "dealloc return type must be correctly specified as 'void' under ARC, "
  "instead of %0">;
def warn_undeclared_selector : Warning<
  "未声明的选择器 %0">, InGroup<UndeclaredSelector>, DefaultIgnore;
def warn_undeclared_selector_with_typo : Warning<
  "未声明的选择器 %0; 你是说 %1吗?">,
  InGroup<UndeclaredSelector>, DefaultIgnore;
def warn_implicit_atomic_property : Warning<
  "默认情况下假定属性是原子的">, InGroup<ImplicitAtomic>, DefaultIgnore;
def note_auto_readonly_iboutlet_fixup_suggest : Note<
  "property should be changed to be readwrite">;
def warn_auto_readonly_iboutlet_property : Warning<
  "自动合成时的只读 IBOutlet 属性 %0 "
  "可能无法与 'nib' 加载器一起正常工作">,
  InGroup<DiagGroup<"readonly-iboutlet-property">>;
def warn_auto_implicit_atomic_property : Warning<
  "自动合成属性时假定属性是原子的 ">,
  InGroup<ImplicitAtomic>, DefaultIgnore;
def warn_unimplemented_selector:  Warning<
  "在这个翻译单元中没有实现选择器 %0 的方法">,
  InGroup<Selector>, DefaultIgnore;
def warn_unimplemented_protocol_method : Warning<
  "协议 %1 中的方法 %0 未实现">, InGroup<Protocol>;
def warn_multiple_selectors: Warning<
  "several methods with selector %0 of mismatched types are found "
  "for the @selector expression">,
  InGroup<SelectorTypeMismatch>, DefaultIgnore;
def err_direct_selector_expression : Error<
  "@selector expression formed with direct selector %0">;
def warn_potentially_direct_selector_expression : Warning<
  "@selector 达式由潜在的直接选择器 %0 组成">,
  InGroup<ObjCPotentiallyDirectSelector>;
def warn_strict_potentially_direct_selector_expression : Warning<
  warn_potentially_direct_selector_expression.Text>,
  InGroup<ObjCStrictPotentiallyDirectSelector>, DefaultIgnore;

def err_objc_kindof_nonobject : Error<
  "'__kindof' specifier cannot be applied to non-object type %0">;
def err_objc_kindof_wrong_position : Error<
  "'__kindof' type specifier must precede the declarator">;

def err_objc_method_unsupported_param_ret_type : Error<
  "%0 %select{parameter|return}1 type is unsupported; "
  "support for vector types for this target is introduced in %2">;

def warn_messaging_unqualified_id : Warning<
  "消息不合格 id">, DefaultIgnore,
  InGroup<DiagGroup<"objc-messaging-id">>;
def err_messaging_unqualified_id_with_direct_method : Error<
  "messaging unqualified id with a method that is possibly direct">;
def err_messaging_super_with_direct_method : Error<
  "messaging super with a direct method">;
def err_messaging_class_with_direct_method : Error<
  "messaging a Class with a method that is possibly direct">;

// C++ declarations
def err_static_assert_expression_is_not_constant : Error<
  "static_assert expression is not an integral constant expression">;
def err_static_assert_failed : Error<"static_assert failed%select{ %1|}0">;
def err_static_assert_requirement_failed : Error<
  "static_assert failed due to requirement '%0'%select{ %2|}1">;

def ext_inline_variable : ExtWarn<
  "inline variables are a C++17 extension">, InGroup<CXX17>;
def warn_cxx14_compat_inline_variable : Warning<
  "内联变量与 C++17 之前的 C++ 标准不兼容">,
  DefaultIgnore, InGroup<CXXPre17Compat>;

def warn_inline_namespace_reopened_noninline : Warning<
  "内联命名空间作为非内联命名空间重新打开">,
  InGroup<InlineNamespaceReopenedNoninline>;
def err_inline_namespace_mismatch : Error<
  "non-inline namespace cannot be reopened as inline">;

def err_unexpected_friend : Error<
  "friends can only be classes or functions">;
def ext_enum_friend : ExtWarn<
  "befriending enumeration type %0 is a C++11 extension">, InGroup<CXX11>;
def warn_cxx98_compat_enum_friend : Warning<
  "友好枚举类型 %0 与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def ext_nonclass_type_friend : ExtWarn<
  "non-class friend type %0 is a C++11 extension">, InGroup<CXX11>;
def warn_cxx98_compat_nonclass_type_friend : Warning<
  "非类友元类型 %0 与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_friend_is_member : Error<
  "friends cannot be members of the declaring class">;
def warn_cxx98_compat_friend_is_member : Warning<
  "命名声明类成员的友元声明不兼容 "
  "with C++98">, InGroup<CXX98Compat>, DefaultIgnore;
def ext_unelaborated_friend_type : ExtWarn<
  "unelaborated friend declaration is a C++11 extension; specify "
  "'%select{struct|interface|union|class|enum}0' to befriend %1">,
  InGroup<CXX11>;
def warn_cxx98_compat_unelaborated_friend_type : Warning<
  "交友 %1 它没有 '%select{struct|interface|union|class|enum}0' "
  "关键字, 与C++98不兼容">, InGroup<CXX98Compat>, DefaultIgnore;
def err_qualified_friend_no_match : Error<
  "friend declaration of %0 does not match any declaration in %1">;
def err_introducing_special_friend : Error<
  "%plural{[0,2]:must use a qualified name when declaring|3:cannot declare}0"
  " a %select{constructor|destructor|conversion operator|deduction guide}0 "
  "as a friend">;
def err_tagless_friend_type_template : Error<
  "friend type templates must use an elaborated type">;
def err_no_matching_local_friend : Error<
  "no matching function found in local scope">;
def err_no_matching_local_friend_suggest : Error<
  "no matching function %0 found in local scope; did you mean %3?">;
def err_partial_specialization_friend : Error<
  "partial specialization cannot be declared as a friend">;
def err_qualified_friend_def : Error<
  "friend function definition cannot be qualified with '%0'">;
def err_friend_def_in_local_class : Error<
  "friend function cannot be defined in a local class">;
def err_friend_not_first_in_declaration : Error<
  "'friend' must appear first in a non-function declaration">;
def err_using_decl_friend : Error<
  "cannot befriend target of using declaration">;
def warn_template_qualified_friend_unsupported : Warning<
  "不支持友元类声明的依赖嵌套名称说明符'%0'; "
  "在关闭%1的访问控制">,
  InGroup<UnsupportedFriend>;
def warn_template_qualified_friend_ignored : Warning<
  "不支持友元模板声明的依赖嵌套名称说明符'%0'; "
  "忽略此好友声明">,
  InGroup<UnsupportedFriend>;
def ext_friend_tag_redecl_outside_namespace : ExtWarn<
  "unqualified friend declaration referring to type outside of the nearest "
  "enclosing namespace is a Microsoft extension; add a nested name specifier">,
  InGroup<MicrosoftUnqualifiedFriend>;
def err_pure_friend : Error<"friend declaration cannot have a pure-specifier">;

def err_invalid_base_in_interface : Error<
  "interface type cannot inherit from "
  "%select{struct|non-public interface|class}0 %1">;

def err_abstract_type_in_decl : Error<
  "%select{return|parameter|variable|field|instance variable|"
  "synthesized instance variable}0 type %1 is an abstract class">;
def err_allocation_of_abstract_type : Error<
  "allocating an object of abstract class type %0">;
def err_throw_abstract_type : Error<
  "cannot throw an object of abstract type %0">;
def err_array_of_abstract_type : Error<"array of abstract class type %0">;
def err_capture_of_abstract_type : Error<
  "by-copy capture of value of abstract type %0">;
def err_capture_of_incomplete_or_sizeless_type : Error<
  "by-copy capture of variable %0 with %select{incomplete|sizeless}1 type %2">;
def err_capture_default_non_local : Error<
  "non-local lambda expression cannot have a capture-default">;

def err_multiple_final_overriders : Error<
  "virtual function %q0 has more than one final overrider in %1">;
def note_final_overrider : Note<"final overrider of %q0 in %1">;

def err_type_defined_in_type_specifier : Error<
  "%0 cannot be defined in a type specifier">;
def err_type_defined_in_result_type : Error<
  "%0 cannot be defined in the result type of a function">;
def err_type_defined_in_param_type : Error<
  "%0 cannot be defined in a parameter type">;
def err_type_defined_in_alias_template : Error<
  "%0 cannot be defined in a type alias template">;
def err_type_defined_in_condition : Error<
  "%0 cannot be defined in a condition">;
def err_type_defined_in_enum : Error<
  "%0 cannot be defined in an enumeration">;

def note_pure_virtual_function : Note<
  "unimplemented pure virtual method %0 in %1">;

def note_pure_qualified_call_kext : Note<
  "qualified call to %0::%1 is treated as a virtual call to %1 due to -fapple-kext">;

def err_deleted_decl_not_first : Error<
  "deleted definition must be first declaration">;

def err_deleted_override : Error<
  "deleted function %0 cannot override a non-deleted function">;
def err_non_deleted_override : Error<
  "non-deleted function %0 cannot override a deleted function">;
def err_consteval_override : Error<
  "consteval function %0 cannot override a non-consteval function">;
def err_non_consteval_override : Error<
  "non-consteval function %0 cannot override a consteval function">;

def warn_weak_vtable : Warning<
  "%0没有离线虚拟方法定义; "
  "其vtable将在每个翻译单元中发出">,
  InGroup<DiagGroup<"weak-vtables">>, DefaultIgnore;
def warn_weak_template_vtable : Warning<
  "显式模板实例化%0将在每个转换单元中 "
  "发出vtable">,
  InGroup<DiagGroup<"weak-template-vtables">>, DefaultIgnore;

def ext_using_undefined_std : ExtWarn<
  "using directive refers to implicitly-defined namespace 'std'">;

// C++ exception specifications
def err_exception_spec_in_typedef : Error<
  "exception specifications are not allowed in %select{typedefs|type aliases}0">;
def err_distant_exception_spec : Error<
  "exception specifications are not allowed beyond a single level "
  "of indirection">;
def err_incomplete_in_exception_spec : Error<
  "%select{|pointer to |reference to }0incomplete type %1 is not allowed "
  "in exception specification">;
def err_sizeless_in_exception_spec : Error<
  "%select{|reference to }0sizeless type %1 is not allowed "
  "in exception specification">;
def ext_incomplete_in_exception_spec : ExtWarn<err_incomplete_in_exception_spec.Text>,
  InGroup<MicrosoftExceptionSpec>;
def err_rref_in_exception_spec : Error<
  "rvalue reference type %0 is not allowed in exception specification">;
def err_mismatched_exception_spec : Error<
  "exception specification in declaration does not match previous declaration">;
def ext_mismatched_exception_spec : ExtWarn<err_mismatched_exception_spec.Text>,
  InGroup<MicrosoftExceptionSpec>;
def err_override_exception_spec : Error<
  "exception specification of overriding function is more lax than "
  "base version">;
def ext_override_exception_spec : ExtWarn<err_override_exception_spec.Text>,
  InGroup<MicrosoftExceptionSpec>;
def err_incompatible_exception_specs : Error<
  "target exception specification is not superset of source">;
def warn_incompatible_exception_specs : Warning<
  err_incompatible_exception_specs.Text>, InGroup<IncompatibleExceptionSpec>;
def err_deep_exception_specs_differ : Error<
  "exception specifications of %select{return|argument}0 types differ">;
def warn_deep_exception_specs_differ : Warning<
  err_deep_exception_specs_differ.Text>, InGroup<IncompatibleExceptionSpec>;
def err_missing_exception_specification : Error<
  "%0 is missing exception specification '%1'">;
def ext_missing_exception_specification : ExtWarn<
  err_missing_exception_specification.Text>,
  InGroup<DiagGroup<"missing-exception-spec">>;
def ext_ms_missing_exception_specification : ExtWarn<
  err_missing_exception_specification.Text>,
  InGroup<MicrosoftExceptionSpec>;
def err_noexcept_needs_constant_expression : Error<
  "argument to noexcept specifier must be a constant expression">;
def err_exception_spec_not_parsed : Error<
  "exception specification is not available until end of class definition">;
def err_exception_spec_cycle : Error<
  "exception specification of %0 uses itself">;
def err_exception_spec_incomplete_type : Error<
  "exception specification needed for member of incomplete class %0">;
def warn_wasm_dynamic_exception_spec_ignored : ExtWarn<
  "dynamic exception specifications with types are currently ignored in wasm">,
  InGroup<WebAssemblyExceptionSpec>;

// C++ access checking
def err_class_redeclared_with_different_access : Error<
  "%0 redeclared with '%1' access">;
def err_access : Error<
  "%1 is a %select{private|protected}0 member of %3">, AccessControl;
def ext_ms_using_declaration_inaccessible : ExtWarn<
  "using declaration referring to inaccessible member '%0' (which refers "
  "to accessible member '%1') is a Microsoft compatibility extension">,
    AccessControl, InGroup<MicrosoftUsingDecl>;
def err_access_ctor : Error<
  "calling a %select{private|protected}0 constructor of class %2">,
  AccessControl;
def ext_rvalue_to_reference_access_ctor : Extension<
  "C++98 requires an accessible copy constructor for class %2 when binding "
  "a reference to a temporary; was %select{private|protected}0">,
  AccessControl, InGroup<BindToTemporaryCopy>;
def err_access_base_ctor : Error<
  // The ERRORs represent other special members that aren't constructors, in
  // hopes that someone will bother noticing and reporting if they appear
  "%select{base class|inherited virtual base class}0 %1 has %select{private|"
  "protected}3 %select{default |copy |move |*ERROR* |*ERROR* "
  "|*ERROR*|}2constructor">, AccessControl;
def err_access_field_ctor : Error<
  // The ERRORs represent other special members that aren't constructors, in
  // hopes that someone will bother noticing and reporting if they appear
  "field of type %0 has %select{private|protected}2 "
  "%select{default |copy |move |*ERROR* |*ERROR* |*ERROR* |}1constructor">,
  AccessControl;
def err_access_friend_function : Error<
  "friend function %1 is a %select{private|protected}0 member of %3">,
  AccessControl;

def err_access_dtor : Error<
  "calling a %select{private|protected}1 destructor of class %0">,
  AccessControl;
def err_access_dtor_base :
    Error<"base class %0 has %select{private|protected}1 destructor">,
    AccessControl;
def err_access_dtor_vbase :
    Error<"inherited virtual base class %1 has "
    "%select{private|protected}2 destructor">,
    AccessControl;
def err_access_dtor_temp :
    Error<"temporary of type %0 has %select{private|protected}1 destructor">,
    AccessControl;
def err_access_dtor_exception :
    Error<"exception object of type %0 has %select{private|protected}1 "
          "destructor">, AccessControl;
def err_access_dtor_field :
    Error<"field of type %1 has %select{private|protected}2 destructor">,
    AccessControl;
def err_access_dtor_var :
    Error<"variable of type %1 has %select{private|protected}2 destructor">,
    AccessControl;
def err_access_dtor_ivar :
    Error<"instance variable of type %0 has %select{private|protected}1 "
          "destructor">,
    AccessControl;
def note_previous_access_declaration : Note<
  "previously declared '%1' here">;
def note_access_natural : Note<
  "%select{|implicitly }1declared %select{private|protected}0 here">;
def note_access_constrained_by_path : Note<
  "constrained by %select{|implicitly }1%select{private|protected}0"
  " inheritance here">;
def note_access_protected_restricted_noobject : Note<
  "must name member using the type of the current context %0">;
def note_access_protected_restricted_ctordtor : Note<
  "protected %select{constructor|destructor}0 can only be used to "
  "%select{construct|destroy}0 a base class subobject">;
def note_access_protected_restricted_object : Note<
  "can only access this member on an object of type %0">;
def warn_cxx98_compat_sfinae_access_control : Warning<
  "由于访问控制导致的替换失败与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore, NoSFINAE;

// C++ name lookup
def err_incomplete_nested_name_spec : Error<
  "incomplete type %0 named in nested name specifier">;
def err_dependent_nested_name_spec : Error<
  "nested name specifier for a declaration cannot depend on a template "
  "parameter">;
def err_nested_name_member_ref_lookup_ambiguous : Error<
  "lookup of %0 in member access expression is ambiguous">;
def ext_nested_name_member_ref_lookup_ambiguous : ExtWarn<
  "lookup of %0 in member access expression is ambiguous; using member of %1">,
  InGroup<AmbigMemberTemplate>;
def note_ambig_member_ref_object_type : Note<
  "lookup in the object type %0 refers here">;
def note_ambig_member_ref_scope : Note<
  "lookup from the current scope refers here">;
def err_qualified_member_nonclass : Error<
  "qualified member access refers to a member in %0">;
def err_incomplete_member_access : Error<
  "member access into incomplete type %0">;
def err_incomplete_type : Error<
  "incomplete type %0 where a complete type is required">;
def warn_cxx98_compat_enum_nested_name_spec : Warning<
  "嵌套名称说明符中的枚举类型与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_nested_name_spec_is_not_class : Error<
  "%0 cannot appear before '::' because it is not a class"
  "%select{ or namespace|, namespace, or enumeration}1; did you mean ':'?">;
def ext_nested_name_spec_is_enum : ExtWarn<
  "use of enumeration in a nested name specifier is a C++11 extension">,
  InGroup<CXX11>;
def err_out_of_line_qualified_id_type_names_constructor : Error<
  "qualified reference to %0 is a constructor name rather than a "
  "%select{template name|type}1 in this context">;
def ext_out_of_line_qualified_id_type_names_constructor : ExtWarn<
  "ISO C++ specifies that "
  "qualified reference to %0 is a constructor name rather than a "
  "%select{template name|type}1 in this context, despite preceding "
  "%select{'typename'|'template'}2 keyword">, SFINAEFailure,
  InGroup<DiagGroup<"injected-class-name">>;

// C++ class members
def err_storageclass_invalid_for_member : Error<
  "storage class specified for a member declaration">;
def err_mutable_function : Error<"'mutable' cannot be applied to functions">;
def err_mutable_reference : Error<"'mutable' cannot be applied to references">;
def ext_mutable_reference : ExtWarn<
  "'mutable' on a reference type is a Microsoft extension">,
  InGroup<MicrosoftMutableReference>;
def err_mutable_const : Error<"'mutable' and 'const' cannot be mixed">;
def err_mutable_nonmember : Error<
  "'mutable' can only be applied to member variables">;
def err_virtual_in_union : Error<
  "unions cannot have virtual functions">;
def err_virtual_non_function : Error<
  "'virtual' can only appear on non-static member functions">;
def err_virtual_out_of_class : Error<
  "'virtual' can only be specified inside the class definition">;
def err_virtual_member_function_template : Error<
  "'virtual' cannot be specified on member function templates">;
def err_static_overrides_virtual : Error<
  "'static' member function %0 overrides a virtual function in a base class">;
def err_explicit_non_function : Error<
  "'explicit' can only appear on non-static member functions">;
def err_explicit_out_of_class : Error<
  "'explicit' can only be specified inside the class definition">;
def err_explicit_non_ctor_or_conv_function : Error<
  "'explicit' can only be applied to a constructor or conversion function">;
def err_static_not_bitfield : Error<"static member %0 cannot be a bit-field">;
def err_static_out_of_line : Error<
  "'static' can only be specified inside the class definition">;
def ext_static_out_of_line : ExtWarn<
  err_static_out_of_line.Text>,
  InGroup<MicrosoftTemplate>;
def err_storage_class_for_static_member : Error<
  "static data member definition cannot specify a storage class">;
def err_typedef_not_bitfield : Error<"typedef member %0 cannot be a bit-field">;
def err_not_integral_type_bitfield : Error<
  "bit-field %0 has non-integral type %1">;
def err_not_integral_type_anon_bitfield : Error<
  "anonymous bit-field has non-integral type %0">;
def err_anon_bitfield_qualifiers : Error<
  "anonymous bit-field cannot have qualifiers">;
def err_member_function_initialization : Error<
  "initializer on function does not look like a pure-specifier">;
def err_non_virtual_pure : Error<
  "%0 is not virtual and cannot be declared pure">;
def ext_pure_function_definition : ExtWarn<
  "function definition with pure-specifier is a Microsoft extension">,
  InGroup<MicrosoftPureDefinition>;
def err_qualified_member_of_unrelated : Error<
  "%q0 is not a member of class %1">;

def err_member_function_call_bad_cvr : Error<
  "'this' argument to member function %0 has type %1, but function is not marked "
  "%select{const|restrict|const or restrict|volatile|const or volatile|"
  "volatile or restrict|const, volatile, or restrict}2">;
def err_member_function_call_bad_ref : Error<
  "'this' argument to member function %0 is an %select{lvalue|rvalue}1, "
  "but function has %select{non-const lvalue|rvalue}2 ref-qualifier">;
def err_member_function_call_bad_type : Error<
  "cannot initialize object parameter of type %0 with an expression "
  "of type %1">;

def warn_call_to_pure_virtual_member_function_from_ctor_dtor : Warning<
  "对纯虚成员函数 %0 的调用具有未定义的行为; "
  "无法覆盖子类中的 %0 "
  "在%select{constructor|destructor}1 中,它属于%2">, InGroup<PureVirtualCallFromCtorDtor>;

def select_special_member_kind : TextSubstitution<
  "%select{default constructor|copy constructor|move constructor|"
  "copy assignment operator|move assignment operator|destructor}0">;

def note_member_declared_at : Note<"member is declared here">;
def note_ivar_decl : Note<"instance variable is declared here">;
def note_bitfield_decl : Note<"bit-field is declared here">;
def note_implicit_param_decl : Note<"%0 is an implicit parameter">;
def note_member_synthesized_at : Note<
  "in %select{implicit|defaulted}0 %sub{select_special_member_kind}1 for %2 "
  "first required here">;
def note_comparison_synthesized_at : Note<
  "in defaulted %sub{select_defaulted_comparison_kind}0 for %1 "
  "first required here">;
def err_missing_default_ctor : Error<
  "%select{constructor for %1 must explicitly initialize the|"
  "implicit default constructor for %1 must explicitly initialize the|"
  "cannot use constructor inherited from base class %4;}0 "
  "%select{base class|member}2 %3 %select{which|which|of %1}0 "
  "does not have a default constructor">;
def note_due_to_dllexported_class : Note<
  "due to %0 being dllexported%select{|; try compiling in C++11 mode}1">;

def err_illegal_union_or_anon_struct_member : Error<
  "%select{anonymous struct|union}0 member %1 has a non-trivial "
  "%sub{select_special_member_kind}2">;

def warn_frame_address : Warning<
  "使用非零参数调用'%0'是不安全的">,
  InGroup<FrameAddress>, DefaultIgnore;

def warn_cxx98_compat_nontrivial_union_or_anon_struct_member : Warning<
  "%select{anonymous struct|union}0 成员 %1 具有一个重要第 "
  "%sub{select_special_member_kind}2 与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;

def note_nontrivial_virtual_dtor : Note<
  "destructor for %0 is not trivial because it is virtual">;
def note_nontrivial_has_virtual : Note<
  "because type %0 has a virtual %select{member function|base class}1">;
def note_nontrivial_no_def_ctor : Note<
  "because %select{base class of |field of |}0type %1 has no "
  "default constructor">;
def note_user_declared_ctor : Note<
  "implicit default constructor suppressed by user-declared constructor">;
def note_nontrivial_no_copy : Note<
  "because no %select{<<ERROR>>|constructor|constructor|assignment operator|"
  "assignment operator|<<ERROR>>}2 can be used to "
  "%select{<<ERROR>>|copy|move|copy|move|<<ERROR>>}2 "
  "%select{base class|field|an object}0 of type %3">;
def note_nontrivial_user_provided : Note<
  "because %select{base class of |field of |}0type %1 has a user-provided "
  "%sub{select_special_member_kind}2">;
def note_nontrivial_default_member_init : Note<
  "because field %0 has an initializer">;
def note_nontrivial_param_type : Note<
  "because its parameter is %diff{of type $, not $|of the wrong type}2,3">;
def note_nontrivial_default_arg : Note<"because it has a default argument">;
def note_nontrivial_variadic : Note<"because it is a variadic function">;
def note_nontrivial_subobject : Note<
  "because the function selected to %select{construct|copy|move|copy|move|"
  "destroy}2 %select{base class|field}0 of type %1 is not trivial">;
def note_nontrivial_objc_ownership : Note<
  "because type %0 has a member with %select{no|no|__strong|__weak|"
  "__autoreleasing}1 ownership">;

/// Selector for a TagTypeKind value.
def select_tag_type_kind : TextSubstitution<
  "%select{struct|interface|union|class|enum}0">;

def err_static_data_member_not_allowed_in_anon_struct : Error<
  "static data member %0 not allowed in anonymous "
  "%sub{select_tag_type_kind}1">;
def ext_static_data_member_in_union : ExtWarn<
  "static data member %0 in union is a C++11 extension">, InGroup<CXX11>;
def warn_cxx98_compat_static_data_member_in_union : Warning<
  "联合体中的静态数据成员 %0 与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def ext_union_member_of_reference_type : ExtWarn<
  "union member %0 has reference type %1, which is a Microsoft extension">,
  InGroup<MicrosoftUnionMemberReference>;
def err_union_member_of_reference_type : Error<
  "union member %0 has reference type %1">;
def ext_anonymous_struct_union_qualified : Extension<
  "anonymous %select{struct|union}0 cannot be '%1'">;
def err_different_return_type_for_overriding_virtual_function : Error<
  "virtual function %0 has a different return type "
  "%diff{($) than the function it overrides (which has return type $)|"
  "than the function it overrides}1,2">;
def note_overridden_virtual_function : Note<
  "overridden virtual function is here">;
def err_conflicting_overriding_cc_attributes : Error<
  "virtual function %0 has different calling convention attributes "
  "%diff{($) than the function it overrides (which has calling convention $)|"
  "than the function it overrides}1,2">;
def warn_overriding_method_missing_noescape : Warning<
  "重写方法的参数应该用 "
  "__attribute__((noescape))注释">, InGroup<MissingNoEscape>;
def note_overridden_marked_noescape : Note<
  "parameter of overridden method is annotated with __attribute__((noescape))">;
def note_cat_conform_to_noescape_prot : Note<
  "%select{category|class extension}0 conforms to protocol %1 which defines method %2">;

def err_covariant_return_inaccessible_base : Error<
  "invalid covariant return for virtual function: %1 is a "
  "%select{private|protected}2 base class of %0">, AccessControl;
def err_covariant_return_ambiguous_derived_to_base_conv : Error<
  "return type of virtual function %3 is not covariant with the return type of "
  "the function it overrides (ambiguous conversion from derived class "
  "%0 to base class %1:%2)">;
def err_covariant_return_not_derived : Error<
  "return type of virtual function %0 is not covariant with the return type of "
  "the function it overrides (%1 is not derived from %2)">;
def err_covariant_return_incomplete : Error<
  "return type of virtual function %0 is not covariant with the return type of "
  "the function it overrides (%1 is incomplete)">;
def err_covariant_return_type_different_qualifications : Error<
  "return type of virtual function %0 is not covariant with the return type of "
  "the function it overrides (%1 has different qualifiers than %2)">;
def err_covariant_return_type_class_type_more_qualified : Error<
  "return type of virtual function %0 is not covariant with the return type of "
  "the function it overrides (class type %1 is more qualified than class "
  "type %2">;

// C++ implicit special member functions
def note_in_declaration_of_implicit_special_member : Note<
  "while declaring the implicit %sub{select_special_member_kind}1"
  " for %0">;

// C++ constructors
def err_constructor_cannot_be : Error<"constructor cannot be declared '%0'">;
def err_invalid_qualified_constructor : Error<
  "'%0' qualifier is not allowed on a constructor">;
def err_ref_qualifier_constructor : Error<
  "ref-qualifier '%select{&&|&}0' is not allowed on a constructor">;

def err_constructor_return_type : Error<
  "constructor cannot have a return type">;
def err_constructor_redeclared : Error<"constructor cannot be redeclared">;
def err_constructor_byvalue_arg : Error<
  "copy constructor must pass its first argument by reference">;
def warn_no_constructor_for_refconst : Warning<
  "%select{struct|interface|union|class|enum}0 %1 不声明任何 "
  "构造函数来初始化其不可修改的成员">;
def note_refconst_member_not_initialized : Note<
  "%select{const|reference}0 member %1 will never be initialized">;
def ext_ms_explicit_constructor_call : ExtWarn<
  "explicit constructor calls are a Microsoft extension">,
  InGroup<MicrosoftExplicitConstructorCall>;

// C++ destructors
def err_destructor_not_member : Error<
  "destructor must be a non-static member function">;
def err_destructor_cannot_be : Error<"destructor cannot be declared '%0'">;
def err_invalid_qualified_destructor : Error<
  "'%0' qualifier is not allowed on a destructor">;
def err_ref_qualifier_destructor : Error<
  "ref-qualifier '%select{&&|&}0' is not allowed on a destructor">;
def err_destructor_return_type : Error<"destructor cannot have a return type">;
def err_destructor_redeclared : Error<"destructor cannot be redeclared">;
def err_destructor_with_params : Error<"destructor cannot have any parameters">;
def err_destructor_variadic : Error<"destructor cannot be variadic">;
def ext_destructor_typedef_name : ExtWarn<
  "destructor cannot be declared using a %select{typedef|type alias}1 %0 "
  "of the class name">, DefaultError, InGroup<DiagGroup<"dtor-typedef">>;
def err_undeclared_destructor_name : Error<
  "undeclared identifier %0 in destructor name">;
def err_destructor_name : Error<
  "expected the class name after '~' to name the enclosing class">;
def err_destructor_name_nontype : Error<
  "identifier %0 after '~' in destructor name does not name a type">;
def err_destructor_expr_mismatch : Error<
  "identifier %0 in object destruction expression does not name the type "
  "%1 of the object being destroyed">;
def err_destructor_expr_nontype : Error<
  "identifier %0 in object destruction expression does not name a type">;
def err_destructor_expr_type_mismatch : Error<
  "destructor type %0 in object destruction expression does not match the "
  "type %1 of the object being destroyed">;
def note_destructor_type_here : Note<
  "type %0 found by destructor name lookup">;
def note_destructor_nontype_here : Note<
  "non-type declaration found by destructor name lookup">;
def ext_dtor_named_in_wrong_scope : Extension<
  "ISO C++ requires the name after '::~' to be found in the same scope as "
  "the name before '::~'">, InGroup<DtorName>;
def ext_qualified_dtor_named_in_lexical_scope : ExtWarn<
  "qualified destructor name only found in lexical scope; omit the qualifier "
  "to find this type name by unqualified lookup">, InGroup<DtorName>;
def ext_dtor_name_ambiguous : Extension<
  "ISO C++ considers this destructor name lookup to be ambiguous">,
  InGroup<DtorName>;

def err_destroy_attr_on_non_static_var : Error<
  "%select{no_destroy|always_destroy}0 attribute can only be applied to a"
  " variable with static or thread storage duration">;

def err_destructor_template : Error<
  "destructor cannot be declared as a template">;

// C++ initialization
def err_init_conversion_failed : Error<
  "cannot initialize %select{a variable|a parameter|template parameter|"
  "return object|statement expression result|an "
  "exception object|a member subobject|an array element|a new value|a value|a "
  "base class|a constructor delegation|a vector element|a block element|a "
  "block element|a complex element|a lambda capture|a compound literal "
  "initializer|a related result|a parameter of CF audited function}0 "
  "%diff{of type $ with an %select{rvalue|lvalue}2 of type $|"
  "with an %select{rvalue|lvalue}2 of incompatible type}1,3"
  "%select{|: different classes%diff{ ($ vs $)|}5,6"
  "|: different number of parameters (%5 vs %6)"
  "|: type mismatch at %ordinal5 parameter%diff{ ($ vs $)|}6,7"
  "|: different return type%diff{ ($ vs $)|}5,6"
  "|: different qualifiers (%5 vs %6)"
  "|: different exception specifications}4">;
def note_forward_class_conversion : Note<"%0 is not defined, but forward "
  "declared here; conversion would be valid if it was derived from %1">;

def err_lvalue_to_rvalue_ref : Error<"rvalue reference %diff{to type $ cannot "
  "bind to lvalue of type $|cannot bind to incompatible lvalue}0,1">;
def err_lvalue_reference_bind_to_initlist : Error<
  "%select{non-const|volatile}0 lvalue reference to type %1 cannot bind to an "
  "initializer list temporary">;
def err_lvalue_reference_bind_to_temporary : Error<
  "%select{non-const|volatile}0 lvalue reference %diff{to type $ cannot bind "
  "to a temporary of type $|cannot bind to incompatible temporary}1,2">;
def err_lvalue_reference_bind_to_unrelated : Error<
  "%select{non-const|volatile}0 lvalue reference "
  "%diff{to type $ cannot bind to a value of unrelated type $|"
  "cannot bind to a value of unrelated type}1,2">;
def err_reference_bind_drops_quals : Error<
  "binding reference %diff{of type $ to value of type $|to value}0,1 "
  "%select{drops %3 qualifier%plural{1:|2:|4:|:s}4|changes address space|"
  "not permitted due to incompatible qualifiers}2">;
def err_reference_bind_failed : Error<
  "reference %diff{to %select{type|incomplete type}1 $ could not bind to an "
  "%select{rvalue|lvalue}2 of type $|could not bind to %select{rvalue|lvalue}2 of "
  "incompatible type}0,3">;
def err_reference_bind_temporary_addrspace : Error<
  "reference of type %0 cannot bind to a temporary object because of "
  "address space mismatch">;
def err_reference_bind_init_list : Error<
  "reference to type %0 cannot bind to an initializer list">;
def err_init_list_bad_dest_type : Error<
  "%select{|non-aggregate }0type %1 cannot be initialized with an initializer "
  "list">;
def warn_cxx20_compat_aggregate_init_with_ctors : Warning<
  "使用用户声明的构造函数的类型 %0 的聚合初始化 "
  "与 C++20 不兼容">, DefaultIgnore, InGroup<CXX20Compat>;

def err_reference_bind_to_bitfield : Error<
  "%select{non-const|volatile}0 reference cannot bind to "
  "bit-field%select{| %1}2">;
def err_reference_bind_to_vector_element : Error<
  "%select{non-const|volatile}0 reference cannot bind to vector element">;
def err_reference_bind_to_matrix_element : Error<
  "%select{non-const|volatile}0 reference cannot bind to matrix element">;
def err_reference_var_requires_init : Error<
  "declaration of reference variable %0 requires an initializer">;
def err_reference_without_init : Error<
  "reference to type %0 requires an initializer">;
def note_value_initialization_here : Note<
  "in value-initialization of type %0 here">;
def err_reference_has_multiple_inits : Error<
  "reference cannot be initialized with multiple values">;
def err_init_non_aggr_init_list : Error<
  "initialization of non-aggregate type %0 with an initializer list">;
def err_init_reference_member_uninitialized : Error<
  "reference member of type %0 uninitialized">;
def note_uninit_reference_member : Note<
  "uninitialized reference member is here">;
def warn_field_is_uninit : Warning<"字段 %0 在此处使用时未初始化">,
  InGroup<Uninitialized>;
def warn_base_class_is_uninit : Warning<
  "基类 %0 在此处用于访问 %q1 时未初始化">,
  InGroup<Uninitialized>;
def warn_reference_field_is_uninit : Warning<
  "在此处使用时，引用 %0 尚未绑定到值">,
  InGroup<Uninitialized>;
def note_uninit_in_this_constructor : Note<
  "during field initialization in %select{this|the implicit default}0 "
  "constructor">;
def warn_static_self_reference_in_init : Warning<
  "静态变量 %0 在其自己的初始化中被可疑地使用">,
  InGroup<UninitializedStaticSelfInit>;
def warn_uninit_self_reference_in_init : Warning<
  "变量 %0 在其自己的初始化中使用时未初始化">,
  InGroup<Uninitialized>;
def warn_uninit_self_reference_in_reference_init : Warning<
  "引用 %0 在其自己的初始化中使用时 "
  "尚未绑定到值">,
  InGroup<Uninitialized>;
def warn_uninit_var : Warning<
  "变量 %0 未初始化当 %select{used here|captured by block}1">,
  InGroup<Uninitialized>, DefaultIgnore;
def warn_sometimes_uninit_var : Warning<
  "变量 %0 是 %select{used|captured}1 无论何时都未初始化 "
  "%select{'%3' condition is %select{true|false}4|"
  "'%3' 循环 %select{is entered|exits because its condition is false}4|"
  "'%3' 循环 %select{condition is true|exits because its condition is false}4|"
  "switch %3 is taken|"
  "its declaration is reached|"
  "%3 is called}2">,
  InGroup<UninitializedSometimes>, DefaultIgnore;
def warn_maybe_uninit_var : Warning<
  "变量 %0 可能在以下情况下未初始化 "
  "%select{used here|captured by block}1">,
  InGroup<UninitializedMaybe>, DefaultIgnore;
def note_var_declared_here : Note<"variable %0 is declared here">;
def note_uninit_var_use : Note<
  "%select{uninitialized use occurs|variable is captured by block}0 here">;
def warn_uninit_byref_blockvar_captured_by_block : Warning<
  "块指针变量 %0 是 %select{uninitialized|null}1 当被块 "
  "捕获时">, InGroup<Uninitialized>, DefaultIgnore;
def note_block_var_fixit_add_initialization : Note<
  "did you mean to use __block %0?">;
def note_in_omitted_aggregate_initializer : Note<
  "in implicit initialization of %select{"
  "array element %1 with omitted initializer|"
  "field %1 with omitted initializer|"
  "trailing array elements in runtime-sized array new}0">;
def note_in_reference_temporary_list_initializer : Note<
  "in initialization of temporary of type %0 created to "
  "list-initialize this reference">;
def note_var_fixit_add_initialization : Note<
  "初始化变量 %0 以消除此警告">;
def note_uninit_fixit_remove_cond : Note<
  "remove the %select{'%1' if its condition|condition if it}0 "
  "is always %select{false|true}2">;
def err_init_incomplete_type : Error<"initialization of incomplete type %0">;
def err_list_init_in_parens : Error<
  "cannot initialize %select{non-class|reference}0 type %1 with a "
  "parenthesized initializer list">;

def warn_uninit_const_reference : Warning<
  "变量 %0 在此处作为 const 引用参数传递时 "
  "未初始化">, InGroup<UninitializedConstReference>, DefaultIgnore;

def warn_unsequenced_mod_mod : Warning<
  "对 %0 的多个未排序的修改">, InGroup<Unsequenced>;
def warn_unsequenced_mod_use : Warning<
  "未排序的修改和访问 %0">, InGroup<Unsequenced>;

def select_initialized_entity_kind : TextSubstitution<
  "%select{copying variable|copying parameter|initializing template parameter|"
  "returning object|initializing statement expression result|"
  "throwing object|copying member subobject|copying array element|"
  "allocating object|copying temporary|initializing base subobject|"
  "initializing vector element|capturing value}0">;

def err_temp_copy_no_viable : Error<
  "no viable constructor %sub{select_initialized_entity_kind}0 of type %1">;
def ext_rvalue_to_reference_temp_copy_no_viable : Extension<
  "no viable constructor %sub{select_initialized_entity_kind}0 of type %1; "
  "C++98 requires a copy constructor when binding a reference to a temporary">,
  InGroup<BindToTemporaryCopy>;
def err_temp_copy_ambiguous : Error<
  "ambiguous constructor call when %sub{select_initialized_entity_kind}0 "
  "of type %1">;
def err_temp_copy_deleted : Error<
  "%sub{select_initialized_entity_kind}0 of type %1 "
  "invokes deleted constructor">;
def err_temp_copy_incomplete : Error<
  "copying a temporary object of incomplete type %0">;
def warn_cxx98_compat_temp_copy : Warning<
  "%sub{select_initialized_entity_kind}1 "
  "属于类型 %2 当将引用绑定到临时对象时将会 %select{invoke "
  "an inaccessible constructor|find no viable constructor|find ambiguous "
  "constructors|invoke a deleted constructor}0 在C++98中">,
  InGroup<CXX98CompatBindToTemporaryCopy>, DefaultIgnore;
def err_selected_explicit_constructor : Error<
  "chosen constructor is explicit in copy-initialization">;
def note_explicit_ctor_deduction_guide_here : Note<
  "explicit %select{constructor|deduction guide}0 declared here">;

// C++11 decltype
def err_decltype_in_declarator : Error<
    "'decltype' cannot be used to name a declaration">;

// C++11 auto
def warn_cxx98_compat_auto_type_specifier : Warning<
  "'auto' 类型说明符与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_auto_variable_cannot_appear_in_own_initializer : Error<
  "variable %0 declared with deduced type %1 "
  "cannot appear in its own initializer">;
def err_binding_cannot_appear_in_own_initializer : Error<
  "binding %0 cannot appear in the initializer of its own "
  "decomposition declaration">;
def err_illegal_decl_array_of_auto : Error<
  "'%0' declared as array of %1">;
def err_new_array_of_auto : Error<
  "cannot allocate array of 'auto'">;
def err_auto_not_allowed : Error<
  "%select{'auto'|'decltype(auto)'|'__auto_type'|"
  "use of "
  "%select{class template|function template|variable template|alias template|"
  "template template parameter|concept|template}2 %3 requires template "
  "arguments; argument deduction}0 not allowed "
  "%select{in function prototype"
  "|in non-static struct member|in struct member"
  "|in non-static union member|in union member"
  "|in non-static class member|in interface member"
  "|in exception declaration|in template parameter until C++17|in block literal"
  "|in template argument|in typedef|in type alias|in function return type"
  "|in conversion function type|here|in lambda parameter"
  "|in type allocated by 'new'|in K&R-style function parameter"
  "|in template parameter|in friend declaration|in function prototype that is "
  "not a function declaration|in requires expression parameter}1">;
def err_dependent_deduced_tst : Error<
  "typename specifier refers to "
  "%select{class template|function template|variable template|alias template|"
  "template template parameter|template}0 member in %1; "
  "argument deduction not allowed here">;
def err_deduced_tst : Error<
  "typename specifier refers to "
  "%select{class template|function template|variable template|alias template|"
  "template template parameter|template}0; argument deduction not allowed "
  "here">;
def err_auto_not_allowed_var_inst : Error<
  "'auto' variable template instantiation is not allowed">;
def err_auto_var_requires_init : Error<
  "declaration of variable %0 with deduced type %1 requires an initializer">;
def err_auto_new_requires_ctor_arg : Error<
  "new expression for type %0 requires a constructor argument">;
def ext_auto_new_list_init : Extension<
  "ISO C++ standards before C++17 do not allow new expression for "
  "type %0 to use list-initialization">, InGroup<CXX17>;
def err_auto_var_init_no_expression : Error<
  "initializer for variable %0 with type %1 is empty">;
def err_auto_var_init_multiple_expressions : Error<
  "initializer for variable %0 with type %1 contains multiple expressions">;
def err_auto_var_init_paren_braces : Error<
  "cannot deduce type for variable %1 with type %2 from "
  "%select{parenthesized|nested}0 initializer list">;
def err_auto_new_ctor_multiple_expressions : Error<
  "new expression for type %0 contains multiple constructor arguments">;
def err_auto_missing_trailing_return : Error<
  "'auto' return without trailing return type; deduced return types are a "
  "C++14 extension">;
def err_deduced_return_type : Error<
  "deduced return types are a C++14 extension">;
def err_trailing_return_without_auto : Error<
  "function with trailing return type must specify return type 'auto', not %0">;
def err_trailing_return_in_parens : Error<
  "trailing return type may not be nested within parentheses">;
def err_auto_var_deduction_failure : Error<
  "variable %0 with type %1 has incompatible initializer of type %2">;
def err_auto_var_deduction_failure_from_init_list : Error<
  "cannot deduce actual type for variable %0 with type %1 from initializer list">;
def err_auto_new_deduction_failure : Error<
  "new expression for type %0 has incompatible constructor argument of type %1">;
def err_auto_inconsistent_deduction : Error<
  "deduced conflicting types %diff{($ vs $) |}0,1"
  "for initializer list element type">;
def err_auto_different_deductions : Error<
  "%select{'auto'|'decltype(auto)'|'__auto_type'|template arguments}0 "
  "deduced as %1 in declaration of %2 and "
  "deduced as %3 in declaration of %4">;
def err_auto_non_deduced_not_alone : Error<
  "%select{function with deduced return type|"
  "declaration with trailing return type}0 "
  "must be the only declaration in its group">;
def err_implied_std_initializer_list_not_found : Error<
  "cannot deduce type of initializer list because std::initializer_list was "
  "not found; include <initializer_list>">;
def err_malformed_std_initializer_list : Error<
  "std::initializer_list must be a class template with a single type parameter">;
def err_auto_init_list_from_c : Error<
  "cannot use __auto_type with initializer list in C">;
def err_auto_bitfield : Error<
  "cannot pass bit-field as __auto_type initializer in C">;

// C++1y decltype(auto) type
def err_decltype_auto_invalid : Error<
  "'decltype(auto)' not allowed here">;
def err_decltype_auto_cannot_be_combined : Error<
  "'decltype(auto)' cannot be combined with other type specifiers">;
def err_decltype_auto_function_declarator_not_declaration : Error<
  "'decltype(auto)' can only be used as a return type "
  "in a function declaration">;
def err_decltype_auto_compound_type : Error<
  "cannot form %select{pointer to|reference to|array of}0 'decltype(auto)'">;
def err_decltype_auto_initializer_list : Error<
  "cannot deduce 'decltype(auto)' from initializer list">;

// C++17 deduced class template specialization types
def err_deduced_class_template_compound_type : Error<
  "cannot %select{form pointer to|form reference to|form array of|"
  "form function returning|use parentheses when declaring variable with}0 "
  "deduced class template specialization type">;
def err_deduced_non_class_template_specialization_type : Error<
  "%select{<error>|function template|variable template|alias template|"
  "template template parameter|concept|template}0 %1 requires template "
  "arguments; argument deduction only allowed for class templates">;
def err_deduced_class_template_ctor_ambiguous : Error<
  "ambiguous deduction for template arguments of %0">;
def err_deduced_class_template_ctor_no_viable : Error<
  "no viable constructor or deduction guide for deduction of "
  "template arguments of %0">;
def err_deduced_class_template_incomplete : Error<
  "template %0 has no definition and no %select{|viable }1deduction guides "
  "for deduction of template arguments">;
def err_deduced_class_template_deleted : Error<
  "class template argument deduction for %0 selected a deleted constructor">;
def err_deduced_class_template_explicit : Error<
  "class template argument deduction for %0 selected an explicit "
  "%select{constructor|deduction guide}1 for copy-list-initialization">;
def err_deduction_guide_no_trailing_return_type : Error<
  "deduction guide declaration without trailing return type">;
def err_deduction_guide_bad_trailing_return_type : Error<
  "deduced type %1 of deduction guide is not %select{|written as }2"
  "a specialization of template %0">;
def err_deduction_guide_with_complex_decl : Error<
  "cannot specify any part of a return type in the "
  "declaration of a deduction guide">;
def err_deduction_guide_invalid_specifier : Error<
  "deduction guide cannot be declared '%0'">;
def err_deduction_guide_name_not_class_template : Error<
  "cannot specify deduction guide for "
  "%select{<error>|function template|variable template|alias template|"
  "template template parameter|concept|dependent template name}0 %1">;
def err_deduction_guide_wrong_scope : Error<
  "deduction guide must be declared in the same scope as template %q0">;
def err_deduction_guide_defines_function : Error<
  "deduction guide cannot have a function definition">;
def err_deduction_guide_redeclared : Error<
  "redeclaration of deduction guide">;
def err_deduction_guide_specialized : Error<"deduction guide cannot be "
  "%select{explicitly instantiated|explicitly specialized}0">;
def err_deduction_guide_template_not_deducible : Error<
  "deduction guide template contains "
  "%select{a template parameter|template parameters}0 that cannot be "
  "deduced">;
def err_deduction_guide_wrong_access : Error<
  "deduction guide has different access from the corresponding "
  "member template">;
def note_deduction_guide_template_access : Note<
  "member template declared %0 here">;
def note_deduction_guide_access : Note<
  "deduction guide declared %0 by intervening access specifier">;
def warn_cxx14_compat_class_template_argument_deduction : Warning<
  "类模板参数推导与 C++17 之前的 "
  "%select{|; for compatibility, use explicit type name %1}0 C++ 标准不兼容">,
  InGroup<CXXPre17Compat>, DefaultIgnore;
def warn_ctad_maybe_unsupported : Warning<
  "%0 可能不打算支持类模板参数推导">,
  InGroup<CTADMaybeUnsupported>, DefaultIgnore;
def note_suppress_ctad_maybe_unsupported : Note<
  "添加一个推导指南以抑制此警告">;


// C++14 deduced return types
def err_auto_fn_deduction_failure : Error<
  "cannot deduce return type %0 from returned value of type %1">;
def err_auto_fn_different_deductions : Error<
  "'%select{auto|decltype(auto)}0' in return type deduced as %1 here but "
  "deduced as %2 in earlier return statement">;
def err_auto_fn_used_before_defined : Error<
  "function %0 with deduced return type cannot be used before it is defined">;
def err_auto_fn_no_return_but_not_auto : Error<
  "cannot deduce return type %0 for function with no return statements">;
def err_auto_fn_return_void_but_not_auto : Error<
  "cannot deduce return type %0 from omitted return expression">;
def err_auto_fn_return_init_list : Error<
  "cannot deduce return type from initializer list">;
def err_auto_fn_virtual : Error<
  "function with deduced return type cannot be virtual">;
def warn_cxx11_compat_deduced_return_type : Warning<
  "返回类型推导与 C++14 之前的 C++ 标准不兼容 ">,
  InGroup<CXXPre14Compat>, DefaultIgnore;

// C++11 override control
def override_keyword_only_allowed_on_virtual_member_functions : Error<
  "only virtual member functions can be marked '%0'">;
def override_keyword_hides_virtual_member_function : Error<
  "non-virtual member function marked '%0' hides virtual member "
  "%select{function|functions}1">;
def err_function_marked_override_not_overriding : Error<
  "%0 marked 'override' but does not override any member functions">;
def warn_destructor_marked_not_override_overriding : TextSubstitution <
  "%0 overrides a destructor but is not marked 'override'">;
def warn_function_marked_not_override_overriding : TextSubstitution <
  "%0 overrides a member function but is not marked 'override'">;
def warn_inconsistent_destructor_marked_not_override_overriding : Warning <
  "%sub{warn_destructor_marked_not_override_overriding}0">,
  InGroup<CXX11WarnInconsistentOverrideDestructor>, DefaultIgnore;
def warn_inconsistent_function_marked_not_override_overriding : Warning <
  "%sub{warn_function_marked_not_override_overriding}0">,
  InGroup<CXX11WarnInconsistentOverrideMethod>;
def warn_suggest_destructor_marked_not_override_overriding : Warning <
  "%sub{warn_destructor_marked_not_override_overriding}0">,
  InGroup<CXX11WarnSuggestOverrideDestructor>, DefaultIgnore;
def warn_suggest_function_marked_not_override_overriding : Warning <
  "%sub{warn_function_marked_not_override_overriding}0">,
  InGroup<CXX11WarnSuggestOverride>, DefaultIgnore;
def err_class_marked_final_used_as_base : Error<
  "base %0 is marked '%select{final|sealed}1'">;
def warn_abstract_final_class : Warning<
  "抽象类被标记'%select{final|sealed}0'">, InGroup<AbstractFinalClass>;
def warn_final_dtor_non_final_class : Warning<
  "带有标记的析构函数的类 '%select{final|sealed}0' 不能被继承">,
  InGroup<FinalDtorNonFinalClass>;
def note_final_dtor_non_final_class_silence : Note<
  "标记 %0 为 '%select{final|sealed}1' 让这个警告静音">;

// C++11 attributes
def err_repeat_attribute : Error<"%0 attribute cannot be repeated">;

// C++11 final
def err_final_function_overridden : Error<
  "declaration of %0 overrides a '%select{final|sealed}1' function">;

// C++11 scoped enumerations
def err_enum_invalid_underlying : Error<
  "non-integral type %0 is an invalid underlying type">;
def err_enumerator_too_large : Error<
  "enumerator value is not representable in the underlying type %0">;
def ext_enumerator_too_large : Extension<
  "enumerator value is not representable in the underlying type %0">,
  InGroup<MicrosoftEnumValue>;
def err_enumerator_wrapped : Error<
  "enumerator value %0 is not representable in the underlying type %1">;
def err_enum_redeclare_type_mismatch : Error<
  "enumeration redeclared with different underlying type %0 (was %1)">;
def err_enum_redeclare_fixed_mismatch : Error<
  "enumeration previously declared with %select{non|}0fixed underlying type">;
def err_enum_redeclare_scoped_mismatch : Error<
  "enumeration previously declared as %select{un|}0scoped">;
def err_only_enums_have_underlying_types : Error<
  "only enumeration types have underlying types">;
def err_underlying_type_of_incomplete_enum : Error<
  "cannot determine underlying type of incomplete enumeration type %0">;

// C++11 delegating constructors
def err_delegating_ctor : Error<
  "delegating constructors are permitted only in C++11">;
def warn_cxx98_compat_delegating_ctor : Warning<
  "委托构造函数与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_delegating_initializer_alone : Error<
  "an initializer for a delegating constructor must appear alone">;
def warn_delegating_ctor_cycle : Warning<
  "%0 的构造函数创建一个委托循环">, DefaultError,
  InGroup<DelegatingCtorCycles>;
def note_it_delegates_to : Note<"it delegates to">;
def note_which_delegates_to : Note<"which delegates to">;

// C++11 range-based for loop
def err_for_range_decl_must_be_var : Error<
  "for range declaration must declare a variable">;
def err_for_range_storage_class : Error<
  "loop variable %0 may not be declared %select{'extern'|'static'|"
  "'__private_extern__'|'auto'|'register'|'constexpr'|'thread_local'}1">;
def err_type_defined_in_for_range : Error<
  "types may not be defined in a for range declaration">;
def err_for_range_deduction_failure : Error<
  "cannot use type %0 as a range">;
def err_for_range_incomplete_type : Error<
  "cannot use incomplete type %0 as a range">;
def err_for_range_iter_deduction_failure : Error<
  "cannot use type %0 as an iterator">;
def ext_for_range_begin_end_types_differ : ExtWarn<
  "'begin' and 'end' returning different types (%0 and %1) is a C++17 extension">,
  InGroup<CXX17>;
def warn_for_range_begin_end_types_differ : Warning<
  "'begin' 和 'end' 返回不同类型（%0 和 %1） "
  "与 C++17 之前的 C++ 标准不兼容">, InGroup<CXXPre17Compat>, DefaultIgnore;
def note_in_for_range: Note<
  "when looking up '%select{begin|end}0' function for range expression "
  "of type %1">;
def err_for_range_invalid: Error<
  "invalid range expression of type %0; no viable '%select{begin|end}1' "
  "function available">;
def note_for_range_member_begin_end_ignored : Note<
  "member is not a candidate because range type %0 has no '%select{end|begin}1' member">;
def err_range_on_array_parameter : Error<
  "cannot build range expression with array function parameter %0 since "
  "parameter with array type %1 is treated as pointer type %2">;
def err_for_range_dereference : Error<
  "invalid range expression of type %0; did you mean to dereference it "
  "with '*'?">;
def note_for_range_invalid_iterator : Note <
  "in implicit call to 'operator%select{!=|*|++}0' for iterator of type %1">;
def note_for_range_begin_end : Note<
  "selected '%select{begin|end}0' %select{function|template }1%2 with iterator type %3">;
def warn_for_range_const_ref_binds_temp_built_from_ref : Warning<
  "循环变量%0 "
  "%diff{of type $ binds to a temporary constructed from type $"
  "|binds to a temporary constructed from a different type}1,2">,
  InGroup<RangeLoopConstruct>, DefaultIgnore;
def note_use_type_or_non_reference : Note<
  "use non-reference type %0 to make construction explicit or type %1 to prevent copying">;
def warn_for_range_ref_binds_ret_temp : Warning<
  "循环变量 %0 绑定到由 %1 类型的范围产生的临时值">,
  InGroup<RangeLoopBindReference>, DefaultIgnore;
def note_use_non_reference_type : Note<"use non-reference type %0">;
def warn_for_range_copy : Warning<
  "循环变量 %0 从类型 %1 创建一个副本">,
  InGroup<RangeLoopConstruct>, DefaultIgnore;
def note_use_reference_type : Note<"use reference type %0 to prevent copying">;
def err_objc_for_range_init_stmt : Error<
  "initialization statement is not supported when iterating over Objective-C "
  "collection">;

// C++11 constexpr
def warn_cxx98_compat_constexpr : Warning<
  "'constexpr' 说明符与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
// FIXME: Maybe this should also go in -Wc++14-compat?
def warn_cxx14_compat_constexpr_not_const : Warning<
  "'constexpr' 非静态成员函数在 C++14 中不会隐式为'const'; "
  "添加'const'以避免行为改变">,
  InGroup<DiagGroup<"constexpr-not-const">>;
def err_invalid_consteval_take_address : Error<
  "cannot take address of consteval function %0 outside"
  " of an immediate invocation">;
def err_invalid_consteval_call : Error<
  "call to consteval function %q0 is not a constant expression">;
def err_invalid_consteval_decl_kind : Error<
  "%0 cannot be declared consteval">;
def err_invalid_constexpr : Error<
  "%select{function parameter|typedef}0 "
  "cannot be %sub{select_constexpr_spec_kind}1">;
def err_invalid_constexpr_member : Error<"non-static data member cannot be "
  "constexpr%select{; did you intend to make it %select{const|static}0?|}1">;
def err_constexpr_tag : Error<
  "%select{class|struct|interface|union|enum}0 "
  "cannot be marked %sub{select_constexpr_spec_kind}1">;
def err_constexpr_dtor : Error<
  "destructor cannot be declared %sub{select_constexpr_spec_kind}0">;
def err_constexpr_dtor_subobject : Error<
  "destructor cannot be declared %sub{select_constexpr_spec_kind}0 because "
  "%select{data member %2|base class %3}1 does not have a "
  "constexpr destructor">;
def note_constexpr_dtor_subobject : Note<
  "%select{data member %1|base class %2}0 declared here">;
def err_constexpr_wrong_decl_kind : Error<
  "%sub{select_constexpr_spec_kind}0 can only be used "
  "in %select{|variable and function|function|variable}0 declarations">;
def err_invalid_constexpr_var_decl : Error<
  "constexpr variable declaration must be a definition">;
def err_constexpr_static_mem_var_requires_init : Error<
  "declaration of constexpr static data member %0 requires an initializer">;
def err_constexpr_var_non_literal : Error<
  "constexpr variable cannot have non-literal type %0">;
def err_constexpr_var_requires_const_init : Error<
  "constexpr variable %0 must be initialized by a constant expression">;
def err_constexpr_var_requires_const_destruction : Error<
  "constexpr variable %0 must have constant destruction">;
def err_constexpr_redecl_mismatch : Error<
  "%select{non-constexpr|constexpr|consteval}1 declaration of %0"
  " follows %select{non-constexpr|constexpr|consteval}2 declaration">;
def err_constexpr_virtual : Error<"virtual function cannot be constexpr">;
def warn_cxx17_compat_constexpr_virtual : Warning<
  "虚拟的 constexpr函数与 C++20 之前的 C++ 标准 "
  "不兼容f">, InGroup<CXXPre20Compat>, DefaultIgnore;
def err_constexpr_virtual_base : Error<
  "constexpr %select{member function|constructor}0 not allowed in "
  "%select{struct|interface|class}1 with virtual base "
  "%plural{1:class|:classes}2">;
def note_non_literal_incomplete : Note<
  "incomplete type %0 is not a literal type">;
def note_non_literal_virtual_base : Note<"%select{struct|interface|class}0 "
  "with virtual base %plural{1:class|:classes}1 is not a literal type">;
def note_constexpr_virtual_base_here : Note<"virtual base class declared here">;
def err_constexpr_non_literal_return : Error<
  "%select{constexpr|consteval}0 function's return type %1 is not a literal type">;
def err_constexpr_non_literal_param : Error<
  "%select{constexpr|consteval}2 %select{function|constructor}1's %ordinal0 parameter type %3 is "
  "not a literal type">;
def err_constexpr_body_invalid_stmt : Error<
  "statement not allowed in %select{constexpr|consteval}1 %select{function|constructor}0">;
def ext_constexpr_body_invalid_stmt : ExtWarn<
  "use of this statement in a constexpr %select{function|constructor}0 "
  "is a C++14 extension">, InGroup<CXX14>;
def warn_cxx11_compat_constexpr_body_invalid_stmt : Warning<
  "在 constexpr 中使用此语句 %select{function|constructor}0 "
  "与 C++14 之前的 C++ 标准不兼容">,
  InGroup<CXXPre14Compat>, DefaultIgnore;
def ext_constexpr_body_invalid_stmt_cxx20 : ExtWarn<
  "use of this statement in a constexpr %select{function|constructor}0 "
  "is a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_constexpr_body_invalid_stmt : Warning<
  "在 constexpr 中使用此语句  %select{function|constructor}0 "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def ext_constexpr_type_definition : ExtWarn<
  "type definition in a constexpr %select{function|constructor}0 "
  "is a C++14 extension">, InGroup<CXX14>;
def warn_cxx11_compat_constexpr_type_definition : Warning<
  "constexpr 中的类型定义r %select{function|constructor}0 "
  "与 C++14 之前的 C++ 标准不兼容">,
  InGroup<CXXPre14Compat>, DefaultIgnore;
def err_constexpr_vla : Error<
  "variably-modified type %0 cannot be used in a constexpr "
  "%select{function|constructor}1">;
def ext_constexpr_local_var : ExtWarn<
  "variable declaration in a constexpr %select{function|constructor}0 "
  "is a C++14 extension">, InGroup<CXX14>;
def warn_cxx11_compat_constexpr_local_var : Warning<
  "constexpr 中的变量声明 %select{function|constructor}0 "
  "与 C++14 之前的 C++ 标准不兼容">,
  InGroup<CXXPre14Compat>, DefaultIgnore;
def err_constexpr_local_var_static : Error<
  "%select{static|thread_local}1 variable not permitted in a constexpr "
  "%select{function|constructor}0">;
def err_constexpr_local_var_non_literal_type : Error<
  "variable of non-literal type %1 cannot be defined in a constexpr "
  "%select{function|constructor}0">;
def ext_constexpr_local_var_no_init : ExtWarn<
  "uninitialized variable in a constexpr %select{function|constructor}0 "
  "is a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_constexpr_local_var_no_init : Warning<
  "constexpr 中的未初始化变量 %select{function|constructor}0 "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def ext_constexpr_function_never_constant_expr : ExtWarn<
  "%select{constexpr|consteval}1 %select{function|constructor}0 never produces a "
  "constant expression">, InGroup<DiagGroup<"invalid-constexpr">>, DefaultError;
def err_attr_cond_never_constant_expr : Error<
  "%0 attribute expression never produces a constant expression">;
def err_diagnose_if_invalid_diagnostic_type : Error<
  "invalid diagnostic type for 'diagnose_if'; use \"error\" or \"warning\" "
  "instead">;
def err_constexpr_body_no_return : Error<
  "no return statement in %select{constexpr|consteval}0 function">;
def err_constexpr_return_missing_expr : Error<
  "non-void %select{constexpr|consteval}1 function %0 should return a value">;
def warn_cxx11_compat_constexpr_body_no_return : Warning<
  "没有返回语句的 constexpr 函数 "
  "与 C++14 之前的 C++ 标准不兼容">, InGroup<CXXPre14Compat>, DefaultIgnore;
def ext_constexpr_body_multiple_return : ExtWarn<
  "multiple return statements in constexpr function is a C++14 extension">,
  InGroup<CXX14>;
def warn_cxx11_compat_constexpr_body_multiple_return : Warning<
  "constexpr 函数中的多个返回语句 "
  "与 C++14 之前的 C++ 标准不兼容">,
  InGroup<CXXPre14Compat>, DefaultIgnore;
def note_constexpr_body_previous_return : Note<
  "previous return statement is here">;

// C++20 function try blocks in constexpr
def ext_constexpr_function_try_block_cxx20 : ExtWarn<
  "function try block in constexpr %select{function|constructor}0 is "
  "a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_constexpr_function_try_block : Warning<
  "constexpr 中的函数 try 块 %select{function|constructor}0 is "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;

def ext_constexpr_union_ctor_no_init : ExtWarn<
  "constexpr union constructor that does not initialize any member "
  "is a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_constexpr_union_ctor_no_init : Warning<
  "不初始化任何成员的 constexpr 联合体构造函数"
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def ext_constexpr_ctor_missing_init : ExtWarn<
  "constexpr constructor that does not initialize all members "
  "is a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_constexpr_ctor_missing_init : Warning<
  "不初始化所有成员的 constexpr 构造函数 "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def note_constexpr_ctor_missing_init : Note<
  "member not initialized by constructor">;
def note_non_literal_no_constexpr_ctors : Note<
  "%0 is not literal because it is not an aggregate and has no constexpr "
  "constructors other than copy or move constructors">;
def note_non_literal_base_class : Note<
  "%0 is not literal because it has base class %1 of non-literal type">;
def note_non_literal_field : Note<
  "%0 is not literal because it has data member %1 of "
  "%select{non-literal|volatile}3 type %2">;
def note_non_literal_user_provided_dtor : Note<
  "%0 is not literal because it has a user-provided destructor">;
def note_non_literal_nontrivial_dtor : Note<
  "%0 is not literal because it has a non-trivial destructor">;
def note_non_literal_non_constexpr_dtor : Note<
  "%0 is not literal because its destructor is not constexpr">;
def note_non_literal_lambda : Note<
  "lambda closure types are non-literal types before C++17">;
def warn_private_extern : Warning<
  "在声明中使用 __private_extern__ 可能不会产生链接单元私有的外部符号 "
  "因此不推荐使用">, InGroup<PrivateExtern>;
def note_private_extern : Note<
  "use __attribute__((visibility(\"hidden\"))) attribute instead">;

// C++ Concepts
def err_concept_decls_may_only_appear_in_global_namespace_scope : Error<
  "concept declarations may only appear in global or namespace scope">;
def err_concept_no_parameters : Error<
  "concept template parameter list must have at least one parameter; explicit "
  "specialization of concepts is not allowed">;
def err_concept_extra_headers : Error<
  "extraneous template parameter list in concept definition">;
def err_concept_no_associated_constraints : Error<
  "concept cannot have associated constraints">;
def err_non_constant_constraint_expression : Error<
  "substitution into constraint expression resulted in a non-constant "
  "expression">;
def err_non_bool_atomic_constraint : Error<
  "atomic constraint must be of type 'bool' (found %0)">;
def err_template_arg_list_constraints_not_satisfied : Error<
  "constraints not satisfied for %select{class template|function template|variable template|alias template|"
  "template template parameter|template}0 %1%2">;
def note_substituted_constraint_expr_is_ill_formed : Note<
  "because substituted constraint expression is ill-formed%0">;
def note_atomic_constraint_evaluated_to_false : Note<
  "%select{and|because}0 '%1' evaluated to false">;
def note_concept_specialization_constraint_evaluated_to_false : Note<
  "%select{and|because}0 '%1' evaluated to false">;
def note_single_arg_concept_specialization_constraint_evaluated_to_false : Note<
  "%select{and|because}0 %1 does not satisfy %2">;
def note_atomic_constraint_evaluated_to_false_elaborated : Note<
  "%select{and|because}0 '%1' (%2 %3 %4) evaluated to false">;
def err_constrained_virtual_method : Error<
  "virtual function cannot have a requires clause">;
def err_trailing_requires_clause_on_deduction_guide : Error<
  "deduction guide cannot have a requires clause">;
def err_reference_to_function_with_unsatisfied_constraints : Error<
  "invalid reference to function %0: constraints not satisfied">;
def err_requires_expr_local_parameter_default_argument : Error<
  "default arguments not allowed for parameters of a requires expression">;
def err_requires_expr_parameter_referenced_in_evaluated_context : Error<
  "constraint variable %0 cannot be used in an evaluated context">;
def note_expr_requirement_expr_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid: %2">;
def note_expr_requirement_expr_unknown_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid">;
def note_expr_requirement_noexcept_not_met : Note<
  "%select{and|because}0 '%1' may throw an exception">;
def note_expr_requirement_type_requirement_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid: %2">;
def note_expr_requirement_type_requirement_unknown_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid">;
def note_expr_requirement_constraints_not_satisfied : Note<
  "%select{and|because}0 type constraint '%1' was not satisfied:">;
def note_expr_requirement_constraints_not_satisfied_simple : Note<
  "%select{and|because}0 %1 does not satisfy %2:">;
def note_type_requirement_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid: %2">;
def note_type_requirement_unknown_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid">;
def note_nested_requirement_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid: %2">;
def note_nested_requirement_unknown_substitution_error : Note<
  "%select{and|because}0 '%1' would be invalid">;
def note_ambiguous_atomic_constraints : Note<
  "similar constraint expressions not considered equivalent; constraint "
  "expressions cannot be considered equivalent unless they originate from the "
  "same concept">;
def note_ambiguous_atomic_constraints_similar_expression : Note<
  "similar constraint expression here">;
def err_unsupported_placeholder_constraint : Error<
  "constrained placeholder types other than simple 'auto' on non-type template "
  "parameters not supported yet">;

def err_template_different_requires_clause : Error<
  "requires clause differs in template redeclaration">;
def err_template_different_type_constraint : Error<
  "type constraint differs in template redeclaration">;
def err_template_template_parameter_not_at_least_as_constrained : Error<
  "template template argument %0 is more constrained than template template "
  "parameter %1">;

def err_type_constraint_non_type_concept : Error<
  "concept named in type constraint is not a type concept">;
def err_type_constraint_missing_arguments : Error<
  "%0 requires more than 1 template argument; provide the remaining arguments "
  "explicitly to use it here">;
def err_placeholder_constraints_not_satisfied : Error<
  "deduced type %0 does not satisfy %1">;

// C++11 char16_t/char32_t
def warn_cxx98_compat_unicode_type : Warning<
  "'%0' 类型说明符与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_cxx17_compat_unicode_type : Warning<
  "'char8_t' 类型说明符与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;

// __make_integer_seq
def err_integer_sequence_negative_length : Error<
  "integer sequences must have non-negative sequence length">;
def err_integer_sequence_integral_element_type : Error<
  "integer sequences must have integral element type">;

// __type_pack_element
def err_type_pack_element_out_of_bounds : Error<
  "a parameter pack may not be accessed at an out of bounds index">;

// Objective-C++
def err_objc_decls_may_only_appear_in_global_scope : Error<
  "Objective-C declarations may only appear in global scope">;
def warn_auto_var_is_id : Warning<
  "'auto' 在 %0 的声明中被推断为 'id'">,
  InGroup<DiagGroup<"auto-var-id">>;

// Attributes
def warn_nomerge_attribute_ignored_in_stmt: Warning<
  "0 属性被忽略，因为语句中不存在 "
  "调用表达式">,
  InGroup<IgnoredAttributes>;
def err_nsobject_attribute : Error<
  "'NSObject' attribute is for pointer types only">;
def err_attributes_are_not_compatible : Error<
  "%0 and %1 attributes are not compatible">;
def err_attribute_invalid_argument : Error<
  "%select{a reference type|an array type|a non-vector or "
  "non-vectorizable scalar type}0 is an invalid argument to attribute %1">;
def err_attribute_wrong_number_arguments : Error<
  "%0 attribute %plural{0:takes no arguments|1:takes one argument|"
  ":requires exactly %1 arguments}1">;
def err_attribute_too_many_arguments : Error<
  "%0 attribute takes no more than %1 argument%s1">;
def err_attribute_too_few_arguments : Error<
  "%0 attribute takes at least %1 argument%s1">;
def err_attribute_invalid_vector_type : Error<"invalid vector element type %0">;
def err_attribute_invalid_matrix_type : Error<"invalid matrix element type %0">;
def err_attribute_bad_neon_vector_size : Error<
  "Neon vector size must be 64 or 128 bits">;
def err_attribute_invalid_sve_type : Error<
  "%0 attribute applied to non-SVE type %1">;
def err_attribute_bad_sve_vector_size : Error<
  "invalid SVE vector size '%0', must match value set by "
  "'-msve-vector-bits' ('%1')">;
def err_attribute_arm_feature_sve_bits_unsupported : Error<
  "%0 is only supported when '-msve-vector-bits=<bits>' is specified with a "
  "value of 128, 256, 512, 1024 or 2048.">;
def err_attribute_requires_positive_integer : Error<
  "%0 attribute requires a %select{positive|non-negative}1 "
  "integral compile time constant expression">;
def err_attribute_requires_opencl_version : Error<
  "%0 attribute requires OpenCL version %1%select{| or above}2">;
def err_invalid_branch_protection_spec : Error<
  "invalid or misplaced branch protection specification '%0'">;
def warn_unsupported_target_attribute
    : Warning<"%select{unsupported|duplicate|unknown}0%select{| architecture|"
              " tune CPU}1 '%2' 在'target'属性字符串中; "
              "'target'属性被忽略">,
      InGroup<IgnoredAttributes>;
def err_attribute_unsupported
    : Error<"%0 attribute is not supported on targets missing %1;"
            " specify an appropriate -march= or -mcpu=">;
// The err_*_attribute_argument_not_int are separate because they're used by
// VerifyIntegerConstantExpression.
def err_aligned_attribute_argument_not_int : Error<
  "'aligned' attribute requires integer constant">;
def err_align_value_attribute_argument_not_int : Error<
  "'align_value' attribute requires integer constant">;
def err_alignas_attribute_wrong_decl_type : Error<
  "%0 attribute cannot be applied to a %select{function parameter|"
  "variable with 'register' storage class|'catch' variable|bit-field}1">;
def err_alignas_missing_on_definition : Error<
  "%0 must be specified on definition if it is specified on any declaration">;
def note_alignas_on_declaration : Note<"declared with %0 attribute here">;
def err_alignas_mismatch : Error<
  "redeclaration has different alignment requirement (%1 vs %0)">;
def err_alignas_underaligned : Error<
  "requested alignment is less than minimum alignment of %1 for type %0">;
def err_attribute_sizeless_type : Error<
  "%0 attribute cannot be applied to sizeless type %1">;
def err_attribute_argument_n_type : Error<
  "%0 attribute requires parameter %1 to be %select{int or bool|an integer "
  "constant|a string|an identifier|a constant expression}2">;
def err_attribute_argument_type : Error<
  "%0 attribute requires %select{int or bool|an integer "
  "constant|a string|an identifier}1">;
def err_attribute_argument_out_of_range : Error<
  "%0 attribute requires integer constant between %1 and %2 inclusive">;
def err_init_priority_object_attr : Error<
  "can only use 'init_priority' attribute on file-scope definitions "
  "of objects of class type">;
def err_attribute_argument_out_of_bounds : Error<
  "%0 attribute parameter %1 is out of bounds">;
def err_attribute_only_once_per_parameter : Error<
  "%0 attribute can only be applied once per parameter">;
def err_mismatched_uuid : Error<"uuid does not match previous declaration">;
def note_previous_uuid : Note<"previous uuid specified here">;
def warn_attribute_pointers_only : Warning<
  "%0 属性仅适用于%select{| constant}1 指针参数">,
  InGroup<IgnoredAttributes>;
def err_attribute_pointers_only : Error<warn_attribute_pointers_only.Text>;
def err_attribute_integers_only : Error<
  "%0 attribute argument may only refer to a function parameter of integer "
  "type">;
def warn_attribute_return_pointers_only : Warning<
  "%0 属性仅适用于作为指针的返回值">,
  InGroup<IgnoredAttributes>;
def warn_attribute_return_pointers_refs_only : Warning<
  "%0 属性仅适用于指针或引用的返回值">,
  InGroup<IgnoredAttributes>;
def warn_attribute_pointer_or_reference_only : Warning<
  "%0 属性仅适用于指针或引用（%1 无效）">,
  InGroup<IgnoredAttributes>;
def err_attribute_no_member_pointers : Error<
  "%0 attribute cannot be used with pointers to members">;
def err_attribute_invalid_implicit_this_argument : Error<
  "%0 attribute is invalid for the implicit this argument">;
def err_ownership_type : Error<
  "%0 attribute only applies to %select{pointer|integer}1 arguments">;
def err_ownership_returns_index_mismatch : Error<
  "'ownership_returns' attribute index does not match; here it is %0">;
def note_ownership_returns_index_mismatch : Note<
  "declared with index %0 here">;
def err_format_strftime_third_parameter : Error<
  "strftime format attribute requires 3rd parameter to be 0">;
def err_format_attribute_requires_variadic : Error<
  "format attribute requires variadic function">;
def err_format_attribute_not : Error<"format argument not %0">;
def err_format_attribute_result_not : Error<"function does not return %0">;
def err_format_attribute_implicit_this_format_string : Error<
  "format attribute cannot specify the implicit this argument as the format "
  "string">;
def err_callback_attribute_no_callee : Error<
  "'callback' attribute specifies no callback callee">;
def err_callback_attribute_invalid_callee : Error<
  "'callback' attribute specifies invalid callback callee">;
def err_callback_attribute_multiple : Error<
  "multiple 'callback' attributes specified">;
def err_callback_attribute_argument_unknown : Error<
  "'callback' attribute argument %0 is not a known function parameter">;
def err_callback_callee_no_function_type : Error<
  "'callback' attribute callee does not have function type">;
def err_callback_callee_is_variadic : Error<
  "'callback' attribute callee may not be variadic">;
def err_callback_implicit_this_not_available : Error<
  "'callback' argument at position %0 references unavailable implicit 'this'">;
def err_init_method_bad_return_type : Error<
  "init methods must return an object pointer type, not %0">;
def err_attribute_invalid_size : Error<
  "vector size not an integral multiple of component size">;
def err_attribute_zero_size : Error<"zero %0 size">;
def err_attribute_size_too_large : Error<"%0 size too large">;
def err_typecheck_sve_ambiguous : Error<
  "cannot combine fixed-length and sizeless SVE vectors in expression, result is ambiguous (%0 and %1)">;
def err_typecheck_sve_gnu_ambiguous : Error<
  "cannot combine GNU and SVE vectors in expression, result is ambiguous (%0 and %1)">;
def err_typecheck_vector_not_convertable_implict_truncation : Error<
   "cannot convert between %select{scalar|vector}0 type %1 and vector type"
   " %2 as implicit conversion would cause truncation">;
def err_typecheck_vector_not_convertable : Error<
  "cannot convert between vector values of different size (%0 and %1)">;
def err_typecheck_vector_not_convertable_non_scalar : Error<
  "cannot convert between vector and non-scalar values (%0 and %1)">;
def err_typecheck_vector_lengths_not_equal : Error<
  "vector operands do not have the same number of elements (%0 and %1)">;
def warn_typecheck_vector_element_sizes_not_equal : Warning<
  "向量操作数的元素大小不同（%0 和 %1）">,
  InGroup<DiagGroup<"vec-elem-size">>, DefaultError;
def err_ext_vector_component_exceeds_length : Error<
  "vector component access exceeds type %0">;
def err_ext_vector_component_name_illegal : Error<
  "illegal vector component name '%0'">;
def err_attribute_address_space_negative : Error<
  "address space is negative">;
def err_attribute_address_space_too_high : Error<
  "address space is larger than the maximum supported (%0)">;
def err_attribute_address_multiple_qualifiers : Error<
  "multiple address spaces specified for type">;
def warn_attribute_address_multiple_identical_qualifiers : Warning<
  "为类型指定的多个相同地址空间">,
  InGroup<DuplicateDeclSpecifier>;
def err_attribute_not_clinkage : Error<
  "function type with %0 attribute must have C linkage">;
def err_function_decl_cmse_ns_call : Error<
  "functions may not be declared with 'cmse_nonsecure_call' attribute">;
def err_attribute_address_function_type : Error<
  "function type may not be qualified with an address space">;
def err_as_qualified_auto_decl : Error<
  "automatic variable qualified with an%select{| invalid}0 address space">;
def err_arg_with_address_space : Error<
  "parameter may not be qualified with an address space">;
def err_field_with_address_space : Error<
  "field may not be qualified with an address space">;
def err_compound_literal_with_address_space : Error<
  "compound literal in function scope may not be qualified with an address space">;
def err_address_space_mismatch_templ_inst : Error<
  "conflicting address space qualifiers are provided between types %0 and %1">;
def err_attr_objc_ownership_redundant : Error<
  "the type %0 is already explicitly ownership-qualified">;
def err_invalid_nsnumber_type : Error<
  "%0 is not a valid literal type for NSNumber">;
def err_objc_illegal_boxed_expression_type : Error<
  "illegal type %0 used in a boxed expression">;
def err_objc_non_trivially_copyable_boxed_expression_type : Error<
  "non-trivially copyable type %0 cannot be used in a boxed expression">;
def err_objc_incomplete_boxed_expression_type : Error<
  "incomplete type %0 used in a boxed expression">;
def err_undeclared_objc_literal_class : Error<
  "definition of class %0 must be available to use Objective-C "
  "%select{array literals|dictionary literals|numeric literals|boxed expressions|"
  "string literals}1">;
def err_undeclared_boxing_method : Error<
  "declaration of %0 is missing in %1 class">;
def err_objc_literal_method_sig : Error<
  "literal construction method %0 has incompatible signature">;
def note_objc_literal_method_param : Note<
  "%select{first|second|third}0 parameter has unexpected type %1 "
  "(should be %2)">;
def note_objc_literal_method_return : Note<
  "method returns unexpected type %0 (should be an object type)">;
def err_invalid_collection_element : Error<
  "collection element of type %0 is not an Objective-C object">;
def err_box_literal_collection : Error<
  "%select{string|character|boolean|numeric}0 literal must be prefixed by '@' "
  "in a collection">;
def warn_objc_literal_comparison : Warning<
  "直接比较%select{an array literal|a dictionary literal|"
  "a numeric literal|a boxed expression|}0 有未定义的行为">,
  InGroup<ObjCLiteralComparison>;
def err_missing_atsign_prefix : Error<
  "%select{string|numeric}0 literal must be prefixed by '@'">;
def warn_objc_string_literal_comparison : Warning<
  "字符串常量的直接比较有未定义的行为">,
  InGroup<ObjCStringComparison>;
def warn_concatenated_literal_array_init : Warning<
  "数组初始化中字符串常量的可疑串联; "
  "你的意思是用逗号分隔元素吗?">,
  InGroup<StringConcatation>, DefaultIgnore;
def warn_concatenated_nsarray_literal : Warning<
  "NSArray 表达式的连接 NSString 文字 - "
  "可能缺少一个逗号">,
  InGroup<ObjCStringConcatenation>;
def note_objc_literal_comparison_isequal : Note<
  "use 'isEqual:' instead">;
def warn_objc_collection_literal_element : Warning<
  "%0 类型的对象不兼容 "
  "%select{array element type|dictionary key type|dictionary value type}1 %2">,
  InGroup<ObjCLiteralConversion>;
def warn_nsdictionary_duplicate_key : Warning<
  "duplicate key in dictionary literal">,
  InGroup<DiagGroup<"objc-dictionary-duplicate-keys">>;
def note_nsdictionary_duplicate_key_here : Note<
  "previous equal key is here">;
def err_swift_param_attr_not_swiftcall : Error<
  "'%0' parameter can only be used with swiftcall calling convention">;
def err_swift_indirect_result_not_first : Error<
  "'swift_indirect_result' parameters must be first parameters of function">;
def err_swift_error_result_not_after_swift_context : Error<
  "'swift_error_result' parameter must follow 'swift_context' parameter">;
def err_swift_abi_parameter_wrong_type : Error<
  "'%0' parameter must have pointer%select{| to unqualified pointer}1 type; "
  "type here is %2">;

def err_attribute_argument_invalid : Error<
  "%0 attribute argument is invalid: %select{max must be 0 since min is 0|"
  "min must not be greater than max}1">;
def err_attribute_argument_is_zero : Error<
  "%0 attribute must be greater than 0">;
def warn_attribute_argument_n_negative : Warning<
  "%0 属性参数 %1 为负数，将被忽略">,
  InGroup<CudaCompat>;
def err_property_function_in_objc_container : Error<
  "use of Objective-C property in function nested in Objective-C "
  "container not supported, move function outside its container">;

let CategoryName = "Cocoa API Issue" in {
def warn_objc_redundant_literal_use : Warning<
  "将 %0 与文字一起使用是多余的">, InGroup<ObjCRedundantLiteralUse>;
}

def err_attr_tlsmodel_arg : Error<"tls_model must be \"global-dynamic\", "
  "\"local-dynamic\", \"initial-exec\" or \"local-exec\"">;

def err_tls_var_aligned_over_maximum : Error<
  "alignment (%0) of thread-local variable %1 is greater than the maximum supported "
  "alignment (%2) for a thread-local variable on this target">;

def err_only_annotate_after_access_spec : Error<
  "access specifier can only have annotation attributes">;

def err_attribute_section_invalid_for_target : Error<
  "argument to %select{'code_seg'|'section'}1 attribute is not valid for this target: %0">;
def warn_attribute_section_drectve : Warning<
  "#pragma %0(\".drectve\") 具有未定义的行为, "
  "改用 #pragma comment(linker, ...) 替代">, InGroup<MicrosoftDrectveSection>;
def warn_mismatched_section : Warning<
  "%select{codeseg|section}0 与先前的声明不匹配">, InGroup<Section>;
def warn_attribute_section_on_redeclaration : Warning<
  "在重新声明的变量上指定了 section 属性">, InGroup<Section>;
def err_mismatched_code_seg_base : Error<
  "derived class must specify the same code segment as its base classes">;
def err_mismatched_code_seg_override : Error<
  "overriding virtual function must specify the same code segment as its overridden function">;
def err_conflicting_codeseg_attribute : Error<
  "conflicting code segment specifiers">;
def warn_duplicate_codeseg_attribute : Warning<
  "复的代码段说明符">, InGroup<Section>;

def err_anonymous_property: Error<
  "anonymous property is not supported">;
def err_property_is_variably_modified : Error<
  "property %0 has a variably modified type">;
def err_no_accessor_for_property : Error<
  "no %select{getter|setter}0 defined for property %1">;
def err_cannot_find_suitable_accessor : Error<
  "cannot find suitable %select{getter|setter}0 for property %1">;

def warn_alloca : Warning<
  "不鼓励使用函数 %0; 无法检查故障, "
  "但故障仍可能发生，导致可能存在可利用的安全漏洞">,
  InGroup<DiagGroup<"alloca">>, DefaultIgnore;

def warn_alloca_align_alignof : Warning<
  "__builtin_alloca_with_align 的第二个参数应该是位">,
  InGroup<DiagGroup<"alloca-with-align-alignof">>;

def err_alignment_too_small : Error<
  "requested alignment must be %0 or greater">;
def err_alignment_too_big : Error<
  "requested alignment must be %0 or smaller">;
def err_alignment_not_power_of_two : Error<
  "requested alignment is not a power of 2">;
def warn_alignment_not_power_of_two : Warning<
  err_alignment_not_power_of_two.Text>,
  InGroup<DiagGroup<"non-power-of-two-alignment">>;
def err_alignment_dependent_typedef_name : Error<
  "requested alignment is dependent but declaration is not dependent">;

def warn_alignment_builtin_useless : Warning<
  "%select{aligning a value|the result of checking whether a value is aligned}0"
  " 到 1 字节是 %select{a no-op|always true}0">, InGroup<TautologicalCompare>;
def err_attribute_aligned_too_great : Error<
  "requested alignment must be %0 bytes or smaller">;
def warn_assume_aligned_too_great
    : Warning<"请求的对齐必须为%0字节或更小; "
              "假定的最大对齐">,
      InGroup<DiagGroup<"builtin-assume-aligned-alignment">>;
def warn_redeclaration_without_attribute_prev_attribute_ignored : Warning<
  "%q0 已重新声明，但没有%1属性：已忽略以前的%1">,
  InGroup<MicrosoftInconsistentDllImport>;
def warn_redeclaration_without_import_attribute : Warning<
  "%q0 已重新声明,没有“dllimport”属性:添加了“dllexport”属性">,
  InGroup<MicrosoftInconsistentDllImport>;
def warn_dllimport_dropped_from_inline_function : Warning<
  "%q0 重新声明内联;%1 属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_attribute_ignored : Warning<"%0 属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_nothrow_attribute_ignored : Warning<"'nothrow' 属性与异常规范冲突;"
  "属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_attribute_ignored_on_inline :
  Warning<"%0 属性被内联函数忽略">,
  InGroup<IgnoredAttributes>;
def warn_nocf_check_attribute_ignored :
  Warning<"'nocf_check' 属性被忽略;使用 -fcf-protection 启用该属性">,
  InGroup<IgnoredAttributes>;
def warn_attribute_after_definition_ignored : Warning<
  "定义后的属性 %0 被忽略">,
   InGroup<IgnoredAttributes>;
def warn_attributes_likelihood_ifstmt_conflict
    : Warning<"冲突的属性 %0 被忽略">,
      InGroup<IgnoredAttributes>;
def warn_cxx11_gnu_attribute_on_type : Warning<
  "属性 %0 被忽略，因为它不能应用于类型">,
  InGroup<IgnoredAttributes>;
def warn_unhandled_ms_attribute_ignored : Warning<
  "__declspec 属性 %0 不受支持">,
  InGroup<IgnoredAttributes>;
def warn_attribute_has_no_effect_on_infinite_loop : Warning<
  "注释无限循环时，属性 %0 无效">,
   InGroup<IgnoredAttributes>;
def note_attribute_has_no_effect_on_infinite_loop_here : Note<
  "annotating the infinite loop here">;
def warn_attribute_has_no_effect_on_if_constexpr : Warning<
  "注释 'if constexpr' 语句时，属性 %0 无效">,
   InGroup<IgnoredAttributes>;
def note_attribute_has_no_effect_on_if_constexpr_here : Note<
  "annotating the 'if constexpr' statement here">;
def err_decl_attribute_invalid_on_stmt : Error<
  "%0 attribute cannot be applied to a statement">;
def err_stmt_attribute_invalid_on_decl : Error<
  "%0 attribute cannot be applied to a declaration">;
def warn_declspec_attribute_ignored : Warning<
  "属性 %0 被忽略，将其放在 "
  "\"%select{class|struct|interface|union|enum}1\"后面,将属性应用于 "
  "类型声明">, InGroup<IgnoredAttributes>;
def warn_attribute_precede_definition : Warning<
  "属性声明必须在定义之前">,
  InGroup<IgnoredAttributes>;
def warn_attribute_void_function_method : Warning<
  "属性 %0 无法应用于 "
  "%select{functions|Objective-C method}1 没有返回值">,
  InGroup<IgnoredAttributes>;
def warn_attribute_weak_on_field : Warning<
  "__weak 属性不能在字段声明中指定">,
  InGroup<IgnoredAttributes>;
def warn_gc_attribute_weak_on_local : Warning<
  "Objective-C GC 不允许栈上的弱变量">,
  InGroup<IgnoredAttributes>;
def warn_nsobject_attribute : Warning<
  "'NSObject' 属性只能放在 typedef 上;属性被忽略">,
  InGroup<NSobjectAttribute>;
def warn_independentclass_attribute : Warning<
  "'objc_independent_class' 属性只能放在 typedef 上; "
  "attribute is ignored">,
  InGroup<IndependentClassAttribute>;
def warn_ptr_independentclass_attribute : Warning<
  "'objc_independent_class' 属性只能放在 Objective-C 对象指针类型上; "
  "属性被忽略">,
  InGroup<IndependentClassAttribute>;
def warn_attribute_weak_on_local : Warning<
  "未启用 ARC 时，无法在自动变量上 "
  "指定 __weak 属性">,
  InGroup<IgnoredAttributes>;
def warn_weak_identifier_undeclared : Warning<
  "弱标识符 %0 从未声明">;
def warn_attribute_cmse_entry_static : Warning<
  "'cmse_nonsecure_entry' 不能应用于具有内部链接的函数">,
  InGroup<IgnoredAttributes>;
def warn_cmse_nonsecure_union : Warning<
  "跨安全边界传递联合体通过%select{parameter %1|return value}0 "
  "可能会泄漏信息">,
  InGroup<DiagGroup<"cmse-union-leak">>;
def err_attribute_weak_static : Error<
  "weak declaration cannot have internal linkage">;
def err_attribute_selectany_non_extern_data : Error<
  "'selectany' can only be applied to data items with external linkage">;
def err_declspec_thread_on_thread_variable : Error<
  "'__declspec(thread)' applied to variable that already has a "
  "thread-local storage specifier">;
def err_attribute_dll_not_extern : Error<
  "%q0 must have external linkage when declared %q1">;
def err_attribute_dll_thread_local : Error<
  "%q0 cannot be thread local when declared %q1">;
def err_attribute_dll_lambda : Error<
  "lambda cannot be declared %0">;
def warn_attribute_invalid_on_definition : Warning<
  "无法在定义上指定'%0'属性">,
  InGroup<IgnoredAttributes>;
def err_attribute_dll_redeclaration : Error<
  "redeclaration of %q0 cannot add %q1 attribute">;
def warn_attribute_dll_redeclaration : Warning<
  "%q0 的重新声明不应添加 %q1 属性">,
  InGroup<DiagGroup<"dll-attribute-on-redeclaration">>;
def err_attribute_dllimport_function_definition : Error<
  "dllimport cannot be applied to non-inline function definition">;
def err_attribute_dll_deleted : Error<
  "attribute %q0 cannot be applied to a deleted function">;
def err_attribute_dllimport_data_definition : Error<
  "definition of dllimport data">;
def err_attribute_dllimport_static_field_definition : Error<
  "definition of dllimport static field not allowed">;
def warn_attribute_dllimport_static_field_definition : Warning<
  "dllimport静态字段的定义">,
  InGroup<DiagGroup<"dllimport-static-field-def">>;
def warn_attribute_dllexport_explicit_instantiation_decl : Warning<
  "显式实例化声明不应是'dllexport'">,
  InGroup<DiagGroup<"dllexport-explicit-instantiation-decl">>;
def warn_attribute_dllexport_explicit_instantiation_def : Warning<
  "'dllexport' 属性在显式实例化定义中被忽略">,
  InGroup<IgnoredAttributes>;
def warn_invalid_initializer_from_system_header : Warning<
  "系统头中类的无效构造函数，不应该是显式的">,
  InGroup<DiagGroup<"invalid-initializer-from-system-header">>;
def note_used_in_initialization_here : Note<"used in initialization here">;
def err_attribute_dll_member_of_dll_class : Error<
  "attribute %q0 cannot be applied to member of %q1 class">;
def warn_attribute_dll_instantiated_base_class : Warning<
  "将 dll 属性传播到 %select{already instantiated|explicitly specialized}0 "
  "没有 dll 属性的基类模板是不支持的">,
  InGroup<DiagGroup<"unsupported-dll-base-class-template">>, DefaultIgnore;
def err_attribute_dll_ambiguous_default_ctor : Error<
  "'__declspec(dllexport)' cannot be applied to more than one default constructor in %0">;
def err_attribute_weakref_not_static : Error<
  "weakref declaration must have internal linkage">;
def err_attribute_weakref_not_global_context : Error<
  "weakref declaration of %0 must be in a global context">;
def err_attribute_weakref_without_alias : Error<
  "weakref declaration of %0 must also have an alias attribute">;
def err_alias_not_supported_on_darwin : Error <
  "aliases are not supported on darwin">;
def warn_attribute_wrong_decl_type_str : Warning<
  "%0 属性仅适用于 %1">, InGroup<IgnoredAttributes>;
def err_attribute_wrong_decl_type_str : Error<
  warn_attribute_wrong_decl_type_str.Text>;
def warn_attribute_wrong_decl_type : Warning<
  "%0 属性仅适用于 %select{"
  "functions"
  "|unions"
  "|variables and functions"
  "|functions and methods"
  "|functions, methods and blocks"
  "|functions, methods, and parameters"
  "|variables"
  "|variables and fields"
  "|variables, data members and tag types"
  "|types and namespaces"
  "|variables, functions and classes"
  "|kernel functions"
  "|non-K&R-style functions}1">,
  InGroup<IgnoredAttributes>;
def err_attribute_wrong_decl_type : Error<warn_attribute_wrong_decl_type.Text>;
def warn_type_attribute_wrong_type : Warning<
  "'%0'仅适用于 %select{function|pointer|"
  "Objective-C object or block pointer}1 类型;这里的类型是 %2">,
  InGroup<IgnoredAttributes>;
def warn_incomplete_encoded_type : Warning<
  "%0 类型的编码不完整，因为 %1 组件的编码未知">,
  InGroup<DiagGroup<"encode-type">>;
def warn_gnu_inline_attribute_requires_inline : Warning<
  "'gnu_inline' 属性要求将函数标记为'inline',"
  " 属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_gnu_inline_cplusplus_without_extern : Warning<
  "没有'extern'的'gnu_inline' 属性在C++中被视为外部可用"
  "这在 Clang 10 中发生了变化">,
  InGroup<DiagGroup<"gnu-inline-cpp-without-extern">>;
def err_attribute_vecreturn_only_vector_member : Error<
  "the vecreturn attribute can only be used on a class or structure with one member, which must be a vector">;
def err_attribute_vecreturn_only_pod_record : Error<
  "the vecreturn attribute can only be used on a POD (plain old data) class or structure (i.e. no virtual functions)">;
def err_cconv_change : Error<
  "function declared '%0' here was previously declared "
  "%select{'%2'|without calling convention}1">;
def warn_cconv_unsupported : Warning<
  "不支持 %0 调用约定 %select{"
  // Use CallingConventionIgnoredReason Enum to specify these.
  "for this target"
  "|on variadic function"
  "|on constructor/destructor"
  "|on builtin function"
  "}1">,
  InGroup<IgnoredAttributes>;
def error_cconv_unsupported : Error<warn_cconv_unsupported.Text>;
def err_cconv_knr : Error<
  "function with no prototype cannot use the %0 calling convention">;
def warn_cconv_knr : Warning<
  err_cconv_knr.Text>,
  InGroup<DiagGroup<"missing-prototype-for-cc">>;
def err_cconv_varargs : Error<
  "variadic function cannot use %0 calling convention">;
def err_regparm_mismatch : Error<"function declared with regparm(%0) "
  "attribute was previously declared "
  "%plural{0:without the regparm|:with the regparm(%1)}1 attribute">;
def err_function_attribute_mismatch : Error<
  "function declared with %0 attribute "
  "was previously declared without the %0 attribute">;
def err_objc_precise_lifetime_bad_type : Error<
  "objc_precise_lifetime only applies to retainable types; type here is %0">;
def warn_objc_precise_lifetime_meaningless : Error<
  "objc_precise_lifetime is not meaningful for "
  "%select{__unsafe_unretained|__autoreleasing}0 objects">;
def err_invalid_pcs : Error<"invalid PCS type">;
def warn_attribute_not_on_decl : Warning<
  "分析类型时忽略 %0 属性">, InGroup<IgnoredAttributes>;
def err_base_specifier_attribute : Error<
  "%0 attribute cannot be applied to a base specifier">;
def err_invalid_attribute_on_virtual_function : Error<
  "%0 attribute cannot be applied to virtual functions">;
def warn_declspec_allocator_nonpointer : Warning<
  "忽略 __declspec(allocator)  "
  "因为函数返回类型 %0 不是指针或引用类型">, InGroup<IgnoredAttributes>;
def err_cconv_incomplete_param_type : Error<
  "parameter %0 must have a complete type to use function %1 with the %2 "
  "calling convention">;
def err_attribute_output_parameter : Error<
  "attribute only applies to output parameters">;

def ext_cannot_use_trivial_abi : ExtWarn<
  "'trivial_abi' cannot be applied to %0">, InGroup<IgnoredAttributes>;
def note_cannot_use_trivial_abi_reason : Note<
  "'trivial_abi' is disallowed on %0 because %select{"
  "its copy constructors and move constructors are all deleted|"
  "it is polymorphic|"
  "it has a base of a non-trivial class type|it has a virtual base|"
  "it has a __weak field|it has a field of a non-trivial class type}1">;

// Availability attribute
def warn_availability_unknown_platform : Warning<
  "可用性宏中的未知平台 %0">, InGroup<Availability>;
def warn_availability_version_ordering : Warning<
  "功能不能是 %select{introduced|deprecated|obsoleted}0 在 %1 版本中 "
  "%2 之前它是 %select{introduced|deprecated|obsoleted}3 在版本 %4中; "
  "attribute ignored">, InGroup<Availability>;
def warn_mismatched_availability: Warning<
  "可用性与之前的声明不符">, InGroup<Availability>;
def warn_mismatched_availability_override : Warning<
  "%select{|overriding }4方法 %select{introduced after|"
  "deprecated before|obsoleted before}0 "
  "%select{the protocol method it implements|overridden method}4 "
  "on %1 (%2 vs. %3)">, InGroup<Availability>;
def warn_mismatched_availability_override_unavail : Warning<
  "%select{|overriding }1方法不是可用的在 %0 wh上，当 "
  "%select{the protocol method it implements|its overridden method}1 是 "
  "可用的">,
  InGroup<Availability>;
def warn_availability_on_static_initializer : Warning<
  "ignoring availability attribute %select{on '+load' method|"
  "with constructor attribute|with destructor attribute}0">,
  InGroup<Availability>;
def note_overridden_method : Note<
  "overridden method is here">;
def warn_availability_swift_unavailable_deprecated_only : Warning<
  "Swift 可用性仅支持'unavailable'和'deprecated'">,
  InGroup<Availability>;
def note_protocol_method : Note<
  "protocol method is here">;

def warn_unguarded_availability :
  Warning<"%0 仅在 %1 %2 或更新版本上可用">,
  InGroup<UnguardedAvailability>, DefaultIgnore;
def warn_unguarded_availability_new :
  Warning<warn_unguarded_availability.Text>,
  InGroup<UnguardedAvailabilityNew>;
def note_decl_unguarded_availability_silence : Note<
  "annotate %select{%1|anonymous %1}0 with an availability attribute to silence this warning">;
def note_unguarded_available_silence : Note<
  "enclose %0 in %select{an @available|a __builtin_available}1 check to silence"
  " this warning">;
def warn_at_available_unchecked_use : Warning<
  "%select{@available|__builtin_available}0 在这里不保护可用性； "
  "使用如果替代为 (%select{@available|__builtin_available}0) ">,
  InGroup<DiagGroup<"unsupported-availability-guard">>;

// Thread Safety Attributes
def warn_thread_attribute_ignored : Warning<
  "忽略 %0 属性，因为它的参数无效">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def warn_thread_attribute_not_on_non_static_member : Warning<
  "没有能力参数的 %0 属性只能应用于 "
  "类的非静态方法">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def warn_thread_attribute_not_on_capability_member : Warning<
  "没有能力参数的 %0 属性引用了 'this'，但 %1 没有用 "
  "'capability' 或 'scoped_lockable' 属性注释 ">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def warn_thread_attribute_argument_not_lockable : Warning<
  "%0 属性需要其类型用 'capability' 属性注释的参数; "
  "这里的类型是 %1">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def warn_thread_attribute_decl_not_lockable : Warning<
  "%0 属性只能在使用'capability'属性注释的 "
  "上下文中应用">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def warn_thread_attribute_decl_not_pointer : Warning<
  "%0 仅适用于指针类型;这里的类型是 %1">,
  InGroup<ThreadSafetyAttributes>, DefaultIgnore;
def err_attribute_argument_out_of_bounds_extra_info : Error<
  "%0 attribute parameter %1 is out of bounds: "
  "%plural{0:no parameters to index into|"
  "1:can only be 1, since there is one parameter|"
  ":must be between 1 and %2}2">;

// Thread Safety Analysis
def warn_unlock_but_no_lock : Warning<"释放未持有的 %0 '%1'">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_unlock_kind_mismatch : Warning<
  "释放 %0 '%1' 使用 %select{shared|exclusive}2 访问, 预期 "
  "%select{shared|exclusive}3 访问">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_double_lock : Warning<"获取已持有的 %0 '%1'">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_no_unlock : Warning<
  "%0 '%1' 仍保留在函数结束时">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_expecting_locked : Warning<
  "期望 %0 '%1' 在函数结束时保持">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
// FIXME: improve the error message about locks not in scope
def warn_lock_some_predecessors : Warning<
  "%0 '%1' 并非在通过此处的每条路径上都保持">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_expecting_lock_held_on_loop : Warning<
  "期望在每个循环开始时保持 %0 '%1'">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def note_locked_here : Note<"%0 acquired here">;
def note_unlocked_here : Note<"%0 released here">;
def warn_lock_exclusive_and_shared : Warning<
  "%0 '%1' 以独占方式获取并在同一范围内共享">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def note_lock_exclusive_and_shared : Note<
  "the other acquisition of %0 '%1' is here">;
def warn_variable_requires_any_lock : Warning<
  "%select{reading|writing}1变量 %0 需要持有"
  "%select{any mutex|any mutex exclusively}1">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_var_deref_requires_any_lock : Warning<
  "%select{reading|writing}1 %0 指向的值需要持有 "
  "%select{any mutex|any mutex exclusively}1">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_fun_excludes_mutex : Warning<
  "无法调用函数 '%1'当%0 '%2' 被持有时">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_cannot_resolve_lock : Warning<
  "无法解析锁表达式">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_acquired_before : Warning<
  "%0 '%1' 必须在 '%2' 之前获取">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_acquired_before_after_cycle : Warning<
  "从 '%0' 开始循环获取之前/之后的依赖项">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;


// Thread safety warnings negative capabilities
def warn_acquire_requires_negative_cap : Warning<
  "获取 %0 '%1' 需要负能力 '%2'">,
  InGroup<ThreadSafetyNegative>, DefaultIgnore;
def warn_fun_requires_negative_cap : Warning<
  "调用函数 %0 需要负能力 '%1'">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;

// Thread safety warnings on pass by reference
def warn_guarded_pass_by_reference : Warning<
  "通过引用传递变量 %1 需要持有 %0 "
  "%select{'%2'|'%2' exclusively}3">,
  InGroup<ThreadSafetyReference>, DefaultIgnore;
def warn_pt_guarded_pass_by_reference : Warning<
  "通过引用传递 %1 指向的值需要持有 %0 "
  "%select{'%2'|'%2' exclusively}3">,
  InGroup<ThreadSafetyReference>, DefaultIgnore;

// Imprecise thread safety warnings
def warn_variable_requires_lock : Warning<
  "%select{reading|writing}3 变量 %1 需要持有 %0 "
  "%select{'%2'|'%2' exclusively}3">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_var_deref_requires_lock : Warning<
  "%select{reading|writing}3 %1 指向的值需要 "
  "持有 %0 %select{'%2'|'%2' exclusively}3">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;
def warn_fun_requires_lock : Warning<
  "调用函数 %1 需要持有 %0 %select{'%2'|'%2' exclusively}3">,
  InGroup<ThreadSafetyAnalysis>, DefaultIgnore;

// Precise thread safety warnings
def warn_variable_requires_lock_precise :
  Warning<warn_variable_requires_lock.Text>,
  InGroup<ThreadSafetyPrecise>, DefaultIgnore;
def warn_var_deref_requires_lock_precise :
  Warning<warn_var_deref_requires_lock.Text>,
  InGroup<ThreadSafetyPrecise>, DefaultIgnore;
def warn_fun_requires_lock_precise :
  Warning<warn_fun_requires_lock.Text>,
  InGroup<ThreadSafetyPrecise>, DefaultIgnore;
def note_found_mutex_near_match : Note<"found near match '%0'">;

// Verbose thread safety warnings
def warn_thread_safety_verbose : Warning<"线程安全详细警告.">,
  InGroup<ThreadSafetyVerbose>, DefaultIgnore;
def note_thread_warning_in_fun : Note<"Thread warning in function %0">;
def note_guarded_by_declared_here : Note<"Guarded_by declared here.">;

// Dummy warning that will trigger "beta" warnings from the analysis if enabled.
def warn_thread_safety_beta : Warning<"线程安全beta警告.">,
  InGroup<ThreadSafetyBeta>, DefaultIgnore;

// Consumed warnings
def warn_use_in_invalid_state : Warning<
  "对方法 '%0' 的无效调用,在对象 '%1' 处于 '%2'  "
  "状态时">, InGroup<Consumed>, DefaultIgnore;
def warn_use_of_temp_in_invalid_state : Warning<
  "对方法'%0'的无效调用当临时对象处于 "
  "'%1'状态时">, InGroup<Consumed>, DefaultIgnore;
def warn_attr_on_unconsumable_class : Warning<
  "消耗的分析属性附加到未标记为 "
  "消耗的类 %0 的成员">, InGroup<Consumed>, DefaultIgnore;
def warn_return_typestate_for_unconsumable_type : Warning<
  "返回为不可使用类型'%0'设置的状态">, InGroup<Consumed>,
  DefaultIgnore;
def warn_return_typestate_mismatch : Warning<
  "返回值不在预期状态； 预期 '%0'，观察到 '%1'">,
  InGroup<Consumed>, DefaultIgnore;
def warn_loop_state_mismatch : Warning<
  "变量'%0'的状态必须在循环的入口和出口匹配">,
  InGroup<Consumed>, DefaultIgnore;
def warn_param_return_typestate_mismatch : Warning<
  "函数返回时参数'%0'未处于预期状态:"
  "：预期'%1'，观察到'%2' ">, InGroup<Consumed>, DefaultIgnore;
def warn_param_typestate_mismatch : Warning<
  "参数未处于预期状态;预期 '%0'，观察到 '%1'">,
  InGroup<Consumed>, DefaultIgnore;

// no_sanitize attribute
def warn_unknown_sanitizer_ignored : Warning<
  "未知的清理程序'%0'被忽略">, InGroup<UnknownSanitizers>;

def warn_impcast_vector_scalar : Warning<
  "隐式转换将向量转换为标量: %0 到 %1">,
  InGroup<Conversion>, DefaultIgnore;
def warn_impcast_complex_scalar : Warning<
  "隐式转换将放弃虚组件：%0到%1">,
  InGroup<Conversion>, DefaultIgnore;
def err_impcast_complex_scalar : Error<
  "implicit conversion from %0 to %1 is not permitted in C++">;
def warn_impcast_float_precision : Warning<
  "隐式转换丢失浮点精度：%0到%1">,
  InGroup<ImplicitFloatConversion>, DefaultIgnore;
def warn_impcast_float_result_precision : Warning<
  "分配计算结果时的隐式转换将丢失浮点精度：%0到%1">,
  InGroup<ImplicitFloatConversion>, DefaultIgnore;
def warn_impcast_double_promotion : Warning<
  "隐式转换将浮点精度：%0增加到%1">,
  InGroup<DoublePromotion>, DefaultIgnore;
def warn_impcast_integer_sign : Warning<
  "隐式转换将签名：%0更改为%1">,
  InGroup<SignConversion>, DefaultIgnore;
def warn_impcast_integer_sign_conditional : Warning<
  "？的操作数 更改签名: %0 到 %1">,
  InGroup<SignConversion>, DefaultIgnore;
def warn_impcast_integer_precision : Warning<
  "隐式转换丢失整数精度: %0 到 %1">,
  InGroup<ImplicitIntConversion>, DefaultIgnore;
def warn_impcast_high_order_zero_bits : Warning<
  "隐式转换后高阶位为零">,
  InGroup<ImplicitIntConversion>, DefaultIgnore;
def warn_impcast_nonnegative_result : Warning<
  "隐式转换后的结果值始终为非负数">,
  InGroup<SignConversion>, DefaultIgnore;
def warn_impcast_integer_64_32 : Warning<
  "隐式转换丢失整数精度: %0 到 %1">,
  InGroup<Shorten64To32>, DefaultIgnore;
def warn_impcast_integer_precision_constant : Warning<
  "从 %2 到 %3 的隐式转换将值从 %0 更改为 %1">,
  InGroup<ConstantConversion>;
def warn_impcast_bitfield_precision_constant : Warning<
  "从 %2 到位域的隐式截断将值从 %0 更改为 %1">,
  InGroup<BitFieldConstantConversion>;
def warn_impcast_constant_value_to_objc_bool : Warning<
  "从常量值 %0 到 'BOOL' 的隐式转换; "
  "'BOOL' 唯一定义明确的值是 YES 和 NO">,
  InGroup<ObjCBoolConstantConversion>;

def warn_impcast_fixed_point_range : Warning<
  "来自 %0 的隐式转换不能位于 %1 的值范围内">,
  InGroup<ImplicitFixedPointConversion>;

def warn_impcast_literal_float_to_integer : Warning<
  "从 %0 到 %1 的隐式转换将值从 %2 更改为 %3">,
  InGroup<LiteralConversion>;
def warn_impcast_literal_float_to_integer_out_of_range : Warning<
  "未定义从 %0 到 %1 的超出范围值的隐式转换">,
  InGroup<LiteralConversion>;
def warn_impcast_float_integer : Warning<
  "隐式转换将浮点数转换为整数：%0 到 %1">,
  InGroup<FloatConversion>, DefaultIgnore;
def warn_impcast_float_to_objc_signed_char_bool : Warning<
  "从浮点类型 %0 到 'BOOL' 的隐式转换">,
  InGroup<ObjCSignedCharBoolImplicitFloatConversion>;
def warn_impcast_int_to_objc_signed_char_bool : Warning<
  "从整数类型 %0 到 'BOOL' 的隐式转换">,
  InGroup<ObjCSignedCharBoolImplicitIntConversion>, DefaultIgnore;

// Implicit int -> float conversion precision loss warnings.
def warn_impcast_integer_float_precision : Warning<
  "从 %0 到 %1 的隐式转换可能会丢失精度">,
  InGroup<ImplicitIntFloatConversion>, DefaultIgnore;
def warn_impcast_integer_float_precision_constant : Warning<
  "从 %2 到 %3 的隐式转换将值从 %0 更改为 %1">,
  InGroup<ImplicitConstIntFloatConversion>;

def warn_impcast_float_to_integer : Warning<
  "从 %0 到 %1 的隐式转换将值从 %2 更改为 %3">,
  InGroup<FloatOverflowConversion>, DefaultIgnore;
def warn_impcast_float_to_integer_out_of_range : Warning<
  "未定义从 %0 到 %1 的超出范围值的隐式转换">,
  InGroup<FloatOverflowConversion>, DefaultIgnore;
def warn_impcast_float_to_integer_zero : Warning<
  "从 %0 到 %1 的隐式转换将非零值从 %2 更改为 %3">,
  InGroup<FloatZeroConversion>, DefaultIgnore;

def warn_impcast_string_literal_to_bool : Warning<
  "隐式转换将字符串文字转换为布尔值：%0 到 %1">,
  InGroup<StringConversion>, DefaultIgnore;
def warn_impcast_different_enum_types : Warning<
  "从枚举类型 %0 到不同枚举类型 %1 的 "
  "隐式转换">, InGroup<EnumConversion>;
def warn_impcast_bool_to_null_pointer : Warning<
    "常量布尔表达式将 %0 类型的指针初始化 "
    "为 null">, InGroup<BoolConversion>;
def warn_non_literal_null_pointer : Warning<
    "计算为 0 的表达式被视为 %0 类型的 "
    "空指针常量">, InGroup<NonLiteralNullConversion>;
def warn_pointer_compare : Warning<
    "比较指向空字符常量的指针; "
    "你的意思是比较 %select{NULL|(void *)0}0?">,
    InGroup<DiagGroup<"pointer-compare">>;
def warn_impcast_null_pointer_to_integer : Warning<
    "隐式转换 %select{NULL|nullptr}0 从常量到 %1 ">,
    InGroup<NullConversion>;
def warn_impcast_floating_point_to_bool : Warning<
    "隐式转换将浮点数转换为布尔值：%0 到 %1">,
    InGroup<ImplicitConversionFloatingPointToBool>;
def ext_ms_impcast_fn_obj : ExtWarn<
  "函数指针和对象指针之间的隐式转换是  "
  "Microsoft 扩展">, InGroup<MicrosoftCast>;

def warn_impcast_pointer_to_bool : Warning<
    "%select{| function| array}0 的地址'%1' 将始终等于 "
    "'true'">,
    InGroup<PointerBoolConversion>;
def warn_cast_nonnull_to_bool : Warning<
    "非空 %select{function call|parameter}0 '%1' 将始终等于 "
    "'true' 第一次遇到时">,
    InGroup<PointerBoolConversion>;
def warn_this_bool_conversion : Warning<
  "'this' 指针在定义明确的 C++ 代码中不能为空; "
  "可以假定指针总是转换为真">, InGroup<UndefinedBoolConversion>;
def warn_address_of_reference_bool_conversion : Warning<
  "在定义明确的 C++ 代码中，引用不能绑定到取消引用的空指针; "
  "可以假定指针总是转换为真">,
  InGroup<UndefinedBoolConversion>;

def warn_xor_used_as_pow : Warning<
  "'%0'的结果是 %1; 你的意思是指数吗？">,
  InGroup<XorUsedAsPow>;
def warn_xor_used_as_pow_base_extra : Warning<
  "'%0'的结果是 %1; 您的意思是'%2'（%3）吗？">,
  InGroup<XorUsedAsPow>;
def warn_xor_used_as_pow_base : Warning<
  "'%0'的结果是 %1; 你的意思是'%2'吗？">,
  InGroup<XorUsedAsPow>;
def note_xor_used_as_pow_silence : Note<
  "replace expression with '%0' %select{|or use 'xor' instead of '^' }1to silence this warning">;

def warn_null_pointer_compare : Warning<
    "比较属于%select{address of|function|array}0 '%1' %select{not |}2"
    "等于空指针总是 %select{true|false}2">,
    InGroup<TautologicalPointerCompare>;
def warn_nonnull_expr_compare : Warning<
    "非空的比较 %select{function call|parameter}0 '%1' "
    "%select{not |}2等于空指针 '%select{true|false}2' 在第一次遇到 "
    "的时候">,
    InGroup<TautologicalPointerCompare>;
def warn_this_null_compare : Warning<
  "'this' 指针在定义明确的 C++ 代码中不能为空  "
  "可以假设比较总是评估为%select{true|false}0">,
  InGroup<TautologicalUndefinedCompare>;
def warn_address_of_reference_null_compare : Warning<
  "在定义明确的 C++ 代码中，引用不能绑定到取消引用的空指针; "
  "可以假设比较总是评估为 "
  "%select{true|false}0">,
  InGroup<TautologicalUndefinedCompare>;
def note_reference_is_return_value : Note<"%0 returns a reference">;

def note_pointer_declared_here : Note<
  "pointer %0 declared here">;
def warn_division_sizeof_ptr : Warning<
  "'%0' 将返回指针的大小，而不是数组本身">,
  InGroup<DiagGroup<"sizeof-pointer-div">>;
def warn_division_sizeof_array : Warning<
  "表达式不计算此数组中的元素数; "
  "元素类型是 %0，而不是 %1">,
  InGroup<DiagGroup<"sizeof-array-div">>;

def note_function_warning_silence : Note<
    "prefix with the address-of operator to silence this warning">;
def note_function_to_function_call : Note<
    "suffix with parentheses to turn this into a function call">;
def warn_impcast_objective_c_literal_to_bool : Warning<
    "Objective-C 对象字面量的隐式布尔转换总是"
    "计算为真">,
    InGroup<ObjCLiteralConversion>;

def warn_cast_align : Warning<
  "从 %0 到 %1 的转换将所需的对齐方式从 %2 增加到 %3">,
  InGroup<CastAlign>, DefaultIgnore;
def warn_old_style_cast : Warning<
  "旧样式转换的使用">, InGroup<OldStyleCast>, DefaultIgnore;

// Separate between casts to void* and non-void* pointers.
// Some APIs use (abuse) void* for something like a user context,
// and often that value is an integer even if it isn't a pointer itself.
// Having a separate warning flag allows users to control the warning
// for their workflow.
def warn_int_to_pointer_cast : Warning<
  "转换为 %1 从更小的整数类型 %0">,
  InGroup<IntToPointerCast>;
def warn_int_to_void_pointer_cast : Warning<
  "转换为 %1 从更小的整数类型 %0">,
  InGroup<IntToVoidPointerCast>;
def warn_pointer_to_int_cast : Warning<
  "转换为更小的整数类型%1 从 %0">,
  InGroup<PointerToIntCast>;
def warn_pointer_to_enum_cast : Warning<
  warn_pointer_to_int_cast.Text>,
  InGroup<PointerToEnumCast>;
def warn_void_pointer_to_int_cast : Warning<
  "转换为更小的整数类型%1 从 %0">,
  InGroup<VoidPointerToIntCast>;
def warn_void_pointer_to_enum_cast : Warning<
  warn_void_pointer_to_int_cast.Text>,
  InGroup<VoidPointerToEnumCast>;

def warn_attribute_ignored_for_field_of_type : Warning<
  "%0 属性被忽略，对于类型为 %1 的字段">,
  InGroup<IgnoredAttributes>;
def warn_no_underlying_type_specified_for_enum_bitfield : Warning<
  "Microsoft ABI 中的枚举默认为有符号整数; "
  "考虑给枚举 %0 一个无符号的基础类型，以使此代码可移植">,
  InGroup<SignedEnumBitfield>, DefaultIgnore;
def warn_attribute_packed_for_bitfield : Warning<
  "在具有单字节对齐的位字段上忽略了'packed'属性 "
  "在旧版本的 GCC 和 Clang 中">,
  InGroup<DiagGroup<"attribute-packed-for-bitfield">>;
def warn_transparent_union_attribute_field_size_align : Warning<
  "%select{alignment|size}0 属于字段 %1 (%2 bits)不匹配  "
  "%select{alignment|size}0 在透明联合体中的第一个字段; "
  "transparent_union 属性被忽略">,
  InGroup<IgnoredAttributes>;
def note_transparent_union_first_field_size_align : Note<
  "%select{alignment|size}0 of first field is %1 bits">;
def warn_transparent_union_attribute_not_definition : Warning<
  "transparent_union 属性只能应用于联合体的定义;  "
  "属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_transparent_union_attribute_floating : Warning<
  "透明联合体的第一个字段不能有 %select{floating point|"
  "vector}0 类型 %1; transparent_union 属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_transparent_union_attribute_zero_fields : Warning<
  "透明联合定义必须至少包含一个字段 "
  "transparent_union 属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_attribute_type_not_supported : Warning<
  "不支持 %0 属性参数: %1">,
  InGroup<IgnoredAttributes>;
def warn_attribute_unknown_visibility : Warning<"未知的能见度 %0">,
  InGroup<IgnoredAttributes>;
def warn_attribute_protected_visibility :
  Warning<"目标不支持'protected'可见性;使用'default' ">,
  InGroup<DiagGroup<"unsupported-visibility">>;
def err_mismatched_visibility: Error<"visibility does not match previous declaration">;
def note_previous_attribute : Note<"previous attribute is here">;
def note_conflicting_attribute : Note<"conflicting attribute is here">;
def note_attribute : Note<"attribute is here">;
def err_mismatched_ms_inheritance : Error<
  "inheritance model does not match %select{definition|previous declaration}0">;
def warn_ignored_ms_inheritance : Warning<
  "继承模型被忽略 %select{primary template|partial specialization}0">,
  InGroup<IgnoredAttributes>;
def note_previous_ms_inheritance : Note<
  "previous inheritance model specified here">;
def err_machine_mode : Error<"%select{unknown|unsupported}0 machine mode %1">;
def err_mode_not_primitive : Error<
  "mode attribute only supported for integer and floating-point types">;
def err_mode_wrong_type : Error<
  "type of machine mode does not match type of base type">;
def warn_vector_mode_deprecated : Warning<
  "不推荐使用“mode”属性指定向量类型; "
  "改用'vector_size'属性">,
  InGroup<DeprecatedAttributes>;
def err_complex_mode_vector_type : Error<
  "type of machine mode does not support base vector types">;
def err_enum_mode_vector_type : Error<
  "mode %0 is not supported for enumeration types">;
def warn_attribute_nonnull_no_pointers : Warning<
  "'nonnull' 属性应用于没有指针参数的函数">,
  InGroup<IgnoredAttributes>;
def warn_attribute_nonnull_parm_no_args : Warning<
  "'nonnull' 属性在参数上使用时不带参数">,
  InGroup<IgnoredAttributes>;
def note_declared_nonnull : Note<
  "declared %select{'returns_nonnull'|'nonnull'}0 here">;
def warn_attribute_sentinel_named_arguments : Warning<
  "'sentinel' 属性需要命名参数">,
  InGroup<IgnoredAttributes>;
def warn_attribute_sentinel_not_variadic : Warning<
  "'sentinel' 属性仅支持可变参数 %select{functions|blocks}0">,
  InGroup<IgnoredAttributes>;
def err_attribute_sentinel_less_than_zero : Error<
  "'sentinel' parameter 1 less than zero">;
def err_attribute_sentinel_not_zero_or_one : Error<
  "'sentinel' parameter 2 not 0 or 1">;
def warn_cleanup_ext : Warning<
  "GCC 不允许 'cleanup' 属性参数是 "
  "简单标识符以外的任何内容">,
  InGroup<GccCompat>;
def err_attribute_cleanup_arg_not_function : Error<
  "'cleanup' argument %select{|%1 |%1 }0is not a %select{||single }0function">;
def err_attribute_cleanup_func_must_take_one_arg : Error<
  "'cleanup' function %0 must take 1 parameter">;
def err_attribute_cleanup_func_arg_incompatible_type : Error<
  "'cleanup' function %0 parameter has "
  "%diff{type $ which is incompatible with type $|incompatible type}1,2">;
def err_attribute_regparm_wrong_platform : Error<
  "'regparm' is not valid on this platform">;
def err_attribute_regparm_invalid_number : Error<
  "'regparm' parameter must be between 0 and %0 inclusive">;
def err_attribute_not_supported_in_lang : Error<
  "%0 attribute is not supported in %select{C|C++|Objective-C}1">;
def err_attribute_not_supported_on_arch
    : Error<"%0 attribute is not supported on '%1'">;
def warn_gcc_ignores_type_attr : Warning<
  "GCC 不允许将 %0 属性写入类型">,
  InGroup<GccCompat>;

// Clang-Specific Attributes
def warn_attribute_iboutlet : Warning<
  "%0 属性只能应用于实例变量或属性">,
  InGroup<IgnoredAttributes>;
def err_iboutletcollection_type : Error<
  "invalid type %0 as argument of iboutletcollection attribute">;
def err_iboutletcollection_builtintype : Error<
  "type argument of iboutletcollection attribute cannot be a builtin type">;
def warn_iboutlet_object_type : Warning<
  "%select{instance variable|property}2 具有 %0 属性的 "
  "必须是对象类型 (无效的 %1)">, InGroup<ObjCInvalidIBOutletProperty>;
def warn_iboutletcollection_property_assign : Warning<
  "IBOutletCollection 属性应该是复制/强而不是分配">,
  InGroup<ObjCInvalidIBOutletProperty>;

def err_attribute_overloadable_mismatch : Error<
  "redeclaration of %0 must %select{not |}1have the 'overloadable' attribute">;
def note_attribute_overloadable_prev_overload : Note<
  "previous %select{unmarked |}0overload of function is here">;
def err_attribute_overloadable_no_prototype : Error<
  "'overloadable' function %0 must have a prototype">;
def err_attribute_overloadable_multiple_unmarked_overloads : Error<
  "at most one overload for a given name may lack the 'overloadable' "
  "attribute">;
def warn_attribute_no_builtin_invalid_builtin_name : Warning<
  "'%0'不是 %1 的有效内置名称">,
  InGroup<DiagGroup<"invalid-no-builtin-names">>;
def err_attribute_no_builtin_wildcard_or_builtin_name : Error<
  "empty %0 cannot be composed with named ones">;
def err_attribute_no_builtin_on_non_definition : Error<
  "%0 attribute is permitted on definitions only">;
def err_attribute_no_builtin_on_defaulted_deleted_function : Error<
  "%0 attribute has no effect on defaulted or deleted functions">;
def warn_ns_attribute_wrong_return_type : Warning<
  "%0 属性仅适用于 %select{functions|methods|properties}1 它 "
  "返回 %select{an Objective-C object|a pointer|a non-retainable pointer}2">,
  InGroup<IgnoredAttributes>;
def err_ns_attribute_wrong_parameter_type : Error<
  "%0 attribute only applies to "
  "%select{Objective-C object|pointer|pointer-to-CF-pointer}1 parameters">;
def warn_ns_attribute_wrong_parameter_type : Warning<
  "%0 属性仅适用于 "
  "%select{Objective-C object|pointer|pointer-to-CF-pointer|pointer/reference-to-OSObject-pointer}1 参数">,
  InGroup<IgnoredAttributes>;
def warn_objc_requires_super_protocol : Warning<
  "%0 属性不能适用于 %select{methods in protocols|dealloc}1">,
  InGroup<DiagGroup<"requires-super-attribute">>;
def note_protocol_decl : Note<
  "protocol is declared here">;
def note_protocol_decl_undefined : Note<
  "protocol %0 has no definition">;
def err_attribute_preferred_name_arg_invalid : Error<
  "argument %0 to 'preferred_name' attribute is not a typedef for "
  "a specialization of %1">;

// called-once attribute diagnostics.
def err_called_once_attribute_wrong_type : Error<
  "'called_once' attribute only applies to function-like parameters">;

def warn_completion_handler_never_called : Warning<
  "%select{|captured }1完成处理程序永远不会被调用">,
  InGroup<CompletionHandler>, DefaultIgnore;
def warn_called_once_never_called : Warning<
  "%select{|captured }1%0 参数被标记为 'called_once' 的永远不会被调用">,
  InGroup<CalledOnceParameter>;

def warn_completion_handler_never_called_when : Warning<
  "完全处理程序永远不会 %select{used|called}1 当 "
  "%select{taking true branch|taking false branch|"
  "handling this case|none of the cases applies|"
  "entering the loop|skipping the loop|taking one of the branches}2时">,
  InGroup<CompletionHandler>, DefaultIgnore;
def warn_called_once_never_called_when : Warning<
  "%0 参数标记为 'call_once' 永远不会 %select{used|called}1 当 "
  "%select{taking true branch|taking false branch|"
  "handling this case|none of the cases applies|"
  "entering the loop|skipping the loop|taking one of the branches}2时">,
  InGroup<CalledOnceParameter>;

def warn_completion_handler_called_twice : Warning<
  "完成处理程序被调用两次">,
  InGroup<CompletionHandler>, DefaultIgnore;
def warn_called_once_gets_called_twice : Warning<
  "%0 参数标记为 'call_once' 被调用两次">,
  InGroup<CalledOnceParameter>;
def note_called_once_gets_called_twice : Note<
  "previous call is here%select{; set to nil to indicate "
  "it cannot be called afterwards|}0">;

// objc_designated_initializer attribute diagnostics.
def warn_objc_designated_init_missing_super_call : Warning<
  "指定的初始化程序缺少对超类的指定初始化程序的'super'调用">,
  InGroup<ObjCDesignatedInit>;
def note_objc_designated_init_marked_here : Note<
  "method marked as designated initializer of the class here">;
def warn_objc_designated_init_non_super_designated_init_call : Warning<
  "指定的初始化程序应该只在'super'上调用指定的初始化程序">,
  InGroup<ObjCDesignatedInit>;
def warn_objc_designated_init_non_designated_init_call : Warning<
  "指定初始化程序调用了非指定初始化程序">,
  InGroup<ObjCDesignatedInit>;
def warn_objc_secondary_init_super_init_call : Warning<
  "便利初始化程序不应在'super'上调用初始化程序">,
  InGroup<ObjCDesignatedInit>;
def warn_objc_secondary_init_missing_init_call : Warning<
  "便利初始化程序缺少对另一个初始化程序的“自我”调用">,
  InGroup<ObjCDesignatedInit>;
def warn_objc_implementation_missing_designated_init_override : Warning<
  "未找到超类 %objcinstance0 的指定初始化程序的方法覆盖">,
  InGroup<ObjCDesignatedInit>;
def err_designated_init_attr_non_init : Error<
  "'objc_designated_initializer' attribute only applies to init methods "
  "of interface or class extension declarations">;

// objc_bridge attribute diagnostics.
def err_objc_attr_not_id : Error<
  "parameter of %0 attribute must be a single name of an Objective-C %select{class|protocol}1">;
def err_objc_attr_typedef_not_id : Error<
  "parameter of %0 attribute must be 'id' when used on a typedef">;
def err_objc_attr_typedef_not_void_pointer : Error<
  "'objc_bridge(id)' is only allowed on structs and typedefs of void pointers">;
def err_objc_cf_bridged_not_interface : Error<
  "CF object of type %0 is bridged to %1, which is not an Objective-C class">;
def err_objc_ns_bridged_invalid_cfobject : Error<
  "ObjectiveC object of type %0 is bridged to %1, which is not valid CF object">;
def warn_objc_invalid_bridge : Warning<
  "%0 桥接到 %1，而不是 %2">, InGroup<ObjCBridge>;
def warn_objc_invalid_bridge_to_cf : Warning<
  "%0 不能桥接到 %1">, InGroup<ObjCBridge>;

// objc_bridge_related attribute diagnostics.
def err_objc_bridged_related_invalid_class : Error<
  "could not find Objective-C class %0 to convert %1 to %2">;
def err_objc_bridged_related_invalid_class_name : Error<
  "%0 must be name of an Objective-C class to be able to convert %1 to %2">;
def err_objc_bridged_related_known_method : Error<
 "%0 must be explicitly converted to %1; use %select{%objcclass2|%objcinstance2}3 "
 "method for this conversion">;

def err_objc_attr_protocol_requires_definition : Error<
  "attribute %0 can only be applied to @protocol definitions, not forward declarations">;

// Swift attributes.
def warn_attr_swift_name_function
  : Warning<"%0 属性参数必须是指定 Swift 函数名称的字符串文字">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_invalid_identifier
  : Warning<"%0 属性的标识符无效 %select{base|context|parameter}1 名字">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_decl_kind
  : Warning<"%0 属性不能应用于此声明">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_subscript_invalid_parameter
  : Warning<"'subscript' 的 %0 属性必须 %select{be a getter or setter|"
        "have at least one parameter|"
        "have a 'self:' parameter}1">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_missing_parameters
  : Warning<"%0 属性缺少参数标签子句">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_setter_parameters
  : Warning<"setter 的 %0 属性必须有一个新值参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_multiple_selfs
  : Warning<"%0 属性不能指定多个 'self:' 参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_getter_parameters
  : Warning<"getter 的 %0 属性不能有除 'self:' 之外的任何参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_subscript_setter_no_newValue
  : Warning<"'subscript' setter 的 %0 属性必须有一个 'newValue:' 参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_subscript_setter_multiple_newValues
  : Warning<"'subscript' setter 的 %0 属性不能有多个 'newValue:' 参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_subscript_getter_newValue
  : Warning<"'subscript' getter 的 %0 属性不能有 'newValue:' 参数">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_num_params
  : Warning<"太 %select{few|many}0 由 %1 属性指定的签名中的参数 "
            "（预期为 %2；得到 %3）">,
    InGroup<SwiftNameAttribute>;
def warn_attr_swift_name_decl_missing_params
  : Warning<"%0 属性不能应用于一个 %select{function|method}1 "
            "没有参数">,
    InGroup<SwiftNameAttribute>;

def err_attr_swift_error_no_error_parameter : Error<
  "%0 attribute can only be applied to a %select{function|method}1 with an "
  "error parameter">;
def err_attr_swift_error_return_type : Error<
  "%0 attribute with '%1' convention can only be applied to a "
  "%select{function|method}2 returning %select{an integral type|a pointer}3">;

def err_swift_async_no_access : Error<
  "first argument to 'swift_async' must be either 'none', 'swift_private', or "
  "'not_swift_private'">;
def err_swift_async_bad_block_type : Error<
  "'swift_async' completion handler parameter must have block type returning"
  " 'void', type here is %0">;

def warn_ignored_objc_externally_retained : Warning<
  "'objc_externally_retained' 只能应用于局部变量 "
  "%select{of retainable type|with strong ownership}0">,
  InGroup<IgnoredAttributes>;

// Function Parameter Semantic Analysis.
def err_param_with_void_type : Error<"argument may not have 'void' type">;
def err_void_only_param : Error<
  "'void' must be the first and only parameter if specified">;
def err_void_param_qualified : Error<
  "'void' as parameter must not have type qualifiers">;
def err_ident_list_in_fn_declaration : Error<
  "a parameter list without types is only allowed in a function definition">;
def ext_param_not_declared : Extension<
  "parameter %0 was not declared, defaulting to type 'int'">;
def err_param_default_argument : Error<
  "C does not support default arguments">;
def err_param_default_argument_redefinition : Error<
  "redefinition of default argument">;
def ext_param_default_argument_redefinition : ExtWarn<
  err_param_default_argument_redefinition.Text>,
  InGroup<MicrosoftDefaultArgRedefinition>;
def err_param_default_argument_missing : Error<
  "missing default argument on parameter">;
def err_param_default_argument_missing_name : Error<
  "missing default argument on parameter %0">;
def err_param_default_argument_references_param : Error<
  "default argument references parameter %0">;
def err_param_default_argument_references_local : Error<
  "default argument references local variable %0 of enclosing function">;
def err_param_default_argument_references_this : Error<
  "default argument references 'this'">;
def err_param_default_argument_nonfunc : Error<
  "default arguments can only be specified for parameters in a function "
  "declaration">;
def err_param_default_argument_template_redecl : Error<
  "default arguments cannot be added to a function template that has already "
  "been declared">;
def err_param_default_argument_member_template_redecl : Error<
  "default arguments cannot be added to an out-of-line definition of a member "
  "of a %select{class template|class template partial specialization|nested "
  "class in a template}0">;
def err_param_default_argument_on_parameter_pack : Error<
  "parameter pack cannot have a default argument">;
def err_uninitialized_member_for_assign : Error<
  "cannot define the implicit copy assignment operator for %0, because "
  "non-static %select{reference|const}1 member %2 cannot use copy "
  "assignment operator">;
def err_uninitialized_member_in_ctor : Error<
  "%select{constructor for %1|"
  "implicit default constructor for %1|"
  "cannot use constructor inherited from %1:}0 must explicitly "
  "initialize the %select{reference|const}2 member %3">;
def err_default_arg_makes_ctor_special : Error<
  "addition of default argument on redeclaration makes this constructor a "
  "%select{default|copy|move}0 constructor">;

def err_use_of_default_argument_to_function_declared_later : Error<
  "use of default argument to function %0 that is declared later in class %1">;
def note_default_argument_declared_here : Note<
  "default argument declared here">;
def err_recursive_default_argument : Error<"recursive evaluation of default argument">;
def note_recursive_default_argument_used_here : Note<
  "default argument used here">;

def ext_param_promoted_not_compatible_with_prototype : ExtWarn<
  "%diff{promoted type $ of K&R function parameter is not compatible with the "
  "parameter type $|promoted type of K&R function parameter is not compatible "
  "with parameter type}0,1 declared in a previous prototype">,
  InGroup<KNRPromotedParameter>;


// C++ Overloading Semantic Analysis.
def err_ovl_diff_return_type : Error<
  "函数声明和函数定义中的返回类型必须一致">;
def err_ovl_static_nonstatic_member : Error<
  "static and non-static member functions with the same parameter types "
  "cannot be overloaded">;

let Deferrable = 1 in {

def err_ovl_no_viable_function_in_call : Error<
  "没有用于调用的匹配函数:%0">;
def err_ovl_no_viable_member_function_in_call : Error<
  "no matching member function for call to %0">;
def err_ovl_ambiguous_call : Error<
  "call to %0 is ambiguous">;
def err_ovl_deleted_call : Error<"call to deleted function %0">;
def err_ovl_ambiguous_member_call : Error<
  "call to member function %0 is ambiguous">;
def err_ovl_deleted_member_call : Error<
  "call to deleted member function %0">;
def note_ovl_too_many_candidates : Note<
    "remaining %0 candidate%s0 omitted; "
    "pass -fshow-overloads=all to show them">;

def select_ovl_candidate_kind : TextSubstitution<
  "%select{function|function|function (with reversed parameter order)|"
    "constructor|"
    "constructor (the implicit default constructor)|"
    "constructor (the implicit copy constructor)|"
    "constructor (the implicit move constructor)|"
    "function (the implicit copy assignment operator)|"
    "function (the implicit move assignment operator)|"
    "function (the implicit 'operator==' for this 'operator<=>)'|"
    "inherited constructor}0%select{| template| %2}1">;

def note_ovl_candidate : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,3"
    "%select{| has different class%diff{ (expected $ but has $)|}5,6"
    "| has different number of parameters (expected %5 but has %6)"
    "| has type mismatch at %ordinal5 parameter"
    "%diff{ (expected $ but has $)|}6,7"
    "| has different return type%diff{ ($ expected but has $)|}5,6"
    "| has different qualifiers (expected %5 but found %6)"
    "| has different exception specification}4">;

def note_ovl_candidate_explicit : Note<
    "explicit %select{constructor|conversion function|deduction guide}0 "
    "is not a candidate%select{| (explicit specifier evaluates to true)}1">;
def note_ovl_candidate_inherited_constructor : Note<
    "constructor from base class %0 inherited here">;
def note_ovl_candidate_inherited_constructor_slice : Note<
    "candidate %select{constructor|template}0 ignored: "
    "inherited constructor cannot be used to %select{copy|move}1 object">;
def note_ovl_candidate_illegal_constructor : Note<
    "candidate %select{constructor|template}0 ignored: "
    "instantiation %select{takes|would take}0 its own class type by value">;
def note_ovl_candidate_illegal_constructor_adrspace_mismatch : Note<
    "candidate constructor ignored: cannot be used to construct an object "
    "in address space %0">;
def note_ovl_candidate_bad_deduction : Note<
    "candidate template ignored: failed template argument deduction">;
def note_ovl_candidate_incomplete_deduction : Note<"candidate template ignored: "
    "couldn't infer template argument %0">;
def note_ovl_candidate_incomplete_deduction_pack : Note<
    "candidate template ignored: "
    "deduced too few arguments for expanded pack %0; no argument for %ordinal1 "
    "expanded parameter in deduced argument pack %2">;
def note_ovl_candidate_inconsistent_deduction : Note<
    "candidate template ignored: deduced %select{conflicting types|"
    "conflicting values|conflicting templates|packs of different lengths}0 "
    "for parameter %1%diff{ ($ vs. $)|}2,3">;
def note_ovl_candidate_inconsistent_deduction_types : Note<
    "candidate template ignored: deduced values %diff{"
    "of conflicting types for parameter %0 (%1 of type $ vs. %3 of type $)|"
    "%1 and %3 of conflicting types for parameter %0}2,4">;
def note_ovl_candidate_explicit_arg_mismatch_named : Note<
    "candidate template ignored: invalid explicitly-specified argument "
    "for template parameter %0">;
def note_ovl_candidate_unsatisfied_constraints : Note<
    "candidate template ignored: constraints not satisfied%0">;
def note_ovl_candidate_explicit_arg_mismatch_unnamed : Note<
    "candidate template ignored: invalid explicitly-specified argument "
    "for %ordinal0 template parameter">;
def note_ovl_candidate_instantiation_depth : Note<
    "candidate template ignored: substitution exceeded maximum template "
    "instantiation depth">;
def note_ovl_candidate_underqualified : Note<
    "candidate template ignored: cannot deduce a type for %0 that would "
    "make %2 equal %1">;
def note_ovl_candidate_substitution_failure : Note<
    "candidate template ignored: substitution failure%0%1">;
def note_ovl_candidate_disabled_by_enable_if : Note<
    "candidate template ignored: disabled by %0%1">;
def note_ovl_candidate_disabled_by_requirement : Note<
    "candidate template ignored: requirement '%0' was not satisfied%1">;
def note_ovl_candidate_has_pass_object_size_params: Note<
    "candidate address cannot be taken because parameter %0 has "
    "pass_object_size attribute">;
def err_diagnose_if_succeeded : Error<"%0">;
def warn_diagnose_if_succeeded : Warning<"%0">, InGroup<UserDefinedWarnings>,
    ShowInSystemHeader;
def note_ovl_candidate_disabled_by_function_cond_attr : Note<
    "candidate disabled: %0">;
def note_ovl_candidate_disabled_by_extension : Note<
    "candidate unavailable as it requires OpenCL extension '%0' to be enabled">;
def err_addrof_function_disabled_by_enable_if_attr : Error<
    "cannot take address of function %0 because it has one or more "
    "non-tautological enable_if conditions">;
def err_addrof_function_constraints_not_satisfied : Error<
    "cannot take address of function %0 because its constraints are not "
    "satisfied">;
def note_addrof_ovl_candidate_disabled_by_enable_if_attr : Note<
    "candidate function made ineligible by enable_if">;
def note_ovl_candidate_deduced_mismatch : Note<
    "candidate template ignored: deduced type "
    "%diff{$ of %select{|element of }4%ordinal0 parameter does not match "
    "adjusted type $ of %select{|element of }4argument"
    "|of %select{|element of }4%ordinal0 parameter does not match "
    "adjusted type of %select{|element of }4argument}1,2%3">;
def note_ovl_candidate_non_deduced_mismatch : Note<
    "candidate template ignored: could not match %diff{$ against $|types}0,1">;
// This note is needed because the above note would sometimes print two
// different types with the same name.  Remove this note when the above note
// can handle that case properly.
def note_ovl_candidate_non_deduced_mismatch_qualified : Note<
    "candidate template ignored: could not match %q0 against %q1">;

// Note that we don't treat templates differently for this diagnostic.
def note_ovl_candidate_arity : Note<"candidate "
    "%sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "requires%select{ at least| at most|}3 %4 argument%s4, but %5 "
    "%plural{1:was|:were}5 provided">;

def note_ovl_candidate_arity_one : Note<"candidate "
    "%sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "%select{requires at least|allows at most single|requires single}3 "
    "argument %4, but %plural{0:no|:%5}5 arguments were provided">;

def note_ovl_candidate_deleted : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 has been "
    "%select{explicitly made unavailable|explicitly deleted|"
    "implicitly deleted}3">;

// Giving the index of the bad argument really clutters this message, and
// it's relatively unimportant because 1) it's generally obvious which
// argument(s) are of the given object type and 2) the fix is usually
// to complete the type, which doesn't involve changes to the call line
// anyway.  If people complain, we can change it.
def note_ovl_candidate_bad_conv_incomplete : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "cannot convert argument of incomplete type "
    "%diff{$ to $|to parameter type}3,4 for "
    "%select{%ordinal6 argument|object argument}5"
    "%select{|; dereference the argument with *|"
    "; take the address of the argument with &|"
    "; remove *|"
    "; remove &}7">;
def note_ovl_candidate_bad_list_argument : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "cannot convert initializer list argument to %4">;
def note_ovl_candidate_bad_overload : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "no overload of %4 matching %3 for %ordinal5 argument">;
def note_ovl_candidate_bad_conv : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "no known conversion "
    "%diff{from $ to $|from argument type to parameter type}3,4 for "
    "%select{%ordinal6 argument|object argument}5"
    "%select{|; dereference the argument with *|"
    "; take the address of the argument with &|"
    "; remove *|"
    "; remove &}7">;
def note_ovl_candidate_bad_arc_conv : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "cannot implicitly convert argument "
    "%diff{of type $ to $|type to parameter type}3,4 for "
    "%select{%ordinal6 argument|object argument}5 under ARC">;
def note_ovl_candidate_bad_value_category : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "expects an %select{lvalue|rvalue}5 for "
    "%select{%ordinal4 argument|object argument}3">;
def note_ovl_candidate_bad_addrspace : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "cannot %select{pass pointer to|bind reference in}5 %3 "
    "%select{as a pointer to|to object in}5 %4 in %ordinal6 "
    "argument">;
def note_ovl_candidate_bad_addrspace_this : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "'this' object is in %3, but method expects object in %4">;
def note_ovl_candidate_bad_gc : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "%select{%ordinal7|'this'}6 argument (%3) has %select{no|__weak|__strong}4 "
    "ownership, but parameter has %select{no|__weak|__strong}5 ownership">;
def note_ovl_candidate_bad_ownership : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "%select{%ordinal7|'this'}6 argument (%3) has "
    "%select{no|__unsafe_unretained|__strong|__weak|__autoreleasing}4 ownership,"
    " but parameter has %select{no|__unsafe_unretained|__strong|__weak|"
    "__autoreleasing}5 ownership">;
def note_ovl_candidate_bad_cvr_this : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "'this' argument has type %3, but method is not marked "
    "%select{const|restrict|const or restrict|volatile|const or volatile|"
    "volatile or restrict|const, volatile, or restrict}4">;
def note_ovl_candidate_bad_cvr : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "%ordinal5 argument (%3) would lose "
    "%select{const|restrict|const and restrict|volatile|const and volatile|"
    "volatile and restrict|const, volatile, and restrict}4 qualifier"
    "%select{||s||s|s|s}4">;
def note_ovl_candidate_bad_unaligned : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "%ordinal5 argument (%3) would lose __unaligned qualifier">;
def note_ovl_candidate_bad_base_to_derived_conv : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "cannot %select{convert from|convert from|bind}3 "
    "%select{base class pointer|superclass|base class object of type}3 %4 to "
    "%select{derived class pointer|subclass|derived class reference}3 %5 for "
    "%ordinal6 argument">;
def note_ovl_candidate_bad_target : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: "
    "call to "
    "%select{__device__|__global__|__host__|__host__ __device__|invalid}3 function from"
    " %select{__device__|__global__|__host__|__host__ __device__|invalid}4 function">;
def note_ovl_candidate_constraints_not_satisfied : Note<
    "candidate %sub{select_ovl_candidate_kind}0,1,2 not viable: constraints "
    "not satisfied">;
def note_implicit_member_target_infer_collision : Note<
    "implicit %sub{select_special_member_kind}0 inferred target collision: call to both "
    "%select{__device__|__global__|__host__|__host__ __device__}1 and "
    "%select{__device__|__global__|__host__|__host__ __device__}2 members">;

def note_ambiguous_type_conversion: Note<
    "because of ambiguity in conversion %diff{of $ to $|between types}0,1">;
def note_ovl_builtin_candidate : Note<"built-in candidate %0">;
def err_ovl_no_viable_function_in_init : Error<
  "no matching constructor for initialization of %0">;
def err_ovl_no_conversion_in_cast : Error<
  "cannot convert %1 to %2 without a conversion operator">;
def err_ovl_no_viable_conversion_in_cast : Error<
  "no matching conversion for %select{|static_cast|reinterpret_cast|"
  "dynamic_cast|C-style cast|functional-style cast|}0 from %1 to %2">;
def err_ovl_ambiguous_conversion_in_cast : Error<
  "ambiguous conversion for %select{|static_cast|reinterpret_cast|"
  "dynamic_cast|C-style cast|functional-style cast|}0 from %1 to %2">;
def err_ovl_deleted_conversion_in_cast : Error<
  "%select{|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|}0 from %1 to %2 uses deleted function">;
def err_ovl_ambiguous_init : Error<"call to constructor of %0 is ambiguous">;
def err_ref_init_ambiguous : Error<
  "reference initialization of type %0 with initializer of type %1 is ambiguous">;
def err_ovl_deleted_init : Error<
  "call to deleted constructor of %0">;
def err_ovl_deleted_special_init : Error<
  "call to implicitly-deleted %select{default constructor|copy constructor|"
  "move constructor|copy assignment operator|move assignment operator|"
  "destructor|function}0 of %1">;
def err_ovl_ambiguous_oper_unary : Error<
  "use of overloaded operator '%0' is ambiguous (operand type %1)">;
def err_ovl_ambiguous_oper_binary : Error<
  "use of overloaded operator '%0' is ambiguous (with operand types %1 and %2)">;
def ext_ovl_ambiguous_oper_binary_reversed : ExtWarn<
  "ISO C++20 considers use of overloaded operator '%0' (with operand types %1 "
  "and %2) to be ambiguous despite there being a unique best viable function"
  "%select{ with non-reversed arguments|}3">,
  InGroup<DiagGroup<"ambiguous-reversed-operator">>, SFINAEFailure;
def note_ovl_ambiguous_oper_binary_reversed_self : Note<
  "ambiguity is between a regular call to this operator and a call with the "
  "argument order reversed">;
def note_ovl_ambiguous_oper_binary_selected_candidate : Note<
  "candidate function with non-reversed arguments">;
def note_ovl_ambiguous_oper_binary_reversed_candidate : Note<
  "ambiguous candidate function with reversed arguments">;
def err_ovl_no_viable_oper : Error<"no viable overloaded '%0'">;
def note_assign_lhs_incomplete : Note<"type %0 is incomplete">;
def err_ovl_deleted_oper : Error<
  "overload resolution selected deleted operator '%0'">;
def err_ovl_deleted_special_oper : Error<
  "object of type %0 cannot be %select{constructed|copied|moved|assigned|"
  "assigned|destroyed}1 because its %sub{select_special_member_kind}1 is "
  "implicitly deleted">;
def err_ovl_deleted_comparison : Error<
  "object of type %0 cannot be compared because its %1 is implicitly deleted">;
def err_ovl_rewrite_equalequal_not_bool : Error<
  "return type %0 of selected 'operator==' function for rewritten "
  "'%1' comparison is not 'bool'">;
def ext_ovl_rewrite_equalequal_not_bool : ExtWarn<
  "ISO C++20 requires return type of selected 'operator==' function for "
  "rewritten '%1' comparison to be 'bool', not %0">,
  InGroup<DiagGroup<"rewrite-not-bool">>, SFINAEFailure;
def err_ovl_no_viable_subscript :
    Error<"no viable overloaded operator[] for type %0">;
def err_ovl_no_oper :
    Error<"type %0 does not provide a %select{subscript|call}1 operator">;
def err_ovl_unresolvable : Error<
  "reference to %select{overloaded|multiversioned}1 function could not be "
  "resolved; did you mean to call it%select{| with no arguments}0?">;
def err_bound_member_function : Error<
  "reference to non-static member function must be called"
  "%select{|; did you mean to call it with no arguments?}0">;
def note_possible_target_of_call : Note<"possible target for call">;

def err_ovl_no_viable_object_call : Error<
  "no matching function for call to object of type %0">;
def err_ovl_ambiguous_object_call : Error<
  "call to object of type %0 is ambiguous">;
def err_ovl_deleted_object_call : Error<
  "call to deleted function call operator in type %0">;
def note_ovl_surrogate_cand : Note<"conversion candidate of type %0">;
def err_member_call_without_object : Error<
  "call to non-static member function without an object argument">;

// C++ Address of Overloaded Function
def err_addr_ovl_no_viable : Error<
  "address of overloaded function %0 does not match required type %1">;
def err_addr_ovl_ambiguous : Error<
  "address of overloaded function %0 is ambiguous">;
def err_addr_ovl_not_func_ptrref : Error<
  "address of overloaded function %0 cannot be converted to type %1">;
def err_addr_ovl_no_qualifier : Error<
  "cannot form member pointer of type %0 without '&' and class name">;

} // let Deferrable

// C++11 Literal Operators
def err_ovl_no_viable_literal_operator : Error<
  "no matching literal operator for call to %0"
  "%select{| with argument of type %2| with arguments of types %2 and %3}1"
  "%select{| or 'const char *'}4"
  "%select{|, and no matching literal operator template}5">;

// C++ Template Declarations
def err_template_param_shadow : Error<
  "declaration of %0 shadows template parameter">;
def ext_template_param_shadow : ExtWarn<
  err_template_param_shadow.Text>, InGroup<MicrosoftTemplateShadow>;
def note_template_param_here : Note<"template parameter is declared here">;
def warn_template_export_unsupported : Warning<
  "不支持导出的模板">;
def err_template_outside_namespace_or_class_scope : Error<
  "templates can only be declared in namespace or class scope">;
def err_template_inside_local_class : Error<
  "templates cannot be declared inside of a local class">;
def err_template_linkage : Error<"templates must have C++ linkage">;
def err_template_typedef : Error<"a typedef cannot be a template">;
def err_template_unnamed_class : Error<
  "cannot declare a class template with no name">;
def err_template_param_list_different_arity : Error<
  "%select{too few|too many}0 template parameters in template "
  "%select{|template parameter }1redeclaration">;
def note_template_param_list_different_arity : Note<
  "%select{too few|too many}0 template parameters in template template "
  "argument">;
def note_template_prev_declaration : Note<
  "previous template %select{declaration|template parameter}0 is here">;
def err_template_param_different_kind : Error<
  "template parameter has a different kind in template "
  "%select{|template parameter }0redeclaration">;
def note_template_param_different_kind : Note<
  "template parameter has a different kind in template argument">;

def err_invalid_decl_specifier_in_nontype_parm : Error<
  "invalid declaration specifier in template non-type parameter">;

def err_template_nontype_parm_different_type : Error<
  "template non-type parameter has a different type %0 in template "
  "%select{|template parameter }1redeclaration">;

def note_template_nontype_parm_different_type : Note<
  "template non-type parameter has a different type %0 in template argument">;
def note_template_nontype_parm_prev_declaration : Note<
  "previous non-type template parameter with type %0 is here">;
def err_template_nontype_parm_bad_type : Error<
  "a non-type template parameter cannot have type %0">;
def err_template_nontype_parm_bad_structural_type : Error<
  "a non-type template parameter cannot have type %0 before C++20">;
def err_template_nontype_parm_incomplete : Error<
  "non-type template parameter has incomplete type %0">;
def err_template_nontype_parm_not_literal : Error<
  "non-type template parameter has non-literal type %0">;
def err_template_nontype_parm_rvalue_ref : Error<
  "non-type template parameter has rvalue reference type %0">;
def err_template_nontype_parm_not_structural : Error<
  "type %0 of non-type template parameter is not a structural type">;
def note_not_structural_non_public : Note<
  "%0 is not a structural type because it has a "
  "%select{non-static data member|base class}1 that is not public">;
def note_not_structural_mutable_field : Note<
  "%0 is not a structural type because it has a mutable "
  "non-static data member">;
def note_not_structural_rvalue_ref_field : Note<
  "%0 is not a structural type because it has a non-static data member "
  "of rvalue reference type">;
def note_not_structural_subobject : Note<
  "%0 is not a structural type because it has a "
  "%select{non-static data member|base class}1 of non-structural type %2">;
def warn_cxx17_compat_template_nontype_parm_type : Warning<
  "%0 类型的非类型模板参数 "
  "与 C++20 之前的 C++ 标准不兼容">,
  DefaultIgnore, InGroup<CXXPre20Compat>;
def warn_cxx14_compat_template_nontype_parm_auto_type : Warning<
  "用 %0 声明的非类型模板参数 "
  "与 C++17 之前的 C++ 标准不兼容">,
  DefaultIgnore, InGroup<CXXPre17Compat>;
def err_template_param_default_arg_redefinition : Error<
  "template parameter redefines default argument">;
def note_template_param_prev_default_arg : Note<
  "previous default template argument defined here">;
def err_template_param_default_arg_missing : Error<
  "template parameter missing a default argument">;
def ext_template_parameter_default_in_function_template : ExtWarn<
  "default template arguments for a function template are a C++11 extension">,
  InGroup<CXX11>;
def warn_cxx98_compat_template_parameter_default_in_function_template : Warning<
  "函数模板的默认模板参数与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_template_parameter_default_template_member : Error<
  "cannot add a default template argument to the definition of a member of a "
  "class template">;
def err_template_parameter_default_friend_template : Error<
  "default template argument not permitted on a friend template">;
def err_template_template_parm_no_parms : Error<
  "template template parameter must have its own template parameters">;

def ext_variable_template : ExtWarn<"variable templates are a C++14 extension">,
  InGroup<CXX14>;
def warn_cxx11_compat_variable_template : Warning<
  "变量模板与 C++14 之前的 C++ 标准不兼容">,
  InGroup<CXXPre14Compat>, DefaultIgnore;
def err_template_variable_noparams : Error<
  "extraneous 'template<>' in declaration of variable %0">;
def err_template_member : Error<"member %0 declared as a template">;
def err_template_member_noparams : Error<
  "extraneous 'template<>' in declaration of member %0">;
def err_template_tag_noparams : Error<
  "extraneous 'template<>' in declaration of %0 %1">;

def warn_cxx17_compat_adl_only_template_id : Warning<
  "在具有显式模板参数的函数调用中 "
  "使用没有事先函数模板声明的函数模板名称 "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20Compat>, DefaultIgnore;
def ext_adl_only_template_id : ExtWarn<
  "use of function template name with no prior declaration in function call "
  "with explicit template arguments is a C++20 extension">, InGroup<CXX20>;

// C++ Template Argument Lists
def err_template_missing_args : Error<
  "use of "
  "%select{class template|function template|variable template|alias template|"
  "template template parameter|concept|template}0 %1 requires template "
  "arguments">;
def err_template_arg_list_different_arity : Error<
  "%select{too few|too many}0 template arguments for "
  "%select{class template|function template|variable template|alias template|"
  "template template parameter|concept|template}1 %2">;
def note_template_decl_here : Note<"template is declared here">;
def err_template_arg_must_be_type : Error<
  "template argument for template type parameter must be a type">;
def err_template_arg_must_be_type_suggest : Error<
  "template argument for template type parameter must be a type; "
  "did you forget 'typename'?">;
def ext_ms_template_type_arg_missing_typename : ExtWarn<
  "template argument for template type parameter must be a type; "
  "omitted 'typename' is a Microsoft extension">,
  InGroup<MicrosoftTemplate>;
def err_template_arg_must_be_expr : Error<
  "template argument for non-type template parameter must be an expression">;
def err_template_arg_nontype_ambig : Error<
  "template argument for non-type template parameter is treated as function type %0">;
def err_template_arg_must_be_template : Error<
  "template argument for template template parameter must be a class template%select{| or type alias template}0">;
def ext_template_arg_local_type : ExtWarn<
  "template argument uses local type %0">, InGroup<LocalTypeTemplateArgs>;
def ext_template_arg_unnamed_type : ExtWarn<
  "template argument uses unnamed type">, InGroup<UnnamedTypeTemplateArgs>;
def warn_cxx98_compat_template_arg_local_type : Warning<
  "本地类型 %0 作为模板参数与 C++98 不兼容">,
  InGroup<CXX98CompatLocalTypeTemplateArgs>, DefaultIgnore;
def warn_cxx98_compat_template_arg_unnamed_type : Warning<
  "未命名类型作为模板参数与 C++98 不兼容">,
  InGroup<CXX98CompatUnnamedTypeTemplateArgs>, DefaultIgnore;
def note_template_unnamed_type_here : Note<
  "unnamed type used in template argument was declared here">;
def err_template_arg_overload_type : Error<
  "template argument is the type of an unresolved overloaded function">;
def err_template_arg_not_valid_template : Error<
  "template argument does not refer to a class or alias template, or template "
  "template parameter">;
def note_template_arg_refers_here_func : Note<
  "template argument refers to function template %0, here">;
def err_template_arg_template_params_mismatch : Error<
  "template template argument has different template parameters than its "
  "corresponding template template parameter">;
def err_template_arg_not_integral_or_enumeral : Error<
  "non-type template argument of type %0 must have an integral or enumeration"
  " type">;
def err_template_arg_not_ice : Error<
  "non-type template argument of type %0 is not an integral constant "
  "expression">;
def err_template_arg_not_address_constant : Error<
  "non-type template argument of type %0 is not a constant expression">;
def warn_cxx98_compat_template_arg_null : Warning<
  "使用空指针作为非类型模板参数 "
  "与 C++98 不兼容">, InGroup<CXX98Compat>, DefaultIgnore;
def err_template_arg_untyped_null_constant : Error<
  "null non-type template argument must be cast to template parameter type %0">;
def err_template_arg_wrongtype_null_constant : Error<
  "null non-type template argument of type %0 does not match template parameter "
  "of type %1">;
def err_non_type_template_parm_type_deduction_failure : Error<
  "non-type template parameter %0 with type %1 has incompatible initializer of type %2">;
def err_deduced_non_type_template_arg_type_mismatch : Error<
  "deduced non-type template argument does not have the same type as the "
  "corresponding template parameter%diff{ ($ vs $)|}0,1">;
def err_non_type_template_arg_subobject : Error<
  "non-type template argument refers to subobject '%0'">;
def err_non_type_template_arg_addr_label_diff : Error<
  "template argument / label address difference / what did you expect?">;
def err_non_type_template_arg_unsupported : Error<
  "sorry, non-type template argument of type %0 is not yet supported">;
def err_template_arg_not_convertible : Error<
  "non-type template argument of type %0 cannot be converted to a value "
  "of type %1">;
def warn_template_arg_negative : Warning<
  "值为 '%0' 的非类型模板参数转换为 '%1' "
  "对于 %2 类型的无符号模板参数">, InGroup<Conversion>, DefaultIgnore;
def warn_template_arg_too_large : Warning<
  "非类型模板参数值'%0'截断为'%1' "
  "对于 %2 类型的模板参数">, InGroup<Conversion>, DefaultIgnore;
def err_template_arg_no_ref_bind : Error<
  "non-type template parameter of reference type "
  "%diff{$ cannot bind to template argument of type $"
  "|cannot bind to template of incompatible argument type}0,1">;
def err_template_arg_ref_bind_ignores_quals : Error<
  "reference binding of non-type template parameter "
  "%diff{of type $ to template argument of type $|to template argument}0,1 "
  "ignores qualifiers">;
def err_template_arg_not_decl_ref : Error<
  "non-type template argument does not refer to any declaration">;
def err_template_arg_not_address_of : Error<
  "non-type template argument for template parameter of pointer type %0 must "
  "have its address taken">;
def err_template_arg_address_of_non_pointer : Error<
  "address taken in non-type template argument for template parameter of "
  "reference type %0">;
def err_template_arg_reference_var : Error<
  "non-type template argument of reference type %0 is not an object">;
def err_template_arg_field : Error<
  "non-type template argument refers to non-static data member %0">;
def err_template_arg_method : Error<
  "non-type template argument refers to non-static member function %0">;
def err_template_arg_object_no_linkage : Error<
  "non-type template argument refers to %select{function|object}0 %1 that "
  "does not have linkage">;
def warn_cxx98_compat_template_arg_object_internal : Warning<
  "引用具有内部链接的 %select{function|object}0 %1 的非类型模板参数 "
  "与 C++98 不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def ext_template_arg_object_internal : ExtWarn<
  "non-type template argument referring to %select{function|object}0 %1 with "
  "internal linkage is a C++11 extension">, InGroup<CXX11>;
def err_template_arg_thread_local : Error<
  "non-type template argument refers to thread-local object">;
def note_template_arg_internal_object : Note<
  "non-type template argument refers to %select{function|object}0 here">;
def note_template_arg_refers_here : Note<
  "non-type template argument refers here">;
def err_template_arg_not_object_or_func : Error<
  "non-type template argument does not refer to an object or function">;
def err_template_arg_not_pointer_to_member_form : Error<
  "non-type template argument is not a pointer to member constant">;
def err_template_arg_member_ptr_base_derived_not_supported : Error<
  "sorry, non-type template argument of pointer-to-member type %1 that refers "
  "to member %q0 of a different class is not supported yet">;
def ext_template_arg_extra_parens : ExtWarn<
  "address non-type template argument cannot be surrounded by parentheses">;
def warn_cxx98_compat_template_arg_extra_parens : Warning<
  "围绕地址非类型模板参数的冗余括号 "
  "与 C++98 不兼容">, InGroup<CXX98Compat>, DefaultIgnore;
def err_pointer_to_member_type : Error<
  "invalid use of pointer to member type after %select{.*|->*}0">;
def err_pointer_to_member_call_drops_quals : Error<
  "call to pointer to member function of type %0 drops '%1' qualifier%s2">;
def err_pointer_to_member_oper_value_classify: Error<
  "pointer-to-member function type %0 can only be called on an "
  "%select{rvalue|lvalue}1">;
def ext_pointer_to_const_ref_member_on_rvalue : Extension<
  "invoking a pointer to a 'const &' member function on an rvalue is a C++20 extension">,
  InGroup<CXX20>, SFINAEFailure;
def warn_cxx17_compat_pointer_to_const_ref_member_on_rvalue : Warning<
  "在右值上调用指向'const &'成员函数的指针 "
  "与 C++20 之前的 C++ 标准不兼容">,
  InGroup<CXXPre20CompatPedantic>, DefaultIgnore;
def ext_ms_deref_template_argument: ExtWarn<
  "non-type template argument containing a dereference operation is a "
  "Microsoft extension">, InGroup<MicrosoftTemplate>;
def ext_ms_delayed_template_argument: ExtWarn<
  "using the undeclared type %0 as a default template argument is a "
  "Microsoft extension">, InGroup<MicrosoftTemplate>;
def err_template_arg_deduced_incomplete_pack : Error<
  "deduced incomplete pack %0 for template parameter %1">;

// C++ template specialization
def err_template_spec_unknown_kind : Error<
  "can only provide an explicit specialization for a class template, function "
  "template, variable template, or a member function, static data member, "
  "%select{or member class|member class, or member enumeration}0 of a "
  "class template">;
def note_specialized_entity : Note<
  "explicitly specialized declaration is here">;
def note_explicit_specialization_declared_here : Note<
  "explicit specialization declared here">;
def err_template_spec_decl_function_scope : Error<
  "explicit specialization of %0 in function scope">;
def err_template_spec_decl_friend : Error<
  "cannot declare an explicit specialization in a friend">;
def err_template_spec_redecl_out_of_scope : Error<
  "%select{class template|class template partial|variable template|"
  "variable template partial|function template|member "
  "function|static data member|member class|member enumeration}0 "
  "specialization of %1 not in %select{a namespace enclosing %2|"
  "class %2 or an enclosing namespace}3">;
def ext_ms_template_spec_redecl_out_of_scope: ExtWarn<
  "%select{class template|class template partial|variable template|"
  "variable template partial|function template|member "
  "function|static data member|member class|member enumeration}0 "
  "specialization of %1 not in %select{a namespace enclosing %2|"
  "class %2 or an enclosing namespace}3 "
  "is a Microsoft extension">, InGroup<MicrosoftTemplate>;
def err_template_spec_redecl_global_scope : Error<
  "%select{class template|class template partial|variable template|"
  "variable template partial|function template|member "
  "function|static data member|member class|member enumeration}0 "
  "specialization of %1 must occur at global scope">;
def err_spec_member_not_instantiated : Error<
  "specialization of member %q0 does not specialize an instantiated member">;
def note_specialized_decl : Note<"attempt to specialize declaration here">;
def err_specialization_after_instantiation : Error<
  "explicit specialization of %0 after instantiation">;
def note_instantiation_required_here : Note<
  "%select{implicit|explicit}0 instantiation first required here">;
def err_template_spec_friend : Error<
  "template specialization declaration cannot be a friend">;
def err_template_spec_default_arg : Error<
  "default argument not permitted on an explicit "
  "%select{instantiation|specialization}0 of function %1">;
def err_not_class_template_specialization : Error<
  "cannot specialize a %select{dependent template|template template "
  "parameter}0">;
def ext_explicit_specialization_storage_class : ExtWarn<
  "explicit specialization cannot have a storage class">;
def err_explicit_specialization_inconsistent_storage_class : Error<
  "explicit specialization has extraneous, inconsistent storage class "
  "'%select{none|extern|static|__private_extern__|auto|register}0'">;
def err_dependent_function_template_spec_no_match : Error<
  "no candidate function template was found for dependent"
  " friend function template specialization">;
def note_dependent_function_template_spec_discard_reason : Note<
  "candidate ignored: %select{not a function template"
  "|not a member of the enclosing namespace;"
  " did you mean to explicitly qualify the specialization?}0">;

// C++ class template specializations and out-of-line definitions
def err_template_spec_needs_header : Error<
  "template specialization requires 'template<>'">;
def err_template_spec_needs_template_parameters : Error<
  "template specialization or definition requires a template parameter list "
  "corresponding to the nested type %0">;
def err_template_param_list_matches_nontemplate : Error<
  "template parameter list matching the non-templated nested type %0 should "
  "be empty ('template<>')">;
def err_alias_template_extra_headers : Error<
  "extraneous template parameter list in alias template declaration">;
def err_template_spec_extra_headers : Error<
  "extraneous template parameter list in template specialization or "
  "out-of-line template definition">;
def warn_template_spec_extra_headers : Warning<
  "模板特化中的无关模板参数列表">;
def note_explicit_template_spec_does_not_need_header : Note<
  "'template<>' header not required for explicitly-specialized class %0 "
  "declared here">;
def err_template_qualified_declarator_no_match : Error<
  "nested name specifier '%0' for declaration does not refer into a class, "
  "class template or class template partial specialization">;
def err_specialize_member_of_template : Error<
  "cannot specialize %select{|(with 'template<>') }0a member of an "
  "unspecialized template">;

// C++ Class Template Partial Specialization
def err_default_arg_in_partial_spec : Error<
    "default template argument in a class template partial specialization">;
def err_dependent_non_type_arg_in_partial_spec : Error<
    "type of specialized non-type template argument depends on a template "
    "parameter of the partial specialization">;
def note_dependent_non_type_default_arg_in_partial_spec : Note<
    "template parameter is used in default argument declared here">;
def err_dependent_typed_non_type_arg_in_partial_spec : Error<
    "non-type template argument specializes a template parameter with "
    "dependent type %0">;
def err_partial_spec_args_match_primary_template : Error<
    "%select{class|variable}0 template partial specialization does not "
    "specialize any template argument; to %select{declare|define}1 the "
    "primary template, remove the template argument list">;
def ext_partial_spec_not_more_specialized_than_primary : ExtWarn<
    "%select{class|variable}0 template partial specialization is not "
    "more specialized than the primary template">, DefaultError,
    InGroup<DiagGroup<"invalid-partial-specialization">>;
def note_partial_spec_not_more_specialized_than_primary : Note<"%0">;
def ext_partial_specs_not_deducible : ExtWarn<
    "%select{class|variable}0 template partial specialization contains "
    "%select{a template parameter|template parameters}1 that cannot be "
    "deduced; this partial specialization will never be used">,
    DefaultError, InGroup<DiagGroup<"unusable-partial-specialization">>;
def note_non_deducible_parameter : Note<
    "non-deducible template parameter %0">;
def err_partial_spec_ordering_ambiguous : Error<
    "ambiguous partial specializations of %0">;
def note_partial_spec_match : Note<"partial specialization matches %0">;
def err_partial_spec_redeclared : Error<
  "class template partial specialization %0 cannot be redeclared">;
def note_partial_specialization_declared_here : Note<
  "explicit specialization declared here">;
def note_prev_partial_spec_here : Note<
  "previous declaration of class template partial specialization %0 is here">;
def err_partial_spec_fully_specialized : Error<
  "partial specialization of %0 does not use any of its template parameters">;

// C++ Variable Template Partial Specialization
def err_var_partial_spec_redeclared : Error<
  "variable template partial specialization %0 cannot be redefined">;
def note_var_prev_partial_spec_here : Note<
  "previous declaration of variable template partial specialization is here">;
def err_var_spec_no_template : Error<
  "no variable template matches%select{| partial}0 specialization">;
def err_var_spec_no_template_but_method : Error<
  "no variable template matches specialization; "
  "did you mean to use %0 as function template instead?">;

// C++ Function template specializations
def err_function_template_spec_no_match : Error<
    "no function template matches function template specialization %0">;
def err_function_template_spec_ambiguous : Error<
    "function template specialization %0 ambiguously refers to more than one "
    "function template; explicitly specify%select{| additional}1 template "
    "arguments to identify a particular function template">;
def note_function_template_spec_matched : Note<
    "function template %q0 matches specialization %1">;
def err_function_template_partial_spec : Error<
    "function template partial specialization is not allowed">;

// C++ Template Instantiation
def err_template_recursion_depth_exceeded : Error<
  "recursive template instantiation exceeded maximum depth of %0">,
  DefaultFatal, NoSFINAE;
def note_template_recursion_depth : Note<
  "use -ftemplate-depth=N to increase recursive template instantiation depth">;

def err_template_instantiate_within_definition : Error<
  "%select{implicit|explicit}0 instantiation of template %1 within its"
  " own definition">;
def err_template_instantiate_undefined : Error<
  "%select{implicit|explicit}0 instantiation of undefined template %1">;
def err_implicit_instantiate_member_undefined : Error<
  "implicit instantiation of undefined member %0">;
def note_template_class_instantiation_was_here : Note<
  "class template %0 was instantiated here">;
def note_template_class_explicit_specialization_was_here : Note<
  "class template %0 was explicitly specialized here">;
def note_template_class_instantiation_here : Note<
  "in instantiation of template class %q0 requested here">;
def note_template_member_class_here : Note<
  "in instantiation of member class %q0 requested here">;
def note_template_member_function_here : Note<
  "in instantiation of member function %q0 requested here">;
def note_function_template_spec_here : Note<
  "in instantiation of function template specialization %q0 requested here">;
def note_template_static_data_member_def_here : Note<
  "in instantiation of static data member %q0 requested here">;
def note_template_variable_def_here : Note<
  "in instantiation of variable template specialization %q0 requested here">;
def note_template_enum_def_here : Note<
  "in instantiation of enumeration %q0 requested here">;
def note_template_nsdmi_here : Note<
  "in instantiation of default member initializer %q0 requested here">;
def note_template_type_alias_instantiation_here : Note<
  "in instantiation of template type alias %0 requested here">;
def note_template_exception_spec_instantiation_here : Note<
  "in instantiation of exception specification for %0 requested here">;
def note_template_requirement_instantiation_here : Note<
  "in instantiation of requirement here">;
def warn_var_template_missing : Warning<"变量 %q0 的实例化 "
  "required here, but no definition is available">,
  InGroup<UndefinedVarTemplate>;
def warn_func_template_missing : Warning<"函数 %q0 的实例化 "
  "required here, but no definition is available">,
  InGroup<UndefinedFuncTemplate>, DefaultIgnore;
def note_forward_template_decl : Note<
  "forward declaration of template entity is here">;
def note_inst_declaration_hint : Note<"add an explicit instantiation "
  "declaration to suppress this warning if %q0 is explicitly instantiated in "
  "another translation unit">;
def note_evaluating_exception_spec_here : Note<
  "in evaluation of exception specification for %q0 needed here">;

def note_default_arg_instantiation_here : Note<
  "in instantiation of default argument for '%0' required here">;
def note_default_function_arg_instantiation_here : Note<
  "in instantiation of default function argument expression "
  "for '%0' required here">;
def note_explicit_template_arg_substitution_here : Note<
  "while substituting explicitly-specified template arguments into function "
  "template %0 %1">;
def note_function_template_deduction_instantiation_here : Note<
  "while substituting deduced template arguments into function template %0 "
  "%1">;
def note_deduced_template_arg_substitution_here : Note<
  "during template argument deduction for %select{class|variable}0 template "
  "%select{partial specialization |}1%2 %3">;
def note_prior_template_arg_substitution : Note<
  "while substituting prior template arguments into %select{non-type|template}0"
  " template parameter%1 %2">;
def note_template_default_arg_checking : Note<
  "while checking a default template argument used here">;
def note_concept_specialization_here : Note<
  "while checking the satisfaction of concept '%0' requested here">;
def note_nested_requirement_here : Note<
  "while checking the satisfaction of nested requirement requested here">;
def note_checking_constraints_for_template_id_here : Note<
  "while checking constraint satisfaction for template '%0' required here">;
def note_checking_constraints_for_var_spec_id_here : Note<
  "while checking constraint satisfaction for variable template "
  "partial specialization '%0' required here">;
def note_checking_constraints_for_class_spec_id_here : Note<
  "while checking constraint satisfaction for class template partial "
  "specialization '%0' required here">;
def note_checking_constraints_for_function_here : Note<
  "while checking constraint satisfaction for function '%0' required here">;
def note_constraint_substitution_here : Note<
  "while substituting template arguments into constraint expression here">;
def note_constraint_normalization_here : Note<
  "while calculating associated constraint of template '%0' here">;
def note_parameter_mapping_substitution_here : Note<
  "while substituting into concept arguments here; substitution failures not "
  "allowed in concept arguments">;
def note_instantiation_contexts_suppressed : Note<
  "(skipping %0 context%s0 in backtrace; use -ftemplate-backtrace-limit=0 to "
  "see all)">;

def err_field_instantiates_to_function : Error<
  "data member instantiated with function type %0">;
def err_variable_instantiates_to_function : Error<
  "%select{variable|static data member}0 instantiated with function type %1">;
def err_nested_name_spec_non_tag : Error<
  "type %0 cannot be used prior to '::' because it has no members">;

def err_using_pack_expansion_empty : Error<
  "%select{|member}0 using declaration %1 instantiates to an empty pack">;

// C++ Explicit Instantiation
def err_explicit_instantiation_duplicate : Error<
    "duplicate explicit instantiation of %0">;
def ext_explicit_instantiation_duplicate : ExtWarn<
    "duplicate explicit instantiation of %0 ignored as a Microsoft extension">,
    InGroup<MicrosoftTemplate>;
def note_previous_explicit_instantiation : Note<
    "previous explicit instantiation is here">;
def warn_explicit_instantiation_after_specialization : Warning<
  "显式特化后发生的 %0  "
  "显式实例化无效">,
  InGroup<DiagGroup<"instantiation-after-specialization">>;
def note_previous_template_specialization : Note<
    "previous template specialization is here">;
def err_explicit_instantiation_nontemplate_type : Error<
    "explicit instantiation of non-templated type %0">;
def note_nontemplate_decl_here : Note<
    "non-templated declaration is here">;
def err_explicit_instantiation_in_class : Error<
  "explicit instantiation of %0 in class scope">;
def err_explicit_instantiation_out_of_scope : Error<
  "explicit instantiation of %0 not in a namespace enclosing %1">;
def err_explicit_instantiation_must_be_global : Error<
  "explicit instantiation of %0 must occur at global scope">;
def warn_explicit_instantiation_out_of_scope_0x : Warning<
  "%0的显式实例化不在包含%1的命名空间中">,
  InGroup<CXX11Compat>, DefaultIgnore;
def warn_explicit_instantiation_must_be_global_0x : Warning<
  "%0的显式实例化必须在全局范围内进行">,
  InGroup<CXX11Compat>, DefaultIgnore;

def err_explicit_instantiation_requires_name : Error<
  "explicit instantiation declaration requires a name">;
def err_explicit_instantiation_of_typedef : Error<
  "explicit instantiation of typedef %0">;
def err_explicit_instantiation_storage_class : Error<
  "explicit instantiation cannot have a storage class">;
def err_explicit_instantiation_internal_linkage : Error<
  "explicit instantiation declaration of %0 with internal linkage">;
def err_explicit_instantiation_not_known : Error<
  "explicit instantiation of %0 does not refer to a function template, "
  "variable template, member function, member class, or static data member">;
def note_explicit_instantiation_here : Note<
  "explicit instantiation refers here">;
def err_explicit_instantiation_data_member_not_instantiated : Error<
  "explicit instantiation refers to static data member %q0 that is not an "
  "instantiation">;
def err_explicit_instantiation_member_function_not_instantiated : Error<
  "explicit instantiation refers to member function %q0 that is not an "
  "instantiation">;
def err_explicit_instantiation_ambiguous : Error<
  "partial ordering for explicit instantiation of %0 is ambiguous">;
def note_explicit_instantiation_candidate : Note<
  "explicit instantiation candidate function %q0 template here %1">;
def err_explicit_instantiation_inline : Error<
  "explicit instantiation cannot be 'inline'">;
def warn_explicit_instantiation_inline_0x : Warning<
  "显式实例化不能为'inline'">, InGroup<CXX11Compat>,
  DefaultIgnore;
def err_explicit_instantiation_constexpr : Error<
  "explicit instantiation cannot be 'constexpr'">;
def ext_explicit_instantiation_without_qualified_id : Extension<
  "qualifier in explicit instantiation of %q0 requires a template-id "
  "(a typedef is not permitted)">;
def err_explicit_instantiation_without_template_id : Error<
  "explicit instantiation of %q0 must specify a template argument list">;
def err_explicit_instantiation_unqualified_wrong_namespace : Error<
  "explicit instantiation of %q0 must occur in namespace %1">;
def warn_explicit_instantiation_unqualified_wrong_namespace_0x : Warning<
  "%q0的显式实例化必须在命名空间%1中发生">,
  InGroup<CXX11Compat>, DefaultIgnore;
def err_explicit_instantiation_undefined_member : Error<
  "explicit instantiation of undefined %select{member class|member function|"
  "static data member}0 %1 of class template %2">;
def err_explicit_instantiation_undefined_func_template : Error<
  "explicit instantiation of undefined function template %0">;
def err_explicit_instantiation_undefined_var_template : Error<
  "explicit instantiation of undefined variable template %q0">;
def err_explicit_instantiation_declaration_after_definition : Error<
  "explicit instantiation declaration (with 'extern') follows explicit "
  "instantiation definition (without 'extern')">;
def note_explicit_instantiation_definition_here : Note<
  "explicit instantiation definition is here">;
def err_invalid_var_template_spec_type : Error<"type %2 "
  "of %select{explicit instantiation|explicit specialization|"
  "partial specialization|redeclaration}0 of %1 does not match"
  " expected type %3">;
def err_mismatched_exception_spec_explicit_instantiation : Error<
  "exception specification in explicit instantiation does not match "
  "instantiated one">;
def ext_mismatched_exception_spec_explicit_instantiation : ExtWarn<
  err_mismatched_exception_spec_explicit_instantiation.Text>,
  InGroup<MicrosoftExceptionSpec>;
def err_explicit_instantiation_dependent : Error<
  "explicit instantiation has dependent template arguments">;

// C++ typename-specifiers
def err_typename_nested_not_found : Error<"no type named %0 in %1">;
def err_typename_nested_not_found_enable_if : Error<
  "no type named 'type' in %0; 'enable_if' cannot be used to disable "
  "this declaration">;
def err_typename_nested_not_found_requirement : Error<
  "failed requirement '%0'; 'enable_if' cannot be used to disable this "
  "declaration">;
def err_typename_nested_not_type : Error<
    "typename specifier refers to non-type member %0 in %1">;
def err_typename_not_type : Error<
    "typename specifier refers to non-type %0">;
def note_typename_member_refers_here : Note<
    "referenced member %0 is declared here">;
def note_typename_refers_here : Note<
    "referenced %0 is declared here">;
def err_typename_missing : Error<
  "missing 'typename' prior to dependent type name '%0%1'">;
def err_typename_missing_template : Error<
  "missing 'typename' prior to dependent type template name '%0%1'">;
def ext_typename_missing : ExtWarn<
  "missing 'typename' prior to dependent type name '%0%1'">,
  InGroup<DiagGroup<"typename-missing">>;
def ext_typename_outside_of_template : ExtWarn<
  "'typename' occurs outside of a template">, InGroup<CXX11>;
def warn_cxx98_compat_typename_outside_of_template : Warning<
  "在模板外使用'typename'与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_typename_refers_to_using_value_decl : Error<
  "typename specifier refers to a dependent using declaration for a value "
  "%0 in %1">;
def note_using_value_decl_missing_typename : Note<
  "add 'typename' to treat this using declaration as a type">;

def err_template_kw_refers_to_non_template : Error<
  "%0%select{| following the 'template' keyword}1 "
  "does not refer to a template">;
def note_template_kw_refers_to_non_template : Note<
  "declared as a non-template here">;
def err_template_kw_refers_to_dependent_non_template : Error<
  "%0%select{| following the 'template' keyword}1 "
  "cannot refer to a dependent template">;
def err_template_kw_refers_to_class_template : Error<
  "'%0%1' instantiated to a class template, not a function template">;
def note_referenced_class_template : Note<
  "class template declared here">;
def err_template_kw_missing : Error<
  "missing 'template' keyword prior to dependent template name '%0%1'">;
def ext_template_outside_of_template : ExtWarn<
  "'template' keyword outside of a template">, InGroup<CXX11>;
def warn_cxx98_compat_template_outside_of_template : Warning<
  "在模板外使用'template'关键字与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;

def err_non_type_template_in_nested_name_specifier : Error<
  "qualified name refers into a specialization of %select{function|variable}0 "
  "template %1">;
def err_template_id_not_a_type : Error<
  "template name refers to non-type template %0">;
def note_template_declared_here : Note<
  "%select{function template|class template|variable template"
  "|type alias template|template template parameter}0 "
  "%1 declared here">;
def err_template_expansion_into_fixed_list : Error<
  "pack expansion used as argument for non-pack parameter of %select{alias "
  "template|concept}0">;
def note_parameter_type : Note<
  "parameter of type %0 is declared here">;

// C++11 Variadic Templates
def err_template_param_pack_default_arg : Error<
  "template parameter pack cannot have a default argument">;
def err_template_param_pack_must_be_last_template_parameter : Error<
  "template parameter pack must be the last template parameter">;

def err_template_parameter_pack_non_pack : Error<
  "%select{template type|non-type template|template template}0 parameter"
  "%select{| pack}1 conflicts with previous %select{template type|"
  "non-type template|template template}0 parameter%select{ pack|}1">;
def note_template_parameter_pack_non_pack : Note<
  "%select{template type|non-type template|template template}0 parameter"
  "%select{| pack}1 does not match %select{template type|non-type template"
  "|template template}0 parameter%select{ pack|}1 in template argument">;
def note_template_parameter_pack_here : Note<
  "previous %select{template type|non-type template|template template}0 "
  "parameter%select{| pack}1 declared here">;

def err_unexpanded_parameter_pack : Error<
  "%select{expression|base type|declaration type|data member type|bit-field "
  "size|static assertion|fixed underlying type|enumerator value|"
  "using declaration|friend declaration|qualifier|initializer|default argument|"
  "non-type template parameter type|exception type|partial specialization|"
  "__if_exists name|__if_not_exists name|lambda|block|type constraint|"
  "requirement|requires clause}0 "
  "contains%plural{0: an|:}1 unexpanded parameter pack"
  "%plural{0:|1: %2|2:s %2 and %3|:s %2, %3, ...}1">;

def err_pack_expansion_without_parameter_packs : Error<
  "pack expansion does not contain any unexpanded parameter packs">;
def err_pack_expansion_length_conflict : Error<
  "pack expansion contains parameter packs %0 and %1 that have different "
  "lengths (%2 vs. %3)">;
def err_pack_expansion_length_conflict_multilevel : Error<
  "pack expansion contains parameter pack %0 that has a different "
  "length (%1 vs. %2) from outer parameter packs">;
def err_pack_expansion_length_conflict_partial : Error<
  "pack expansion contains parameter pack %0 that has a different "
  "length (at least %1 vs. %2) from outer parameter packs">;
def err_pack_expansion_member_init : Error<
  "pack expansion for initialization of member %0">;

def err_function_parameter_pack_without_parameter_packs : Error<
  "type %0 of function parameter pack does not contain any unexpanded "
  "parameter packs">;
def err_ellipsis_in_declarator_not_parameter : Error<
  "only function and template parameters can be parameter packs">;

def err_sizeof_pack_no_pack_name : Error<
  "%0 does not refer to the name of a parameter pack">;

def err_fold_expression_packs_both_sides : Error<
  "binary fold expression has unexpanded parameter packs in both operands">;
def err_fold_expression_empty : Error<
  "unary fold expression has empty expansion for operator '%0' "
  "with no fallback value">;
def err_fold_expression_bad_operand : Error<
  "expression not permitted as operand of fold expression">;
def err_fold_expression_limit_exceeded: Error<
  "instantiating fold expression with %0 arguments exceeded expression nesting "
  "limit of %1">, DefaultFatal, NoSFINAE;

def err_unexpected_typedef : Error<
  "unexpected type name %0: expected expression">;
def err_unexpected_namespace : Error<
  "unexpected namespace name %0: expected expression">;
def err_undeclared_var_use : Error<"未识别的变量名%0">;
def ext_undeclared_unqual_id_with_dependent_base : ExtWarn<
  "use of undeclared identifier %0; "
  "unqualified lookup into dependent bases of class template %1 is a Microsoft extension">,
  InGroup<MicrosoftTemplate>;
def err_found_in_dependent_base : Error<
  "explicit qualification required to use member %0 from dependent base class">;
def ext_found_in_dependent_base : ExtWarn<"use of member %0 "
  "found via unqualified lookup into dependent bases of class templates is a "
  "Microsoft extension">, InGroup<MicrosoftTemplate>;
def err_found_later_in_class : Error<"member %0 used before its declaration">;
def ext_found_later_in_class : ExtWarn<
  "use of member %0 before its declaration is a Microsoft extension">,
  InGroup<MicrosoftTemplate>;
def note_dependent_member_use : Note<
  "must qualify identifier to find this declaration in dependent base class">;
def err_not_found_by_two_phase_lookup : Error<"call to function %0 that is neither "
    "visible in the template definition nor found by argument-dependent lookup">;
def note_not_found_by_two_phase_lookup : Note<"%0 should be declared prior to the "
    "call site%select{| or in %2| or in an associated namespace of one of its arguments}1">;
def err_undeclared_use : Error<"use of undeclared %0">;
def warn_deprecated : Warning<"%0已弃用">,
    InGroup<DeprecatedDeclarations>;
def note_from_diagnose_if : Note<"from 'diagnose_if' attribute on %0:">;
def warn_property_method_deprecated :
    Warning<"属性访问正在使用已弃用的%0方法">,
    InGroup<DeprecatedDeclarations>;
def warn_deprecated_message : Warning<"%0已弃用: %1">,
    InGroup<DeprecatedDeclarations>;
def warn_deprecated_anonymous_namespace : Warning<
  "匿名命名空间上的'deprecated'属性被忽略">,
  InGroup<IgnoredAttributes>;
def warn_deprecated_fwdclass_message : Warning<
    "%0 may be deprecated because the receiver type is unknown">,
    InGroup<DeprecatedDeclarations>;
def warn_deprecated_def : Warning<
  "实现已弃用%select{method|class|category}0">,
  InGroup<DeprecatedImplementations>, DefaultIgnore;
def warn_unavailable_def : Warning<
  "实现不可用的方法">,
  InGroup<DeprecatedImplementations>, DefaultIgnore;
def err_unavailable : Error<"%0 is unavailable">;
def err_property_method_unavailable :
    Error<"property access is using %0 method which is unavailable">;
def err_unavailable_message : Error<"%0 is unavailable: %1">;
def warn_unavailable_fwdclass_message : Warning<
    "%0 可能不可用，因为接收器类型未知">,
    InGroup<UnavailableDeclarations>;
def note_availability_specified_here : Note<
  "%0 has been explicitly marked "
  "%select{unavailable|deleted|deprecated}1 here">;
def note_partial_availability_specified_here : Note<
  "%0 has been marked as being introduced in %1 %2 here, "
  "but the deployment target is %1 %3">;
def note_implicitly_deleted : Note<
  "explicitly defaulted function was implicitly deleted here">;
def warn_not_enough_argument : Warning<
  "%0 声明中没有足够的变量参数来适应哨兵">,
  InGroup<Sentinel>;
def warn_missing_sentinel : Warning <
  "缺少哨兵在 %select{function call|method dispatch|block call}0中">,
  InGroup<Sentinel>;
def note_sentinel_here : Note<
  "%select{function|method|block}0 has been explicitly marked sentinel here">;
def warn_missing_prototype : Warning<
  "函数 %0 没有以前的原型">,
  InGroup<DiagGroup<"missing-prototypes">>, DefaultIgnore;
def note_declaration_not_a_prototype : Note<
  "this declaration is not a prototype; add %select{'void'|parameter declarations}0 "
  "to make it %select{a prototype for a zero-parameter function|one}0">;
def warn_strict_prototypes : Warning<
  "此 %select{function declaration is not|block declaration is not|"
  "old-style function definition is not preceded by}0 一个原型">,
  InGroup<DiagGroup<"strict-prototypes">>, DefaultIgnore;
def warn_missing_variable_declarations : Warning<
  "非静态变量 %0 没有先前的外部声明">,
  InGroup<DiagGroup<"missing-variable-declarations">>, DefaultIgnore;
def note_static_for_internal_linkage : Note<
  "declare 'static' if the %select{variable|function}0 is not intended to be "
  "used outside of this translation unit">;
def err_static_data_member_reinitialization :
  Error<"static data member %0 already has an initializer">;
def err_redefinition : Error<"redefinition of %0">;
def err_alias_after_tentative :
  Error<"alias definition of %0 after tentative definition">;
def err_alias_is_definition :
  Error<"definition %0 cannot also be an %select{alias|ifunc}1">;
def err_definition_of_implicitly_declared_member : Error<
  "definition of implicitly declared %select{default constructor|copy "
  "constructor|move constructor|copy assignment operator|move assignment "
  "operator|destructor|function}1">;
def err_definition_of_explicitly_defaulted_member : Error<
  "definition of explicitly defaulted %select{default constructor|copy "
  "constructor|move constructor|copy assignment operator|move assignment "
  "operator|destructor|function}0">;
def err_redefinition_extern_inline : Error<
  "redefinition of a 'extern inline' function %0 is not supported in "
  "%select{C99 mode|C++}1">;
def warn_attr_abi_tag_namespace : Warning<
  "'abi_tag' 属性在 %select{non-inline|anonymous}0 命名空间被忽略">,
  InGroup<IgnoredAttributes>;
def err_abi_tag_on_redeclaration : Error<
  "cannot add 'abi_tag' attribute in a redeclaration">;
def err_new_abi_tag_on_redeclaration : Error<
  "'abi_tag' %0 missing in original declaration">;
def note_use_ifdef_guards : Note<
  "unguarded header; consider using #ifdef guards or #pragma once">;

def note_deleted_dtor_no_operator_delete : Note<
  "virtual destructor requires an unambiguous, accessible 'operator delete'">;
def note_deleted_special_member_class_subobject : Note<
  "%select{default constructor of|copy constructor of|move constructor of|"
  "copy assignment operator of|move assignment operator of|destructor of|"
  "constructor inherited by}0 "
  "%1 is implicitly deleted because "
  "%select{base class %3|%select{||||variant }4field %3}2 "
  "%select{has "
  "%select{no|a deleted|multiple|an inaccessible|a non-trivial}4 "
  "%select{%select{default constructor|copy constructor|move constructor|copy "
  "assignment operator|move assignment operator|destructor|"
  "%select{default|corresponding|default|default|default}4 constructor}0|"
  "destructor}5"
  "%select{||s||}4"
  "|is an ObjC pointer}6">;
def note_deleted_default_ctor_uninit_field : Note<
  "%select{default constructor of|constructor inherited by}0 "
  "%1 is implicitly deleted because field %2 of "
  "%select{reference|const-qualified}4 type %3 would not be initialized">;
def note_deleted_default_ctor_all_const : Note<
  "%select{default constructor of|constructor inherited by}0 "
  "%1 is implicitly deleted because all "
  "%select{data members|data members of an anonymous union member}2"
  " are const-qualified">;
def note_deleted_copy_ctor_rvalue_reference : Note<
  "copy constructor of %0 is implicitly deleted because field %1 is of "
  "rvalue reference type %2">;
def note_deleted_copy_user_declared_move : Note<
  "copy %select{constructor|assignment operator}0 is implicitly deleted because"
  " %1 has a user-declared move %select{constructor|assignment operator}2">;
def note_deleted_assign_field : Note<
  "%select{copy|move}0 assignment operator of %1 is implicitly deleted "
  "because field %2 is of %select{reference|const-qualified}4 type %3">;

// These should be errors.
def warn_undefined_internal : Warning<
  "%select{function|variable}0 %q1 具有内部链接但未定义">,
  InGroup<DiagGroup<"undefined-internal">>;
def err_undefined_internal_type : Error<
  "%select{function|variable}0 %q1 is used but not defined in this "
  "translation unit, and cannot be defined in any other translation unit "
  "because its type does not have linkage">;
def ext_undefined_internal_type : Extension<
  "ISO C++ requires a definition in this translation unit for "
  "%select{function|variable}0 %q1 because its type does not have linkage">,
  InGroup<DiagGroup<"undefined-internal-type">>;
def warn_undefined_inline : Warning<"内联函数 %q0 未定义">,
  InGroup<DiagGroup<"undefined-inline">>;
def err_undefined_inline_var : Error<"inline variable %q0 is not defined">;
def note_used_here : Note<"used here">;

def err_internal_linkage_redeclaration : Error<
  "'internal_linkage' attribute does not appear on the first declaration of %0">;
def warn_internal_linkage_local_storage : Warning<
  "非静态局部变量的'internal_linkage'属性被忽略">,
  InGroup<IgnoredAttributes>;

def ext_internal_in_extern_inline : ExtWarn<
  "static %select{function|variable}0 %1 is used in an inline function with "
  "external linkage">, InGroup<StaticInInline>;
def ext_internal_in_extern_inline_quiet : Extension<
  "static %select{function|variable}0 %1 is used in an inline function with "
  "external linkage">, InGroup<StaticInInline>;
def warn_static_local_in_extern_inline : Warning<
  "内联函数中的非常量静态局部变量 "
  "在不同文件中可能不同">, InGroup<StaticLocalInInline>;
def note_convert_inline_to_static : Note<
  "use 'static' to give inline function %0 internal linkage">;

def ext_redefinition_of_typedef : ExtWarn<
  "redefinition of typedef %0 is a C11 feature">,
  InGroup<DiagGroup<"typedef-redefinition"> >;
def err_redefinition_variably_modified_typedef : Error<
  "redefinition of %select{typedef|type alias}0 for variably-modified type %1">;

def err_inline_decl_follows_def : Error<
  "inline declaration of %0 follows non-inline definition">;
def err_inline_declaration_block_scope : Error<
  "inline declaration of %0 not allowed in block scope">;
def err_static_non_static : Error<
  "static declaration of %0 follows non-static declaration">;
def err_different_language_linkage : Error<
  "declaration of %0 has a different language linkage">;
def ext_retained_language_linkage : Extension<
  "friend function %0 retaining previous language linkage is an extension">,
  InGroup<DiagGroup<"retained-language-linkage">>;
def err_extern_c_global_conflict : Error<
  "declaration of %1 %select{with C language linkage|in global scope}0 "
  "conflicts with declaration %select{in global scope|with C language linkage}0">;
def note_extern_c_global_conflict : Note<
  "declared %select{in global scope|with C language linkage}0 here">;
def note_extern_c_begins_here : Note<
  "extern \"C\" language linkage specification begins here">;
def warn_weak_import : Warning <
  "一个已经声明的变量是一个weak_import声明%0">;
def ext_static_non_static : Extension<
  "redeclaring non-static %0 as static is a Microsoft extension">,
  InGroup<MicrosoftRedeclareStatic>;
def err_non_static_static : Error<
  "non-static declaration of %0 follows static declaration">;
def err_extern_non_extern : Error<
  "extern declaration of %0 follows non-extern declaration">;
def err_non_extern_extern : Error<
  "non-extern declaration of %0 follows extern declaration">;
def err_non_thread_thread : Error<
  "non-thread-local declaration of %0 follows thread-local declaration">;
def err_thread_non_thread : Error<
  "thread-local declaration of %0 follows non-thread-local declaration">;
def err_thread_thread_different_kind : Error<
  "thread-local declaration of %0 with %select{static|dynamic}1 initialization "
  "follows declaration with %select{dynamic|static}1 initialization">;
def err_mismatched_owning_module : Error<
  "declaration of %0 in %select{the global module|module %2}1 follows "
  "declaration in %select{the global module|module %4}3">;
def err_redefinition_different_type : Error<
  "redefinition of %0 with a different type%diff{: $ vs $|}1,2">;
def err_redefinition_different_kind : Error<
  "redefinition of %0 as different kind of symbol">;
def err_redefinition_different_namespace_alias : Error<
  "redefinition of %0 as an alias for a different namespace">;
def note_previous_namespace_alias : Note<
  "previously defined as an alias for %0">;
def warn_forward_class_redefinition : Warning<
  "对象类型的 typedef 名称的前向类 %0 的重新定义被忽略">,
  InGroup<DiagGroup<"objc-forward-class-redefinition">>;
def err_redefinition_different_typedef : Error<
  "%select{typedef|type alias|type alias template}0 "
  "redefinition with different types%diff{ ($ vs $)|}1,2">;
def err_tag_reference_non_tag : Error<
  "%select{non-struct type|non-class type|non-union type|non-enum "
  "type|typedef|type alias|template|type alias template|template "
  "template argument}1 %0 cannot be referenced with a "
  "%select{struct|interface|union|class|enum}2 specifier">;
def err_tag_reference_conflict : Error<
  "implicit declaration introduced by elaborated type conflicts with a "
  "%select{non-struct type|non-class type|non-union type|non-enum "
  "type|typedef|type alias|template|type alias template|template "
  "template argument}0 of the same name">;
def err_dependent_tag_decl : Error<
  "%select{declaration|definition}0 of "
  "%select{struct|interface|union|class|enum}1 in a dependent scope">;
def err_tag_definition_of_typedef : Error<
  "definition of type %0 conflicts with %select{typedef|type alias}1 of the same name">;
def warn_conflicting_types : Warning<"conflicting types for %0">;
def err_different_pass_object_size_params : Error<
  "conflicting pass_object_size attributes on parameters">;
def err_late_asm_label_name : Error<
  "cannot apply asm label to %select{variable|function}0 after its first use">;
def err_different_asm_label : Error<"conflicting asm label">;
def err_nested_redefinition : Error<"nested redefinition of %0">;
def err_use_with_wrong_tag : Error<
  "use of %0 with tag type that does not match previous declaration">;
def warn_struct_class_tag_mismatch : Warning<
  "%select{struct|interface|class}0%select{| template}1 %2 被先前 "
  "声明为 %select{struct|interface|class}3%select{| template}1; "
  "this is valid, but may result in linker errors under the Microsoft C++ ABI">,
  InGroup<MismatchedTags>, DefaultIgnore;
def warn_struct_class_previous_tag_mismatch : Warning<
  "%2 在这里被定义为 %select{a struct|an interface|a class}0%select{| template}1 "
  "但是被先前声明为 "
  "%select{a struct|an interface|a class}3%select{| template}1; "
  "这是有效的，但可能会导致Microsoft C++ABI下的链接器错误">,
  InGroup<MismatchedTags>, DefaultIgnore;
def note_struct_class_suggestion : Note<
  "did you mean %select{struct|interface|class}0 here?">;
def ext_forward_ref_enum : Extension<
  "ISO C forbids forward references to 'enum' types">;
def err_forward_ref_enum : Error<
  "ISO C++ forbids forward references to 'enum' types">;
def ext_ms_forward_ref_enum : ExtWarn<
  "forward references to 'enum' types are a Microsoft extension">,
  InGroup<MicrosoftEnumForwardReference>;
def ext_forward_ref_enum_def : Extension<
  "redeclaration of already-defined enum %0 is a GNU extension">,
  InGroup<GNURedeclaredEnum>;

def err_redefinition_of_enumerator : Error<"redefinition of enumerator %0">;
def err_duplicate_member : Error<"duplicate member %0">;
def err_misplaced_ivar : Error<
  "instance variables may not be placed in %select{categories|class extension}0">;
def warn_ivars_in_interface : Warning<
  "不推荐在接口中声明实例变量">,
  InGroup<DiagGroup<"objc-interface-ivars">>, DefaultIgnore;
def ext_enum_value_not_int : Extension<
  "ISO C restricts enumerator values to range of 'int' (%0 is too "
  "%select{small|large}1)">;
def ext_enum_too_large : ExtWarn<
  "enumeration values exceed range of largest integer">, InGroup<EnumTooLarge>;
def ext_enumerator_increment_too_large : ExtWarn<
  "incremented enumerator value %0 is not representable in the "
  "largest integer type">, InGroup<EnumTooLarge>;
def warn_flag_enum_constant_out_of_range : Warning<
  "枚举值%0超出枚举类型%1中的标志范围">,
  InGroup<FlagEnum>;

def err_vm_decl_in_file_scope : Error<
  "variably modified type declaration not allowed at file scope">;
def err_vm_decl_has_extern_linkage : Error<
  "variably modified type declaration cannot have 'extern' linkage">;
def err_typecheck_field_variable_size : Error<
  "fields must have a constant size: 'variable length array in structure' "
  "extension will never be supported">;
def err_vm_func_decl : Error<
  "function declaration cannot have variably modified type">;
def err_array_too_large : Error<
  "array is too large (%0 elements)">;

def err_typecheck_negative_array_size : Error<"array size is negative">;
def warn_typecheck_function_qualifiers_ignored : Warning<
  "'%0'限定符在函数类型%1上无效">,
  InGroup<IgnoredQualifiers>;
def warn_typecheck_function_qualifiers_unspecified : Warning<
  "'%0' 限定符在函数类型%1上具有未指定的行为">;
def warn_typecheck_reference_qualifiers : Warning<
  "'%0' 限定符在引用类型%1上无效">,
  InGroup<IgnoredQualifiers>;
def err_typecheck_invalid_restrict_not_pointer : Error<
  "restrict requires a pointer or reference (%0 is invalid)">;
def err_typecheck_invalid_restrict_not_pointer_noarg : Error<
  "restrict requires a pointer or reference">;
def err_typecheck_invalid_restrict_invalid_pointee : Error<
  "pointer to function type %0 may not be 'restrict' qualified">;
def ext_typecheck_zero_array_size : Extension<
  "zero size arrays are an extension">, InGroup<ZeroLengthArray>;
def err_typecheck_zero_array_size : Error<
  "zero-length arrays are not permitted in C++">;
def err_array_size_non_int : Error<"size of array has non-integer type %0">;
def err_init_element_not_constant : Error<
  "initializer element is not a compile-time constant">;
def ext_aggregate_init_not_constant : Extension<
  "initializer for aggregate is not a compile-time constant">, InGroup<C99>;
def err_local_cant_init : Error<
  "'__local' variable cannot have an initializer">;
def err_loader_uninitialized_cant_init
    : Error<"variable with 'loader_uninitialized' attribute cannot have an "
            "initializer">;
def err_loader_uninitialized_trivial_ctor
    : Error<"variable with 'loader_uninitialized' attribute must have a "
            "trivial default constructor">;
def err_loader_uninitialized_redeclaration
    : Error<"redeclaration cannot add 'loader_uninitialized' attribute">;
def err_loader_uninitialized_extern_decl
    : Error<"variable %0 cannot be declared both 'extern' and with the "
            "'loader_uninitialized' attribute">;
def err_block_extern_cant_init : Error<
  "'extern' variable cannot have an initializer">;
def warn_extern_init : Warning<"'extern' 变量具有一个初始值设定项">,
  InGroup<DiagGroup<"extern-initializer">>;
def err_variable_object_no_init : Error<
  "variable-sized object may not be initialized">;
def err_excess_initializers : Error<
  "excess elements in %select{array|vector|scalar|union|struct}0 initializer">;
def ext_excess_initializers : ExtWarn<
  "excess elements in %select{array|vector|scalar|union|struct}0 initializer">,
  InGroup<ExcessInitializers>;
def err_excess_initializers_for_sizeless_type : Error<
  "excess elements in initializer for indivisible sizeless type %0">;
def ext_excess_initializers_for_sizeless_type : ExtWarn<
  "excess elements in initializer for indivisible sizeless type %0">,
  InGroup<ExcessInitializers>;
def err_excess_initializers_in_char_array_initializer : Error<
  "excess elements in char array initializer">;
def ext_excess_initializers_in_char_array_initializer : ExtWarn<
  "excess elements in char array initializer">,
  InGroup<ExcessInitializers>;
def err_initializer_string_for_char_array_too_long : Error<
  "initializer-string for char array is too long">;
def ext_initializer_string_for_char_array_too_long : ExtWarn<
  "initializer-string for char array is too long">,
  InGroup<ExcessInitializers>;
def warn_missing_field_initializers : Warning<
  "缺少字段%0初始值设定项">,
  InGroup<MissingFieldInitializers>, DefaultIgnore;
def warn_braces_around_init : Warning<
  "周围的大括号 %select{scalar |}0初始值设定项">,
  InGroup<DiagGroup<"braced-scalar-init">>;
def ext_many_braces_around_init : ExtWarn<
  "too many braces around %select{scalar |}0initializer">,
  InGroup<DiagGroup<"many-braces-around-scalar-init">>, SFINAEFailure;
def ext_complex_component_init : Extension<
  "complex initialization specifying real and imaginary components "
  "is an extension">, InGroup<DiagGroup<"complex-component-init">>;
def err_empty_scalar_initializer : Error<"scalar initializer cannot be empty">;
def err_empty_sizeless_initializer : Error<
  "initializer for sizeless type %0 cannot be empty">;
def warn_cxx98_compat_empty_scalar_initializer : Warning<
  "从空初始值设定项列表初始化的标量与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_cxx98_compat_empty_sizeless_initializer : Warning<
  "从空初始值设定项列表初始化%0与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_cxx98_compat_reference_list_init : Warning<
  "从初始值设定项列表初始化的引用与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_cxx98_compat_initializer_list_init : Warning<
  "initializer_list list对象的初始化与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_cxx98_compat_ctor_list_init : Warning<
  "来自初始值设定项列表的构造函数调用与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_illegal_initializer : Error<
  "illegal initializer (only variables can be initialized)">;
def err_illegal_initializer_type : Error<"illegal initializer type %0">;
def ext_init_list_type_narrowing : ExtWarn<
  "type %0 cannot be narrowed to %1 in initializer list">,
  InGroup<CXX11Narrowing>, DefaultError, SFINAEFailure;
def ext_init_list_variable_narrowing : ExtWarn<
  "non-constant-expression cannot be narrowed from type %0 to %1 in "
  "initializer list">, InGroup<CXX11Narrowing>, DefaultError, SFINAEFailure;
def ext_init_list_constant_narrowing : ExtWarn<
  "constant expression evaluates to %0 which cannot be narrowed to type %1">,
  InGroup<CXX11Narrowing>, DefaultError, SFINAEFailure;
def warn_init_list_type_narrowing : Warning<
  "在C++11的初始值设定项列表中，类型%0不能缩小为%1">,
  InGroup<CXX11Narrowing>, DefaultIgnore;
def warn_init_list_variable_narrowing : Warning<
  "非常量表达式不能从类型%0缩小到%1 "
  "在C++11的初始值设定项列表中">,
  InGroup<CXX11Narrowing>, DefaultIgnore;
def warn_init_list_constant_narrowing : Warning<
  "常量表达式的计算结果为%0，无法将其缩小为类型%1 "
  "在C++11中">,
  InGroup<CXX11Narrowing>, DefaultIgnore;
def note_init_list_narrowing_silence : Note<
  "insert an explicit cast to silence this issue">;
def err_init_objc_class : Error<
  "cannot initialize Objective-C class type %0">;
def err_implicit_empty_initializer : Error<
  "initializer for aggregate with no elements requires explicit braces">;
def err_bitfield_has_negative_width : Error<
  "bit-field %0 has negative width (%1)">;
def err_anon_bitfield_has_negative_width : Error<
  "anonymous bit-field has negative width (%0)">;
def err_bitfield_has_zero_width : Error<"named bit-field %0 has zero width">;
def err_bitfield_width_exceeds_type_width : Error<
  "width of bit-field %0 (%1 bits) exceeds %select{width|size}2 "
  "of its type (%3 bit%s3)">;
def err_anon_bitfield_width_exceeds_type_width : Error<
  "width of anonymous bit-field (%0 bits) exceeds %select{width|size}1 "
  "of its type (%2 bit%s2)">;
def err_incorrect_number_of_vector_initializers : Error<
  "number of elements must be either one or match the size of the vector">;

// Used by C++ which allows bit-fields that are wider than the type.
def warn_bitfield_width_exceeds_type_width: Warning<
  "位字段%0的宽度 %0 (%1 位) 超过了其类型的宽度;值将被截断为 "
  "%2 位%s2">, InGroup<BitFieldWidth>;
def err_bitfield_too_wide : Error<
  "%select{bit-field %1|anonymous bit-field}0 is too wide (%2 bits)">;
def warn_bitfield_too_small_for_enum : Warning<
  "位字段%0不够宽，无法存储%1的所有枚举数">,
  InGroup<BitFieldEnumConversion>, DefaultIgnore;
def note_widen_bitfield : Note<
  "widen this field to %0 bits to store all values of %1">;
def warn_unsigned_bitfield_assigned_signed_enum : Warning<
  "正在将有符号枚举类型%1的值赋给无符号位字段%0; "
  "枚举%1的负枚举数将转换为正值">,
  InGroup<BitFieldEnumConversion>, DefaultIgnore;
def warn_signed_bitfield_enum_conversion : Warning<
  "有符号位字段%0需要一个额外的位来表示%1的 "
  "最大正枚举数">,
  InGroup<BitFieldEnumConversion>, DefaultIgnore;
def note_change_bitfield_sign : Note<
  "consider making the bitfield type %select{unsigned|signed}0">;

def warn_missing_braces : Warning<
  "建议围绕子对象的初始化使用大括号">,
  InGroup<MissingBraces>, DefaultIgnore;

def err_redefinition_of_label : Error<"redefinition of label %0">;
def err_undeclared_label_use : Error<"use of undeclared label %0">;
def err_goto_ms_asm_label : Error<
  "cannot jump from this goto statement to label %0 inside an inline assembly block">;
def note_goto_ms_asm_label : Note<
  "inline assembly label %0 declared here">;
def warn_unused_label : Warning<"未使用的标签 %0">,
  InGroup<UnusedLabel>, DefaultIgnore;

def err_goto_into_protected_scope : Error<
  "cannot jump from this goto statement to its label">;
def ext_goto_into_protected_scope : ExtWarn<
  "jump from this goto statement to its label is a Microsoft extension">,
  InGroup<MicrosoftGoto>;
def warn_cxx98_compat_goto_into_protected_scope : Warning<
  "从此goto语句跳转到其标签与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_switch_into_protected_scope : Error<
  "cannot jump from switch statement to this case label">;
def warn_cxx98_compat_switch_into_protected_scope : Warning<
  "从switch语句跳转到此case标签与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def err_indirect_goto_without_addrlabel : Error<
  "indirect goto in function with no address-of-label expressions">;
def err_indirect_goto_in_protected_scope : Error<
  "cannot jump from this %select{indirect|asm}0 goto statement to one of its possible targets">;
def warn_cxx98_compat_indirect_goto_in_protected_scope : Warning<
  "从此 %select{indirect|asm}0 goto语句跳转到它的一个可能目标 "
  "与C++98不兼容">, InGroup<CXX98Compat>, DefaultIgnore;
def note_indirect_goto_target : Note<
  "possible target of %select{indirect|asm}0 goto statement">;
def note_protected_by_variable_init : Note<
  "jump bypasses variable initialization">;
def note_protected_by_variable_nontriv_destructor : Note<
  "jump bypasses variable with a non-trivial destructor">;
def note_protected_by_variable_non_pod : Note<
  "jump bypasses initialization of non-POD variable">;
def note_protected_by_cleanup : Note<
  "jump bypasses initialization of variable with __attribute__((cleanup))">;
def note_protected_by_vla_typedef : Note<
  "jump bypasses initialization of VLA typedef">;
def note_protected_by_vla_type_alias : Note<
  "jump bypasses initialization of VLA type alias">;
def note_protected_by_constexpr_if : Note<
  "jump enters controlled statement of constexpr if">;
def note_protected_by_if_available : Note<
  "jump enters controlled statement of if available">;
def note_protected_by_vla : Note<
  "jump bypasses initialization of variable length array">;
def note_protected_by_objc_fast_enumeration : Note<
  "jump enters Objective-C fast enumeration loop">;
def note_protected_by_objc_try : Note<
  "jump bypasses initialization of @try block">;
def note_protected_by_objc_catch : Note<
  "jump bypasses initialization of @catch block">;
def note_protected_by_objc_finally : Note<
  "jump bypasses initialization of @finally block">;
def note_protected_by_objc_synchronized : Note<
  "jump bypasses initialization of @synchronized block">;
def note_protected_by_objc_autoreleasepool : Note<
  "jump bypasses auto release push of @autoreleasepool block">;
def note_protected_by_cxx_try : Note<
  "jump bypasses initialization of try block">;
def note_protected_by_cxx_catch : Note<
  "jump bypasses initialization of catch block">;
def note_protected_by_seh_try : Note<
  "jump bypasses initialization of __try block">;
def note_protected_by_seh_except : Note<
  "jump bypasses initialization of __except block">;
def note_protected_by_seh_finally : Note<
  "jump bypasses initialization of __finally block">;
def note_protected_by___block : Note<
  "jump bypasses setup of __block variable">;
def note_protected_by_objc_strong_init : Note<
  "jump bypasses initialization of __strong variable">;
def note_protected_by_objc_weak_init : Note<
  "jump bypasses initialization of __weak variable">;
def note_protected_by_non_trivial_c_struct_init : Note<
  "jump bypasses initialization of variable of non-trivial C struct type">;
def note_enters_block_captures_cxx_obj : Note<
  "jump enters lifetime of block which captures a destructible C++ object">;
def note_enters_block_captures_strong : Note<
  "jump enters lifetime of block which strongly captures a variable">;
def note_enters_block_captures_weak : Note<
  "jump enters lifetime of block which weakly captures a variable">;
def note_enters_block_captures_non_trivial_c_struct : Note<
  "jump enters lifetime of block which captures a C struct that is non-trivial "
  "to destroy">;
def note_enters_compound_literal_scope : Note<
  "jump enters lifetime of a compound literal that is non-trivial to destruct">;

def note_exits_cleanup : Note<
  "jump exits scope of variable with __attribute__((cleanup))">;
def note_exits_dtor : Note<
  "jump exits scope of variable with non-trivial destructor">;
def note_exits_temporary_dtor : Note<
  "jump exits scope of lifetime-extended temporary with non-trivial "
  "destructor">;
def note_exits___block : Note<
  "jump exits scope of __block variable">;
def note_exits_objc_try : Note<
  "jump exits @try block">;
def note_exits_objc_catch : Note<
  "jump exits @catch block">;
def note_exits_objc_finally : Note<
  "jump exits @finally block">;
def note_exits_objc_synchronized : Note<
  "jump exits @synchronized block">;
def note_exits_cxx_try : Note<
  "jump exits try block">;
def note_exits_cxx_catch : Note<
  "jump exits catch block">;
def note_exits_seh_try : Note<
  "jump exits __try block">;
def note_exits_seh_except : Note<
  "jump exits __except block">;
def note_exits_seh_finally : Note<
  "jump exits __finally block">;
def note_exits_objc_autoreleasepool : Note<
  "jump exits autoreleasepool block">;
def note_exits_objc_strong : Note<
  "jump exits scope of __strong variable">;
def note_exits_objc_weak : Note<
  "jump exits scope of __weak variable">;
def note_exits_block_captures_cxx_obj : Note<
  "jump exits lifetime of block which captures a destructible C++ object">;
def note_exits_block_captures_strong : Note<
  "jump exits lifetime of block which strongly captures a variable">;
def note_exits_block_captures_weak : Note<
  "jump exits lifetime of block which weakly captures a variable">;
def note_exits_block_captures_non_trivial_c_struct : Note<
  "jump exits lifetime of block which captures a C struct that is non-trivial "
  "to destroy">;
def note_exits_compound_literal_scope : Note<
  "jump exits lifetime of a compound literal that is non-trivial to destruct">;

def err_func_returning_qualified_void : ExtWarn<
  "function cannot return qualified void type %0">,
  InGroup<DiagGroup<"qualified-void-return-type">>;
def err_func_returning_array_function : Error<
  "function cannot return %select{array|function}0 type %1">;
def err_field_declared_as_function : Error<"field %0 declared as a function">;
def err_field_incomplete_or_sizeless : Error<
  "field has %select{incomplete|sizeless}0 type %1">;
def ext_variable_sized_type_in_struct : ExtWarn<
  "field %0 with variable sized type %1 not at the end of a struct or class is"
  " a GNU extension">, InGroup<GNUVariableSizedTypeNotAtEnd>;

def ext_c99_flexible_array_member : Extension<
  "flexible array members are a C99 feature">, InGroup<C99>;
def err_flexible_array_virtual_base : Error<
  "flexible array member %0 not allowed in "
  "%select{struct|interface|union|class|enum}1 which has a virtual base class">;
def err_flexible_array_empty_aggregate : Error<
  "flexible array member %0 not allowed in otherwise empty "
  "%select{struct|interface|union|class|enum}1">;
def err_flexible_array_has_nontrivial_dtor : Error<
  "flexible array member %0 of type %1 with non-trivial destruction">;
def ext_flexible_array_in_struct : Extension<
  "%0 may not be nested in a struct due to flexible array member">,
  InGroup<FlexibleArrayExtensions>;
def ext_flexible_array_in_array : Extension<
  "%0 may not be used as an array element due to flexible array member">,
  InGroup<FlexibleArrayExtensions>;
def err_flexible_array_init : Error<
  "initialization of flexible array member is not allowed">;
def ext_flexible_array_empty_aggregate_ms : Extension<
  "flexible array member %0 in otherwise empty "
  "%select{struct|interface|union|class|enum}1 is a Microsoft extension">,
  InGroup<MicrosoftFlexibleArray>;
def err_flexible_array_union : Error<
  "flexible array member %0 in a union is not allowed">;
def ext_flexible_array_union_ms : Extension<
  "flexible array member %0 in a union is a Microsoft extension">,
  InGroup<MicrosoftFlexibleArray>;
def ext_flexible_array_empty_aggregate_gnu : Extension<
  "flexible array member %0 in otherwise empty "
  "%select{struct|interface|union|class|enum}1 is a GNU extension">,
  InGroup<GNUEmptyStruct>;
def ext_flexible_array_union_gnu : Extension<
  "flexible array member %0 in a union is a GNU extension">, InGroup<GNUFlexibleArrayUnionMember>;

def err_flexible_array_not_at_end : Error<
  "flexible array member %0 with type %1 is not at the end of"
  " %select{struct|interface|union|class|enum}2">;
def err_objc_variable_sized_type_not_at_end : Error<
  "field %0 with variable sized type %1 is not at the end of class">;
def note_next_field_declaration : Note<
  "next field declaration is here">;
def note_next_ivar_declaration : Note<
  "next %select{instance variable declaration|synthesized instance variable}0"
  " is here">;
def err_synthesize_variable_sized_ivar : Error<
  "synthesized property with variable size type %0"
  " requires an existing instance variable">;
def err_flexible_array_arc_retainable : Error<
  "ARC forbids flexible array members with retainable object type">;
def warn_variable_sized_ivar_visibility : Warning<
  "字段%0属于具有变量大小类型%1，对子类不可见，"
  "并且可能与其实例变量冲突">, InGroup<ObjCFlexibleArray>;
def warn_superclass_variable_sized_type_not_at_end : Warning<
  "字段%0可以重写实例变量%1用变量大小类型%2"
  "在超类%3中">, InGroup<ObjCFlexibleArray>;

let CategoryName = "ARC Semantic Issue" in {

// ARC-mode diagnostics.

let CategoryName = "ARC Weak References" in {

def err_arc_weak_no_runtime : Error<
  "cannot create __weak reference because the current deployment target "
  "does not support weak references">;
def err_arc_weak_disabled : Error<
  "cannot create __weak reference in file using manual reference counting">;
def err_synthesizing_arc_weak_property_disabled : Error<
  "cannot synthesize weak property in file using manual reference counting">;
def err_synthesizing_arc_weak_property_no_runtime : Error<
  "cannot synthesize weak property because the current deployment target "
  "does not support weak references">;
def err_arc_unsupported_weak_class : Error<
  "class is incompatible with __weak references">;
def err_arc_weak_unavailable_assign : Error<
  "assignment of a weak-unavailable object to a __weak object">;
def err_arc_weak_unavailable_property : Error<
  "synthesizing __weak instance variable of type %0, which does not "
  "support weak references">;
def note_implemented_by_class : Note<
  "when implemented by class %0">;
def err_arc_convesion_of_weak_unavailable : Error<
  "%select{implicit conversion|cast}0 of weak-unavailable object of type %1 to"
  " a __weak object of type %2">;

} // end "ARC Weak References" category

let CategoryName = "ARC Restrictions" in {

def err_unavailable_in_arc : Error<
  "%0 is unavailable in ARC">;
def note_arc_forbidden_type : Note<
  "declaration uses type that is ill-formed in ARC">;
def note_performs_forbidden_arc_conversion : Note<
  "inline function performs a conversion which is forbidden in ARC">;
def note_arc_init_returns_unrelated : Note<
  "init method must return a type related to its receiver type">;
def note_arc_weak_disabled : Note<
  "declaration uses __weak, but ARC is disabled">;
def note_arc_weak_no_runtime : Note<"declaration uses __weak, which "
  "the current deployment target does not support">;
def note_arc_field_with_ownership : Note<
  "field has non-trivial ownership qualification">;

def err_arc_illegal_explicit_message : Error<
  "ARC forbids explicit message send of %0">;
def err_arc_unused_init_message : Error<
  "the result of a delegate init call must be immediately returned "
  "or assigned to 'self'">;
def err_arc_mismatched_cast : Error<
  "%select{implicit conversion|cast}0 of "
  "%select{%2|a non-Objective-C pointer type %2|a block pointer|"
  "an Objective-C pointer|an indirect pointer to an Objective-C pointer}1"
  " to %3 is disallowed with ARC">;
def err_arc_nolifetime_behavior : Error<
  "explicit ownership qualifier on cast result has no effect">;
def err_arc_objc_property_default_assign_on_object : Error<
  "ARC forbids synthesizing a property of an Objective-C object "
  "with unspecified ownership or storage attribute">;
def err_arc_illegal_selector : Error<
  "ARC forbids use of %0 in a @selector">;
def err_arc_illegal_method_def : Error<
  "ARC forbids %select{implementation|synthesis}0 of %1">;
def warn_arc_strong_pointer_objc_pointer : Warning<
  "没有显式所有权的%0类型的方法参数">,
  InGroup<DiagGroup<"explicit-ownership-type">>, DefaultIgnore;

} // end "ARC Restrictions" category

def err_arc_lost_method_convention : Error<
  "method was declared as %select{an 'alloc'|a 'copy'|an 'init'|a 'new'}0 "
  "method, but its implementation doesn't match because %select{"
  "its result type is not an object pointer|"
  "its result type is unrelated to its receiver type}1">;
def note_arc_lost_method_convention : Note<"declaration in interface">;
def err_arc_gained_method_convention : Error<
  "method implementation does not match its declaration">;
def note_arc_gained_method_convention : Note<
  "declaration in interface is not in the '%select{alloc|copy|init|new}0' "
  "family because %select{its result type is not an object pointer|"
  "its result type is unrelated to its receiver type}1">;
def err_typecheck_arc_assign_self : Error<
  "cannot assign to 'self' outside of a method in the init family">;
def err_typecheck_arc_assign_self_class_method : Error<
  "cannot assign to 'self' in a class method">;
def err_typecheck_arr_assign_enumeration : Error<
  "fast enumeration variables cannot be modified in ARC by default; "
  "declare the variable __strong to allow this">;
def err_typecheck_arc_assign_externally_retained : Error<
  "variable declared with 'objc_externally_retained' "
  "cannot be modified in ARC">;
def warn_arc_retained_assign : Warning<
  "将保留对象分配给 %select{weak|unsafe_unretained}0 "
  "%select{property|variable}1"
  "; 对象将在赋值后被释放">,
  InGroup<ARCUnsafeRetainedAssign>;
def warn_arc_retained_property_assign : Warning<
  "将保留对象分配给不安全的属性"
  "; 对象将在赋值后被释放">,
  InGroup<ARCUnsafeRetainedAssign>;
def warn_arc_literal_assign : Warning<
  "分配 %select{array literal|dictionary literal|numeric literal|boxed expression|<should not happen>|block literal}0"
  "给一个弱的 %select{property|variable}1"
  "; 对象将在赋值后被释放">,
  InGroup<ARCUnsafeRetainedAssign>;
def err_arc_new_array_without_ownership : Error<
  "'new' cannot allocate an array of %0 with no explicit ownership">;
def err_arc_autoreleasing_var : Error<
  "%select{__block variables|global variables|fields|instance variables}0 cannot have "
  "__autoreleasing ownership">;
def err_arc_autoreleasing_capture : Error<
  "cannot capture __autoreleasing variable in a "
  "%select{block|lambda by copy}0">;
def err_arc_thread_ownership : Error<
  "thread-local variable has non-trivial ownership: type is %0">;
def err_arc_indirect_no_ownership : Error<
  "%select{pointer|reference}1 to non-const type %0 with no explicit ownership">;
def err_arc_array_param_no_ownership : Error<
  "must explicitly describe intended ownership of an object array parameter">;
def err_arc_pseudo_dtor_inconstant_quals : Error<
  "pseudo-destructor destroys object of type %0 with inconsistently-qualified "
  "type %1">;
def err_arc_init_method_unrelated_result_type : Error<
  "init methods must return a type related to the receiver type">;
def err_arc_nonlocal_writeback : Error<
  "passing address of %select{non-local|non-scalar}0 object to "
  "__autoreleasing parameter for write-back">;
def err_arc_method_not_found : Error<
  "no known %select{instance|class}1 method for selector %0">;
def err_arc_receiver_forward_class : Error<
  "receiver %0 for class message is a forward declaration">;
def err_arc_may_not_respond : Error<
  "no visible @interface for %0 declares the selector %1">;
def err_arc_receiver_forward_instance : Error<
  "receiver type %0 for instance message is a forward declaration">;
def warn_receiver_forward_instance : Warning<
  "实例消息的接收器类型%0是一个转发声明">,
  InGroup<ForwardClassReceiver>, DefaultIgnore;
def err_arc_collection_forward : Error<
  "collection expression type %0 is a forward declaration">;
def err_arc_multiple_method_decl : Error<
  "multiple methods named %0 found with mismatched result, "
  "parameter type or attributes">;
def warn_arc_lifetime_result_type : Warning<
  "ARC %select{unused|__unsafe_unretained|__strong|__weak|__autoreleasing}0 "
  "忽略返回类型上的生存期限定符">,
  InGroup<IgnoredQualifiers>;

let CategoryName = "ARC Retain Cycle" in {

def warn_arc_retain_cycle : Warning<
  "在此块中强烈捕获 %0 可能会导致一个保留周期">,
  InGroup<ARCRetainCycles>;
def note_arc_retain_cycle_owner : Note<
  "block will be retained by %select{the captured object|an object strongly "
  "retained by the captured object}0">;

} // end "ARC Retain Cycle" category

def warn_arc_object_memaccess : Warning<
  "%select{destination for|source of}0 此 %1 调用是一个指向 "
  "所有权限定类型 %2 的指针">, InGroup<ARCNonPodMemAccess>;

let CategoryName = "ARC and @properties" in {

def err_arc_strong_property_ownership : Error<
  "existing instance variable %1 for strong property %0 may not be "
  "%select{|__unsafe_unretained||__weak}2">;
def err_arc_assign_property_ownership : Error<
  "existing instance variable %1 for property %0 with %select{unsafe_unretained|assign}2 "
  "attribute must be __unsafe_unretained">;
def err_arc_inconsistent_property_ownership : Error<
  "%select{|unsafe_unretained|strong|weak}1 property %0 may not also be "
  "declared %select{|__unsafe_unretained|__strong|__weak|__autoreleasing}2">;

} // end "ARC and @properties" category

def warn_block_capture_autoreleasing : Warning<
  "块捕获自动释放的输出参数，这可能导致 "
  "释放后使用错误">,
  InGroup<BlockCaptureAutoReleasing>;
def note_declare_parameter_strong : Note<
  "declare the parameter __strong or capture a __block __strong variable to "
  "keep values alive across autorelease pools">;

def err_arc_atomic_ownership : Error<
  "cannot perform atomic operation on a pointer to type %0: type has "
  "non-trivial ownership">;

let CategoryName = "ARC Casting Rules" in {

def err_arc_bridge_cast_incompatible : Error<
  "incompatible types casting %0 to %1 with a %select{__bridge|"
  "__bridge_transfer|__bridge_retained}2 cast">;
def err_arc_bridge_cast_wrong_kind : Error<
  "cast of %select{Objective-C|block|C}0 pointer type %1 to "
  "%select{Objective-C|block|C}2 pointer type %3 cannot use %select{__bridge|"
  "__bridge_transfer|__bridge_retained}4">;
def err_arc_cast_requires_bridge : Error<
  "%select{cast|implicit conversion}0 of %select{Objective-C|block|C}1 "
  "pointer type %2 to %select{Objective-C|block|C}3 pointer type %4 "
  "requires a bridged cast">;
def note_arc_bridge : Note<
  "use __bridge to convert directly (no change in ownership)">;
def note_arc_cstyle_bridge : Note<
  "use __bridge with C-style cast to convert directly (no change in ownership)">;
def note_arc_bridge_transfer : Note<
  "use %select{__bridge_transfer|CFBridgingRelease call}1 to transfer "
  "ownership of a +1 %0 into ARC">;
def note_arc_cstyle_bridge_transfer : Note<
  "use __bridge_transfer with C-style cast to transfer "
  "ownership of a +1 %0 into ARC">;
def note_arc_bridge_retained : Note<
  "use %select{__bridge_retained|CFBridgingRetain call}1 to make an "
  "ARC object available as a +1 %0">;
def note_arc_cstyle_bridge_retained : Note<
  "use __bridge_retained with C-style cast to make an "
  "ARC object available as a +1 %0">;

} // ARC Casting category

} // ARC category name

def err_flexible_array_init_needs_braces : Error<
  "flexible array requires brace-enclosed initializer">;
def err_illegal_decl_array_of_functions : Error<
  "'%0' declared as array of functions of type %1">;
def err_array_incomplete_or_sizeless_type : Error<
  "array has %select{incomplete|sizeless}0 element type %1">;
def err_illegal_message_expr_incomplete_type : Error<
  "Objective-C message has incomplete result type %0">;
def err_illegal_decl_array_of_references : Error<
  "'%0' declared as array of references of type %1">;
def err_decl_negative_array_size : Error<
  "'%0' declared as an array with a negative size">;
def err_array_static_outside_prototype : Error<
  "%0 used in array declarator outside of function prototype">;
def err_array_static_not_outermost : Error<
  "%0 used in non-outermost array type derivation">;
def err_array_star_outside_prototype : Error<
  "star modifier used outside of function prototype">;
def err_illegal_decl_pointer_to_reference : Error<
  "'%0' declared as a pointer to a reference of type %1">;
def err_illegal_decl_mempointer_to_reference : Error<
  "'%0' declared as a member pointer to a reference of type %1">;
def err_illegal_decl_mempointer_to_void : Error<
  "'%0' declared as a member pointer to void">;
def err_illegal_decl_mempointer_in_nonclass : Error<
  "'%0' does not point into a class">;
def err_mempointer_in_nonclass_type : Error<
  "member pointer refers into non-class type %0">;
def err_reference_to_void : Error<"cannot form a reference to 'void'">;
def err_nonfunction_block_type : Error<
  "block pointer to non-function type is invalid">;
def err_return_block_has_expr : Error<"void block should not return a value">;
def err_block_return_missing_expr : Error<
  "non-void block should return a value">;
def err_func_def_incomplete_result : Error<
  "incomplete result type %0 in function definition">;
def err_atomic_specifier_bad_type
    : Error<"_Atomic cannot be applied to "
            "%select{incomplete |array |function |reference |atomic |qualified "
            "|sizeless ||integer }0type "
            "%1 %select{|||||||which is not trivially copyable|}0">;

// Expressions.
def ext_sizeof_alignof_function_type : Extension<
  "invalid application of '%0' to a function type">, InGroup<PointerArith>;
def ext_sizeof_alignof_void_type : Extension<
  "invalid application of '%0' to a void type">, InGroup<PointerArith>;
def err_opencl_sizeof_alignof_type : Error<
  "invalid application of '%0' to a void type">;
def err_sizeof_alignof_incomplete_or_sizeless_type : Error<
  "invalid application of '%0' to %select{an incomplete|sizeless}1 type %2">;
def err_sizeof_alignof_function_type : Error<
  "invalid application of '%0' to a function type">;
def err_openmp_default_simd_align_expr : Error<
  "invalid application of '__builtin_omp_required_simd_align' to an expression, only type is allowed">;
def err_sizeof_alignof_typeof_bitfield : Error<
  "invalid application of '%select{sizeof|alignof|typeof}0' to bit-field">;
def err_alignof_member_of_incomplete_type : Error<
  "invalid application of 'alignof' to a field of a class still being defined">;
def err_vecstep_non_scalar_vector_type : Error<
  "'vec_step' requires built-in scalar or vector type, %0 invalid">;
def err_offsetof_incomplete_type : Error<
  "offsetof of incomplete type %0">;
def err_offsetof_record_type : Error<
  "offsetof requires struct, union, or class type, %0 invalid">;
def err_offsetof_array_type : Error<"offsetof requires array type, %0 invalid">;
def ext_offsetof_non_pod_type : ExtWarn<"offset of on non-POD type %0">,
  InGroup<InvalidOffsetof>;
def ext_offsetof_non_standardlayout_type : ExtWarn<
  "offset of on non-standard-layout type %0">, InGroup<InvalidOffsetof>;
def err_offsetof_bitfield : Error<"cannot compute offset of bit-field %0">;
def err_offsetof_field_of_virtual_base : Error<
  "invalid application of 'offsetof' to a field of a virtual base">;
def warn_sub_ptr_zero_size_types : Warning<
  "减去指向大小为零的类型 %0 的指针具有未定义的行为">,
  InGroup<PointerArith>;
def warn_pointer_arith_null_ptr : Warning<
  "对空指针执行指针运算具有未定义的行为%select{| if the offset is nonzero}0">,
  InGroup<NullPointerArithmetic>, DefaultIgnore;
def warn_gnu_null_ptr_arith : Warning<
  "将空指针视为从整数到指针的转换的算术是 GNU 扩展">,
  InGroup<NullPointerArithmetic>, DefaultIgnore;

def warn_floatingpoint_eq : Warning<
  "将浮点数与 == 或 != 进行比较是不安全的">,
  InGroup<DiagGroup<"float-equal">>, DefaultIgnore;

def warn_remainder_division_by_zero : Warning<
  "%select{remainder|division}0 用零是未定义的">,
  InGroup<DivZero>;
def warn_shift_lhs_negative : Warning<"移动带负号的值是未定义的">,
  InGroup<DiagGroup<"shift-negative-value">>;
def warn_shift_negative : Warning<"移位计数为负数">,
  InGroup<DiagGroup<"shift-count-negative">>;
def warn_shift_gt_typewidth : Warning<"移位计数>=类型宽度">,
  InGroup<DiagGroup<"shift-count-overflow">>;
def warn_shift_result_gt_typewidth : Warning<
  "有符号移位结果（%0）需要%1位来表示，但%2只有 "
  "%3位">, InGroup<DiagGroup<"shift-overflow">>;
def warn_shift_result_sets_sign_bit : Warning<
  "有符号移位结果（%0）设置移位表达式类型（%1）的符号位 "
  "并变为负数">,
  InGroup<DiagGroup<"shift-sign-overflow">>, DefaultIgnore;

def warn_precedence_bitwise_rel : Warning<
  "%0的优先级低于%1;%1将首先进行计算">,
  InGroup<Parentheses>;
def note_precedence_bitwise_first : Note<
  "place parentheses around the %0 expression to evaluate it first">;
def note_precedence_silence : Note<
  "place parentheses around the '%0' expression to silence this warning">;

def warn_precedence_conditional : Warning<
  "运算符'?:'优先级低于'%0';'%0'将首先计算">,
  InGroup<Parentheses>;
def warn_precedence_bitwise_conditional : Warning<
  "运算符'?:'优先级低于'%0';'%0'将首先计算”">,
  InGroup<BitwiseConditionalParentheses>;
def note_precedence_conditional_first : Note<
  "place parentheses around the '?:' expression to evaluate it first">;

def warn_enum_constant_in_bool_context : Warning<
  "将枚举常量转换为布尔值">,
  InGroup<IntInBoolContext>, DefaultIgnore;
def warn_left_shift_in_bool_context : Warning<
  "将'<<'的结果转换为布尔值；您的意思是'(%0) != 0'?">,
  InGroup<IntInBoolContext>, DefaultIgnore;
def warn_logical_instead_of_bitwise : Warning<
  "对常量操作数使用逻辑'%0'">,
  InGroup<DiagGroup<"constant-logical-operand">>;
def note_logical_instead_of_bitwise_change_operator : Note<
  "use '%0' for a bitwise operation">;
def note_logical_instead_of_bitwise_remove_constant : Note<
  "remove constant to silence this warning">;

def warn_bitwise_op_in_bitwise_op : Warning<
  "'%0' 在 '%1'之中">, InGroup<BitwiseOpParentheses>, DefaultIgnore;

def warn_logical_and_in_logical_or : Warning<
  "'&&' 在 '||'之中">, InGroup<LogicalOpParentheses>, DefaultIgnore;

def warn_overloaded_shift_in_comparison :Warning<
  "重载运算符%select{>>|<<}0的优先级高于 "
  "比较运算符">,
  InGroup<OverloadedShiftOpParentheses>;
def note_evaluate_comparison_first :Note<
  "place parentheses around comparison expression to evaluate it first">;

def note_concatenated_string_literal_silence :Note<
  "在字符串文字周围加括号以消除警告">;

def warn_addition_in_bitshift : Warning<
  "运算符 '%0' 的优先级低于 '%1'; "
  "'%1' 将首先计算">, InGroup<ShiftOpParentheses>;

def warn_self_assignment_builtin : Warning<
  "将类型为%0的变量的值显式分配给自身">,
  InGroup<SelfAssignment>, DefaultIgnore;
def warn_self_assignment_overloaded : Warning<
  "将类型为%0的变量的值显式分配给自身">,
  InGroup<SelfAssignmentOverloaded>, DefaultIgnore;
def warn_self_move : Warning<
  "将类型为%0的变量显式移动到自身">,
  InGroup<SelfMove>, DefaultIgnore;

def warn_redundant_move_on_return : Warning<
  "冗余移入返回语句">,
  InGroup<RedundantMove>, DefaultIgnore;
def warn_pessimizing_move_on_return : Warning<
  "在return语句中移动本地对象可防止复制省略">,
  InGroup<PessimizingMove>, DefaultIgnore;
def warn_pessimizing_move_on_initialization : Warning<
  "移动临时对象可防止复制省略">,
  InGroup<PessimizingMove>, DefaultIgnore;
def note_remove_move : Note<"remove std::move call here">;

def warn_return_std_move : Warning<
  "局部变量%0将被复制，尽管按名称为%select{returned|thrown}1 ">,
  InGroup<ReturnStdMove>, DefaultIgnore;
def note_add_std_move : Note<
  "call 'std::move' explicitly to avoid copying">;
def warn_return_std_move_in_cxx11 : Warning<
  "在根据ISO C++11解决缺陷报告之前 "
  "局部变量%0将会被复制，尽管局按名称返回 "
  "由于其与函数返回类型不匹配%diff{ ($ vs $)|}1,2">,
  InGroup<ReturnStdMoveInCXX11>, DefaultIgnore;
def note_add_std_move_in_cxx11 : Note<
  "call 'std::move' explicitly to avoid copying on older compilers">;

def warn_string_plus_int : Warning<
  "将%0添加到字符串不会附加到字符串">,
  InGroup<StringPlusInt>;
def warn_string_plus_char : Warning<
  "将%0添加到字符串指针不会附加到字符串">,
  InGroup<StringPlusChar>;
def note_string_plus_scalar_silence : Note<
  "使用数组索引使此警告静音">;

def warn_sizeof_array_param : Warning<
  "数组函数参数上的sizeof将返回%0的大小，而不是%1">,
  InGroup<SizeofArrayArgument>;

def warn_sizeof_array_decay : Warning<
  "指针操作上的sizeof将返回%0的大小，而不是%1">,
  InGroup<SizeofArrayDecay>;

def err_sizeof_nonfragile_interface : Error<
  "application of '%select{alignof|sizeof}1' to interface %0 is "
  "not supported on this architecture and platform">;
def err_atdef_nonfragile_interface : Error<
  "use of @defs is not supported on this architecture and platform">;
def err_subscript_nonfragile_interface : Error<
  "subscript requires size of interface %0, which is not constant for "
  "this architecture and platform">;

def err_arithmetic_nonfragile_interface : Error<
  "arithmetic on pointer to interface %0, which is not a constant size for "
  "this architecture and platform">;

def warn_deprecated_comma_subscript : Warning<
  "不推荐使用数组下标中的顶级逗号表达式">,
  InGroup<DeprecatedCommaSubscript>;

def ext_subscript_non_lvalue : Extension<
  "ISO C90 does not allow subscripting non-lvalue array">;
def err_typecheck_subscript_value : Error<
  "subscripted value is not an array, pointer, or vector">;
def err_typecheck_subscript_not_integer : Error<
  "array subscript is not an integer">;
def err_subscript_function_type : Error<
  "subscript of pointer to function type %0">;
def err_subscript_incomplete_or_sizeless_type : Error<
  "subscript of pointer to %select{incomplete|sizeless}0 type %1">;
def err_dereference_incomplete_type : Error<
  "dereference of pointer to incomplete type %0">;
def ext_gnu_subscript_void_type : Extension<
  "subscript of a pointer to void is a GNU extension">, InGroup<PointerArith>;
def err_typecheck_member_reference_struct_union : Error<
  "member reference base type %0 is not a structure or union">;
def err_typecheck_member_reference_ivar : Error<
  "%0 does not have a member named %1">;
def err_arc_weak_ivar_access : Error<
  "dereferencing a __weak pointer is not allowed due to possible "
  "null value caused by race condition, assign it to strong variable first">;
def err_typecheck_member_reference_arrow : Error<
  "member reference type %0 is not a pointer">;
def err_typecheck_member_reference_suggestion : Error<
  "member reference type %0 is %select{a|not a}1 pointer; did you mean to use '%select{->|.}1'?">;
def note_typecheck_member_reference_suggestion : Note<
  "did you mean to use '.' instead?">;
def note_member_reference_arrow_from_operator_arrow : Note<
  "'->' applied to return value of the operator->() declared here">;
def err_typecheck_member_reference_type : Error<
  "cannot refer to type member %0 in %1 with '%select{.|->}2'">;
def err_typecheck_member_reference_unknown : Error<
  "cannot refer to member %0 in %1 with '%select{.|->}2'">;
def err_member_reference_needs_call : Error<
  "base of member reference is a function; perhaps you meant to call "
  "it%select{| with no arguments}0?">;
def warn_subscript_is_char : Warning<"数组下标的类型为'char'">,
  InGroup<CharSubscript>, DefaultIgnore;

def err_typecheck_incomplete_tag : Error<"incomplete definition of type %0">;
def err_no_member : Error<"no member named %0 in %1">;
def err_no_member_overloaded_arrow : Error<
  "no member named %0 in %1; did you mean to use '->' instead of '.'?">;

def err_member_not_yet_instantiated : Error<
  "no member %0 in %1; it has not yet been instantiated">;
def note_non_instantiated_member_here : Note<
  "not-yet-instantiated member is declared here">;

def err_enumerator_does_not_exist : Error<
  "enumerator %0 does not exist in instantiation of %1">;
def note_enum_specialized_here : Note<
  "enum %0 was explicitly specialized here">;

def err_specialization_not_primary_template : Error<
  "cannot reference member of primary template because deduced class "
  "template specialization %0 is %select{instantiated from a partial|"
  "an explicit}1 specialization">;

def err_member_redeclared : Error<"class member cannot be redeclared">;
def ext_member_redeclared : ExtWarn<"class member cannot be redeclared">,
  InGroup<RedeclaredClassMember>;
def err_member_redeclared_in_instantiation : Error<
  "multiple overloads of %0 instantiate to the same signature %1">;
def err_member_name_of_class : Error<"member %0 has the same name as its class">;
def err_member_def_undefined_record : Error<
  "out-of-line definition of %0 from class %1 without definition">;
def err_member_decl_does_not_match : Error<
  "out-of-line %select{declaration|definition}2 of %0 "
  "does not match any declaration in %1">;
def err_friend_decl_with_def_arg_must_be_def : Error<
  "friend declaration specifying a default argument must be a definition">;
def err_friend_decl_with_def_arg_redeclared : Error<
  "friend declaration specifying a default argument must be the only declaration">;
def err_friend_decl_does_not_match : Error<
  "friend declaration of %0 does not match any declaration in %1">;
def err_member_decl_does_not_match_suggest : Error<
  "out-of-line %select{declaration|definition}2 of %0 "
  "does not match any declaration in %1; did you mean %3?">;
def err_member_def_does_not_match_ret_type : Error<
  "return type of out-of-line definition of %q0 differs from "
  "that in the declaration">;
def err_nonstatic_member_out_of_line : Error<
  "non-static data member defined out-of-line">;
def err_qualified_typedef_declarator : Error<
  "typedef declarator cannot be qualified">;
def err_qualified_param_declarator : Error<
  "parameter declarator cannot be qualified">;
def ext_out_of_line_declaration : ExtWarn<
  "out-of-line declaration of a member must be a definition">,
  InGroup<OutOfLineDeclaration>, DefaultError;
def err_member_extra_qualification : Error<
  "extra qualification on member %0">;
def warn_member_extra_qualification : Warning<
  err_member_extra_qualification.Text>, InGroup<MicrosoftExtraQualification>;
def warn_namespace_member_extra_qualification : Warning<
  "成员 %0 的额外资格">,
  InGroup<DiagGroup<"extra-qualification">>;
def err_member_qualification : Error<
  "non-friend class member %0 cannot have a qualified name">;
def note_member_def_close_match : Note<"member declaration nearly matches">;
def note_member_def_close_const_match : Note<
  "member declaration does not match because "
  "it %select{is|is not}0 const qualified">;
def note_member_def_close_param_match : Note<
  "type of %ordinal0 parameter of member declaration does not match definition"
  "%diff{ ($ vs $)|}1,2">;
def note_local_decl_close_match : Note<"local declaration nearly matches">;
def note_local_decl_close_param_match : Note<
  "type of %ordinal0 parameter of local declaration does not match definition"
  "%diff{ ($ vs $)|}1,2">;
def err_typecheck_ivar_variable_size : Error<
  "instance variables must have a constant size">;
def err_ivar_reference_type : Error<
  "instance variables cannot be of reference type">;
def err_typecheck_illegal_increment_decrement : Error<
  "cannot %select{decrement|increment}1 value of type %0">;
def err_typecheck_expect_int : Error<
  "used type %0 where integer is required">;
def err_typecheck_arithmetic_incomplete_or_sizeless_type : Error<
  "arithmetic on a pointer to %select{an incomplete|sizeless}0 type %1">;
def err_typecheck_pointer_arith_function_type : Error<
  "arithmetic on%select{ a|}0 pointer%select{|s}0 to%select{ the|}2 "
  "function type%select{|s}2 %1%select{| and %3}2">;
def err_typecheck_pointer_arith_void_type : Error<
  "arithmetic on%select{ a|}0 pointer%select{|s}0 to void">;
def err_typecheck_decl_incomplete_type : Error<
  "variable has incomplete type %0">;
def ext_typecheck_decl_incomplete_type : ExtWarn<
  "tentative definition of variable with internal linkage has incomplete non-array type %0">,
  InGroup<DiagGroup<"tentative-definition-incomplete-type">>;
def err_tentative_def_incomplete_type : Error<
  "tentative definition has type %0 that is never completed">;
def warn_tentative_incomplete_array : Warning<
  "假设有一个元素的暂定数组定义">;
def err_typecheck_incomplete_array_needs_initializer : Error<
  "definition of variable with array type needs an explicit size "
  "or an initializer">;
def err_array_init_not_init_list : Error<
  "array initializer must be an initializer "
  "list%select{| or string literal| or wide string literal}0">;
def err_array_init_narrow_string_into_wchar : Error<
  "initializing wide char array with non-wide string literal">;
def err_array_init_wide_string_into_char : Error<
  "initializing char array with wide string literal">;
def err_array_init_incompat_wide_string_into_wchar : Error<
  "initializing wide char array with incompatible wide string literal">;
def err_array_init_plain_string_into_char8_t : Error<
  "initializing 'char8_t' array with plain string literal">;
def note_array_init_plain_string_into_char8_t : Note<
  "add 'u8' prefix to form a 'char8_t' string literal">;
def err_array_init_utf8_string_into_char : Error<
  "%select{|ISO C++20 does not permit }0initialization of char array with "
  "UTF-8 string literal%select{ is not permitted by '-fchar8_t'|}0">;
def warn_cxx20_compat_utf8_string : Warning<
  "UTF-8 字符串文字的类型将在 C++20 中从 const char 数组 "
  "更改为 const char8_t 数组">, InGroup<CXX20Compat>, DefaultIgnore;
def note_cxx20_compat_utf8_string_remove_u8 : Note<
  "remove 'u8' prefix to avoid a change of behavior; "
  "Clang encodes unprefixed narrow string literals as UTF-8">;
def err_array_init_different_type : Error<
  "cannot initialize array %diff{of type $ with array of type $|"
  "with different type of array}0,1">;
def err_array_init_non_constant_array : Error<
  "cannot initialize array %diff{of type $ with non-constant array of type $|"
  "with different type of array}0,1">;
def ext_array_init_copy : Extension<
  "initialization of an array "
  "%diff{of type $ from a compound literal of type $|"
  "from a compound literal}0,1 is a GNU extension">, InGroup<GNUCompoundLiteralInitializer>;
// This is intentionally not disabled by -Wno-gnu.
def ext_array_init_parens : ExtWarn<
  "parenthesized initialization of a member array is a GNU extension">,
  InGroup<DiagGroup<"gnu-array-member-paren-init">>, DefaultError;
def warn_deprecated_string_literal_conversion : Warning<
  "不推荐从字符串文字到 %0 的转换">,
  InGroup<CXX11CompatDeprecatedWritableStr>;
def ext_deprecated_string_literal_conversion : ExtWarn<
  "ISO C++11 does not allow conversion from string literal to %0">,
  InGroup<WritableStrings>, SFINAEFailure;
def err_realimag_invalid_type : Error<"invalid type %0 to %1 operator">;
def err_typecheck_sclass_fscope : Error<
  "illegal storage class on file-scoped variable">;
def warn_standalone_specifier : Warning<"在此声明中忽略了'%0'">,
  InGroup<MissingDeclarations>;
def ext_standalone_specifier : ExtWarn<"'%0' is not permitted on a declaration "
  "of a type">, InGroup<MissingDeclarations>;
def err_standalone_class_nested_name_specifier : Error<
  "forward declaration of %select{class|struct|interface|union|enum}0 cannot "
  "have a nested name specifier">;
def err_typecheck_sclass_func : Error<"illegal storage class on function">;
def err_static_block_func : Error<
  "function declared in block scope cannot have 'static' storage class">;
def err_typecheck_address_of : Error<"address of %select{bit-field"
  "|vector element|property expression|register variable|matrix element}0 requested">;
def ext_typecheck_addrof_void : Extension<
  "ISO C forbids taking the address of an expression of type 'void'">;
def err_unqualified_pointer_member_function : Error<
  "must explicitly qualify name of member function when taking its address">;
def err_invalid_form_pointer_member_function : Error<
  "cannot create a non-constant pointer to member function">;
def err_address_of_function_with_pass_object_size_params: Error<
  "cannot take address of function %0 because parameter %1 has "
  "pass_object_size attribute">;
def err_parens_pointer_member_function : Error<
  "cannot parenthesize the name of a method when forming a member pointer">;
def err_typecheck_invalid_lvalue_addrof_addrof_function : Error<
  "extra '&' taking address of overloaded function">;
def err_typecheck_invalid_lvalue_addrof : Error<
  "cannot take the address of an rvalue of type %0">;
def ext_typecheck_addrof_temporary : ExtWarn<
  "taking the address of a temporary object of type %0">,
  InGroup<AddressOfTemporary>, DefaultError;
def err_typecheck_addrof_temporary : Error<
  "taking the address of a temporary object of type %0">;
def err_typecheck_addrof_dtor : Error<
  "taking the address of a destructor">;
def err_typecheck_unary_expr : Error<
  "invalid argument type %0 to unary expression">;
def err_typecheck_indirection_requires_pointer : Error<
  "indirection requires pointer operand (%0 invalid)">;
def ext_typecheck_indirection_through_void_pointer : ExtWarn<
  "ISO C++ does not allow indirection on operand of type %0">,
  InGroup<DiagGroup<"void-ptr-dereference">>;
def warn_indirection_through_null : Warning<
  "非易失性空指针的间接将被删除，而不是陷阱">,
  InGroup<NullDereference>;
def warn_binding_null_to_reference : Warning<
  "将取消引用的空指针绑定到引用具有未定义的行为">,
  InGroup<NullDereference>;
def note_indirection_through_null : Note<
  "consider using __builtin_trap() or qualifying pointer with 'volatile'">;
def warn_pointer_indirection_from_incompatible_type : Warning<
  "从类型 %0 重新解释转换的类型 %1 的取消引用 "
  "具有未定义的行为">,
  InGroup<UndefinedReinterpretCast>, DefaultIgnore;
def warn_taking_address_of_packed_member : Warning<
  "获取压缩成员 %0 属于类或结构 %q1, 的地址可能会导致指针值未对齐">,
  InGroup<DiagGroup<"address-of-packed-member">>;

def err_objc_object_assignment : Error<
  "cannot assign to class object (%0 invalid)">;
def err_typecheck_invalid_operands : Error<
  "invalid operands to binary expression (%0 and %1)">;
def note_typecheck_invalid_operands_converted : Note<
  "%select{first|second}0 operand was implicitly converted to type %1">;
def err_typecheck_logical_vector_expr_gnu_cpp_restrict : Error<
  "logical expression with vector %select{type %1 and non-vector type %2|types"
  " %1 and %2}0 is only supported in C++">;
def err_typecheck_sub_ptr_compatible : Error<
  "%diff{$ and $ are not pointers to compatible types|"
  "pointers to incompatible types}0,1">;
def ext_typecheck_ordered_comparison_of_pointer_integer : ExtWarn<
  "ordered comparison between pointer and integer (%0 and %1)">;
def ext_typecheck_ordered_comparison_of_pointer_and_zero : Extension<
  "ordered comparison between pointer and zero (%0 and %1) is an extension">;
def err_typecheck_ordered_comparison_of_pointer_and_zero : Error<
  "ordered comparison between pointer and zero (%0 and %1)">;
def err_typecheck_three_way_comparison_of_pointer_and_zero : Error<
  "three-way comparison between pointer and zero">;
def ext_typecheck_compare_complete_incomplete_pointers : Extension<
  "pointer comparisons before C11 "
  "need to be between two complete or two incomplete types; "
  "%0 is %select{|in}2complete and "
  "%1 is %select{|in}3complete">,
  InGroup<C11>;
def ext_typecheck_ordered_comparison_of_function_pointers : ExtWarn<
  "ordered comparison of function pointers (%0 and %1)">,
  InGroup<DiagGroup<"ordered-compare-function-pointers">>;
def ext_typecheck_comparison_of_fptr_to_void : Extension<
  "equality comparison between function pointer and void pointer (%0 and %1)">;
def err_typecheck_comparison_of_fptr_to_void : Error<
  "equality comparison between function pointer and void pointer (%0 and %1)">;
def ext_typecheck_comparison_of_pointer_integer : ExtWarn<
  "comparison between pointer and integer (%0 and %1)">,
  InGroup<DiagGroup<"pointer-integer-compare">>;
def err_typecheck_comparison_of_pointer_integer : Error<
  "comparison between pointer and integer (%0 and %1)">;
def ext_typecheck_comparison_of_distinct_pointers : ExtWarn<
  "comparison of distinct pointer types%diff{ ($ and $)|}0,1">,
  InGroup<CompareDistinctPointerType>;
def ext_typecheck_cond_incompatible_operands : ExtWarn<
  "incompatible operand types (%0 and %1)">;
def err_cond_voidptr_arc : Error <
  "operands to conditional of types%diff{ $ and $|}0,1 are incompatible "
  "in ARC mode">;
def err_typecheck_comparison_of_distinct_pointers : Error<
  "comparison of distinct pointer types%diff{ ($ and $)|}0,1">;
def err_typecheck_op_on_nonoverlapping_address_space_pointers : Error<
  "%select{comparison between %diff{ ($ and $)|}0,1"
  "|arithmetic operation with operands of type %diff{ ($ and $)|}0,1"
  "|conditional operator with the second and third operands of type "
  "%diff{ ($ and $)|}0,1}2"
  " which are pointers to non-overlapping address spaces">;

def select_arith_conv_kind : TextSubstitution<
  "%select{arithmetic between|bitwise operation between|comparison of|"
  "conditional expression between|compound assignment of}0">;
def warn_arith_conv_enum_float : Warning<
  "%sub{select_arith_conv_kind}0 "
  "%select{floating-point|enumeration}1 类型 %2 "
  "%plural{2:with|4:from|:and}0 "
  "%select{enumeration|floating-point}1 类型 %3">,
  InGroup<EnumFloatConversion>, DefaultIgnore;
def warn_arith_conv_enum_float_cxx20 : Warning<
  "%sub{select_arith_conv_kind}0 "
  "%select{floating-point|enumeration}1 类型 %2 "
  "%plural{2:with|4:from|:and}0 "
  "%select{enumeration|floating-point}1 类型 %3 已弃用">,
  InGroup<DeprecatedEnumFloatConversion>;
def warn_arith_conv_mixed_enum_types : Warning<
  "%sub{select_arith_conv_kind}0 "
  "不同的枚举类型%diff{ ($ and $)|}1,2">,
  InGroup<EnumEnumConversion>, DefaultIgnore;
def warn_arith_conv_mixed_enum_types_cxx20 : Warning<
  "%sub{select_arith_conv_kind}0 "
  "不同的枚举类型%diff{ ($ and $)|}1,2 已弃用">,
  InGroup<DeprecatedEnumEnumConversion>;
def warn_arith_conv_mixed_anon_enum_types : Warning<
  warn_arith_conv_mixed_enum_types.Text>,
  InGroup<AnonEnumEnumConversion>, DefaultIgnore;
def warn_arith_conv_mixed_anon_enum_types_cxx20 : Warning<
  warn_arith_conv_mixed_enum_types_cxx20.Text>,
  InGroup<DeprecatedAnonEnumEnumConversion>;
def warn_conditional_mixed_enum_types : Warning<
  warn_arith_conv_mixed_enum_types.Text>,
  InGroup<EnumCompareConditional>, DefaultIgnore;
def warn_conditional_mixed_enum_types_cxx20 : Warning<
  warn_arith_conv_mixed_enum_types_cxx20.Text>,
  InGroup<DeprecatedEnumCompareConditional>;
def warn_comparison_mixed_enum_types : Warning<
  warn_arith_conv_mixed_enum_types.Text>,
  InGroup<EnumCompare>;
def warn_comparison_mixed_enum_types_cxx20 : Warning<
  warn_arith_conv_mixed_enum_types_cxx20.Text>,
  InGroup<DeprecatedEnumCompare>;
def warn_comparison_of_mixed_enum_types_switch : Warning<
  "switch语句中不同枚举类型的比较"
  "%diff{ ($ and $)|}0,1">,
  InGroup<EnumCompareSwitch>;

def err_typecheck_assign_const : Error<
  "%select{"
  "cannot assign to return value because function %1 returns a const value|"
  "cannot assign to variable %1 with const-qualified type %2|"
  "cannot assign to %select{non-|}1static data member %2 "
  "with const-qualified type %3|"
  "cannot assign to non-static data member within const member function %1|"
  "cannot assign to %select{variable %2|non-static data member %2|lvalue}1 "
  "with %select{|nested }3const-qualified data member %4|"
  "read-only variable is not assignable}0">;

def note_typecheck_assign_const : Note<
  "%select{"
  "function %1 which returns const-qualified type %2 declared here|"
  "variable %1 declared const here|"
  "%select{non-|}1static data member %2 declared const here|"
  "member function %q1 is declared const here|"
  "%select{|nested }1data member %2 declared const here}0">;

def warn_unsigned_always_true_comparison : Warning<
  "比较结果 %select{%3|unsigned expression}0 %2 "
  "%select{unsigned expression|%3}0 总是 %4">,
  InGroup<TautologicalUnsignedZeroCompare>, DefaultIgnore;
def warn_unsigned_enum_always_true_comparison : Warning<
  "比较结果 %select{%3|unsigned enum expression}0 %2 "
  "%select{unsigned enum expression|%3}0 总是 %4">,
  InGroup<TautologicalUnsignedEnumZeroCompare>, DefaultIgnore;
def warn_tautological_constant_compare : Warning<
  "比较结果%select{%3|%1}0 %2 "
  "%select{%1|%3}0 总是 %4">,
  InGroup<TautologicalTypeLimitCompare>, DefaultIgnore;
def warn_tautological_compare_objc_bool : Warning<
  "常数 %0 与 'BOOL' 类型表达式的比较结果 "
  " 始终为 %1，因为 'BOOL' 的唯一明确定义的值是 YES 和 NO">,
  InGroup<TautologicalObjCBoolCompare>;
def subst_int_range : TextSubstitution<"%0-bit %select{signed|unsigned}1 value">;
def warn_tautological_compare_value_range : Warning<
  "比较结果 "
  "%select{%4|%sub{subst_int_range}1,2}0 %3 "
  "%select{%sub{subst_int_range}1,2|%4}0 总是 %5">,
  InGroup<TautologicalValueRangeCompare>, DefaultIgnore;

def warn_mixed_sign_comparison : Warning<
  "不同符号整数的比较：%0 和 %1">,
  InGroup<SignCompare>, DefaultIgnore;
def warn_out_of_range_compare : Warning<
  "比较结果 %select{constant %0|true|false}1 和 "
  "%select{expression of type %2|boolean expression}3 总是 %4">,
  InGroup<TautologicalOutOfRangeCompare>;
def warn_tautological_bool_compare : Warning<warn_out_of_range_compare.Text>,
  InGroup<TautologicalConstantCompare>;
def warn_integer_constants_in_conditional_always_true : Warning<
  "将带有整数常量的'?:'的结果转换为布尔值 "
  "始终计算为'true'">,
  InGroup<TautologicalConstantCompare>;
def warn_left_shift_always : Warning<
  "将 '<<' 的结果转换为布尔值始终计算 "
  "为 %select{false|true}0">,
  InGroup<TautologicalConstantCompare>;
def warn_null_in_arithmetic_operation : Warning<
  "在算术运算中使用 NULL">,
  InGroup<NullArithmetic>;
def warn_null_in_comparison_operation : Warning<
  "NULL 和非指针的比较 "
  "%select{(%1 and NULL)|(NULL and %1)}0">,
  InGroup<NullArithmetic>;
def err_shift_rhs_only_vector : Error<
  "requested shift is a vector of type %0 but the first operand is not a "
  "vector (%1)">;

def warn_logical_not_on_lhs_of_check : Warning<
  "逻辑非仅适用于 this 的左侧"
  "%select{comparison|bitwise operator}0">,
  InGroup<LogicalNotParentheses>;
def note_logical_not_fix : Note<
  "add parentheses after the '!' to evaluate the "
  "%select{comparison|bitwise operator}0 first">;
def note_logical_not_silence_with_parens : Note<
  "add parentheses around left hand side expression to silence this warning">;

def err_invalid_this_use : Error<
  "invalid use of 'this' outside of a non-static member function">;
def err_this_static_member_func : Error<
  "'this' cannot be%select{| implicitly}0 used in a static member function "
  "declaration">;
def err_invalid_member_use_in_static_method : Error<
  "invalid use of member %0 in static member function">;
def err_invalid_qualified_function_type : Error<
  "%select{non-member function|static member function|deduction guide}0 "
  "%select{of type %2 |}1cannot have '%3' qualifier">;
def err_compound_qualified_function_type : Error<
  "%select{block pointer|pointer|reference}0 to function type %select{%2 |}1"
  "cannot have '%3' qualifier">;
def err_qualified_function_typeid : Error<
  "type operand %0 of 'typeid' cannot have '%1' qualifier">;

def err_ref_qualifier_overload : Error<
  "cannot overload a member function %select{without a ref-qualifier|with "
  "ref-qualifier '&'|with ref-qualifier '&&'}0 with a member function %select{"
  "without a ref-qualifier|with ref-qualifier '&'|with ref-qualifier '&&'}1">;

def err_invalid_non_static_member_use : Error<
  "invalid use of non-static data member %0">;
def err_nested_non_static_member_use : Error<
  "%select{call to non-static member function|use of non-static data member}0 "
  "%2 of %1 from nested type %3">;
def warn_cxx98_compat_non_static_member_use : Warning<
  "在未计算的上下文中使用非静态数据成员 %0  "
  "与 C++98 不兼容">, InGroup<CXX98Compat>, DefaultIgnore;
def err_invalid_incomplete_type_use : Error<
  "invalid use of incomplete type %0">;
def err_builtin_func_cast_more_than_one_arg : Error<
  "function-style cast to a builtin type can only take one argument">;
def err_value_init_for_array_type : Error<
  "array types cannot be value-initialized">;
def err_init_for_function_type : Error<
  "cannot create object of function type %0">;
def warn_format_nonliteral_noargs : Warning<
  "格式字符串不是字符串文字（可能不安全）">,
  InGroup<FormatSecurity>;
def warn_format_nonliteral : Warning<
  "格式字符串不是字符串文字">,
  InGroup<FormatNonLiteral>, DefaultIgnore;

def err_unexpected_interface : Error<
  "unexpected interface name %0: expected expression">;
def err_ref_non_value : Error<"%0 does not refer to a value">;
def err_ref_vm_type : Error<
  "cannot refer to declaration with a variably modified type inside block">;
def err_ref_flexarray_type : Error<
  "cannot refer to declaration of structure variable with flexible array member "
  "inside block">;
def err_ref_array_type : Error<
  "cannot refer to declaration with an array type inside block">;
def err_property_not_found : Error<
  "property %0 not found on object of type %1">;
def err_invalid_property_name : Error<
  "%0 is not a valid property name (accessing an object of type %1)">;
def err_getter_not_found : Error<
  "no getter method for read from property">;
def err_objc_subscript_method_not_found : Error<
  "expected method to %select{read|write}1 %select{dictionary|array}2 element not "
  "found on object of type %0">;
def err_objc_subscript_index_type : Error<
  "method index parameter type %0 is not integral type">;
def err_objc_subscript_key_type : Error<
  "method key parameter type %0 is not object type">;
def err_objc_subscript_dic_object_type : Error<
  "method object parameter type %0 is not object type">;
def err_objc_subscript_object_type : Error<
  "cannot assign to this %select{dictionary|array}1 because assigning method's "
  "2nd parameter of type %0 is not an Objective-C pointer type">;
def err_objc_subscript_base_type : Error<
  "%select{dictionary|array}1 subscript base type %0 is not an Objective-C object">;
def err_objc_multiple_subscript_type_conversion : Error<
  "indexing expression is invalid because subscript type %0 has "
  "multiple type conversion functions">;
def err_objc_subscript_type_conversion : Error<
  "indexing expression is invalid because subscript type %0 is not an integral"
  " or Objective-C pointer type">;
def err_objc_subscript_pointer : Error<
  "indexing expression is invalid because subscript type %0 is not an"
  " Objective-C pointer">;
def err_objc_indexing_method_result_type : Error<
  "method for accessing %select{dictionary|array}1 element must have Objective-C"
  " object return type instead of %0">;
def err_objc_index_incomplete_class_type : Error<
  "Objective-C index expression has incomplete class type %0">;
def err_illegal_container_subscripting_op : Error<
  "illegal operation on Objective-C container subscripting">;
def err_property_not_found_forward_class : Error<
  "property %0 cannot be found in forward class object %1">;
def err_property_not_as_forward_class : Error<
  "property %0 refers to an incomplete Objective-C class %1 "
  "(with no @interface available)">;
def note_forward_class : Note<
  "forward declaration of class here">;
def err_duplicate_property : Error<
  "property has a previous declaration">;
def ext_gnu_void_ptr : Extension<
  "arithmetic on%select{ a|}0 pointer%select{|s}0 to void is a GNU extension">,
  InGroup<PointerArith>;
def ext_gnu_ptr_func_arith : Extension<
  "arithmetic on%select{ a|}0 pointer%select{|s}0 to%select{ the|}2 function "
  "type%select{|s}2 %1%select{| and %3}2 is a GNU extension">,
  InGroup<PointerArith>;
def err_readonly_message_assignment : Error<
  "assigning to 'readonly' return result of an Objective-C message not allowed">;
def ext_integer_increment_complex : Extension<
  "ISO C does not support '++'/'--' on complex integer type %0">;
def ext_integer_complement_complex : Extension<
  "ISO C does not support '~' for complex conjugation of %0">;
def err_nosetter_property_assignment : Error<
  "%select{assignment to readonly property|"
  "no setter method %1 for assignment to property}0">;
def err_nosetter_property_incdec : Error<
  "%select{%select{increment|decrement}1 of readonly property|"
  "no setter method %2 for %select{increment|decrement}1 of property}0">;
def err_nogetter_property_compound_assignment : Error<
  "a getter method is needed to perform a compound assignment on a property">;
def err_nogetter_property_incdec : Error<
  "no getter method %1 for %select{increment|decrement}0 of property">;
def err_no_subobject_property_setting : Error<
  "expression is not assignable">;
def err_qualified_objc_access : Error<
  "%select{property|instance variable}0 access cannot be qualified with '%1'">;

def ext_freestanding_complex : Extension<
  "complex numbers are an extension in a freestanding C99 implementation">;

// FIXME: Remove when we support imaginary.
def err_imaginary_not_supported : Error<"imaginary types are not supported">;

// Obj-c expressions
def warn_root_inst_method_not_found : Warning<
  "实例方法 %0 正在用于不在根类中的'Class' ">,
  InGroup<MethodAccess>;
def warn_class_method_not_found : Warning<
  "找不到类方法 %objcclass0（返回类型默认为 'id'）">,
  InGroup<MethodAccess>;
def warn_instance_method_on_class_found : Warning<
  "找到实例方法 %0 而不是类方法 %1">,
  InGroup<MethodAccess>;
def warn_inst_method_not_found : Warning<
  "找不到实例方法 %objcinstance0（返回类型默认为 'id'）">,
  InGroup<MethodAccess>;
def warn_instance_method_not_found_with_typo : Warning<
  "找不到实例方法 %objcinstance0（返回类型默认为 'id'）"
  "； 你的意思是 %objcinstance2 吗？">, InGroup<MethodAccess>;
def warn_class_method_not_found_with_typo : Warning<
  "类方法 %objcclass0 未找到（返回类型默认为 'id'） "
  "； 你的意思是%objcclass2吗？">, InGroup<MethodAccess>;
def err_method_not_found_with_typo : Error<
  "%select{instance|class}1 method %0 not found "
  "; did you mean %2?">;
def err_no_super_class_message : Error<
  "no @interface declaration found in class messaging of %0">;
def err_root_class_cannot_use_super : Error<
  "%0 cannot use 'super' because it is a root class">;
def err_invalid_receiver_to_message_super : Error<
  "'super' is only valid in a method body">;
def err_invalid_receiver_class_message : Error<
  "receiver type %0 is not an Objective-C class">;
def err_missing_open_square_message_send : Error<
  "missing '[' at start of message send expression">;
def warn_bad_receiver_type : Warning<
  "接收器类型 %0 不是 'id' 或接口指针，考虑 "
  "将其转换为 'id'">,InGroup<ObjCReceiver>;
def err_bad_receiver_type : Error<"bad receiver type %0">;
def err_incomplete_receiver_type : Error<"incomplete receiver type %0">;
def err_unknown_receiver_suggest : Error<
  "unknown receiver %0; did you mean %1?">;
def err_objc_throw_expects_object : Error<
  "@throw requires an Objective-C object type (%0 invalid)">;
def err_objc_synchronized_expects_object : Error<
  "@synchronized requires an Objective-C object type (%0 invalid)">;
def err_rethrow_used_outside_catch : Error<
  "@throw (rethrow) used outside of a @catch block">;
def err_attribute_multiple_objc_gc : Error<
  "multiple garbage collection attributes specified for type">;
def err_catch_param_not_objc_type : Error<
  "@catch parameter is not a pointer to an interface type">;
def err_illegal_qualifiers_on_catch_parm : Error<
  "illegal qualifiers on @catch parameter">;
def err_storage_spec_on_catch_parm : Error<
  "@catch parameter cannot have storage specifier '%0'">;
def warn_register_objc_catch_parm : Warning<
  "@catch 参数上的'register'存储说明符将被忽略 ">;
def err_qualified_objc_catch_parm : Error<
  "@catch parameter declarator cannot be qualified">;
def warn_objc_pointer_cxx_catch_fragile : Warning<
  "在非统一异常模型中无法捕获 C++ 中使用 @throw  "
  "抛出的异常">, InGroup<ObjCNonUnifiedException>;
def err_objc_object_catch : Error<
  "cannot catch an Objective-C object by value">;
def err_incomplete_type_objc_at_encode : Error<
  "'@encode' of incomplete type %0">;
def warn_objc_circular_container : Warning<
  "将 %0 添加到 %1 可能会导致容器中的循环依赖">,
  InGroup<DiagGroup<"objc-circular-container">>;
def note_objc_circular_container_declared_here : Note<"%0 declared here">;
def warn_objc_unsafe_perform_selector : Warning<
  "%0 和选择器不兼容，它返回一个 "
  "%select{struct|union|vector}1 类型">,
  InGroup<DiagGroup<"objc-unsafe-perform-selector">>;
def note_objc_unsafe_perform_selector_method_declared_here :  Note<
  "method %0 that returns %1 declared here">;
def err_attribute_arm_builtin_alias : Error<
  "'__clang_arm_builtin_alias' attribute can only be applied to an ARM builtin">;
def err_attribute_arm_mve_polymorphism : Error<
  "'__clang_arm_mve_strict_polymorphism' attribute can only be applied to an MVE/NEON vector type">;

def warn_setter_getter_impl_required : Warning<
  "属性 %0 需要定义方法 %1 -  "
  "使用 @synthesize、@dynamic 或 "
  "在此类实现中提供方法实现">,
  InGroup<ObjCPropertyImpl>;
def warn_setter_getter_impl_required_in_category : Warning<
  "属性 %0 需要定义方法 %1 - "
  "使用 @dynamic 或提供此类别中的方法实现">,
  InGroup<ObjCPropertyImpl>;
def note_parameter_named_here : Note<
  "passing argument to parameter %0 here">;
def note_parameter_here : Note<
  "passing argument to parameter here">;
def note_method_return_type_change : Note<
  "compiler has implicitly changed method %0 return type">;

def warn_impl_required_for_class_property : Warning<
  "类属性 %0 需要定义方法 %1 -  "
  "使用 @dynamic 或 "
  "在此类实现中提供方法实现">,
  InGroup<ObjCPropertyImpl>;
def warn_impl_required_in_category_for_class_property : Warning<
  "类属性 %0 需要定义方法 %1 - "
  "使用 @dynamic 或提供此类别中的方法实现">,
  InGroup<ObjCPropertyImpl>;

// C++ casts
// These messages adhere to the TryCast pattern: %0 is an int specifying the
// cast type, %1 is the source type, %2 is the destination type.
def err_bad_reinterpret_cast_overload : Error<
  "reinterpret_cast cannot resolve overloaded function %0 to type %1">;

def warn_reinterpret_different_from_static : Warning<
  "'reinterpret_cast' %select{from|to}3 类 %0 %select{to|from}3 它的 "
  "%select{virtual base|base at non-zero offset}2 %1 行为不同于 "
  "'static_cast'">, InGroup<ReinterpretBaseClass>;
def note_reinterpret_updowncast_use_static: Note<
  "use 'static_cast' to adjust the pointer correctly while "
  "%select{upcasting|downcasting}0">;

def err_bad_static_cast_overload : Error<
  "address of overloaded function %0 cannot be static_cast to type %1">;

def err_bad_cstyle_cast_overload : Error<
  "address of overloaded function %0 cannot be cast to type %1">;


def err_bad_cxx_cast_generic : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|"
  "C-style cast|functional-style cast|addrspace_cast}0 from %1 to %2 is not allowed">;
def err_bad_cxx_cast_unrelated_class : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|}0 from %1 to %2, which are not related by "
  "inheritance, is not allowed">;
def note_type_incomplete : Note<"%0 is incomplete">;
def err_bad_cxx_cast_rvalue : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|addrspace_cast}0 from rvalue to reference type %2">;
def err_bad_cxx_cast_bitfield : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|}0 from bit-field lvalue to reference type %2">;
def err_bad_cxx_cast_qualifiers_away : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|}0 from %1 to %2 casts away qualifiers">;
def err_bad_cxx_cast_addr_space_mismatch : Error<
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|"
  "C-style cast|functional-style cast|addrspace_cast}0 from %1 to %2 converts between mismatching address"
  " spaces">;
def ext_bad_cxx_cast_qualifiers_away_incoherent : ExtWarn<
  "ISO C++ does not allow "
  "%select{const_cast|static_cast|reinterpret_cast|dynamic_cast|C-style cast|"
  "functional-style cast|}0 from %1 to %2 because it casts away qualifiers, "
  "even though the source and destination types are unrelated">,
  SFINAEFailure, InGroup<DiagGroup<"cast-qual-unrelated">>;
def err_bad_const_cast_dest : Error<
  "%select{const_cast||||C-style cast|functional-style cast|}0 to %2, "
  "which is not a reference, pointer-to-object, or pointer-to-data-member">;
def ext_cast_fn_obj : Extension<
  "cast between pointer-to-function and pointer-to-object is an extension">;
def ext_ms_cast_fn_obj : ExtWarn<
  "static_cast between pointer-to-function and pointer-to-object is a "
  "Microsoft extension">, InGroup<MicrosoftCast>;
def warn_cxx98_compat_cast_fn_obj : Warning<
  "函数指针和对象指针之间的强制转换与C++98不兼容">,
  InGroup<CXX98CompatPedantic>, DefaultIgnore;
def err_bad_reinterpret_cast_small_int : Error<
  "cast from pointer to smaller type %2 loses information">;
def err_bad_cxx_cast_vector_to_scalar_different_size : Error<
  "%select{||reinterpret_cast||C-style cast||}0 from vector %1 "
  "to scalar %2 of different size">;
def err_bad_cxx_cast_scalar_to_vector_different_size : Error<
  "%select{||reinterpret_cast||C-style cast||}0 from scalar %1 "
  "to vector %2 of different size">;
def err_bad_cxx_cast_vector_to_vector_different_size : Error<
  "%select{||reinterpret_cast||C-style cast||}0 from vector %1 "
  "to vector %2 of different size">;
def warn_bad_cxx_cast_nested_pointer_addr_space : Warning<
  "%select{reinterpret_cast|C-style cast}0 从 %1 到 %2 "
  "更改嵌套指针的地址空间">,
  InGroup<IncompatiblePointerTypesDiscardsQualifiers>;
def err_bad_lvalue_to_rvalue_cast : Error<
  "cannot cast from lvalue of type %1 to rvalue reference type %2; types are "
  "not compatible">;
def err_bad_rvalue_to_rvalue_cast : Error<
  "cannot cast from rvalue of type %1 to rvalue reference type %2; types are "
  "not compatible">;
def err_bad_static_cast_pointer_nonpointer : Error<
  "cannot cast from type %1 to pointer type %2">;
def err_bad_static_cast_member_pointer_nonmp : Error<
  "cannot cast from type %1 to member pointer type %2">;
def err_bad_cxx_cast_member_pointer_size : Error<
  "cannot %select{||reinterpret_cast||C-style cast||}0 from member pointer "
  "type %1 to member pointer type %2 of different size">;
def err_bad_reinterpret_cast_reference : Error<
  "reinterpret_cast of a %0 to %1 needs its address, which is not allowed">;
def warn_undefined_reinterpret_cast : Warning<
  "reinterpret_cast 从 %0 到 %1 具有未定义的行为">,
  InGroup<UndefinedReinterpretCast>, DefaultIgnore;

// These messages don't adhere to the pattern.
// FIXME: Display the path somehow better.
def err_ambiguous_base_to_derived_cast : Error<
  "ambiguous cast from base %0 to derived %1:%2">;
def err_static_downcast_via_virtual : Error<
  "cannot cast %0 to %1 via virtual base %2">;
def err_downcast_from_inaccessible_base : Error<
  "cannot cast %select{private|protected}2 base class %1 to %0">;
def err_upcast_to_inaccessible_base : Error<
  "cannot cast %0 to its %select{private|protected}2 base class %1">;
def err_bad_dynamic_cast_not_ref_or_ptr : Error<
  "invalid target type %0 for dynamic_cast; target type must be a reference or pointer type to a defined class">;
def err_bad_dynamic_cast_not_class : Error<"%0 is not a class type">;
def err_bad_cast_incomplete : Error<"%0 is an incomplete type">;
def err_bad_dynamic_cast_not_ptr : Error<"cannot use dynamic_cast to convert from %0 to %1">;
def err_bad_dynamic_cast_not_polymorphic : Error<"%0 is not polymorphic">;

// Other C++ expressions
def err_need_header_before_typeid : Error<
  "you need to include <typeinfo> before using the 'typeid' operator">;
def err_need_header_before_ms_uuidof : Error<
  "you need to include <guiddef.h> before using the '__uuidof' operator">;
def err_ms___leave_not_in___try : Error<
  "'__leave' statement not in __try block">;
def err_uuidof_without_guid : Error<
  "cannot call operator __uuidof on a type with no GUID">;
def err_uuidof_with_multiple_guids : Error<
  "cannot call operator __uuidof on a type with multiple GUIDs">;
def err_incomplete_typeid : Error<"'typeid' of incomplete type %0">;
def err_variably_modified_typeid : Error<"'typeid' of variably modified type %0">;
def err_static_illegal_in_new : Error<
  "the 'static' modifier for the array size is not legal in new expressions">;
def err_array_new_needs_size : Error<
  "array size must be specified in new expression with no initializer">;
def err_bad_new_type : Error<
  "cannot allocate %select{function|reference}1 type %0 with new">;
def err_new_incomplete_or_sizeless_type : Error<
  "allocation of %select{incomplete|sizeless}0 type %1">;
def err_new_array_nonconst : Error<
  "only the first dimension of an allocated array may have dynamic size">;
def err_new_array_size_unknown_from_init : Error<
  "cannot determine allocated array size from initializer">;
def err_new_array_init_args : Error<
  "array 'new' cannot have initialization arguments">;
def ext_new_paren_array_nonconst : ExtWarn<
  "when type is in parentheses, array cannot have dynamic size">;
def err_placement_new_non_placement_delete : Error<
  "'new' expression with placement arguments refers to non-placement "
  "'operator delete'">;
def err_array_size_not_integral : Error<
  "array size expression must have integral or %select{|unscoped }0"
  "enumeration type, not %1">;
def err_array_size_incomplete_type : Error<
  "array size expression has incomplete class type %0">;
def err_array_size_explicit_conversion : Error<
  "array size expression of type %0 requires explicit conversion to type %1">;
def note_array_size_conversion : Note<
  "conversion to %select{integral|enumeration}0 type %1 declared here">;
def err_array_size_ambiguous_conversion : Error<
  "ambiguous conversion of array size expression of type %0 to an integral or "
  "enumeration type">;
def ext_array_size_conversion : Extension<
  "implicit conversion from array size expression of type %0 to "
  "%select{integral|enumeration}1 type %2 is a C++11 extension">,
  InGroup<CXX11>;
def warn_cxx98_compat_array_size_conversion : Warning<
  "从类型为%0的数组大小表达式隐式转换为 "
  "%select{integral|enumeration}1 类型为 %2 与C++98不兼容">,
  InGroup<CXX98CompatPedantic>, DefaultIgnore;
def err_address_space_qualified_new : Error<
  "'new' cannot allocate objects of type %0 in address space '%1'">;
def err_address_space_qualified_delete : Error<
  "'delete' cannot delete objects of type %0 in address space '%1'">;

def err_default_init_const : Error<
  "default initialization of an object of const type %0"
  "%select{| without a user-provided default constructor}1">;
def ext_default_init_const : ExtWarn<
  "default initialization of an object of const type %0"
  "%select{| without a user-provided default constructor}1 "
  "is a Microsoft extension">,
  InGroup<MicrosoftConstInit>;
def err_delete_operand : Error<"cannot delete expression of type %0">;
def ext_delete_void_ptr_operand : ExtWarn<
  "cannot delete expression with pointer-to-'void' type %0">,
  InGroup<DeleteIncomplete>;
def err_ambiguous_delete_operand : Error<
  "ambiguous conversion of delete expression of type %0 to a pointer">;
def warn_delete_incomplete : Warning<
  "删除指向不完整类型的指针%0可能会导致未定义的行为">,
  InGroup<DeleteIncomplete>;
def err_delete_incomplete_class_type : Error<
  "deleting incomplete class type %0; no conversions to pointer type">;
def err_delete_explicit_conversion : Error<
  "converting delete expression from type %0 to type %1 invokes an explicit "
  "conversion function">;
def note_delete_conversion : Note<"conversion to pointer type %0">;
def warn_delete_array_type : Warning<
  "'delete' 应用于指向数组类型%0的指针，该指针被视为 'delete[]'">;
def warn_mismatched_delete_new : Warning<
  "'删除%select{|[]}0' 应用于被分配了 "
  "'new%select{[]|}0'的指针; 你的意思是 '删除%select{[]|}0'?">,
  InGroup<DiagGroup<"mismatched-new-delete">>;
def note_allocated_here : Note<"allocated with 'new%select{[]|}0' here">;
def err_no_suitable_delete_member_function_found : Error<
  "no suitable member %0 in %1">;
def err_ambiguous_suitable_delete_member_function_found : Error<
  "multiple suitable %0 functions in %1">;
def warn_ambiguous_suitable_delete_function_found : Warning<
  "多个合适的%0函数属于%1；没有'operator delete' 函数将被调用 "
  "如果初始化引发异常">,
  InGroup<DiagGroup<"ambiguous-delete">>;
def note_member_declared_here : Note<
  "member %0 declared here">;
def note_member_first_declared_here : Note<
  "member %0 first declared here">;
def warn_bitwise_negation_bool : Warning<
  "布尔表达式的位求反%select{;| always evaluates to 'true';}0 "
  "你是说逻辑否定吗?">,
  InGroup<DiagGroup<"bool-operation">>;
def err_decrement_bool : Error<"cannot decrement expression of type bool">;
def warn_increment_bool : Warning<
  "bool类型的递增表达式已被弃用 "
  "并且与C++17不兼容">, InGroup<DeprecatedIncrementBool>;
def ext_increment_bool : ExtWarn<
  "ISO C++17 does not allow incrementing expression of type bool">,
  DefaultError, InGroup<IncrementBool>;
def err_increment_decrement_enum : Error<
  "cannot %select{decrement|increment}0 expression of enum type %1">;

def warn_deprecated_increment_decrement_volatile : Warning<
  "%select{decrement|increment}0 volatile-qualified 类型的对象 %1 "
  "不推荐使用">, InGroup<DeprecatedVolatile>;
def warn_deprecated_simple_assign_volatile : Warning<
  "分配给volatile限定类型的对象的结果%0 "
  "不推荐使用">, InGroup<DeprecatedVolatile>;
def warn_deprecated_compound_assign_volatile : Warning<
  "不推荐对易失性限定类型的对象进行复合分配%0">,
  InGroup<DeprecatedVolatile>;
def warn_deprecated_volatile_return : Warning<
  "不推荐使用volatile限定返回类型%0">,
  InGroup<DeprecatedVolatile>;
def warn_deprecated_volatile_param : Warning<
  "不推荐使用volatile限定参数类型%0">,
  InGroup<DeprecatedVolatile>;
def warn_deprecated_volatile_structured_binding : Warning<
  "不推荐使用结构化绑定声明中的volatile限定符">,
  InGroup<DeprecatedVolatile>;

def err_catch_incomplete_ptr : Error<
  "cannot catch pointer to incomplete type %0">;
def err_catch_incomplete_ref : Error<
  "cannot catch reference to incomplete type %0">;
def err_catch_incomplete : Error<"cannot catch incomplete type %0">;
def err_catch_sizeless : Error<
  "cannot catch %select{|reference to }0sizeless type %1">;
def err_catch_rvalue_ref : Error<"cannot catch exceptions by rvalue reference">;
def err_catch_variably_modified : Error<
  "cannot catch variably modified type %0">;
def err_qualified_catch_declarator : Error<
  "exception declarator cannot be qualified">;
def err_early_catch_all : Error<"catch-all handler must come last">;
def err_bad_memptr_rhs : Error<
  "right hand operand to %0 has non-pointer-to-member type %1">;
def err_bad_memptr_lhs : Error<
  "left hand operand to %0 must be a %select{|pointer to }1class "
  "compatible with the right hand operand, but is %2">;
def err_memptr_incomplete : Error<
  "member pointer has incomplete base type %0">;
def warn_exception_caught_by_earlier_handler : Warning<
  "早期处理程序将捕获类型为%0的异常">,
  InGroup<Exceptions>;
def note_previous_exception_handler : Note<"for type %0">;
def err_exceptions_disabled : Error<
  "cannot use '%0' with exceptions disabled">;
def err_objc_exceptions_disabled : Error<
  "cannot use '%0' with Objective-C exceptions disabled">;
def warn_throw_in_noexcept_func : Warning<
  "%0具有非引发异常规范，但仍可以引发">,
  InGroup<Exceptions>;
def note_throw_in_dtor : Note<
  "%select{destructor|deallocator}0 has a %select{non-throwing|implicit "
  "non-throwing}1 exception specification">;
def note_throw_in_function : Note<"function declared non-throwing here">;
def err_seh_try_outside_functions : Error<
  "cannot use SEH '__try' in blocks, captured regions, or Obj-C method decls">;
def err_mixing_cxx_try_seh_try : Error<
  "cannot use C++ 'try' in the same function as SEH '__try'">;
def err_seh_try_unsupported : Error<
  "SEH '__try' is not supported on this target">;
def note_conflicting_try_here : Note<
  "conflicting %0 here">;
def warn_jump_out_of_seh_finally : Warning<
  "跳出 __finally 块有未定义的行为">,
  InGroup<DiagGroup<"jump-seh-finally">>;
def warn_non_virtual_dtor : Warning<
  "%0 具有虚函数但非虚析构函数">,
  InGroup<NonVirtualDtor>, DefaultIgnore;
def warn_delete_non_virtual_dtor : Warning<
  "%select{delete|destructor}0 在具有虚函数但非虚析构函数的非最终l %1 上 "
  "被调用">,
  InGroup<DeleteNonAbstractNonVirtualDtor>, DefaultIgnore, ShowInSystemHeader;
def note_delete_non_virtual : Note<
  "qualify call to silence this warning">;
def warn_delete_abstract_non_virtual_dtor : Warning<
  "%select{delete|destructor}0 在 %1 上被调用，它是抽象的 "
  "但具有非虚拟析构函数">, InGroup<DeleteAbstractNonVirtualDtor>, ShowInSystemHeader;
def warn_overloaded_virtual : Warning<
  "%q0隐藏重载的虚拟 %select{function|functions}1">,
  InGroup<OverloadedVirtual>, DefaultIgnore;
def note_hidden_overloaded_virtual_declared_here : Note<
  "hidden overloaded virtual function %q0 declared here"
  "%select{|: different classes%diff{ ($ vs $)|}2,3"
  "|: different number of parameters (%2 vs %3)"
  "|: type mismatch at %ordinal2 parameter%diff{ ($ vs $)|}3,4"
  "|: different return type%diff{ ($ vs $)|}2,3"
  "|: different qualifiers (%2 vs %3)"
  "|: different exception specifications}1">;
def warn_using_directive_in_header : Warning<
  "在标头的全局上下文中使用namespace指令">,
  InGroup<HeaderHygiene>, DefaultIgnore;
def warn_overaligned_type : Warning<
  "类型%0需要%1字节的对齐，默认分配器仅 "
  "保证%2字节">,
  InGroup<OveralignedType>, DefaultIgnore;
def err_aligned_allocation_unavailable : Error<
  "aligned %select{allocation|deallocation}0 function of type '%1' is "
  "%select{only|not}4 available on %2%select{ %3 or newer|}4">;
def note_silence_aligned_allocation_unavailable : Note<
  "if you supply your own aligned allocation functions, use "
  "-faligned-allocation to silence this diagnostic">;

def err_conditional_void_nonvoid : Error<
  "%select{left|right}1 operand to ? is void, but %select{right|left}1 operand "
  "is of type %0">;
def err_conditional_ambiguous : Error<
  "conditional expression is ambiguous; "
  "%diff{$ can be converted to $ and vice versa|"
  "types can be convert to each other}0,1">;
def err_conditional_ambiguous_ovl : Error<
  "conditional expression is ambiguous; %diff{$ and $|types}0,1 "
  "can be converted to several common types">;
def err_conditional_vector_size : Error<
  "vector condition type %0 and result type %1 do not have the same number "
  "of elements">;
def err_conditional_vector_element_size : Error<
  "vector condition type %0 and result type %1 do not have elements of the "
  "same size">;
def err_conditional_vector_has_void : Error<
  "GNU vector conditional operand cannot be %select{void|a throw expression}0">;
def err_conditional_vector_operand_type
    : Error<"%select{enumeration|extended vector}0 type %1 is not allowed in a "
            "vector conditional">;
def err_conditional_vector_mismatched_vectors
    : Error<"vector operands to the vector conditional must be the same type "
            "%diff{($ and $)|}0,1}">;

def err_throw_incomplete : Error<
  "cannot throw object of incomplete type %0">;
def err_throw_incomplete_ptr : Error<
  "cannot throw pointer to object of incomplete type %0">;
def err_throw_sizeless : Error<
  "cannot throw object of sizeless type %0">;
def warn_throw_underaligned_obj : Warning<
  "引发未对齐的异常对象">,
  InGroup<UnderalignedExceptionObject>;
def note_throw_underaligned_obj : Note<
  "required alignment of type %0 (%1 bytes) is larger than the supported "
  "alignment of C++ exception objects on this target (%2 bytes)">;
def err_return_in_constructor_handler : Error<
  "return in the catch of a function try block of a constructor is illegal">;
def warn_cdtor_function_try_handler_mem_expr : Warning<
  "无法引用非静态成员从处理程序 "
  "%select{constructor|destructor}0 属于一个函数try块">, InGroup<Exceptions>;

let CategoryName = "Lambda Issue" in {
  def err_capture_more_than_once : Error<
    "%0 can appear only once in a capture list">;
  def err_reference_capture_with_reference_default : Error<
    "'&' cannot precede a capture when the capture default is '&'">;
  def err_copy_capture_with_copy_default : Error<
    "'&' must precede a capture when the capture default is '='">;
  def err_capture_does_not_name_variable : Error<
    "%0 in capture list does not name a variable">;
  def err_capture_non_automatic_variable : Error<
    "%0 cannot be captured because it does not have automatic storage "
    "duration">;
  def err_this_capture : Error<
    "'this' cannot be %select{implicitly |}0captured in this context">;
  def err_lambda_capture_anonymous_var : Error<
    "unnamed variable cannot be implicitly captured in a lambda expression">;
  def err_lambda_capture_flexarray_type : Error<
    "variable %0 with flexible array member cannot be captured in "
    "a lambda expression">;
  def err_lambda_impcap : Error<
    "variable %0 cannot be implicitly captured in a lambda with no "
    "capture-default specified">;
  def note_lambda_decl : Note<"lambda expression begins here">;
  def err_lambda_unevaluated_operand : Error<
    "lambda expression in an unevaluated operand">;
  def err_lambda_in_constant_expression : Error<
    "a lambda expression may not appear inside of a constant expression">;
  def err_lambda_in_invalid_context : Error<
    "a lambda expression cannot appear in this context">;
  def err_lambda_return_init_list : Error<
    "cannot deduce lambda return type from initializer list">;
  def err_lambda_capture_default_arg : Error<
    "lambda expression in default argument cannot capture any entity">;
  def err_lambda_incomplete_result : Error<
    "incomplete result type %0 in lambda expression">;
  def err_noreturn_lambda_has_return_expr : Error<
    "lambda declared 'noreturn' should not return">;
  def warn_maybe_falloff_nonvoid_lambda : Warning<
    "非void lambda不会在所有控制路径中返回值">,
    InGroup<ReturnType>;
  def warn_falloff_nonvoid_lambda : Warning<
    "非void lambda不返回值">,
    InGroup<ReturnType>;
  def err_access_lambda_capture : Error<
    // The ERRORs represent other special members that aren't constructors, in
    // hopes that someone will bother noticing and reporting if they appear
    "capture of variable '%0' as type %1 calls %select{private|protected}3 "
    "%select{default |copy |move |*ERROR* |*ERROR* |*ERROR* |}2constructor">,
    AccessControl;
  def note_lambda_to_block_conv : Note<
    "implicit capture of lambda object due to conversion to block pointer "
    "here">;
  def note_var_explicitly_captured_here : Note<"variable %0 is"
    "%select{| explicitly}1 captured here">;

  // C++14 lambda init-captures.
  def warn_cxx11_compat_init_capture : Warning<
    "初始化的lambda捕获 "
    "与C++14之前的C++标准不兼容">, InGroup<CXXPre14Compat>, DefaultIgnore;
  def ext_init_capture : ExtWarn<
    "initialized lambda captures are a C++14 extension">, InGroup<CXX14>;
  def err_init_capture_no_expression : Error<
    "initializer missing for lambda capture %0">;
  def err_init_capture_multiple_expressions : Error<
    "initializer for lambda capture %0 contains multiple expressions">;
  def err_init_capture_paren_braces : Error<
    "cannot deduce type for lambda capture %1 from "
    "%select{parenthesized|nested}0 initializer list">;
  def err_init_capture_deduction_failure : Error<
    "cannot deduce type for lambda capture %0 from initializer of type %2">;
  def err_init_capture_deduction_failure_from_init_list : Error<
    "cannot deduce type for lambda capture %0 from initializer list">;
  def warn_cxx17_compat_init_capture_pack : Warning<
    "初始化的lambda捕获包 "
    "与C++20之前的C++标准不兼容">, InGroup<CXXPre20Compat>, DefaultIgnore;
  def ext_init_capture_pack : ExtWarn<
    "initialized lambda pack captures are a C++20 extension">, InGroup<CXX20>;

  // C++14 generic lambdas.
  def warn_cxx11_compat_generic_lambda : Warning<
    "通用lambda与C++11不兼容">,
    InGroup<CXXPre14Compat>, DefaultIgnore;

  // C++17 '*this' captures.
  def warn_cxx14_compat_star_this_lambda_capture : Warning<
    "通过值捕获'*this'与C++17之前的C++标准不兼容">,
     InGroup<CXXPre17Compat>, DefaultIgnore;
  def ext_star_this_lambda_capture_cxx17 : ExtWarn<
    "capture of '*this' by copy is a C++17 extension">, InGroup<CXX17>;

  // C++17 parameter shadows capture
  def err_parameter_shadow_capture : Error<
    "a lambda parameter cannot shadow an explicitly captured entity">;

  // C++20 [=, this] captures.
  def warn_cxx17_compat_equals_this_lambda_capture : Warning<
    "捕获默认值为'='的显式捕获'this' "
    "与C++20之前的C++标准不兼容">, InGroup<CXXPre20Compat>, DefaultIgnore;
  def ext_equals_this_lambda_capture_cxx20 : ExtWarn<
    "explicit capture of 'this' with a capture default of '=' "
    "is a C++20 extension">, InGroup<CXX20>;
  def warn_deprecated_this_capture : Warning<
    "不推荐使用捕获默认值为'='的隐式捕获'this'">,
    InGroup<DeprecatedThisCapture>, DefaultIgnore;
  def note_deprecated_this_capture : Note<
    "add an explicit capture of 'this' to capture '*this' by reference">;

  // C++20 default constructible / assignable lambdas.
  def warn_cxx17_compat_lambda_def_ctor_assign : Warning<
    "%select{default construction|assignment}0 属于lambda "
    "与C++20之前的C++标准不兼容">, InGroup<CXXPre20Compat>, DefaultIgnore;
}

def err_return_in_captured_stmt : Error<
  "cannot return from %0">;
def err_capture_block_variable : Error<
  "__block variable %0 cannot be captured in a "
  "%select{lambda expression|captured statement}1">;

def err_operator_arrow_circular : Error<
  "circular pointer delegation detected">;
def err_operator_arrow_depth_exceeded : Error<
  "use of 'operator->' on type %0 would invoke a sequence of more than %1 "
  "'operator->' calls">;
def note_operator_arrow_here : Note<
  "'operator->' declared here produces an object of type %0">;
def note_operator_arrows_suppressed : Note<
  "(skipping %0 'operator->'%s0 in backtrace)">;
def note_operator_arrow_depth : Note<
  "use -foperator-arrow-depth=N to increase 'operator->' limit">;

def err_pseudo_dtor_base_not_scalar : Error<
  "object expression of non-scalar type %0 cannot be used in a "
  "pseudo-destructor expression">;
def ext_pseudo_dtor_on_void : ExtWarn<
  "pseudo-destructors on type void are a Microsoft extension">,
  InGroup<MicrosoftVoidPseudoDtor>;
def err_pseudo_dtor_type_mismatch : Error<
  "the type of object expression "
  "%diff{($) does not match the type being destroyed ($)|"
  "does not match the type being destroyed}0,1 "
  "in pseudo-destructor expression">;
def err_pseudo_dtor_call_with_args : Error<
  "call to pseudo-destructor cannot have any arguments">;
def err_dtor_expr_without_call : Error<
  "reference to %select{destructor|pseudo-destructor}0 must be called"
  "%select{|; did you mean to call it with no arguments?}1">;
def err_pseudo_dtor_destructor_non_type : Error<
  "%0 does not refer to a type name in pseudo-destructor expression; expected "
  "the name of type %1">;
def err_invalid_use_of_function_type : Error<
  "a function type is not allowed here">;
def err_invalid_use_of_array_type : Error<"an array type is not allowed here">;
def err_typecheck_bool_condition : Error<
  "value of type %0 is not contextually convertible to 'bool'">;
def err_typecheck_ambiguous_condition : Error<
  "conversion %diff{from $ to $|between types}0,1 is ambiguous">;
def err_typecheck_nonviable_condition : Error<
  "no viable conversion%select{%diff{ from $ to $|}1,2|"
  "%diff{ from returned value of type $ to function return type $|}1,2}0">;
def err_typecheck_nonviable_condition_incomplete : Error<
  "no viable conversion%diff{ from $ to incomplete type $|}0,1">;
def err_typecheck_deleted_function : Error<
  "conversion function %diff{from $ to $|between types}0,1 "
  "invokes a deleted function">;

def err_expected_class_or_namespace : Error<"%0 is not a class"
  "%select{ or namespace|, namespace, or enumeration}1">;
def err_invalid_declarator_scope : Error<"cannot define or redeclare %0 here "
  "because namespace %1 does not enclose namespace %2">;
def err_invalid_declarator_global_scope : Error<
  "definition or redeclaration of %0 cannot name the global scope">;
def err_invalid_declarator_in_function : Error<
  "definition or redeclaration of %0 not allowed inside a function">;
def err_invalid_declarator_in_block : Error<
  "definition or redeclaration of %0 not allowed inside a block">;
def err_not_tag_in_scope : Error<
  "no %select{struct|interface|union|class|enum}0 named %1 in %2">;

def err_no_typeid_with_fno_rtti : Error<
  "use of typeid requires -frtti">;
def err_no_dynamic_cast_with_fno_rtti : Error<
  "use of dynamic_cast requires -frtti">;
def warn_no_dynamic_cast_with_rtti_disabled: Warning<
  "dynamic_cast 将不起作用由于禁用了RTTI数据 "
  "%select{-fno-rtti-data|/GR-}0">, InGroup<RTTI>;
def warn_no_typeid_with_rtti_disabled: Warning<
  "typeid 将不起作用由于禁用了RTTI数据 "
  "%select{-fno-rtti-data|/GR-}0">, InGroup<RTTI>;

def err_cannot_form_pointer_to_member_of_reference_type : Error<
  "cannot form a pointer-to-member to member %0 of reference type %1">;
def err_incomplete_object_call : Error<
  "incomplete type in call to object of type %0">;

def warn_condition_is_assignment : Warning<"将赋值结果用作 "
  "不带括号的条件">,
  InGroup<Parentheses>;
def warn_free_nonheap_object
  : Warning<"尝试调用%0对非堆对象%1">,
    InGroup<FreeNonHeapObject>;

// Completely identical except off by default.
def warn_condition_is_idiomatic_assignment : Warning<"将赋值结果 "
  "用作不带括号的条件">,
  InGroup<DiagGroup<"idiomatic-parentheses">>, DefaultIgnore;
def note_condition_assign_to_comparison : Note<
  "use '==' to turn this assignment into an equality comparison">;
def note_condition_or_assign_to_comparison : Note<
  "use '!=' to turn this compound assignment into an inequality comparison">;
def note_condition_assign_silence : Note<
  "place parentheses around the assignment to silence this warning">;

def warn_equality_with_extra_parens : Warning<"带外圆括号的 "
  "相等比较">, InGroup<ParenthesesOnEquality>;
def note_equality_comparison_to_assign : Note<
  "use '=' to turn this equality comparison into an assignment">;
def note_equality_comparison_silence : Note<
  "remove extraneous parentheses around the comparison to silence this warning">;

// assignment related diagnostics (also for argument passing, returning, etc).
// In most of these diagnostics the %2 is a value from the
// Sema::AssignmentAction enumeration
def err_typecheck_convert_incompatible : Error<
  "%select{%diff{assigning to $ from incompatible type $|"
  "assigning to type from incompatible type}0,1"
  "|%diff{passing $ to parameter of incompatible type $|"
  "passing type to parameter of incompatible type}0,1"
  "|%diff{returning $ from a function with incompatible result type $|"
  "returning type from a function with incompatible result type}0,1"
  "|%diff{converting $ to incompatible type $|"
  "converting type to incompatible type}0,1"
  "|%diff{initializing $ with an expression of incompatible type $|"
  "initializing type with an expression of incompatible type}0,1"
  "|%diff{sending $ to parameter of incompatible type $|"
  "sending type to parameter of incompatible type}0,1"
  "|%diff{casting $ to incompatible type $|"
  "casting type to incompatible type}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3"
  "%select{|: different classes%diff{ ($ vs $)|}5,6"
  "|: different number of parameters (%5 vs %6)"
  "|: type mismatch at %ordinal5 parameter%diff{ ($ vs $)|}6,7"
  "|: different return type%diff{ ($ vs $)|}5,6"
  "|: different qualifiers (%5 vs %6)"
  "|: different exception specifications}4">;
def err_typecheck_missing_return_type_incompatible : Error<
  "%diff{return type $ must match previous return type $|"
  "return type must match previous return type}0,1 when %select{block "
  "literal|lambda expression}2 has unspecified explicit return type">;

def note_incomplete_class_and_qualified_id : Note<
  "conformance of forward class %0 to protocol %1 can not be confirmed">;
def warn_incompatible_qualified_id : Warning<
  "%select{%diff{assigning to $ from incompatible type $|"
  "assigning to type from incompatible type}0,1"
  "|%diff{passing $ to parameter of incompatible type $|"
  "passing type to parameter of incompatible type}0,1"
  "|%diff{returning $ from a function with incompatible result type $|"
  "returning type from a function with incompatible result type}0,1"
  "|%diff{converting $ to incompatible type $|"
  "converting type to incompatible type}0,1"
  "|%diff{initializing $ with an expression of incompatible type $|"
  "initializing type with an expression of incompatible type}0,1"
  "|%diff{sending $ to parameter of incompatible type $|"
  "sending type to parameter of incompatible type}0,1"
  "|%diff{casting $ to incompatible type $|"
  "casting type to incompatible type}0,1}2">;
def err_incompatible_qualified_id : Error<
  "%select{%diff{assigning to $ from incompatible type $|"
  "assigning to type from incompatible type}0,1"
  "|%diff{passing $ to parameter of incompatible type $|"
  "passing type to parameter of incompatible type}0,1"
  "|%diff{returning $ from a function with incompatible result type $|"
  "returning type from a function with incompatible result type}0,1"
  "|%diff{converting $ to incompatible type $|"
  "converting type to incompatible type}0,1"
  "|%diff{initializing $ with an expression of incompatible type $|"
  "initializing type with an expression of incompatible type}0,1"
  "|%diff{sending $ to parameter of incompatible type $|"
  "sending type to parameter of incompatible type}0,1"
  "|%diff{casting $ to incompatible type $|"
  "casting type to incompatible type}0,1}2">;
def ext_typecheck_convert_pointer_int : ExtWarn<
  "incompatible pointer to integer conversion "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">,
  InGroup<IntConversion>;
def err_typecheck_convert_pointer_int : Error<
  "incompatible pointer to integer conversion "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">;
def ext_typecheck_convert_int_pointer : ExtWarn<
  "incompatible integer to pointer conversion "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">,
  InGroup<IntConversion>, SFINAEFailure;
def err_typecheck_convert_int_pointer : Error<
 "incompatible integer to pointer conversion "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">;
def ext_typecheck_convert_pointer_void_func : Extension<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " converts between void pointer and function pointer">;
def err_typecheck_convert_pointer_void_func : Error<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " converts between void pointer and function pointer">;
def ext_typecheck_convert_incompatible_pointer_sign : ExtWarn<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " converts between pointers to integer types %select{with different sign|"
  "where one is of the unique plain 'char' type and the other is not}3">,
  InGroup<DiagGroup<"pointer-sign">>;
def err_typecheck_convert_incompatible_pointer_sign :
  Error<ext_typecheck_convert_incompatible_pointer_sign.Text>;
def ext_typecheck_convert_incompatible_pointer : ExtWarn<
  "incompatible pointer types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">,
  InGroup<IncompatiblePointerTypes>;
def err_typecheck_convert_incompatible_pointer : Error<
  "incompatible pointer types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">;
def ext_typecheck_convert_incompatible_function_pointer : ExtWarn<
  "incompatible function pointer types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">,
  InGroup<IncompatibleFunctionPointerTypes>;
def err_typecheck_convert_incompatible_function_pointer : Error<
  "incompatible function pointer types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  "%select{|; dereference with *|"
  "; take the address with &|"
  "; remove *|"
  "; remove &}3">;
def ext_typecheck_convert_discards_qualifiers : ExtWarn<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " discards qualifiers">,
  InGroup<IncompatiblePointerTypesDiscardsQualifiers>;
def err_typecheck_convert_discards_qualifiers : Error<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " discards qualifiers">;
def ext_nested_pointer_qualifier_mismatch : ExtWarn<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " discards qualifiers in nested pointer types">,
  InGroup<IncompatiblePointerTypesDiscardsQualifiers>;
def err_nested_pointer_qualifier_mismatch : Error<
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " discards qualifiers in nested pointer types">;
def warn_incompatible_vectors : Warning<
  "不兼容的向量类型 "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2">,
  InGroup<VectorConversion>, DefaultIgnore;
def err_incompatible_vectors : Error<
  "incompatible vector types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2">;
def err_int_to_block_pointer : Error<
  "invalid block pointer conversion "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2">;
def err_typecheck_convert_incompatible_block_pointer : Error<
  "incompatible block pointer types "
  "%select{%diff{assigning to $ from $|assigning to different types}0,1"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2">;
def err_typecheck_incompatible_address_space : Error<
  "%select{%diff{assigning $ to $|assigning to different types}1,0"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " changes address space of pointer">;
def err_typecheck_incompatible_nested_address_space : Error<
  "%select{%diff{assigning $ to $|assigning to different types}1,0"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " changes address space of nested pointer">;
def err_typecheck_incompatible_ownership : Error<
  "%select{%diff{assigning $ to $|assigning to different types}1,0"
  "|%diff{passing $ to parameter of type $|"
  "passing to parameter of different type}0,1"
  "|%diff{returning $ from a function with result type $|"
  "returning from function with different return type}0,1"
  "|%diff{converting $ to type $|converting between types}0,1"
  "|%diff{initializing $ with an expression of type $|"
  "initializing with expression of different type}0,1"
  "|%diff{sending $ to parameter of type $|"
  "sending to parameter of different type}0,1"
  "|%diff{casting $ to type $|casting between types}0,1}2"
  " changes retain/release properties of pointer">;
def err_typecheck_comparison_of_distinct_blocks : Error<
  "comparison of distinct block types%diff{ ($ and $)|}0,1">;

def err_typecheck_array_not_modifiable_lvalue : Error<
  "array type %0 is not assignable">;
def err_typecheck_non_object_not_modifiable_lvalue : Error<
  "non-object type %0 is not assignable">;
def err_typecheck_expression_not_modifiable_lvalue : Error<
  "expression is not assignable">;
def err_typecheck_incomplete_type_not_modifiable_lvalue : Error<
  "incomplete type %0 is not assignable">;
def err_typecheck_lvalue_casts_not_supported : Error<
  "assignment to cast is illegal, lvalue casts are not supported">;

def err_typecheck_duplicate_vector_components_not_mlvalue : Error<
  "vector is not assignable (contains duplicate components)">;
def err_block_decl_ref_not_modifiable_lvalue : Error<
  "variable is not assignable (missing __block type specifier)">;
def err_lambda_decl_ref_not_modifiable_lvalue : Error<
  "cannot assign to a variable captured by copy in a non-mutable lambda">;
def err_typecheck_call_not_function : Error<
  "called object type %0 is not a function or function pointer">;
def err_call_incomplete_return : Error<
  "calling function with incomplete return type %0">;
def err_call_function_incomplete_return : Error<
  "calling %0 with incomplete return type %1">;
def err_call_incomplete_argument : Error<
  "argument type %0 is incomplete">;
def err_typecheck_call_too_few_args : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected %1, have %2">;
def err_typecheck_call_too_few_args_one : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "single argument %1 was not specified">;
def err_typecheck_call_too_few_args_at_least : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected at least %1, have %2">;
def err_typecheck_call_too_few_args_at_least_one : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "at least argument %1 must be specified">;
def err_typecheck_call_too_few_args_suggest : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected %1, have %2; did you mean %3?">;
def err_typecheck_call_too_few_args_at_least_suggest : Error<
  "too few %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected at least %1, have %2; did you mean %3?">;
def err_typecheck_call_too_many_args : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected %1, have %2">;
def err_typecheck_call_too_many_args_one : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected single argument %1, have %2 arguments">;
def err_typecheck_call_too_many_args_at_most : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected at most %1, have %2">;
def err_typecheck_call_too_many_args_at_most_one : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected at most single argument %1, have %2 arguments">;
def err_typecheck_call_too_many_args_suggest : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected %1, have %2; did you mean %3?">;
def err_typecheck_call_too_many_args_at_most_suggest : Error<
  "too many %select{|||execution configuration }0arguments to "
  "%select{function|block|method|kernel function}0 call, "
  "expected at most %1, have %2; did you mean %3?">;

def err_arc_typecheck_convert_incompatible_pointer : Error<
  "incompatible pointer types passing retainable parameter of type %0"
  "to a CF function expecting %1 type">;

def err_builtin_fn_use : Error<"builtin functions must be directly called">;

def warn_call_wrong_number_of_arguments : Warning<
  "太 %select{few|many}0 参数在对%1的调用中">;
def err_atomic_builtin_must_be_pointer : Error<
  "address argument to atomic builtin must be a pointer (%0 invalid)">;
def err_atomic_builtin_must_be_pointer_intptr : Error<
  "address argument to atomic builtin must be a pointer to integer or pointer"
  " (%0 invalid)">;
def err_atomic_builtin_cannot_be_const : Error<
  "address argument to atomic builtin cannot be const-qualified (%0 invalid)">;
def err_atomic_builtin_must_be_pointer_intfltptr : Error<
  "address argument to atomic builtin must be a pointer to integer,"
  " floating-point or pointer (%0 invalid)">;
def err_atomic_builtin_pointer_size : Error<
  "address argument to atomic builtin must be a pointer to 1,2,4,8 or 16 byte "
  "type (%0 invalid)">;
def err_atomic_exclusive_builtin_pointer_size : Error<
  "address argument to load or store exclusive builtin must be a pointer to"
  " 1,2,4 or 8 byte type (%0 invalid)">;
def err_atomic_builtin_ext_int_size : Error<
  "Atomic memory operand must have a power-of-two size">;
def err_atomic_builtin_ext_int_prohibit : Error<
  "argument to atomic builtin of type '_ExtInt' is not supported">;
def err_atomic_op_needs_atomic : Error<
  "address argument to atomic operation must be a pointer to _Atomic "
  "type (%0 invalid)">;
def err_atomic_op_needs_non_const_atomic : Error<
  "address argument to atomic operation must be a pointer to non-%select{const|constant}0 _Atomic "
  "type (%1 invalid)">;
def err_atomic_op_needs_non_const_pointer : Error<
  "address argument to atomic operation must be a pointer to non-const "
  "type (%0 invalid)">;
def err_atomic_op_needs_trivial_copy : Error<
  "address argument to atomic operation must be a pointer to a "
  "trivially-copyable type (%0 invalid)">;
def err_atomic_op_needs_atomic_int_or_ptr : Error<
  "address argument to atomic operation must be a pointer to %select{|atomic }0"
  "integer or pointer (%1 invalid)">;
def err_atomic_op_needs_atomic_int : Error<
  "address argument to atomic operation must be a pointer to "
  "%select{|atomic }0integer (%1 invalid)">;
def warn_atomic_op_has_invalid_memory_order : Warning<
  "原子操作的内存顺序参数无效">,
  InGroup<DiagGroup<"atomic-memory-ordering">>;
def err_atomic_op_has_invalid_synch_scope : Error<
  "synchronization scope argument to atomic operation is invalid">;
def warn_atomic_implicit_seq_cst : Warning<
  "隐式使用顺序一致的原子可能会产生比必要时更强的记忆障碍">,
  InGroup<DiagGroup<"atomic-implicit-seq-cst">>, DefaultIgnore;

def err_overflow_builtin_must_be_int : Error<
  "operand argument to overflow builtin must be an integer (%0 invalid)">;
def err_overflow_builtin_must_be_ptr_int : Error<
  "result argument to overflow builtin must be a pointer "
  "to a non-const integer (%0 invalid)">;
def err_overflow_builtin_ext_int_max_size : Error<
  "__builtin_mul_overflow does not support signed _ExtInt operands of more "
  "than %0 bits">;

def err_atomic_load_store_uses_lib : Error<
  "atomic %select{load|store}0 requires runtime support that is not "
  "available for this target">;

def err_nontemporal_builtin_must_be_pointer : Error<
  "address argument to nontemporal builtin must be a pointer (%0 invalid)">;
def err_nontemporal_builtin_must_be_pointer_intfltptr_or_vector : Error<
  "address argument to nontemporal builtin must be a pointer to integer, float, "
  "pointer, or a vector of such types (%0 invalid)">;

def err_deleted_function_use : Error<"attempt to use a deleted function">;
def err_deleted_inherited_ctor_use : Error<
  "constructor inherited by %0 from base class %1 is implicitly deleted">;

def note_called_by : Note<"called by %0">;
def err_kern_type_not_void_return : Error<
  "kernel function type %0 must have void return type">;
def err_kern_is_nonstatic_method : Error<
  "kernel function %0 must be a free function or static member function">;
def err_config_scalar_return : Error<
  "CUDA special function '%0' must have scalar return type">;
def err_kern_call_not_global_function : Error<
  "kernel call to non-global function %0">;
def err_global_call_not_config : Error<
  "call to global function %0 not configured">;
def err_ref_bad_target : Error<
  "reference to %select{__device__|__global__|__host__|__host__ __device__}0 "
  "%select{function|variable}1 %2 in %select{__device__|__global__|__host__|__host__ __device__}3 function">;
def err_ref_bad_target_global_initializer : Error<
  "reference to %select{__device__|__global__|__host__|__host__ __device__}0 "
  "function %1 in global initializer">;
def err_capture_bad_target : Error<
  "capture host variable %0 by reference in device or host device lambda function">;
def err_capture_bad_target_this_ptr : Error<
  "capture host side class data member by this pointer in device or host device lambda function">;
def warn_kern_is_method : Extension<
  "kernel function %0 is a member function; this may not be accepted by nvcc">,
  InGroup<CudaCompat>;
def warn_kern_is_inline : Warning<
  "已忽略内核函数的'inline'属性%0">,
  InGroup<CudaCompat>;
def err_variadic_device_fn : Error<
  "CUDA device code does not support variadic functions">;
def err_va_arg_in_device : Error<
  "CUDA device code does not support va_arg">;
def err_alias_not_supported_on_nvptx : Error<"CUDA does not support aliases">;
def err_cuda_unattributed_constexpr_cannot_overload_device : Error<
  "constexpr function %0 without __host__ or __device__ attributes cannot "
  "overload __device__ function with same signature.  Add a __host__ "
  "attribute, or build with -fno-cuda-host-device-constexpr.">;
def note_cuda_conflicting_device_function_declared_here : Note<
  "conflicting __device__ function declared here">;
def err_cuda_device_exceptions : Error<
  "cannot use '%0' in "
  "%select{__device__|__global__|__host__|__host__ __device__}1 function">;
def err_dynamic_var_init : Error<
    "dynamic initialization is not supported for "
    "__device__, __constant__, and __shared__ variables.">;
def err_shared_var_init : Error<
    "initialization is not supported for __shared__ variables.">;
def err_cuda_vla : Error<
    "cannot use variable-length arrays in "
    "%select{__device__|__global__|__host__|__host__ __device__}0 functions">;
def err_cuda_extern_shared : Error<"__shared__ variable %0 cannot be 'extern'">;
def err_cuda_host_shared : Error<
    "__shared__ local variables not allowed in "
    "%select{__device__|__global__|__host__|__host__ __device__}0 functions">;
def err_cuda_nonstatic_constdev: Error<"__constant__ and __device__ are not allowed on non-static local variables">;
def err_cuda_ovl_target : Error<
  "%select{__device__|__global__|__host__|__host__ __device__}0 function %1 "
  "cannot overload %select{__device__|__global__|__host__|__host__ __device__}2 function %3">;
def note_cuda_ovl_candidate_target_mismatch : Note<
    "candidate template ignored: target attributes do not match">;

def err_cuda_device_builtin_surftex_cls_template : Error<
    "illegal device builtin %select{surface|texture}0 reference "
    "class template %1 declared here">;
def note_cuda_device_builtin_surftex_cls_should_have_n_args : Note<
    "%0 needs to have exactly %1 template parameters">;
def note_cuda_device_builtin_surftex_cls_should_have_match_arg : Note<
    "the %select{1st|2nd|3rd}1 template parameter of %0 needs to be "
    "%select{a type|an integer or enum value}2">;

def err_cuda_device_builtin_surftex_ref_decl : Error<
    "illegal device builtin %select{surface|texture}0 reference "
    "type %1 declared here">;
def note_cuda_device_builtin_surftex_should_be_template_class : Note<
    "%0 needs to be instantiated from a class template with proper "
    "template arguments">;

def warn_non_pod_vararg_with_format_string : Warning<
  "无法传递 %select{non-POD|non-trivial}0 类型%1的对象给变量 "
  "%select{function|block|method|constructor}2; 格式字符串的类型 "
  "应为%3">, InGroup<NonPODVarargs>, DefaultError;
// The arguments to this diagnostic should match the warning above.
def err_cannot_pass_objc_interface_to_vararg_format : Error<
  "cannot pass object with interface type %1 by value to variadic "
  "%select{function|block|method|constructor}2; expected type from format "
  "string was %3">;
def err_cannot_pass_non_trivial_c_struct_to_vararg : Error<
  "cannot pass non-trivial C object of type %0 by value to variadic "
  "%select{function|block|method|constructor}1">;


def err_cannot_pass_objc_interface_to_vararg : Error<
  "cannot pass object with interface type %0 by value through variadic "
  "%select{function|block|method|constructor}1">;
def warn_cannot_pass_non_pod_arg_to_vararg : Warning<
  "无法传递对象属于%select{non-POD|non-trivial}0 类型 %1 通过变量"
  " %select{function|block|method|constructor}2; 调用将在运行时中止">,
  InGroup<NonPODVarargs>, DefaultError;
def warn_cxx98_compat_pass_non_pod_arg_to_vararg : Warning<
  "传递普通但非POD类型%0的对象通过变量"
  " %select{function|block|method|constructor}1 与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;
def warn_pass_class_arg_to_vararg : Warning<
  "正在通过va变量传递类类型为%0的对象 "
  "%select{function|block|method|constructor}1"
  "%select{|; did you mean to call '%3'?}2">,
  InGroup<ClassVarargs>, DefaultIgnore;
def err_cannot_pass_to_vararg : Error<
  "cannot pass %select{expression of type %1|initializer list}0 to variadic "
  "%select{function|block|method|constructor}2">;
def err_cannot_pass_to_vararg_format : Error<
  "cannot pass %select{expression of type %1|initializer list}0 to variadic "
  "%select{function|block|method|constructor}2; expected type from format "
  "string was %3">;

def err_typecheck_call_invalid_ordered_compare : Error<
  "ordered compare requires two args of floating point type"
  "%diff{ ($ and $)|}0,1">;
def err_typecheck_call_invalid_unary_fp : Error<
  "floating point classification requires argument of floating point type "
  "(passed in %0)">;
def err_typecheck_cond_expect_int_float : Error<
  "used type %0 where integer or floating point type is required">;
def err_typecheck_cond_expect_scalar : Error<
  "used type %0 where arithmetic or pointer type is required">;
def err_typecheck_cond_expect_nonfloat : Error<
  "used type %0 where floating point type is not allowed">;
def ext_typecheck_cond_one_void : Extension<
  "C99 forbids conditional expressions with only one void side">;
def err_typecheck_cast_to_incomplete : Error<
  "cast to incomplete type %0">;
def ext_typecheck_cast_nonscalar : Extension<
  "C99 forbids casting nonscalar type %0 to the same type">;
def ext_typecheck_cast_to_union : Extension<
  "cast to union type is a GNU extension">,
  InGroup<GNUUnionCast>;
def err_typecheck_cast_to_union_no_type : Error<
  "cast to union type from type %0 not present in union">;
def err_cast_pointer_from_non_pointer_int : Error<
  "operand of type %0 cannot be cast to a pointer type">;
def warn_cast_pointer_from_sel : Warning<
  "不推荐将类型%0转换为%1；改用 sel_getName ">,
  InGroup<SelTypeCast>;
def warn_function_def_in_objc_container : Warning<
  "不推荐使用Objective-C容器中的函数定义">,
  InGroup<FunctionDefInObjCContainer>;
def err_typecheck_call_requires_real_fp : Error<
  "argument type %0 is not a real floating point type">;
def err_typecheck_call_different_arg_types : Error<
  "arguments are of different types%diff{ ($ vs $)|}0,1">;

def warn_cast_calling_conv : Warning<
  "在不兼容的调用约定'%0'和'%1'之间转换;  "
  "通过此指针的调用可能会在运行时中止">,
  InGroup<DiagGroup<"cast-calling-convention">>;
def note_change_calling_conv_fixit : Note<
  "consider defining %0 with the '%1' calling convention">;
def warn_bad_function_cast : Warning<
  "从类型 %0 的函数调用强制转换为不匹配类型 %1">,
  InGroup<BadFunctionCast>, DefaultIgnore;
def err_cast_pointer_to_non_pointer_int : Error<
  "pointer cannot be cast to type %0">;
def err_cast_to_bfloat16 : Error<"cannot type-cast to __bf16">;
def err_cast_from_bfloat16 : Error<"cannot type-cast from __bf16">;
def err_typecheck_expect_scalar_operand : Error<
  "operand of type %0 where arithmetic or pointer type is required">;
def err_typecheck_cond_incompatible_operands : Error<
  "incompatible operand types%diff{ ($ and $)|}0,1">;
def err_cast_selector_expr : Error<
  "cannot type cast @selector expression">;
def ext_typecheck_cond_incompatible_pointers : ExtWarn<
  "pointer type mismatch%diff{ ($ and $)|}0,1">,
  InGroup<DiagGroup<"pointer-type-mismatch">>;
def ext_typecheck_cond_pointer_integer_mismatch : ExtWarn<
  "pointer/integer type mismatch in conditional expression"
  "%diff{ ($ and $)|}0,1">,
  InGroup<DiagGroup<"conditional-type-mismatch">>;
def err_typecheck_choose_expr_requires_constant : Error<
  "'__builtin_choose_expr' requires a constant expression">;
def warn_unused_expr : Warning<"表达式结果未使用">,
  InGroup<UnusedValue>;
def warn_unused_voidptr : Warning<
  "表达式结果未使用；该强制转换是否应为'void'？">,
  InGroup<UnusedValue>;
def warn_unused_property_expr : Warning<
 "属性访问结果未使用 - getters不应用于副作用">,
  InGroup<UnusedGetterReturnValue>;
def warn_unused_container_subscript_expr : Warning<
 "容器访问结果未使用- container 访问不应用于副作用">,
  InGroup<UnusedValue>;
def warn_unused_call : Warning<
  "忽略用%0属性声明的函数的返回值">,
  InGroup<UnusedValue>;
def warn_unused_constructor : Warning<
  "忽略由使用 %0 属性声明的构造函数创建的临时变量">,
  InGroup<UnusedValue>;
def warn_unused_constructor_msg : Warning<
  "忽略由使用 %0 属性声明的构造函数创建的临时变量: %1">,
  InGroup<UnusedValue>;
def warn_side_effects_unevaluated_context : Warning<
  "具有副作用的表达式在未评估的上下文中无效">,
  InGroup<UnevaluatedExpression>;
def warn_side_effects_typeid : Warning<
  "仍将计算具有副作用的表达式 "
  "尽管表达式用作'typeid'的操作数">, InGroup<PotentiallyEvaluatedExpression>;
def warn_unused_result : Warning<
  "忽略用%0属性声明的函数的返回值">,
  InGroup<UnusedResult>;
def warn_unused_result_msg : Warning<
  "忽略使用%0属性声明的函数的返回值：%1">,
  InGroup<UnusedResult>;
def warn_unused_volatile : Warning<
  "表达式结果未使用；赋给变量以强制可变负载">,
  InGroup<DiagGroup<"unused-volatile-lvalue">>;

def ext_cxx14_attr : Extension<
  "use of the %0 attribute is a C++14 extension">, InGroup<CXX14>;
def ext_cxx17_attr : Extension<
  "use of the %0 attribute is a C++17 extension">, InGroup<CXX17>;
def ext_cxx20_attr : Extension<
  "use of the %0 attribute is a C++20 extension">, InGroup<CXX20>;

def warn_unused_comparison : Warning<
  "%select{equality|inequality|relational|three-way}0 的比较结果未使用">,
  InGroup<UnusedComparison>;
def note_inequality_comparison_to_or_assign : Note<
  "use '|=' to turn this inequality comparison into an or-assignment">;

def err_incomplete_type_used_in_type_trait_expr : Error<
  "incomplete type %0 used in type trait expression">;

// C++20 constinit and require_constant_initialization attribute
def warn_cxx20_compat_constinit : Warning<
  "'constinit' 说明符与C++20之前的C++标准不兼容">,
  InGroup<CXX20Compat>, DefaultIgnore;
def err_constinit_local_variable : Error<
  "local variable cannot be declared 'constinit'">;
def err_require_constant_init_failed : Error<
  "variable does not have a constant initializer">;
def note_declared_required_constant_init_here : Note<
  "required by %select{'require_constant_initialization' attribute|"
  "'constinit' specifier}0 here">;
def ext_constinit_missing : ExtWarn<
  "'constinit' specifier missing on initializing declaration of %0">,
  InGroup<DiagGroup<"missing-constinit">>;
def note_constinit_specified_here : Note<"variable declared constinit here">;
def err_constinit_added_too_late : Error<
  "'constinit' specifier added after initialization of variable">;
def warn_require_const_init_added_too_late : Warning<
  "'require_constant_initialization' 属性被添加 "
  "在变量初始化之后">, InGroup<IgnoredAttributes>;
def note_constinit_missing_here : Note<
  "add the "
  "%select{'require_constant_initialization' attribute|'constinit' specifier}0 "
  "to the initializing declaration here">;

def err_dimension_expr_not_constant_integer : Error<
  "dimension expression does not evaluate to a constant unsigned int">;

def err_typecheck_cond_incompatible_operands_null : Error<
  "non-pointer operand type %0 incompatible with %select{NULL|nullptr}1">;
def ext_empty_struct_union : Extension<
  "empty %select{struct|union}0 is a GNU extension">, InGroup<GNUEmptyStruct>;
def ext_no_named_members_in_struct_union : Extension<
  "%select{struct|union}0 without named members is a GNU extension">, InGroup<GNUEmptyStruct>;
def warn_zero_size_struct_union_compat : Warning<"%select{|empty }0"
  "%select{struct|union}1 在 C 中大小为 0, %select{size 1|non-zero size}2 在 C++中">,
  InGroup<CXXCompat>, DefaultIgnore;
def warn_zero_size_struct_union_in_extern_c : Warning<"%select{|empty }0"
  "%select{struct|union}1 在 C 中大小为 0, %select{size 1|non-zero size}2 在 C++中">,
  InGroup<ExternCCompat>;
def warn_cast_qual : Warning<"从 %0 转换到 %1 会丢弃 %select{const and "
  "volatile qualifiers|const qualifier|volatile qualifier}2">,
  InGroup<CastQual>, DefaultIgnore;
def warn_cast_qual2 : Warning<"从 %0 到 %1 的强制转换 "
  "必须使所有中间指针 const 限定为安全">, InGroup<CastQual>, DefaultIgnore;
def warn_redefine_extname_not_applied : Warning<
  "#pragma redefine_extname 仅适用于外部 C 声明; "
  "未应用于 %select{function|variable}0 %1">,
  InGroup<Pragmas>;
} // End of general sema category.

// inline asm.
let CategoryName = "Inline Assembly Issue" in {
  def err_asm_invalid_lvalue_in_output : Error<"invalid lvalue in asm output">;
  def err_asm_invalid_output_constraint : Error<
    "invalid output constraint '%0' in asm">;
  def err_asm_invalid_lvalue_in_input : Error<
    "invalid lvalue in asm input for constraint '%0'">;
  def err_asm_invalid_input_constraint : Error<
    "invalid input constraint '%0' in asm">;
  def err_asm_tying_incompatible_types : Error<
    "unsupported inline asm: input with type "
    "%diff{$ matching output with type $|}0,1">;
  def err_asm_unexpected_constraint_alternatives : Error<
    "asm constraint has an unexpected number of alternatives: %0 vs %1">;
  def err_asm_incomplete_type : Error<"asm operand has incomplete type %0">;
  def err_asm_unknown_register_name : Error<"unknown register name '%0' in asm">;
  def err_asm_invalid_global_var_reg : Error<"register '%0' unsuitable for "
    "global register variables on this target">;
  def err_asm_register_size_mismatch : Error<"size of register '%0' does not "
    "match variable size">;
  def err_asm_bad_register_type : Error<"bad type for named register variable">;
  def err_asm_invalid_input_size : Error<
    "invalid input size for constraint '%0'">;
  def err_asm_invalid_output_size : Error<
    "invalid output size for constraint '%0'">;
  def err_invalid_asm_cast_lvalue : Error<
    "invalid use of a cast in a inline asm context requiring an lvalue: "
    "remove the cast or build with -fheinous-gnu-extensions">;
  def err_invalid_asm_value_for_constraint
      : Error <"value '%0' out of range for constraint '%1'">;
  def err_asm_non_addr_value_in_memory_constraint : Error <
    "reference to a %select{bit-field|vector element|global register variable}0"
    " in asm %select{input|output}1 with a memory constraint '%2'">;
  def err_asm_input_duplicate_match : Error<
    "more than one input constraint matches the same output '%0'">;

  def warn_asm_label_on_auto_decl : Warning<
    "忽略自动变量上的 asm 标签'%0'">;
  def warn_invalid_asm_cast_lvalue : Warning<
    "在需要左值的内联asm上下文中无效地使用强制转换："
    "由于-fheinous gnu扩展而被接受，但clang可能会 "
    "在将来取消对这方面的支持">;
  def warn_asm_mismatched_size_modifier : Warning<
    "值大小与约束和修改器指定的寄存器大小 "
    "不匹配">,
    InGroup<ASMOperandWidths>;

  def note_asm_missing_constraint_modifier : Note<
    "use constraint modifier \"%0\"">;
  def note_asm_input_duplicate_first : Note<
    "constraint '%0' is already present here">;
 def error_duplicate_asm_operand_name : Error<
    "duplicate use of asm operand name \"%0\"">;
 def note_duplicate_asm_operand_name : Note<
    "asm operand name \"%0\" first referenced here">;
}

  def error_inoutput_conflict_with_clobber : Error<
    "asm-specifier for input or output variable conflicts with asm"
    " clobber list">;

let CategoryName = "Semantic Issue" in {

def err_invalid_conversion_between_vectors : Error<
  "invalid conversion between vector type%diff{ $ and $|}0,1 of different "
  "size">;
def err_invalid_conversion_between_vector_and_integer : Error<
  "invalid conversion between vector type %0 and integer type %1 "
  "of different size">;

def err_opencl_function_pointer : Error<
  "pointers to functions are not allowed">;

def err_opencl_taking_address_capture : Error<
  "taking address of a capture is not allowed">;

def err_invalid_conversion_between_vector_and_scalar : Error<
  "invalid conversion between vector type %0 and scalar type %1">;

// C++ member initializers.
def err_only_constructors_take_base_inits : Error<
  "only constructors take base initializers">;

def err_multiple_mem_initialization : Error <
  "multiple initializations given for non-static member %0">;
def err_multiple_mem_union_initialization : Error <
  "initializing multiple members of union">;
def err_multiple_base_initialization : Error <
  "multiple initializations given for base %0">;

def err_mem_init_not_member_or_class : Error<
  "member initializer %0 does not name a non-static data member or base "
  "class">;

def warn_initializer_out_of_order : Warning<
  "%select{field|base class}0 %1 将会被初始化，在 "
  "%select{field|base}2 %3之后">,
  InGroup<ReorderCtor>, DefaultIgnore;
def warn_abstract_vbase_init_ignored : Warning<
  "虚拟基类%0属于抽象类%1，它的初始值设定项 "
  "将永远不会使用">,
  InGroup<DiagGroup<"abstract-vbase-init">>, DefaultIgnore;

def err_base_init_does_not_name_class : Error<
  "constructor initializer %0 does not name a class">;
def err_base_init_direct_and_virtual : Error<
  "base class initializer %0 names both a direct base class and an "
  "inherited virtual base class">;
def err_not_direct_base_or_virtual : Error<
  "type %0 is not a direct or virtual base of %1">;

def err_in_class_initializer_non_const : Error<
  "non-const static data member must be initialized out of line">;
def err_in_class_initializer_volatile : Error<
  "static const volatile data member must be initialized out of line">;
def err_in_class_initializer_bad_type : Error<
  "static data member of type %0 must be initialized out of line">;
def ext_in_class_initializer_float_type : ExtWarn<
  "in-class initializer for static data member of type %0 is a GNU extension">,
  InGroup<GNUStaticFloatInit>;
def ext_in_class_initializer_float_type_cxx11 : ExtWarn<
  "in-class initializer for static data member of type %0 requires "
  "'constexpr' specifier">, InGroup<StaticFloatInit>, DefaultError;
def note_in_class_initializer_float_type_cxx11 : Note<"add 'constexpr'">;
def err_in_class_initializer_literal_type : Error<
  "in-class initializer for static data member of type %0 requires "
  "'constexpr' specifier">;
def err_in_class_initializer_non_constant : Error<
  "in-class initializer for static data member is not a constant expression">;
def err_default_member_initializer_not_yet_parsed : Error<
  "default member initializer for %1 needed within definition of enclosing "
  "class %0 outside of member functions">;
def note_default_member_initializer_not_yet_parsed : Note<
  "default member initializer declared here">;
def err_default_member_initializer_cycle
    : Error<"default member initializer for %0 uses itself">;

def ext_in_class_initializer_non_constant : Extension<
  "in-class initializer for static data member is not a constant expression; "
  "folding it to a constant is a GNU extension">, InGroup<GNUFoldingConstant>;

def err_thread_dynamic_init : Error<
  "initializer for thread-local variable must be a constant expression">;
def err_thread_nontrivial_dtor : Error<
  "type of thread-local variable has non-trivial destruction">;
def note_use_thread_local : Note<
  "use 'thread_local' to allow this">;

// C++ anonymous unions and GNU anonymous structs/unions
def ext_anonymous_union : Extension<
  "anonymous unions are a C11 extension">, InGroup<C11>;
def ext_gnu_anonymous_struct : Extension<
  "anonymous structs are a GNU extension">, InGroup<GNUAnonymousStruct>;
def ext_c11_anonymous_struct : Extension<
  "anonymous structs are a C11 extension">, InGroup<C11>;
def err_anonymous_union_not_static : Error<
  "anonymous unions at namespace or global scope must be declared 'static'">;
def err_anonymous_union_with_storage_spec : Error<
  "anonymous union at class scope must not have a storage specifier">;
def err_anonymous_struct_not_member : Error<
  "anonymous %select{structs|structs and classes}0 must be "
  "%select{struct or union|class}0 members">;
def err_anonymous_record_member_redecl : Error<
  "member of anonymous %select{struct|union}0 redeclares %1">;
def err_anonymous_record_with_type : Error<
  "types cannot be declared in an anonymous %select{struct|union}0">;
def ext_anonymous_record_with_type : Extension<
  "types declared in an anonymous %select{struct|union}0 are a Microsoft "
  "extension">, InGroup<MicrosoftAnonTag>;
def ext_anonymous_record_with_anonymous_type : Extension<
  "anonymous types declared in an anonymous %select{struct|union}0 "
  "are an extension">, InGroup<DiagGroup<"nested-anon-types">>;
def err_anonymous_record_with_function : Error<
  "functions cannot be declared in an anonymous %select{struct|union}0">;
def err_anonymous_record_with_static : Error<
  "static members cannot be declared in an anonymous %select{struct|union}0">;
def err_anonymous_record_bad_member : Error<
  "anonymous %select{struct|union}0 can only contain non-static data members">;
def err_anonymous_record_nonpublic_member : Error<
  "anonymous %select{struct|union}0 cannot contain a "
  "%select{private|protected}1 data member">;
def ext_ms_anonymous_record : ExtWarn<
  "anonymous %select{structs|unions}0 are a Microsoft extension">,
  InGroup<MicrosoftAnonTag>;

// C++ local classes
def err_reference_to_local_in_enclosing_context : Error<
  "reference to local %select{variable|binding}1 %0 declared in enclosing "
  "%select{%3|block literal|lambda expression|context}2">;

def err_static_data_member_not_allowed_in_local_class : Error<
  "static data member %0 not allowed in local %sub{select_tag_type_kind}2 %1">;

// C++ derived classes
def err_base_clause_on_union : Error<"unions cannot have base classes">;
def err_base_must_be_class : Error<"base specifier must name a class">;
def err_union_as_base_class : Error<"unions cannot be base classes">;
def err_circular_inheritance : Error<
  "circular inheritance between %0 and %1">;
def err_base_class_has_flexible_array_member : Error<
  "base class %0 has a flexible array member">;
def err_incomplete_base_class : Error<"base class has incomplete type">;
def err_duplicate_base_class : Error<
  "base class %0 specified more than once as a direct base class">;
def warn_inaccessible_base_class : Warning<
  "由于不明确，无法访问直接基类%0:%1">,
  InGroup<DiagGroup<"inaccessible-base">>;
// FIXME: better way to display derivation?  Pass entire thing into diagclient?
def err_ambiguous_derived_to_base_conv : Error<
  "ambiguous conversion from derived class %0 to base class %1:%2">;
def err_ambiguous_memptr_conv : Error<
  "ambiguous conversion from pointer to member of %select{base|derived}0 "
  "class %1 to pointer to member of %select{derived|base}0 class %2:%3">;
def ext_ms_ambiguous_direct_base : ExtWarn<
  "accessing inaccessible direct base %0 of %1 is a Microsoft extension">,
  InGroup<MicrosoftInaccessibleBase>;

def err_memptr_conv_via_virtual : Error<
  "conversion from pointer to member of class %0 to pointer to member "
  "of class %1 via virtual base %2 is not allowed">;

// C++ member name lookup
def err_ambiguous_member_multiple_subobjects : Error<
  "non-static member %0 found in multiple base-class subobjects of type %1:%2">;
def err_ambiguous_member_multiple_subobject_types : Error<
  "member %0 found in multiple base classes of different types">;
def note_ambiguous_member_found : Note<"member found by ambiguous name lookup">;
def note_ambiguous_member_type_found : Note<
  "member type %0 found by ambiguous name lookup">;
def err_ambiguous_reference : Error<"reference to %0 is ambiguous">;
def note_ambiguous_candidate : Note<"candidate found by name lookup is %q0">;
def err_ambiguous_tag_hiding : Error<"a type named %0 is hidden by a "
  "declaration in a different namespace">;
def note_hidden_tag : Note<"type declaration hidden">;
def note_hiding_object : Note<"declaration hides type">;

// C++ operator overloading
def err_operator_overload_needs_class_or_enum : Error<
  "overloaded %0 must have at least one parameter of class "
  "or enumeration type">;

def err_operator_overload_variadic : Error<"overloaded %0 cannot be variadic">;
def err_operator_overload_static : Error<
  "overloaded %0 cannot be a static member function">;
def err_operator_overload_default_arg : Error<
  "parameter of overloaded %0 cannot have a default argument">;
def err_operator_overload_must_be : Error<
  "overloaded %0 must be a %select{unary|binary|unary or binary}2 operator "
  "(has %1 parameter%s1)">;

def err_operator_overload_must_be_member : Error<
  "overloaded %0 must be a non-static member function">;
def err_operator_overload_post_incdec_must_be_int : Error<
  "parameter of overloaded post-%select{increment|decrement}1 operator must "
  "have type 'int' (not %0)">;

// C++ allocation and deallocation functions.
def err_operator_new_delete_declared_in_namespace : Error<
  "%0 cannot be declared inside a namespace">;
def err_operator_new_delete_declared_static : Error<
  "%0 cannot be declared static in global scope">;
def ext_operator_new_delete_declared_inline : ExtWarn<
  "replacement function %0 cannot be declared 'inline'">,
  InGroup<DiagGroup<"inline-new-delete">>;
def err_operator_new_delete_invalid_result_type : Error<
  "%0 must return type %1">;
def err_operator_new_delete_dependent_result_type : Error<
  "%0 cannot have a dependent return type; use %1 instead">;
def err_operator_new_delete_too_few_parameters : Error<
  "%0 must have at least one parameter">;
def err_operator_new_delete_template_too_few_parameters : Error<
  "%0 template must have at least two parameters">;
def warn_operator_new_returns_null : Warning<
  "%0不应返回空指针，除非它声明为 'throw()'"
  "%select{| or 'noexcept'}1">, InGroup<OperatorNewReturnsNull>;

def err_operator_new_dependent_param_type : Error<
  "%0 cannot take a dependent type as first parameter; "
  "use size_t (%1) instead">;
def err_operator_new_param_type : Error<
  "%0 takes type size_t (%1) as first parameter">;
def err_operator_new_default_arg: Error<
  "parameter of %0 cannot have a default argument">;
def err_operator_delete_dependent_param_type : Error<
  "%0 cannot take a dependent type as first parameter; use %1 instead">;
def err_operator_delete_param_type : Error<
  "first parameter of %0 must have type %1">;
def err_destroying_operator_delete_not_usual : Error<
  "destroying operator delete can have only an optional size and optional "
  "alignment parameter">;
def note_implicit_delete_this_in_destructor_here : Note<
  "while checking implicit 'delete this' for virtual destructor">;
def err_builtin_operator_new_delete_not_usual : Error<
  "call to '%select{__builtin_operator_new|__builtin_operator_delete}0' "
  "selects non-usual %select{allocation|deallocation}0 function">;
def note_non_usual_function_declared_here : Note<
  "non-usual %0 declared here">;

// C++ literal operators
def err_literal_operator_outside_namespace : Error<
  "literal operator %0 must be in a namespace or global scope">;
def err_literal_operator_id_outside_namespace : Error<
  "non-namespace scope '%0' cannot have a literal operator member">;
def err_literal_operator_default_argument : Error<
  "literal operator cannot have a default argument">;
def err_literal_operator_bad_param_count : Error<
  "non-template literal operator must have one or two parameters">;
def err_literal_operator_invalid_param : Error<
  "parameter of literal operator must have type 'unsigned long long', 'long double', 'char', 'wchar_t', 'char16_t', 'char32_t', or 'const char *'">;
def err_literal_operator_param : Error<
  "invalid literal operator parameter type %0, did you mean %1?">;
def err_literal_operator_template_with_params : Error<
  "literal operator template cannot have any parameters">;
def err_literal_operator_template : Error<
  "template parameter list for literal operator must be either 'char...' or 'typename T, T...'">;
def err_literal_operator_extern_c : Error<
  "literal operator must have C++ linkage">;
def ext_string_literal_operator_template : ExtWarn<
  "string literal operator templates are a GNU extension">,
  InGroup<GNUStringLiteralOperatorTemplate>;
def warn_user_literal_reserved : Warning<
  "保留不以'_'开头的用户定义文字后缀 "
  "%select{; no literal will invoke this operator|}0">,
  InGroup<UserDefinedLiterals>;

// C++ conversion functions
def err_conv_function_not_member : Error<
  "conversion function must be a non-static member function">;
def err_conv_function_return_type : Error<
  "conversion function cannot have a return type">;
def err_conv_function_with_params : Error<
  "conversion function cannot have any parameters">;
def err_conv_function_variadic : Error<
  "conversion function cannot be variadic">;
def err_conv_function_to_array : Error<
  "conversion function cannot convert to an array type">;
def err_conv_function_to_function : Error<
  "conversion function cannot convert to a function type">;
def err_conv_function_with_complex_decl : Error<
  "cannot specify any part of a return type in the "
  "declaration of a conversion function"
  "%select{"
  "; put the complete type after 'operator'|"
  "; use a typedef to declare a conversion to %1|"
  "; use an alias template to declare a conversion to %1|"
  "}0">;
def err_conv_function_redeclared : Error<
  "conversion function cannot be redeclared">;
def warn_conv_to_self_not_used : Warning<
  "永远不会使用将%0转换为自身的转换函数">,
  InGroup<ClassConversion>;
def warn_conv_to_base_not_used : Warning<
  "将%0转换为其基类%1的转换函数将永远不会使用">,
  InGroup<ClassConversion>;
def warn_conv_to_void_not_used : Warning<
  "将永远不会使用将%0转换为%1的转换函数">,
  InGroup<ClassConversion>;

def warn_not_compound_assign : Warning<
  "使用可能用作复合赋值的一元运算符（%0=）">;

// C++11 explicit conversion operators
def ext_explicit_conversion_functions : ExtWarn<
  "explicit conversion functions are a C++11 extension">, InGroup<CXX11>;
def warn_cxx98_compat_explicit_conversion_functions : Warning<
  "显式转换函数与C++98不兼容">,
  InGroup<CXX98Compat>, DefaultIgnore;

// C++11 defaulted functions
def err_defaulted_special_member_params : Error<
  "an explicitly-defaulted %select{|copy |move }0constructor cannot "
  "have default arguments">;
def err_defaulted_special_member_variadic : Error<
  "an explicitly-defaulted %select{|copy |move }0constructor cannot "
  "be variadic">;
def err_defaulted_special_member_return_type : Error<
  "explicitly-defaulted %select{copy|move}0 assignment operator must "
  "return %1">;
def err_defaulted_special_member_quals : Error<
  "an explicitly-defaulted %select{copy|move}0 assignment operator may not "
  "have 'const'%select{, 'constexpr'|}1 or 'volatile' qualifiers">;
def err_defaulted_special_member_volatile_param : Error<
  "the parameter for an explicitly-defaulted %sub{select_special_member_kind}0 "
  "may not be volatile">;
def err_defaulted_special_member_move_const_param : Error<
  "the parameter for an explicitly-defaulted move "
  "%select{constructor|assignment operator}0 may not be const">;
def err_defaulted_special_member_copy_const_param : Error<
  "the parameter for this explicitly-defaulted copy "
  "%select{constructor|assignment operator}0 is const, but a member or base "
  "requires it to be non-const">;
def err_defaulted_copy_assign_not_ref : Error<
  "the parameter for an explicitly-defaulted copy assignment operator must be an "
  "lvalue reference type">;
def err_incorrect_defaulted_constexpr : Error<
  "defaulted definition of %sub{select_special_member_kind}0 "
  "is not constexpr">;
def err_incorrect_defaulted_consteval : Error<
  "defaulted declaration of %sub{select_special_member_kind}0 "
  "cannot be consteval because implicit definition is not constexpr">;
def warn_defaulted_method_deleted : Warning<
  "显式默认的 %sub{select_special_member_kind}0 被隐式地 "
  "删除">, InGroup<DefaultedFunctionDeleted>;
def err_out_of_line_default_deletes : Error<
  "defaulting this %sub{select_special_member_kind}0 "
  "would delete it after its first declaration">;
def note_deleted_type_mismatch : Note<
  "function is implicitly deleted because its declared type does not match "
  "the type of an implicit %sub{select_special_member_kind}0">;
def warn_cxx17_compat_defaulted_method_type_mismatch : Warning<
  "显式默认此%sub{select_special_member_kind}0 的类型 "
  "与隐式类型不同，这与C++20之前的C++标准 "
  "不兼容">, InGroup<CXXPre20Compat>, DefaultIgnore;
def warn_vbase_moved_multiple_times : Warning<
  "%0的默认移动分配运算符将 "
  "多次移动分配虚拟基类%1">, InGroup<DiagGroup<"multiple-move-vbase">>;
def note_vbase_moved_here : Note<
  "%select{%1 is a virtual base class of base class %2 declared here|"
  "virtual base class %1 declared here}0">;

// C++20 defaulted comparisons
// This corresponds to values of Sema::DefaultedComparisonKind.
def select_defaulted_comparison_kind : TextSubstitution<
  "%select{<ERROR>|equality|three-way|equality|relational}0 comparison "
  "operator">;
def ext_defaulted_comparison : ExtWarn<
  "defaulted comparison operators are a C++20 extension">, InGroup<CXX20>;
def warn_cxx17_compat_defaulted_comparison : Warning<
  "默认比较运算符与C++20之前的C++标准 "
  "不兼容">, InGroup<CXXPre20Compat>, DefaultIgnore;
def err_defaulted_comparison_template : Error<
  "comparison operator template cannot be defaulted">;
def err_defaulted_comparison_out_of_class : Error<
  "%sub{select_defaulted_comparison_kind}0 can only be defaulted in a class "
  "definition">;
def err_defaulted_comparison_param : Error<
  "invalid parameter type for defaulted %sub{select_defaulted_comparison_kind}0"
  "; found %1, expected %2%select{| or %4}3">;
def err_defaulted_comparison_param_mismatch : Error<
  "parameters for defaulted %sub{select_defaulted_comparison_kind}0 "
  "must have the same type%diff{ (found $ vs $)|}1,2">;
def err_defaulted_comparison_non_const : Error<
  "defaulted member %sub{select_defaulted_comparison_kind}0 must be "
  "const-qualified">;
def err_defaulted_comparison_return_type_not_bool : Error<
  "return type for defaulted %sub{select_defaulted_comparison_kind}0 "
  "must be 'bool', not %1">;
def err_defaulted_comparison_deduced_return_type_not_auto : Error<
  "deduced return type for defaulted %sub{select_defaulted_comparison_kind}0 "
  "must be 'auto', not %1">;
def warn_defaulted_comparison_deleted : Warning<
  "显式默认的 %sub{select_defaulted_comparison_kind}0 被隐式地 "
  "删除">, InGroup<DefaultedFunctionDeleted>;
def err_non_first_default_compare_deletes : Error<
  "defaulting %select{this %sub{select_defaulted_comparison_kind}1|"
  "the corresponding implicit 'operator==' for this defaulted 'operator<=>'}0 "
  "would delete it after its first declaration">;
def note_defaulted_comparison_union : Note<
  "defaulted %0 is implicitly deleted because "
  "%2 is a %select{union-like class|union}1 with variant members">;
def note_defaulted_comparison_reference_member : Note<
  "defaulted %0 is implicitly deleted because "
  "class %1 has a reference member">;
def note_defaulted_comparison_ambiguous : Note<
  "defaulted %0 is implicitly deleted because implied %select{|'==' |'<' }1"
  "comparison %select{|for member %3 |for base class %3 }2is ambiguous">;
def note_defaulted_comparison_inaccessible : Note<
  "defaulted %0 is implicitly deleted because it would invoke a "
  "%select{private|protected}3 %4%select{ member of %6|"
  " member of %6 to compare member %2| to compare base class %2}1">;
def note_defaulted_comparison_calls_deleted : Note<
  "defaulted %0 is implicitly deleted because it would invoke a deleted "
  "comparison function%select{| for member %2| for base class %2}1">;
def note_defaulted_comparison_no_viable_function : Note<
  "defaulted %0 is implicitly deleted because there is no viable comparison "
  "function%select{| for member %2| for base class %2}1">;
def note_defaulted_comparison_no_viable_function_synthesized : Note<
  "three-way comparison cannot be synthesized because there is no viable "
  "function for %select{'=='|'<'}0 comparison">;
def note_defaulted_comparison_not_rewritten_callee : Note<
  "defaulted %0 is implicitly deleted because this non-rewritten comparison "
  "function would be the best match for the comparison">;
def note_defaulted_comparison_cannot_deduce : Note<
  "return type of defaulted 'operator<=>' cannot be deduced because "
  "return type %2 of three-way comparison for %select{|member|base class}0 %1 "
  "is not a standard comparison category type">;
def err_defaulted_comparison_cannot_deduce_undeduced_auto : Error<
  "return type of defaulted 'operator<=>' cannot be deduced because "
  "three-way comparison for %select{|member|base class}0 %1 "
  "has a deduced return type and is not yet defined">;
def note_defaulted_comparison_cannot_deduce_undeduced_auto : Note<
  "%select{|member|base class}0 %1 declared here">;
def note_defaulted_comparison_cannot_deduce_callee : Note<
  "selected 'operator<=>' for %select{|member|base class}0 %1 declared here">;
def err_incorrect_defaulted_comparison_constexpr : Error<
  "defaulted definition of %select{%sub{select_defaulted_comparison_kind}1|"
  "three-way comparison operator}0 "
  "cannot be declared %select{constexpr|consteval}2 because "
  "%select{it|the corresponding implicit 'operator=='}0 "
  "invokes a non-constexpr comparison function">;
def note_defaulted_comparison_not_constexpr : Note<
  "non-constexpr comparison function would be used to compare "
  "%select{|member %1|base class %1}0">;
def note_defaulted_comparison_not_constexpr_here : Note<
  "non-constexpr comparison function declared here">;
def note_in_declaration_of_implicit_equality_comparison : Note<
  "while declaring the corresponding implicit 'operator==' "
  "for this defaulted 'operator<=>'">;

def ext_implicit_exception_spec_mismatch : ExtWarn<
  "function previously declared with an %select{explicit|implicit}0 exception "
  "specification redeclared with an %select{implicit|explicit}0 exception "
  "specification">, InGroup<DiagGroup<"implicit-exception-spec-mismatch">>;

def warn_ptr_arith_precedes_bounds : Warning<
  "递减%0的指针引用在数组开头之前">,
  InGroup<ArrayBoundsPointerArithmetic>, DefaultIgnore;
def warn_ptr_arith_exceeds_bounds : Warning<
  "递增了%0的指针引用了数组的结尾（ "
  "包含%1元素%s2)">,
  InGroup<ArrayBoundsPointerArithmetic>, DefaultIgnore;
def warn_array_index_precedes_bounds : Warning<
  "数组索引%0在数组开头之前">,
  InGroup<ArrayBounds>;
def warn_array_index_exceeds_bounds : Warning<
  "数组索引%0超过了数组的结尾（ "
  "其中包含%1元素%s2)">, InGroup<ArrayBounds>;
def note_array_declared_here : Note<
  "array %0 declared here">;

def warn_printf_insufficient_data_args : Warning<
  "'%%'转换多于数据参数 ">, InGroup<FormatInsufficientArgs>;
def warn_printf_data_arg_not_used : Warning<
  "格式字符串未使用数据参数">, InGroup<FormatExtraArgs>;
def warn_format_invalid_conversion : Warning<
  "转换说明符'%0'无效">, InGroup<FormatInvalidSpecifier>;
def warn_printf_incomplete_specifier : Warning<
  "格式说明符不完整">, InGroup<Format>;
def warn_missing_format_string : Warning<
  "缺少格式字符串">, InGroup<Format>;
def warn_scanf_nonzero_width : Warning<
  "未使用scanf格式字符串中的零字段宽度">,
  InGroup<Format>;
def warn_format_conversion_argument_type_mismatch : Warning<
  "format指定类型%0，但参数具有 "
  "%select{type|underlying type}2 %1">,
  InGroup<Format>;
def warn_format_conversion_argument_type_mismatch_pedantic : Extension<
  warn_format_conversion_argument_type_mismatch.Text>,
  InGroup<FormatPedantic>;
def warn_format_conversion_argument_type_mismatch_confusion : Warning<
  warn_format_conversion_argument_type_mismatch.Text>,
  InGroup<FormatTypeConfusion>, DefaultIgnore;
def warn_format_argument_needs_cast : Warning<
  "%select{values of type|enum values with underlying type}2 '%0'不应用作格式参数； "
  "改为向%1添加显式强制转换">,
  InGroup<Format>;
def warn_format_argument_needs_cast_pedantic : Warning<
  warn_format_argument_needs_cast.Text>,
  InGroup<FormatPedantic>, DefaultIgnore;
def warn_printf_positional_arg_exceeds_data_args : Warning <
  "数据参数位置'%0'超过了数据参数的数目（%1）">,
  InGroup<Format>;
def warn_format_zero_positional_specifier : Warning<
  "格式字符串中的位置参数从1开始计数（不是0）">,
  InGroup<Format>;
def warn_format_invalid_positional_specifier : Warning<
  "为指定的位置无效 %select{field width|field precision}0">,
  InGroup<Format>;
def warn_format_mix_positional_nonpositional_args : Warning<
  "无法在格式字符串中混合位置参数和非位置参数">,
  InGroup<Format>;
def warn_static_array_too_small : Warning<
  "数组参数太小；%select{contains %0 elements|is of size %0}2,"
  "被调用方至少需要%1 ">,
  InGroup<ArrayBounds>;
def note_callee_static_array : Note<
  "callee declares array parameter as static here">;
def warn_empty_format_string : Warning<
  "格式字符串为空">, InGroup<FormatZeroLength>;
def warn_format_string_is_wide_literal : Warning<
  "格式字符串不应为宽字符串">, InGroup<Format>;
def warn_printf_format_string_contains_null_char : Warning<
  "格式字符串在字符串正文中包含'\\0'">, InGroup<Format>;
def warn_printf_format_string_not_null_terminated : Warning<
  "格式字符串不是以null结尾的">, InGroup<Format>;
def warn_printf_asterisk_missing_arg : Warning<
  "'%select{*|.*}0' 指定的字段 %select{width|precision}0 缺少匹配的'int'参数">,
  InGroup<Format>;
def warn_printf_asterisk_wrong_type : Warning<
  "字段 %select{width|precision}0 的类型应为%1, 但参数的类型为%2">,
  InGroup<Format>;
def warn_printf_nonsensical_optional_amount: Warning<
  "%select{field width|precision}0 与'%1'转换说明符一起使用, 导致行为未定义">,
  InGroup<Format>;
def warn_printf_nonsensical_flag: Warning<
  "标志'%0'导致转换说明符为'%1'的行为未定义">,
  InGroup<Format>;
def warn_format_nonsensical_length: Warning<
  "长度修饰符'%0'导致未定义的行为或对'%1'转换说明符无效">,
  InGroup<Format>;
def warn_format_non_standard_positional_arg: Warning<
  "ISO C不支持位置参数">, InGroup<FormatNonStandard>, DefaultIgnore;
def warn_format_non_standard: Warning<
  "'%0' %select{length modifier|conversion specifier}1 不被ISO C支持">,
  InGroup<FormatNonStandard>, DefaultIgnore;
def warn_format_non_standard_conversion_spec: Warning<
  "ISO C不支持将长度修饰符'%0'与转换说明符'%1'一起使用">,
  InGroup<FormatNonStandard>, DefaultIgnore;
def err_invalid_mask_type_size : Error<
  "mask type size must be between 1-byte and 8-bytes">;
def warn_format_invalid_annotation : Warning<
  "在os_log()/os_trace()之外使用'%0'格式说明符批注">,
  InGroup<Format>;
def warn_format_P_no_precision : Warning<
  "不精确地使用'%%P'格式说明符">,
  InGroup<Format>;
def warn_printf_ignored_flag: Warning<
  "标志'%0'将被忽略，当标志'%1'存在时">,
  InGroup<Format>;
def warn_printf_empty_objc_flag: Warning<
  "缺少对象格式标志">,
  InGroup<Format>;
def warn_printf_ObjCflags_without_ObjCConversion: Warning<
  "对象格式标志不能与'%0'转换说明符一起使用">,
  InGroup<Format>;
def warn_printf_invalid_objc_flag: Warning<
    "'%0'不是有效的对象格式标志">,
    InGroup<Format>;
def warn_scanf_scanlist_incomplete : Warning<
  "scanf格式字符串中'%%[' in scanf format string没有结束']'">,
  InGroup<Format>;
def warn_format_bool_as_character : Warning<
  "使用'%0'格式说明符，但参数具有布尔值">,
  InGroup<Format>;
def note_format_string_defined : Note<"format string is defined here">;
def note_format_fix_specifier : Note<"did you mean to use '%0'?">;
def note_printf_c_str: Note<"did you mean to call the %0 method?">;
def note_format_security_fixit: Note<
  "treat the string as an argument to avoid this">;

def warn_null_arg : Warning<
  "传递给需要非null参数的被调用方的null">,
  InGroup<NonNull>;
def warn_null_ret : Warning<
  "null 被返回从 %select{function|method}0 该值需要非null返回值">,
  InGroup<NonNull>;

def err_lifetimebound_no_object_param : Error<
  "'lifetimebound' attribute cannot be applied; %select{static |non-}0member "
  "function has no implicit object parameter">;
def err_lifetimebound_ctor_dtor : Error<
  "'lifetimebound' attribute cannot be applied to a "
  "%select{constructor|destructor}0">;

// CHECK: returning address/reference of stack memory
def warn_ret_stack_addr_ref : Warning<
  "%select{address of|reference to}0 关联的堆栈内存 "
  "%select{local variable|parameter}2 %1 被返回">,
  InGroup<ReturnStackAddress>;
def warn_ret_local_temp_addr_ref : Warning<
  "返回 %select{address of|reference to}0 本地临时对象">,
  InGroup<ReturnStackAddress>;
def warn_ret_addr_label : Warning<
  "标签的本地返回地址">,
  InGroup<ReturnStackAddress>;
def err_ret_local_block : Error<
  "returning block that lives on the local stack">;
def note_local_var_initializer : Note<
  "%select{via initialization of|binding reference}0 variable "
  "%select{%2 |}1here">;
def note_lambda_capture_initializer : Note<
  "%select{implicitly |}2captured%select{| by reference}3"
  "%select{%select{ due to use|}2 here|"
  " via initialization of lambda capture %0}1">;
def note_init_with_default_member_initalizer : Note<
  "initializing field %0 with default member initializer">;

// Check for initializing a member variable with the address or a reference to
// a constructor parameter.
def warn_bind_ref_member_to_parameter : Warning<
  "正在将引用成员%0绑定到分配的堆栈 "
  "%select{variable|parameter}2 %1">, InGroup<DanglingField>;
def warn_init_ptr_member_to_parameter_addr : Warning<
  "正在初始化指针成员%0它有堆栈地址为 "
  "%select{variable|parameter}2 %1">, InGroup<DanglingField>;
def note_ref_or_ptr_member_declared_here : Note<
  "%select{reference|pointer}0 member declared here">;

def err_dangling_member : Error<
  "%select{reference|backing array for 'std::initializer_list'}2 "
  "%select{|subobject of }1member %0 "
  "%select{binds to|is}2 a temporary object "
  "whose lifetime would be shorter than the lifetime of "
  "the constructed object">;
def warn_dangling_member : Warning<
  "%select{reference|backing array for 'std::initializer_list'}2 "
  "%select{|subobject of }1member %0 "
  "%select{binds to|is}2 一个临时对象 "
  "其生存期短于构造对象的生存期">,
  InGroup<DanglingField>;
def warn_dangling_lifetime_pointer_member : Warning<
  "正在初始化指针成员%0以指向一个临时对象， "
  "该对象的生存期短于构造对象的生存期">,
  InGroup<DanglingGsl>;
def note_lifetime_extending_member_declared_here : Note<
  "%select{%select{reference|'std::initializer_list'}0 member|"
  "member with %select{reference|'std::initializer_list'}0 subobject}1 "
  "declared here">;
def warn_dangling_variable : Warning<
  "%select{temporary %select{whose address is used as value of|"
  "%select{|implicitly }2bound to}4 "
  "%select{%select{|reference }4member of local variable|"
  "local %select{variable|reference}4}1|"
  "array backing "
  "%select{initializer list subobject of local variable|"
  "local initializer list}1}0 "
  "%select{%3 |}2will be destroyed at the end of the full-expression">,
  InGroup<Dangling>;
def warn_new_dangling_reference : Warning<
  "临时绑定到已分配对象的引用成员 "
  "将在完整表达式结束时销毁">,
  InGroup<DanglingField>;
def warn_dangling_lifetime_pointer : Warning<
  "支持指针的对象 "
  "将在完整表达式结束时销毁">,
  InGroup<DanglingGsl>;
def warn_new_dangling_initializer_list : Warning<
  "阵列背衬"
  "%select{initializer list subobject of the allocated object|"
  "the allocated initializer list}0 "
  "将在完整表达式结束时销毁">,
  InGroup<DanglingInitializerList>;
def warn_unsupported_lifetime_extension : Warning<
  "抱歉，生存期扩展 "
  "%select{temporary|backing array of initializer list}0 被创建 "
  "通过使用默认成员初始值设定项进行聚合初始化的 "
  "不被支持; 生存期 %select{temporary|backing array}0 "
  "将在完整表达式的末尾结束">, InGroup<Dangling>;

// For non-floating point, expressions of the form x == x or x != x
// should result in a warning, since these always evaluate to a constant.
// Array comparisons have similar warnings
def warn_comparison_always : Warning<
  "%select{self-|array }0比较结果始终等于 "
  "%select{a constant|true|false|'std::strong_ordering::equal'}1">,
  InGroup<TautologicalCompare>;
def warn_comparison_bitwise_always : Warning<
  "按位比较的计算结果始终为%select{false|true}0">,
  InGroup<TautologicalBitwiseCompare>, DefaultIgnore;
def warn_comparison_bitwise_or : Warning<
  "具有非零值的按位或的计算结果始终为true">,
  InGroup<TautologicalBitwiseCompare>, DefaultIgnore;
def warn_tautological_overlap_comparison : Warning<
  "重叠比较始终计算为 %select{false|true}0">,
  InGroup<TautologicalOverlapCompare>, DefaultIgnore;
def warn_depr_array_comparison : Warning<
  "不推荐在两个数组之间进行比较； "
  "要比较数组地址，请使用一元'+'将操作数衰减为指针">,
  InGroup<DeprecatedArrayCompare>;

def warn_stringcompare : Warning<
  "未指定与 against %select{a string literal|@encode}0 的 "
  "比较结果（请改用显式字符串比较函数）">,
  InGroup<StringCompare>;

def warn_identity_field_assign : Warning<
  "分配 %select{field|instance variable}0 给自己">,
  InGroup<SelfAssignmentField>;

// Type safety attributes
def err_type_tag_for_datatype_not_ice : Error<
  "'type_tag_for_datatype' attribute requires the initializer to be "
  "an %select{integer|integral}0 constant expression">;
def err_type_tag_for_datatype_too_large : Error<
  "'type_tag_for_datatype' attribute requires the initializer to be "
  "an %select{integer|integral}0 constant expression "
  "that can be represented by a 64 bit integer">;
def err_tag_index_out_of_range : Error<
  "%select{type tag|argument}0 index %1 is greater than the number of arguments specified">;
def warn_type_tag_for_datatype_wrong_kind : Warning<
  "此类型标记不是为与此功能一起使用而设计的">,
  InGroup<TypeSafety>;
def warn_type_safety_type_mismatch : Warning<
  "参数类型%0与指定的%1类型标记不匹配 "
  "%select{that requires %3|}2">, InGroup<TypeSafety>;
def warn_type_safety_null_pointer_required : Warning<
  "指定的%0类型标记需要空指针">, InGroup<TypeSafety>;

// Generic selections.
def err_assoc_type_incomplete : Error<
  "type %0 in generic association incomplete">;
def err_assoc_type_nonobject : Error<
  "type %0 in generic association not an object type">;
def err_assoc_type_variably_modified : Error<
  "type %0 in generic association is a variably modified type">;
def err_assoc_compatible_types : Error<
  "type %0 in generic association compatible with previously specified type %1">;
def note_compat_assoc : Note<
  "compatible type %0 specified here">;
def err_generic_sel_no_match : Error<
  "controlling expression type %0 not compatible with any generic association type">;
def err_generic_sel_multi_match : Error<
  "controlling expression type %0 compatible with %1 generic association types">;


// Blocks
def err_blocks_disable : Error<"blocks support disabled - compile with -fblocks"
  " or %select{pick a deployment target that supports them|for OpenCL 2.0}0">;
def err_block_returning_array_function : Error<
  "block cannot return %select{array|function}0 type %1">;

// Builtin annotation
def err_builtin_annotation_first_arg : Error<
  "first argument to __builtin_annotation must be an integer">;
def err_builtin_annotation_second_arg : Error<
  "second argument to __builtin_annotation must be a non-wide string constant">;
def err_msvc_annotation_wide_str : Error<
  "arguments to __annotation must be wide string constants">;

// CFString checking
def err_cfstring_literal_not_string_constant : Error<
  "CFString literal is not a string constant">;
def warn_cfstring_truncated : Warning<
  "由于输入字节不属于输入代码集UTF-8， "
  "输入转换已停止">,
  InGroup<DiagGroup<"CFString-literal">>;

// os_log checking
// TODO: separate diagnostic for os_trace()
def err_os_log_format_not_string_constant : Error<
  "os_log() format argument is not a string constant">;
def err_os_log_argument_too_big : Error<
  "os_log() argument %0 is too big (%1 bytes, max %2)">;
def warn_os_log_format_narg : Error<
 "os_log() '%%n' format specifier is not allowed">, DefaultError;

// Statements.
def err_continue_not_in_loop : Error<
  "'continue' statement not in loop statement">;
def err_break_not_in_loop_or_switch : Error<
  "'break' statement not in loop or switch statement">;
def warn_loop_ctrl_binds_to_inner : Warning<
  "'%0'已绑定到当前循环，GCC将其绑定到封闭循环">,
  InGroup<GccCompat>;
def warn_break_binds_to_switch : Warning<
  "'break' 绑定到循环，GCC将其绑定到switch">,
  InGroup<GccCompat>;
def err_default_not_in_switch : Error<
  "'default' statement not in switch statement">;
def err_case_not_in_switch : Error<"'case' statement not in switch statement">;
def warn_bool_switch_condition : Warning<
  "switch 条件具有布尔值">, InGroup<SwitchBool>;
def warn_case_value_overflow : Warning<
  "将大小写值转换为切换条件类型时溢出（%0到%1）">,
  InGroup<Switch>;
def err_duplicate_case : Error<"duplicate case value '%0'">;
def err_duplicate_case_differing_expr : Error<
  "duplicate case value: '%0' and '%1' both equal '%2'">;
def warn_case_empty_range : Warning<"指定的空case范围">;
def warn_missing_case_for_condition :
  Warning<"没有 case 匹配的常量switch 条件 '%0'">;

def warn_def_missing_case : Warning<"%plural{"
  "1:enumeration value %1 not explicitly handled in switch|"
  "2:enumeration values %1 and %2 not explicitly handled in switch|"
  "3:enumeration values %1, %2, and %3 not explicitly handled in switch|"
  ":%0 enumeration values not explicitly handled in switch: %1, %2, %3...}0">,
  InGroup<SwitchEnum>, DefaultIgnore;

def warn_missing_case : Warning<"%plural{"
  "1:enumeration value %1 not handled in switch|"
  "2:enumeration values %1 and %2 not handled in switch|"
  "3:enumeration values %1, %2, and %3 not handled in switch|"
  ":%0 enumeration values not handled in switch: %1, %2, %3...}0">,
  InGroup<Switch>;

def warn_unannotated_fallthrough : Warning<
  "switch标签之间未注释的直通">,
  InGroup<ImplicitFallthrough>, DefaultIgnore;
def warn_unannotated_fallthrough_per_function : Warning<
  "部分注释函数中switch标签之间的 "
  "未注释直通">, InGroup<ImplicitFallthroughPerFunction>, DefaultIgnore;
def note_insert_fallthrough_fixit : Note<
  "insert '%0;' to silence this warning">;
def note_insert_break_fixit : Note<
  "insert 'break;' to avoid fall-through">;
def err_fallthrough_attr_wrong_target : Error<
  "%0 attribute is only allowed on empty statements">;
def note_fallthrough_insert_semi_fixit : Note<"did you forget ';'?">;
def err_fallthrough_attr_outside_switch : Error<
  "fallthrough annotation is outside switch statement">;
def err_fallthrough_attr_invalid_placement : Error<
  "fallthrough annotation does not directly precede switch label">;
def warn_fallthrough_attr_unreachable : Warning<
  "不可达代码中的失败注释">,
  InGroup<ImplicitFallthrough>, DefaultIgnore;

def warn_unreachable_default : Warning<
  "覆盖所有枚举值的switch中的默认标签">,
  InGroup<CoveredSwitchDefault>, DefaultIgnore;
def warn_not_in_enum : Warning<"case值不在枚举类型中%0">,
  InGroup<Switch>;
def warn_not_in_enum_assignment : Warning<"整数常量不在枚举类型%0 "
  "的范围内">, InGroup<DiagGroup<"assign-enum">>, DefaultIgnore;
def err_typecheck_statement_requires_scalar : Error<
  "statement requires expression of scalar type (%0 invalid)">;
def err_typecheck_statement_requires_integer : Error<
  "statement requires expression of integer type (%0 invalid)">;
def err_multiple_default_labels_defined : Error<
  "multiple default labels in one switch">;
def err_switch_multiple_conversions : Error<
  "multiple conversions from switch condition type %0 to an integral or "
  "enumeration type">;
def note_switch_conversion : Note<
  "conversion to %select{integral|enumeration}0 type %1">;
def err_switch_explicit_conversion : Error<
  "switch condition type %0 requires explicit conversion to %1">;
def err_switch_incomplete_class_type : Error<
  "switch condition has incomplete class type %0">;

def warn_empty_if_body : Warning<
  "if 语句的主体为空">, InGroup<EmptyBody>;
def warn_empty_for_body : Warning<
  "for循环的主体为空">, InGroup<EmptyBody>;
def warn_empty_range_based_for_body : Warning<
  "基于范围的 for 循环的主体为空">, InGroup<EmptyBody>;
def warn_empty_while_body : Warning<
  "while 循环的主体为空">, InGroup<EmptyBody>;
def warn_empty_switch_body : Warning<
  "switch 语句的主体为空">, InGroup<EmptyBody>;
def note_empty_body_on_separate_line : Note<
  "put the semicolon on a separate line to silence this warning">;

def err_va_start_captured_stmt : Error<
  "'va_start' cannot be used in a captured statement">;
def err_va_start_outside_function : Error<
  "'va_start' cannot be used outside a function">;
def err_va_start_fixed_function : Error<
  "'va_start' used in function with fixed args">;
def err_va_start_used_in_wrong_abi_function : Error<
  "'va_start' used in %select{System V|Win64}0 ABI function">;
def err_ms_va_start_used_in_sysv_function : Error<
  "'__builtin_ms_va_start' used in System V ABI function">;
def warn_second_arg_of_va_start_not_last_named_param : Warning<
  "'va_start' 的第二个参数不是最后一个命名参数">,
  InGroup<Varargs>;
def warn_va_start_type_is_undefined : Warning<
  "传递 %select{an object that undergoes default argument promotion|"
  "an object of reference type|a parameter declared with the 'register' "
  "keyword}0 给 'va_start' 具有未定义的行为">, InGroup<Varargs>;
def err_first_argument_to_va_arg_not_of_type_va_list : Error<
  "first argument to 'va_arg' is of type %0 and not 'va_list'">;
def err_second_parameter_to_va_arg_incomplete: Error<
  "second argument to 'va_arg' is of incomplete type %0">;
def err_second_parameter_to_va_arg_abstract: Error<
  "second argument to 'va_arg' is of abstract type %0">;
def warn_second_parameter_to_va_arg_not_pod : Warning<
  "'va_arg' 的第二个参数是非 POD 类型 %0">,
  InGroup<NonPODVarargs>, DefaultError;
def warn_second_parameter_to_va_arg_ownership_qualified : Warning<
  "'va_arg' 的第二个参数是 ARC 所有权限定类型 %0">,
  InGroup<NonPODVarargs>, DefaultError;
def warn_second_parameter_to_va_arg_never_compatible : Warning<
  "'va_arg' 的第二个参数是可提升类型 %0； 此 va_arg 具有 "
  "未定义的行为，因为参数将被提升为 %1">, InGroup<Varargs>;

def warn_return_missing_expr : Warning<
  "非void %select{function|method}1 %0 应该返回一个值">, DefaultError,
  InGroup<ReturnType>;
def ext_return_missing_expr : ExtWarn<
  "non-void %select{function|method}1 %0 should return a value">, DefaultError,
  InGroup<ReturnType>;
def ext_return_has_expr : ExtWarn<
  "%select{void function|void method|constructor|destructor}1 %0 "
  "should not return a value">,
  DefaultError, InGroup<ReturnType>;
def ext_return_has_void_expr : Extension<
  "void %select{function|method|block}1 %0 should not return void expression">;
def err_return_init_list : Error<
  "%select{void function|void method|constructor|destructor}1 %0 "
  "must not return a value">;
def err_ctor_dtor_returns_void : Error<
  "%select{constructor|destructor}1 %0 must not return void expression">;
def warn_noreturn_function_has_return_expr : Warning<
  "函数 %0 声明 'noreturn' 不应返回">,
  InGroup<InvalidNoreturn>;
def warn_falloff_noreturn_function : Warning<
  "声明为'noreturn'的函数不应返回">,
  InGroup<InvalidNoreturn>;
def err_noreturn_block_has_return_expr : Error<
  "block declared 'noreturn' should not return">;
def err_noreturn_missing_on_first_decl : Error<
  "function declared '[[noreturn]]' after its first declaration">;
def note_noreturn_missing_first_decl : Note<
  "declaration missing '[[noreturn]]' attribute is here">;
def err_carries_dependency_missing_on_first_decl : Error<
  "%select{function|parameter}0 declared '[[carries_dependency]]' "
  "after its first declaration">;
def note_carries_dependency_missing_first_decl : Note<
  "declaration missing '[[carries_dependency]]' attribute is here">;
def err_carries_dependency_param_not_function_decl : Error<
  "'[[carries_dependency]]' attribute only allowed on parameter in a function "
  "declaration or lambda">;
def err_block_on_nonlocal : Error<
  "__block attribute not allowed, only allowed on local variables">;
def err_block_on_vm : Error<
  "__block attribute not allowed on declaration with a variably modified type">;
def err_sizeless_nonlocal : Error<
  "non-local variable with sizeless type %0">;

def err_vec_builtin_non_vector : Error<
 "first two arguments to %0 must be vectors">;
def err_vec_builtin_incompatible_vector : Error<
  "first two arguments to %0 must have the same type">;
def err_vsx_builtin_nonconstant_argument : Error<
  "argument %0 to %1 must be a 2-bit unsigned literal (i.e. 0, 1, 2 or 3)">;

def err_shufflevector_nonconstant_argument : Error<
  "index for __builtin_shufflevector must be a constant integer">;
def err_shufflevector_argument_too_large : Error<
  "index for __builtin_shufflevector must be less than the total number "
  "of vector elements">;

def err_convertvector_non_vector : Error<
  "first argument to __builtin_convertvector must be a vector">;
def err_convertvector_non_vector_type : Error<
  "second argument to __builtin_convertvector must be a vector type">;
def err_convertvector_incompatible_vector : Error<
  "first two arguments to __builtin_convertvector must have the same number of elements">;

def err_first_argument_to_cwsc_not_call : Error<
  "first argument to __builtin_call_with_static_chain must be a non-member call expression">;
def err_first_argument_to_cwsc_block_call : Error<
  "first argument to __builtin_call_with_static_chain must not be a block call">;
def err_first_argument_to_cwsc_builtin_call : Error<
  "first argument to __builtin_call_with_static_chain must not be a builtin call">;
def err_first_argument_to_cwsc_pdtor_call : Error<
  "first argument to __builtin_call_with_static_chain must not be a pseudo-destructor call">;
def err_second_argument_to_cwsc_not_pointer : Error<
  "second argument to __builtin_call_with_static_chain must be of pointer type">;

def err_vector_incorrect_num_initializers : Error<
  "%select{too many|too few}0 elements in vector initialization (expected %1 elements, have %2)">;
def err_altivec_empty_initializer : Error<"expected initializer">;

def err_invalid_neon_type_code : Error<
  "incompatible constant for this __builtin_neon function">;
def err_argument_invalid_range : Error<
  "argument value %0 is outside the valid range [%1, %2]">;
def warn_argument_invalid_range : Warning<
  "参数值 %0 超出有效范围 [%1, %2]">, DefaultError,
  InGroup<DiagGroup<"argument-outside-range">>;
def err_argument_not_multiple : Error<
  "argument should be a multiple of %0">;
def err_argument_not_power_of_2 : Error<
  "argument should be a power of 2">;
def err_argument_not_shifted_byte : Error<
  "argument should be an 8-bit value shifted by a multiple of 8 bits">;
def err_argument_not_shifted_byte_or_xxff : Error<
  "argument should be an 8-bit value shifted by a multiple of 8 bits, or in the form 0x??FF">;
def err_rotation_argument_to_cadd
    : Error<"argument should be the value 90 or 270">;
def err_rotation_argument_to_cmla
    : Error<"argument should be the value 0, 90, 180 or 270">;
def warn_neon_vector_initializer_non_portable : Warning<
  "向量初始化器与大端模式下的 NEON 内在函数 "
  "不兼容">, InGroup<DiagGroup<"nonportable-vector-initialization">>;
def note_neon_vector_initializer_non_portable : Note<
  "consider using vld1_%0%1() to initialize a vector from memory, or "
  "vcreate_%0%1() to initialize from an integer constant">;
def note_neon_vector_initializer_non_portable_q : Note<
  "consider using vld1q_%0%1() to initialize a vector from memory, or "
  "vcombine_%0%1(vcreate_%0%1(), vcreate_%0%1()) to initialize from integer "
  "constants">;
def err_systemz_invalid_tabort_code : Error<
  "invalid transaction abort code">;
def err_64_bit_builtin_32_bit_tgt : Error<
  "this builtin is only available on 64-bit targets">;
def err_32_bit_builtin_64_bit_tgt : Error<
  "this builtin is only available on 32-bit targets">;
def err_builtin_x64_aarch64_only : Error<
  "this builtin is only available on x86-64 and aarch64 targets">;
def err_mips_builtin_requires_dsp : Error<
  "this builtin requires 'dsp' ASE, please use -mdsp">;
def err_mips_builtin_requires_dspr2 : Error<
  "this builtin requires 'dsp r2' ASE, please use -mdspr2">;
def err_mips_builtin_requires_msa : Error<
  "this builtin requires 'msa' ASE, please use -mmsa">;
def err_ppc_builtin_only_on_pwr7 : Error<
  "this builtin is only valid on POWER7 or later CPUs">;
def err_ppc_invalid_use_mma_type : Error<
  "invalid use of PPC MMA type">;
def err_x86_builtin_invalid_rounding : Error<
  "invalid rounding argument">;
def err_x86_builtin_invalid_scale : Error<
  "scale argument must be 1, 2, 4, or 8">;
def err_x86_builtin_tile_arg_duplicate : Error<
  "tile arguments must refer to different tiles">;

def err_builtin_target_unsupported : Error<
  "builtin is not supported on this target">;
def err_builtin_longjmp_unsupported : Error<
  "__builtin_longjmp is not supported for the current target">;
def err_builtin_setjmp_unsupported : Error<
  "__builtin_setjmp is not supported for the current target">;

def err_builtin_longjmp_invalid_val : Error<
  "argument to __builtin_longjmp must be a constant 1">;
def err_builtin_requires_language : Error<"'%0' is only available in %1">;

def err_constant_integer_arg_type : Error<
  "argument to %0 must be a constant integer">;

def ext_mixed_decls_code : Extension<
  "ISO C90 forbids mixing declarations and code">,
  InGroup<DiagGroup<"declaration-after-statement">>;

def err_non_local_variable_decl_in_for : Error<
  "declaration of non-local variable in 'for' loop">;
def err_non_variable_decl_in_for : Error<
  "non-variable declaration in 'for' loop">;
def err_toomany_element_decls : Error<
  "only one element declaration is allowed">;
def err_selector_element_not_lvalue : Error<
  "selector element is not a valid lvalue">;
def err_selector_element_type : Error<
  "selector element type %0 is not a valid object">;
def err_selector_element_const_type : Error<
  "selector element of type %0 cannot be a constant lvalue expression">;
def err_collection_expr_type : Error<
  "the type %0 is not a pointer to a fast-enumerable object">;
def warn_collection_expr_type : Warning<
  "集合表达式类型%0可能没有响应%1">;

def err_invalid_conversion_between_ext_vectors : Error<
  "invalid conversion between ext-vector type %0 and %1">;

def warn_duplicate_attribute_exact : Warning<
  "属性%0已应用">, InGroup<IgnoredAttributes>;

def warn_duplicate_attribute : Warning<
  "属性 %0 已应用不同的参数">,
  InGroup<IgnoredAttributes>;

def warn_sync_fetch_and_nand_semantics_change : Warning<
  "这个内在的语义随着 GCC 版本 4.4 改变了  "
  "- 这里提供了更新的语义">,
  InGroup<DiagGroup<"sync-fetch-and-nand-semantics-changed">>;

// Type
def ext_wchar_t_sign_spec : ExtWarn<"'%0' cannot be signed or unsigned">,
  InGroup<DiagGroup<"signed-unsigned-wchar">>, DefaultError;
def warn_receiver_forward_class : Warning<
  "接收者 %0 是一个转发类，对应的@interface 可能不存在">,
  InGroup<ForwardClassReceiver>;
def note_method_sent_forward_class : Note<"method %0 is used for the forward class">;
def ext_missing_declspec : ExtWarn<
  "declaration specifier missing, defaulting to 'int'">;
def ext_missing_type_specifier : ExtWarn<
  "type specifier missing, defaults to 'int'">,
  InGroup<ImplicitInt>;
def err_decimal_unsupported : Error<
  "GNU decimal type extension not supported">;
def err_missing_type_specifier : Error<
  "C++ requires a type specifier for all declarations">;
def err_objc_array_of_interfaces : Error<
  "array of interface %0 is invalid (probably should be an array of pointers)">;
def ext_c99_array_usage : Extension<
  "%select{qualifier in |static |}0array size %select{||'[*] '}0is a C99 "
  "feature">, InGroup<C99>;
def err_c99_array_usage_cxx : Error<
  "%select{qualifier in |static |}0array size %select{||'[*] '}0is a C99 "
  "feature, not permitted in C++">;
def err_type_unsupported : Error<
  "%0 is not supported on this target">;
def err_nsconsumed_attribute_mismatch : Error<
  "overriding method has mismatched ns_consumed attribute on its"
  " parameter">;
def err_nsreturns_retained_attribute_mismatch : Error<
  "overriding method has mismatched ns_returns_%select{not_retained|retained}0"
  " attributes">;
def err_nserrordomain_invalid_decl : Error<
  "domain argument %select{|%1 }0does not refer to global constant">;
def err_nserrordomain_wrong_type : Error<
  "domain argument %0 does not point to an NSString or CFString constant">;

def warn_nsconsumed_attribute_mismatch : Warning<
  err_nsconsumed_attribute_mismatch.Text>, InGroup<NSConsumedMismatch>;
def warn_nsreturns_retained_attribute_mismatch : Warning<
  err_nsreturns_retained_attribute_mismatch.Text>, InGroup<NSReturnsMismatch>;

def note_getter_unavailable : Note<
  "or because setter is declared here, but no getter method %0 is found">;
def err_invalid_protocol_qualifiers : Error<
  "invalid protocol qualifiers on non-ObjC type">;
def warn_ivar_use_hidden : Warning<
  "%0 的本地声明隐藏了实例变量">,
   InGroup<ShadowIvar>;
def warn_direct_initialize_call : Warning<
  "显式调用 +initialize 会导致重复调用 +initialize">,
   InGroup<ExplicitInitializeCall>;
def warn_direct_super_initialize_call : Warning<
  "显式调用 [super initialize] 应该 "
  "只在 +initialize 的实现中">,
   InGroup<ExplicitInitializeCall>;
def err_ivar_use_in_class_method : Error<
  "instance variable %0 accessed in class method">;
def err_private_ivar_access : Error<"instance variable %0 is private">,
  AccessControl;
def err_protected_ivar_access : Error<"instance variable %0 is protected">,
  AccessControl;
def warn_maynot_respond : Warning<"%0 可能没有响应 %1">;
def ext_typecheck_base_super : Warning<
  "方法参数类型 "
  "%diff{$ does not match super class method parameter type $|"
  "does not match super class method parameter type}0,1">,
   InGroup<SuperSubClassMismatch>, DefaultIgnore;
def warn_missing_method_return_type : Warning<
  "方法没有指定返回类型； 默认为'id'">,
  InGroup<MissingMethodReturnType>, DefaultIgnore;
def warn_direct_ivar_access : Warning<"正在直接访问 "
  "实例变量 %0">, InGroup<DiagGroup<"direct-ivar-access">>, DefaultIgnore;

// Spell-checking diagnostics
def err_unknown_typename : Error<
  "未识别的变量名%0">;
def err_unknown_type_or_class_name_suggest : Error<
  "unknown %select{type|class}1 name %0; did you mean %2?">;
def err_unknown_typename_suggest : Error<
  "unknown type name %0; did you mean %1?">;
def err_unknown_nested_typename_suggest : Error<
  "no type named %0 in %1; did you mean %select{|simply }2%3?">;
def err_no_member_suggest : Error<"no member named %0 in %1; did you mean %select{|simply }2%3?">;
def err_undeclared_use_suggest : Error<
  "use of undeclared %0; did you mean %1?">;
def err_undeclared_var_use_suggest : Error<
  "use of undeclared identifier %0; did you mean %1?">;
def err_no_template : Error<"no template named %0">;
def err_no_template_suggest : Error<"no template named %0; did you mean %1?">;
def err_no_member_template : Error<"no template named %0 in %1">;
def err_no_member_template_suggest : Error<
  "no template named %0 in %1; did you mean %select{|simply }2%3?">;
def err_non_template_in_template_id : Error<
  "%0 does not name a template but is followed by template arguments">;
def err_non_template_in_template_id_suggest : Error<
  "%0 does not name a template but is followed by template arguments; "
  "did you mean %1?">;
def err_non_template_in_member_template_id_suggest : Error<
  "member %0 of %1 is not a template; did you mean %select{|simply }2%3?">;
def note_non_template_in_template_id_found : Note<
  "non-template declaration found by name lookup">;
def err_mem_init_not_member_or_class_suggest : Error<
  "initializer %0 does not name a non-static data member or base "
  "class; did you mean the %select{base class|member}1 %2?">;
def err_field_designator_unknown_suggest : Error<
  "field designator %0 does not refer to any field in type %1; did you mean "
  "%2?">;
def err_typecheck_member_reference_ivar_suggest : Error<
  "%0 does not have a member named %1; did you mean %2?">;
def err_property_not_found_suggest : Error<
  "property %0 not found on object of type %1; did you mean %2?">;
def err_class_property_found : Error<
  "property %0 is a class property; did you mean to access it with class '%1'?">;
def err_ivar_access_using_property_syntax_suggest : Error<
  "property %0 not found on object of type %1; did you mean to access instance variable %2?">;
def warn_property_access_suggest : Warning<
"属性 %0在 %1 类型的对象上找不到； 您的意思是访问属性 %2 吗？">,
InGroup<PropertyAccessDotSyntax>;
def err_property_found_suggest : Error<
  "property %0 found on object of type %1; did you mean to access "
  "it with the \".\" operator?">;
def err_undef_interface_suggest : Error<
  "cannot find interface declaration for %0; did you mean %1?">;
def warn_undef_interface_suggest : Warning<
  "找不到 %0 的接口声明； 你是说%1吗？">;
def err_undef_superclass_suggest : Error<
  "cannot find interface declaration for %0, superclass of %1; did you mean "
  "%2?">;
def err_undeclared_protocol_suggest : Error<
  "cannot find protocol declaration for %0; did you mean %1?">;
def note_base_class_specified_here : Note<
  "base class %0 specified here">;
def err_using_directive_suggest : Error<
  "no namespace named %0; did you mean %1?">;
def err_using_directive_member_suggest : Error<
  "no namespace named %0 in %1; did you mean %select{|simply }2%3?">;
def note_namespace_defined_here : Note<"namespace %0 defined here">;
def err_sizeof_pack_no_pack_name_suggest : Error<
  "%0 does not refer to the name of a parameter pack; did you mean %1?">;
def note_parameter_pack_here : Note<"parameter pack %0 declared here">;

def err_uncasted_use_of_unknown_any : Error<
  "%0 has unknown type; cast it to its declared type to use it">;
def err_uncasted_call_of_unknown_any : Error<
  "%0 has unknown return type; cast the call to its declared return type">;
def err_uncasted_send_to_unknown_any_method : Error<
  "no known method %select{%objcinstance1|%objcclass1}0; cast the "
  "message send to the method's return type">;
def err_unsupported_unknown_any_decl : Error<
  "%0 has unknown type, which is not supported for this kind of declaration">;
def err_unsupported_unknown_any_expr : Error<
  "unsupported expression with unknown type">;
def err_unsupported_unknown_any_call : Error<
  "call to unsupported expression with unknown type">;
def err_unknown_any_addrof : Error<
  "the address of a declaration with unknown type "
  "can only be cast to a pointer type">;
def err_unknown_any_addrof_call : Error<
  "address-of operator cannot be applied to a call to a function with "
  "unknown return type">;
def err_unknown_any_var_function_type : Error<
  "variable %0 with unknown type cannot be given a function type">;
def err_unknown_any_function : Error<
  "function %0 with unknown type must be given a function type">;

def err_filter_expression_integral : Error<
  "filter expression has non-integral type %0">;

def err_non_asm_stmt_in_naked_function : Error<
  "non-ASM statement in naked function is not supported">;
def err_asm_naked_this_ref : Error<
  "'this' pointer references not allowed in naked functions">;
def err_asm_naked_parm_ref : Error<
  "parameter references not allowed in naked functions">;

// OpenCL warnings and errors.
def err_invalid_astype_of_different_size : Error<
  "invalid reinterpretation: sizes of %0 and %1 must match">;
def err_static_kernel : Error<
  "kernel functions cannot be declared static">;
def err_method_kernel : Error<
  "kernel functions cannot be class members">;
def err_template_kernel : Error<
  "kernel functions cannot be used in a template declaration, instantiation or specialization">;
def err_opencl_ptrptr_kernel_param : Error<
  "kernel parameter cannot be declared as a pointer to a pointer">;
def err_kernel_arg_address_space : Error<
  "pointer arguments to kernel functions must reside in '__global', "
  "'__constant' or '__local' address space">;
def err_opencl_ext_vector_component_invalid_length : Error<
  "vector component access has invalid length %0.  Supported: 1,2,3,4,8,16.">;
def err_opencl_function_variable : Error<
  "%select{non-kernel function|function scope}0 variable cannot be declared in %1 address space">;
def err_opencl_addrspace_scope : Error<
  "variables in the %0 address space can only be declared in the outermost "
  "scope of a kernel function">;
def err_static_function_scope : Error<
  "variables in function scope cannot be declared static">;
def err_opencl_bitfields : Error<
  "bit-fields are not supported in OpenCL">;
def err_opencl_vla : Error<
  "variable length arrays are not supported in OpenCL">;
def err_opencl_scalar_type_rank_greater_than_vector_type : Error<
    "scalar operand type has greater rank than the type of the vector "
    "element. (%0 and %1)">;
def err_bad_kernel_param_type : Error<
  "%0 cannot be used as the type of a kernel parameter">;
def err_opencl_implicit_function_decl : Error<
  "implicit declaration of function %0 is invalid in OpenCL">;
def err_record_with_pointers_kernel_param : Error<
  "%select{struct|union}0 kernel parameters may not contain pointers">;
def note_within_field_of_type : Note<
  "within field of type %0 declared here">;
def note_illegal_field_declared_here : Note<
  "field of illegal %select{type|pointer type}0 %1 declared here">;
def err_opencl_type_struct_or_union_field : Error<
  "the %0 type cannot be used to declare a structure or union field">;
def err_event_t_addr_space_qual : Error<
  "the event_t type can only be used with __private address space qualifier">;
def err_expected_kernel_void_return_type : Error<
  "kernel must have void return type">;
def err_sampler_initializer_not_integer : Error<
  "sampler_t initialization requires 32-bit integer, not %0">;
def warn_sampler_initializer_invalid_bits : Warning<
  "采样器初始值设定项具有无效的%0位">, InGroup<SpirCompat>, DefaultIgnore;
def err_sampler_argument_required : Error<
  "sampler_t variable required - got %0">;
def err_wrong_sampler_addressspace: Error<
  "sampler type cannot be used with the __local and __global address space qualifiers">;
def err_opencl_nonconst_global_sampler : Error<
  "global sampler requires a const or constant address space qualifier">;
def err_opencl_cast_non_zero_to_event_t : Error<
  "cannot cast non-zero value '%0' to 'event_t'">;
def err_opencl_global_invalid_addr_space : Error<
  "%select{program scope|static local|extern}0 variable must reside in %1 address space">;
def err_missing_actual_pipe_type : Error<
  "missing actual type specifier for pipe">;
def err_reference_pipe_type : Error <
  "pipes packet types cannot be of reference type">;
def err_opencl_no_main : Error<"%select{function|kernel}0 cannot be called 'main'">;
def err_opencl_kernel_attr :
  Error<"attribute %0 can only be applied to an OpenCL kernel function">;
def err_opencl_return_value_with_address_space : Error<
  "return value cannot be qualified with address space">;
def err_opencl_constant_no_init : Error<
  "variable in constant address space must be initialized">;
def err_opencl_atomic_init: Error<
  "atomic variable can be %select{assigned|initialized}0 to a variable only "
  "in global address space">;
def err_opencl_implicit_vector_conversion : Error<
  "implicit conversions between vector types (%0 and %1) are not permitted">;
def err_opencl_invalid_type_array : Error<
  "array of %0 type is invalid in OpenCL">;
def err_opencl_ternary_with_block : Error<
  "block type cannot be used as expression in ternary expression in OpenCL">;
def err_opencl_pointer_to_type : Error<
  "pointer to type %0 is invalid in OpenCL">;
def err_opencl_type_can_only_be_used_as_function_parameter : Error <
  "type %0 can only be used as a function parameter in OpenCL">;
def warn_opencl_attr_deprecated_ignored : Warning <
  "%0属性属性在%1中已被弃用并忽略">, InGroup<IgnoredAttributes>;
def err_opencl_variadic_function : Error<
  "invalid prototype, variadic arguments are not allowed in OpenCL">;
def err_opencl_requires_extension : Error<
  "use of %select{type|declaration}0 %1 requires %2 extension to be enabled">;
def warn_opencl_generic_address_space_arg : Warning<
  "将非通用地址空间指针传递给%0"
  "可能会导致动态转换影响性能">,
  InGroup<Conversion>, DefaultIgnore;

// OpenCL v2.0 s6.13.6 -- Builtin Pipe Functions
def err_opencl_builtin_pipe_first_arg : Error<
  "first argument to %0 must be a pipe type">;
def err_opencl_builtin_pipe_arg_num : Error<
  "invalid number of arguments to function: %0">;
def err_opencl_builtin_pipe_invalid_arg : Error<
  "invalid argument type to function %0 (expecting %1 having %2)">;
def err_opencl_builtin_pipe_invalid_access_modifier : Error<
  "invalid pipe access modifier (expecting %0)">;

// OpenCL access qualifier
def err_opencl_invalid_access_qualifier : Error<
  "access qualifier can only be used for pipe and image type">;
def err_opencl_invalid_read_write : Error<
  "access qualifier %0 can not be used for %1 %select{|prior to OpenCL version 2.0}2">;
def err_opencl_multiple_access_qualifiers : Error<
  "multiple access qualifiers">;
def note_opencl_typedef_access_qualifier : Note<
  "previously declared '%0' here">;

// OpenCL v2.0 s6.12.5 Blocks restrictions
def err_opencl_block_storage_type : Error<
  "the __block storage type is not permitted">;
def err_opencl_invalid_block_declaration : Error<
  "invalid block variable declaration - must be %select{const qualified|initialized}0">;
def err_opencl_extern_block_declaration : Error<
  "invalid block variable declaration - using 'extern' storage class is disallowed">;
def err_opencl_block_ref_block : Error<
  "cannot refer to a block inside block">;

// OpenCL v2.0 s6.13.9 - Address space qualifier functions.
def err_opencl_builtin_to_addr_invalid_arg : Error<
  "invalid argument %0 to function: %1, expecting a generic pointer argument">;

// OpenCL v2.0 s6.13.17 Enqueue kernel restrictions.
def err_opencl_enqueue_kernel_incorrect_args : Error<
  "illegal call to enqueue_kernel, incorrect argument types">;
def err_opencl_enqueue_kernel_local_size_args : Error<
  "mismatch in number of block parameters and local size arguments passed">;
def err_opencl_enqueue_kernel_invalid_local_size_type : Error<
  "illegal call to enqueue_kernel, parameter needs to be specified as integer type">;
def err_opencl_enqueue_kernel_blocks_non_local_void_args : Error<
  "blocks used in enqueue_kernel call are expected to have parameters of type 'local void*'">;
def err_opencl_enqueue_kernel_blocks_no_args : Error<
  "blocks with parameters are not accepted in this prototype of enqueue_kernel call">;

def err_opencl_builtin_expected_type : Error<
  "illegal call to %0, expected %1 argument type">;

// OpenCL v2.2 s2.1.2.3 - Vector Component Access
def ext_opencl_ext_vector_type_rgba_selector: ExtWarn<
  "vector component name '%0' is an OpenCL version 2.2 feature">,
  InGroup<OpenCLUnsupportedRGBA>;

def err_openclcxx_placement_new : Error<
  "use of placement new requires explicit declaration">;

// MIG routine annotations.
def warn_mig_server_routine_does_not_return_kern_return_t : Warning<
  "'mig_server_routine' 属性仅适用于例程返回一个 kern_return_t">,
  InGroup<IgnoredAttributes>;
} // end of sema category

let CategoryName = "OpenMP Issue" in {
// OpenMP support.
def err_omp_expected_var_arg : Error<
  "%0 is not a global variable, static local variable or static data member">;
def err_omp_expected_var_arg_suggest : Error<
  "%0 is not a global variable, static local variable or static data member; "
  "did you mean %1">;
def err_omp_global_var_arg : Error<
  "arguments of '#pragma omp %0' must have %select{global storage|static storage duration}1">;
def err_omp_ref_type_arg : Error<
  "arguments of '#pragma omp %0' cannot be of reference type %1">;
def err_omp_region_not_file_context : Error<
  "directive must be at file or namespace scope">;
def err_omp_var_scope : Error<
  "'#pragma omp %0' must appear in the scope of the %q1 variable declaration">;
def err_omp_var_used : Error<
  "'#pragma omp %0' must precede all references to variable %q1">;
def err_omp_var_thread_local : Error<
  "variable %0 cannot be threadprivate because it is %select{thread-local|a global named register variable}1">;
def err_omp_private_incomplete_type : Error<
  "a private variable with incomplete type %0">;
def err_omp_firstprivate_incomplete_type : Error<
  "a firstprivate variable with incomplete type %0">;
def err_omp_lastprivate_incomplete_type : Error<
  "a lastprivate variable with incomplete type %0">;
def err_omp_reduction_incomplete_type : Error<
  "a reduction list item with incomplete type %0">;
def err_omp_unexpected_clause_value : Error<
  "expected %0 in OpenMP clause '%1'">;
def err_omp_expected_var_name_member_expr : Error<
  "expected variable name%select{| or data member of current class}0">;
def err_omp_expected_var_name_member_expr_or_array_item : Error<
  "expected variable name%select{|, data member of current class}0, array element or array section">;
def err_omp_expected_addressable_lvalue_or_array_item : Error<
  "expected addressable lvalue expression, array element%select{ or array section|, array section or array shaping expression}0%select{| of non 'omp_depend_t' type}1">;
def err_omp_expected_named_var_member_or_array_expression: Error<
  "expected expression containing only member accesses and/or array sections based on named variables">;
def err_omp_bit_fields_forbidden_in_clause : Error<
  "bit fields cannot be used to specify storage in a '%0' clause">;
def err_array_section_does_not_specify_contiguous_storage : Error<
  "array section does not specify contiguous storage">;
def err_array_section_does_not_specify_length : Error<
  "array section does not specify length for outermost dimension">;
def err_omp_union_type_not_allowed : Error<
  "mapping of union members is not allowed">;
def err_omp_expected_access_to_data_field : Error<
  "expected access to data field">;
def err_omp_multiple_array_items_in_map_clause : Error<
  "multiple array elements associated with the same variable are not allowed in map clauses of the same construct">;
def err_omp_duplicate_map_type_modifier : Error<
  "same map type modifier has been specified more than once">;
def err_omp_duplicate_motion_modifier : Error<
  "same motion modifier has been specified more than once">;
def err_omp_pointer_mapped_along_with_derived_section : Error<
  "pointer cannot be mapped along with a section derived from itself">;
def err_omp_original_storage_is_shared_and_does_not_contain : Error<
  "original storage of expression in data environment is shared but data environment do not fully contain mapped expression storage">;
def err_omp_same_pointer_dereferenced : Error<
  "same pointer dereferenced in multiple different ways in map clause expressions">;
def note_omp_task_predetermined_firstprivate_here : Note<
  "predetermined as a firstprivate in a task construct here">;
def err_omp_threadprivate_incomplete_type : Error<
  "threadprivate variable with incomplete type %0">;
def err_omp_no_dsa_for_variable : Error<
  "variable %0 must have explicitly specified data sharing attributes">;
def err_omp_defaultmap_no_attr_for_variable : Error<
  "variable %0 must have explicitly specified data sharing attributes, data mapping attributes, or in an is_device_ptr clause">;
def note_omp_default_dsa_none : Note<
  "explicit data sharing attribute requested here">;
def note_omp_defaultmap_attr_none : Note<
  "explicit data sharing attribute, data mapping attribute, or is_device_ptr clause requested here">;
def err_omp_wrong_dsa : Error<
  "%0 variable cannot be %1">;
def err_omp_variably_modified_type_not_supported : Error<
  "arguments of OpenMP clause '%0' in '#pragma omp %2' directive cannot be of variably-modified type %1">;
def note_omp_explicit_dsa : Note<
  "defined as %0">;
def note_omp_predetermined_dsa : Note<
  "%select{static data member is predetermined as shared|"
  "variable with static storage duration is predetermined as shared|"
  "loop iteration variable is predetermined as private|"
  "loop iteration variable is predetermined as linear|"
  "loop iteration variable is predetermined as lastprivate|"
  "constant variable is predetermined as shared|"
  "global variable is predetermined as shared|"
  "non-shared variable in a task construct is predetermined as firstprivate|"
  "variable with automatic storage duration is predetermined as private}0"
  "%select{|; perhaps you forget to enclose 'omp %2' directive into a parallel or another task region?}1">;
def note_omp_implicit_dsa : Note<
  "implicitly determined as %0">;
def err_omp_loop_var_dsa : Error<
  "loop iteration variable in the associated loop of 'omp %1' directive may not be %0, predetermined as %2">;
def err_omp_not_for : Error<
  "%select{statement after '#pragma omp %1' must be a for loop|"
  "expected %2 for loops after '#pragma omp %1'%select{|, but found only %4}3}0">;
def note_omp_collapse_ordered_expr : Note<
  "as specified in %select{'collapse'|'ordered'|'collapse' and 'ordered'}0 clause%select{||s}0">;
def err_omp_negative_expression_in_clause : Error<
  "argument to '%0' clause must be a %select{non-negative|strictly positive}1 integer value">;
def err_omp_not_integral : Error<
  "expression must have integral or unscoped enumeration "
  "type, not %0">;
def err_omp_threadprivate_in_target : Error<
  "threadprivate variables cannot be used in target constructs">;
def err_omp_incomplete_type : Error<
  "expression has incomplete class type %0">;
def err_omp_explicit_conversion : Error<
  "expression requires explicit conversion from %0 to %1">;
def note_omp_conversion_here : Note<
  "conversion to %select{integral|enumeration}0 type %1 declared here">;
def err_omp_ambiguous_conversion : Error<
  "ambiguous conversion from type %0 to an integral or unscoped "
  "enumeration type">;
def err_omp_iterator_not_integral_or_pointer : Error<
  "expected integral or pointer type as the iterator-type, not %0">;
def err_omp_iterator_step_not_integral : Error<
  "iterator step expression %0 is not the integral expression">;
def err_omp_iterator_step_constant_zero : Error<
  "iterator step expression %0 evaluates to 0">;
def err_omp_required_access : Error<
  "%0 variable must be %1">;
def err_omp_const_variable : Error<
  "const-qualified variable cannot be %0">;
def err_omp_const_not_mutable_variable : Error<
  "const-qualified variable without mutable fields cannot be %0">;
def err_omp_const_list_item : Error<
  "const-qualified list item cannot be %0">;
def err_omp_linear_incomplete_type : Error<
  "a linear variable with incomplete type %0">;
def err_omp_linear_expected_int_or_ptr : Error<
  "argument of a linear clause should be of integral or pointer "
  "type, not %0">;
def warn_omp_linear_step_zero : Warning<
  "零线性步长 (%0 %select{|and other variables in clause }1可能为const)">,
  InGroup<OpenMPClauses>;
def warn_omp_alignment_not_power_of_two : Warning<
  "将忽略aligned子句，因为请求的对齐不是2的幂">,
  InGroup<OpenMPClauses>;
def err_omp_invalid_target_decl : Error<
  "%0 used in declare target directive is not a variable or a function name">;
def err_omp_declare_target_multiple : Error<
  "%0 appears multiple times in clauses on the same declare target directive">;
def err_omp_declare_target_to_and_link : Error<
  "%0 must not appear in both clauses 'to' and 'link'">;
def warn_omp_not_in_target_context : Warning<
  "未在任何声明目标区域中声明声明">,
  InGroup<OpenMPTarget>;
def err_omp_function_in_link_clause : Error<
  "function name is not allowed in 'link' clause">;
def err_omp_aligned_expected_array_or_ptr : Error<
  "argument of aligned clause should be array"
  "%select{ or pointer|, pointer, reference to array or reference to pointer}1"
  ", not %0">;
def err_omp_used_in_clause_twice : Error<
  "%select{a variable|a parameter|'this'}0 cannot appear in more than one %1 clause">;
def err_omp_local_var_in_threadprivate_init : Error<
  "variable with local storage in initial value of threadprivate variable">;
def err_omp_loop_not_canonical_init : Error<
  "initialization clause of OpenMP for loop is not in canonical form "
  "('var = init' or 'T var = init')">;
def ext_omp_loop_not_canonical_init : ExtWarn<
  "initialization clause of OpenMP for loop is not in canonical form "
  "('var = init' or 'T var = init')">, InGroup<OpenMPLoopForm>;
def err_omp_loop_not_canonical_cond : Error<
  "condition of OpenMP for loop must be a relational comparison "
  "('<', '<=', '>', %select{or '>='|'>=', or '!='}0) of loop variable %1">;
def err_omp_loop_not_canonical_incr : Error<
  "increment clause of OpenMP for loop must perform simple addition "
  "or subtraction on loop variable %0">;
def err_omp_loop_variable_type : Error<
  "variable must be of integer or %select{pointer|random access iterator}0 type">;
def err_omp_loop_incr_not_compatible : Error<
  "increment expression must cause %0 to %select{decrease|increase}1 "
  "on each iteration of OpenMP for loop">;
def note_omp_loop_cond_requres_compatible_incr : Note<
  "loop step is expected to be %select{negative|positive}0 due to this condition">;
def err_omp_loop_diff_cxx : Error<
  "could not calculate number of iterations calling 'operator-' with "
  "upper and lower loop bounds">;
def err_omp_loop_cannot_use_stmt : Error<
  "'%0' statement cannot be used in OpenMP for loop">;
def err_omp_simd_region_cannot_use_stmt : Error<
  "'%0' statement cannot be used in OpenMP simd region">;
def warn_omp_loop_64_bit_var : Warning<
  "OpenMP循环迭代变量的大小不能超过64位，将被缩小">,
  InGroup<OpenMPLoopForm>;
def err_omp_unknown_reduction_identifier : Error<
  "incorrect reduction identifier, expected one of '+', '-', '*', '&', '|', '^', "
  "'&&', '||', 'min' or 'max' or declare reduction for type %0">;
def err_omp_not_resolved_reduction_identifier : Error<
  "unable to resolve declare reduction construct for type %0">;
def err_omp_reduction_ref_type_arg : Error<
  "argument of OpenMP clause '%0' must reference the same object in all threads">;
def err_omp_clause_not_arithmetic_type_arg : Error<
  "arguments of OpenMP clause '%0' for 'min' or 'max' must be of %select{scalar|arithmetic}1 type">;
def err_omp_clause_floating_type_arg : Error<
  "arguments of OpenMP clause '%0' with bitwise operators cannot be of floating type">;
def err_omp_once_referenced : Error<
  "variable can appear only once in OpenMP '%0' clause">;
def err_omp_once_referenced_in_target_update : Error<
  "variable can appear only once in OpenMP 'target update' construct">;
def note_omp_referenced : Note<
  "previously referenced here">;
def err_omp_reduction_in_task : Error<
  "reduction variables may not be accessed in an explicit task">;
def err_omp_reduction_id_not_compatible : Error<
  "list item of type %0 is not valid for specified reduction operation: unable to provide default initialization value">;
def err_omp_reduction_identifier_mismatch : Error<
  "in_reduction variable must have the same reduction operation as in a task_reduction clause">;
def note_omp_previous_reduction_identifier : Note<
  "previously marked as task_reduction with different reduction operation">;
def err_omp_prohibited_region : Error<
  "region cannot be%select{| closely}0 nested inside '%1' region"
  "%select{|; perhaps you forget to enclose 'omp %3' directive into a parallel region?|"
  "; perhaps you forget to enclose 'omp %3' directive into a for or a parallel for region with 'ordered' clause?|"
  "; perhaps you forget to enclose 'omp %3' directive into a target region?|"
  "; perhaps you forget to enclose 'omp %3' directive into a teams region?|"
  "; perhaps you forget to enclose 'omp %3' directive into a for, simd, for simd, parallel for, or parallel for simd region?}2">;
def err_omp_prohibited_region_simd : Error<
  "OpenMP constructs may not be nested inside a simd region%select{| except for ordered simd, simd, scan, or atomic directive}0">;
def err_omp_prohibited_region_atomic : Error<
  "OpenMP constructs may not be nested inside an atomic region">;
def err_omp_prohibited_region_critical_same_name : Error<
  "cannot nest 'critical' regions having the same name %0">;
def note_omp_previous_critical_region : Note<
  "previous 'critical' region starts here">;
def err_omp_several_directives_in_region : Error<
  "exactly one '%0' directive must appear in the loop body of an enclosing directive">;
def note_omp_previous_directive : Note<
  "previous '%0' directive used here">;
def err_omp_sections_not_compound_stmt : Error<
  "the statement for '#pragma omp sections' must be a compound statement">;
def err_omp_parallel_sections_not_compound_stmt : Error<
  "the statement for '#pragma omp parallel sections' must be a compound statement">;
def err_omp_orphaned_section_directive : Error<
  "%select{orphaned 'omp section' directives are prohibited, it|'omp section' directive}0"
  " must be closely nested to a sections region%select{|, not a %1 region}0">;
def err_omp_sections_substmt_not_section : Error<
  "statement in 'omp sections' directive must be enclosed into a section region">;
def err_omp_parallel_sections_substmt_not_section : Error<
  "statement in 'omp parallel sections' directive must be enclosed into a section region">;
def err_omp_parallel_reduction_in_task_firstprivate : Error<
  "argument of a reduction clause of a %0 construct must not appear in a firstprivate clause on a task construct">;
def err_omp_atomic_read_not_expression_statement : Error<
  "the statement for 'atomic read' must be an expression statement of form 'v = x;',"
  " where v and x are both lvalue expressions with scalar type">;
def note_omp_atomic_read_write: Note<
  "%select{expected an expression statement|expected built-in assignment operator|expected expression of scalar type|expected lvalue expression}0">;
def err_omp_atomic_write_not_expression_statement : Error<
  "the statement for 'atomic write' must be an expression statement of form 'x = expr;',"
  " where x is a lvalue expression with scalar type">;
def err_omp_atomic_update_not_expression_statement : Error<
  "the statement for 'atomic update' must be an expression statement of form '++x;', '--x;', 'x++;', 'x--;', 'x binop= expr;', 'x = x binop expr' or 'x = expr binop x',"
  " where x is an lvalue expression with scalar type">;
def err_omp_atomic_not_expression_statement : Error<
  "the statement for 'atomic' must be an expression statement of form '++x;', '--x;', 'x++;', 'x--;', 'x binop= expr;', 'x = x binop expr' or 'x = expr binop x',"
  " where x is an lvalue expression with scalar type">;
def note_omp_atomic_update: Note<
  "%select{expected an expression statement|expected built-in binary or unary operator|expected unary decrement/increment operation|"
  "expected expression of scalar type|expected assignment expression|expected built-in binary operator|"
  "expected one of '+', '*', '-', '/', '&', '^', '%|', '<<', or '>>' built-in operations|expected in right hand side of expression}0">;
def err_omp_atomic_capture_not_expression_statement : Error<
  "the statement for 'atomic capture' must be an expression statement of form 'v = ++x;', 'v = --x;', 'v = x++;', 'v = x--;', 'v = x binop= expr;', 'v = x = x binop expr' or 'v = x = expr binop x',"
  " where x and v are both lvalue expressions with scalar type">;
def err_omp_atomic_capture_not_compound_statement : Error<
  "the statement for 'atomic capture' must be a compound statement of form '{v = x; x binop= expr;}', '{x binop= expr; v = x;}',"
  " '{v = x; x = x binop expr;}', '{v = x; x = expr binop x;}', '{x = x binop expr; v = x;}', '{x = expr binop x; v = x;}' or '{v = x; x = expr;}',"
  " '{v = x; x++;}', '{v = x; ++x;}', '{++x; v = x;}', '{x++; v = x;}', '{v = x; x--;}', '{v = x; --x;}', '{--x; v = x;}', '{x--; v = x;}'"
  " where x is an lvalue expression with scalar type">;
def note_omp_atomic_capture: Note<
  "%select{expected assignment expression|expected compound statement|expected exactly two expression statements|expected in right hand side of the first expression}0">;
def err_omp_atomic_several_clauses : Error<
  "directive '#pragma omp atomic' cannot contain more than one 'read', 'write', 'update' or 'capture' clause">;
def err_omp_several_mem_order_clauses : Error<
  "directive '#pragma omp %0' cannot contain more than one %select{'seq_cst', 'relaxed', |}1'acq_rel', 'acquire' or 'release' clause">;
def err_omp_atomic_incompatible_mem_order_clause : Error<
  "directive '#pragma omp atomic%select{ %0|}1' cannot be used with '%2' clause">;
def note_omp_previous_mem_order_clause : Note<
  "'%0' clause used here">;
def err_omp_target_contains_not_only_teams : Error<
  "target construct with nested teams region contains statements outside of the teams construct">;
def note_omp_nested_teams_construct_here : Note<
  "nested teams construct here">;
def note_omp_nested_statement_here : Note<
  "%select{statement|directive}0 outside teams construct here">;
def err_omp_single_copyprivate_with_nowait : Error<
  "the 'copyprivate' clause must not be used with the 'nowait' clause">;
def note_omp_nowait_clause_here : Note<
  "'nowait' clause is here">;
def err_omp_single_decl_in_declare_simd_variant : Error<
  "single declaration is expected after 'declare %select{simd|variant}0' directive">;
def err_omp_function_expected : Error<
  "'#pragma omp declare %select{simd|variant}0' can only be applied to functions">;
def err_omp_wrong_cancel_region : Error<
  "one of 'for', 'parallel', 'sections' or 'taskgroup' is expected">;
def err_omp_parent_cancel_region_nowait : Error<
  "parent region for 'omp %select{cancellation point|cancel}0' construct cannot be nowait">;
def err_omp_parent_cancel_region_ordered : Error<
  "parent region for 'omp %select{cancellation point|cancel}0' construct cannot be ordered">;
def err_omp_reduction_wrong_type : Error<"reduction type cannot be %select{qualified with 'const', 'volatile' or 'restrict'|a function|a reference|an array}0 type">;
def err_omp_wrong_var_in_declare_reduction : Error<"only %select{'omp_priv' or 'omp_orig'|'omp_in' or 'omp_out'}0 variables are allowed in %select{initializer|combiner}0 expression">;
def err_omp_declare_reduction_redefinition : Error<"redefinition of user-defined reduction for type %0">;
def err_omp_mapper_wrong_type : Error<
  "mapper type must be of struct, union or class type">;
def err_omp_declare_mapper_wrong_var : Error<
  "only variable %0 is allowed in map clauses of this 'omp declare mapper' directive">;
def err_omp_declare_mapper_redefinition : Error<
  "redefinition of user-defined mapper for type %0 with name %1">;
def err_omp_invalid_mapper: Error<
  "cannot find a valid user-defined mapper for type %0 with name %1">;
def err_omp_array_section_use : Error<"OpenMP array section is not allowed here">;
def err_omp_array_shaping_use : Error<"OpenMP array shaping operation is not allowed here">;
def err_omp_iterator_use : Error<"OpenMP iterator is not allowed here">;
def err_omp_typecheck_section_value : Error<
  "subscripted value is not an array or pointer">;
def err_omp_typecheck_section_not_integer : Error<
  "array section %select{lower bound|length}0 is not an integer">;
def err_omp_typecheck_shaping_not_integer : Error<
  "array shaping operation dimension is not an integer">;
def err_omp_shaping_dimension_not_positive : Error<
  "array shaping dimension is evaluated to a non-positive value %0">;
def err_omp_section_function_type : Error<
  "section of pointer to function type %0">;
def warn_omp_section_is_char : Warning<"数组节 %select{lower bound|length}0 的类型为 'char'">,
  InGroup<CharSubscript>, DefaultIgnore;
def err_omp_section_incomplete_type : Error<
  "section of pointer to incomplete type %0">;
def err_omp_section_not_subset_of_array : Error<
  "array section must be a subset of the original array">;
def err_omp_section_length_negative : Error<
  "section length is evaluated to a negative value %0">;
def err_omp_section_stride_non_positive : Error<
  "section stride is evaluated to a non-positive value %0">;
def err_omp_section_length_undefined : Error<
  "section length is unspecified and cannot be inferred because subscripted value is %select{not an array|an array of unknown bound}0">;
def err_omp_wrong_linear_modifier : Error<
  "expected %select{'val' modifier|one of 'ref', val' or 'uval' modifiers}0">;
def err_omp_wrong_linear_modifier_non_reference : Error<
  "variable of non-reference type %0 can be used only with 'val' modifier, but used with '%1'">;
def err_omp_wrong_simdlen_safelen_values : Error<
  "the value of 'simdlen' parameter must be less than or equal to the value of the 'safelen' parameter">;
def err_omp_wrong_if_directive_name_modifier : Error<
  "directive name modifier '%0' is not allowed for '#pragma omp %1'">;
def err_omp_no_more_if_clause : Error<
  "no more 'if' clause is allowed">;
def err_omp_unnamed_if_clause : Error<
  "expected%select{| one of}0 %1 directive name modifier%select{|s}0">;
def note_omp_previous_named_if_clause : Note<
  "previous clause with directive name modifier specified here">;
def err_omp_ordered_directive_with_param : Error<
  "'ordered' directive %select{without any clauses|with 'threads' clause}0 cannot be closely nested inside ordered region with specified parameter">;
def err_omp_ordered_directive_without_param : Error<
  "'ordered' directive with 'depend' clause cannot be closely nested inside ordered region without specified parameter">;
def note_omp_ordered_param : Note<
  "'ordered' clause%select{| with specified parameter}0">;
def err_omp_expected_base_var_name : Error<
  "expected variable name as a base of the array %select{subscript|section}0">;
def err_omp_map_shared_storage : Error<
  "variable already marked as mapped in current construct">;
def err_omp_invalid_map_type_for_directive : Error<
  "%select{map type '%1' is not allowed|map type must be specified}0 for '#pragma omp %2'">;
def err_omp_no_clause_for_directive : Error<
  "expected at least one %0 clause for '#pragma omp %1'">;
def err_omp_threadprivate_in_clause : Error<
  "threadprivate variables are not allowed in '%0' clause">;
def err_omp_wrong_ordered_loop_count : Error<
  "the parameter of the 'ordered' clause must be greater than or equal to the parameter of the 'collapse' clause">;
def note_collapse_loop_count : Note<
  "parameter of the 'collapse' clause">;
def err_omp_clauses_mutually_exclusive : Error<
  "'%0' and '%1' clause are mutually exclusive and may not appear on the same directive">;
def note_omp_previous_clause : Note<
  "'%0' clause is specified here">;
def err_omp_hint_clause_no_name : Error<
  "the name of the construct must be specified in presence of 'hint' clause">;
def err_omp_critical_with_hint : Error<
  "constructs with the same name must have a 'hint' clause with the same value">;
def note_omp_critical_hint_here : Note<
  "%select{|previous }0'hint' clause with value '%1'">;
def note_omp_critical_no_hint : Note<
  "%select{|previous }0directive with no 'hint' clause specified">;
def err_omp_depend_clause_thread_simd : Error<
  "'depend' clauses cannot be mixed with '%0' clause">;
def err_omp_depend_sink_expected_loop_iteration : Error<
  "expected%select{| %1}0 loop iteration variable">;
def err_omp_depend_sink_unexpected_expr : Error<
  "unexpected expression: number of expressions is larger than the number of associated loops">;
def err_omp_depend_sink_expected_plus_minus : Error<
  "expected '+' or '-' operation">;
def err_omp_depend_sink_source_not_allowed : Error<
  "'depend(%select{source|sink:vec}0)' clause%select{|s}0 cannot be mixed with 'depend(%select{sink:vec|source}0)' clause%select{s|}0">;
def err_omp_depend_zero_length_array_section_not_allowed : Error<
  "zero-length array section is not allowed in 'depend' clause">;
def err_omp_depend_sink_source_with_modifier : Error<
  "depend modifier cannot be used with 'sink' or 'source' depend type">;
def err_omp_depend_modifier_not_iterator : Error<
  "expected iterator specification as depend modifier">;
def err_omp_linear_ordered : Error<
  "'linear' clause cannot be specified along with 'ordered' clause with a parameter">;
def err_omp_unexpected_schedule_modifier : Error<
  "modifier '%0' cannot be used along with modifier '%1'">;
def err_omp_schedule_nonmonotonic_static : Error<
  "'nonmonotonic' modifier can only be specified with 'dynamic' or 'guided' schedule kind">;
def err_omp_simple_clause_incompatible_with_ordered : Error<
  "'%0' clause with '%1' modifier cannot be specified if an 'ordered' clause is specified">;
def err_omp_ordered_simd : Error<
  "'ordered' clause with a parameter can not be specified in '#pragma omp %0' directive">;
def err_omp_variable_in_given_clause_and_dsa : Error<
  "%0 variable cannot be in a %1 clause in '#pragma omp %2' directive">;
def err_omp_param_or_this_in_clause : Error<
  "expected reference to one of the parameters of function %0%select{| or 'this'}1">;
def err_omp_expected_uniform_param : Error<
  "expected a reference to a parameter specified in a 'uniform' clause">;
def err_omp_expected_int_param : Error<
  "expected a reference to an integer-typed parameter">;
def err_omp_at_least_one_motion_clause_required : Error<
  "expected at least one 'to' clause or 'from' clause specified to '#pragma omp target update'">;
def err_omp_usedeviceptr_not_a_pointer : Error<
  "expected pointer or reference to pointer in 'use_device_ptr' clause">;
def err_omp_argument_type_isdeviceptr : Error <
  "expected pointer, array, reference to pointer, or reference to array in 'is_device_ptr clause'">;
def warn_omp_nesting_simd : Warning<
  "OpenMP仅允许在simd构造中嵌套simd子句的有序构造">,
  InGroup<SourceUsesOpenMP>;
def err_omp_orphaned_device_directive : Error<
  "orphaned 'omp %0' directives are prohibited"
  "; perhaps you forget to enclose the directive into a "
  "%select{|||target |teams|for, simd, for simd, parallel for, or parallel for simd }1region?">;
def err_omp_reduction_non_addressable_expression : Error<
  "expected addressable reduction item for the task-based directives">;
def err_omp_reduction_with_nogroup : Error<
  "'reduction' clause cannot be used with 'nogroup' clause">;
def err_omp_reduction_vla_unsupported : Error<
  "cannot generate code for reduction on %select{|array section, which requires a }0variable length array">;
def err_omp_linear_distribute_var_non_loop_iteration : Error<
  "only loop iteration variables are allowed in 'linear' clause in distribute directives">;
def warn_omp_non_trivial_type_mapped : Warning<
  "类型%0不可复制，也不能保证正确映射">,
  InGroup<OpenMPMapping>;
def err_omp_requires_clause_redeclaration : Error <
  "Only one %0 clause can appear on a requires directive in a single translation unit">;
def note_omp_requires_previous_clause : Note <
  "%0 clause previously used here">;
def err_omp_directive_before_requires : Error <
  "'%0' region encountered before requires directive with '%1' clause">;
def note_omp_requires_encountered_directive : Note <
  "'%0' previously encountered here">;
def err_omp_invalid_scope : Error <
  "'#pragma omp %0' directive must appear only in file scope">;
def note_omp_invalid_length_on_this_ptr_mapping : Note <
  "expected length on mapping of 'this' array section expression to be '1'">;
def note_omp_invalid_lower_bound_on_this_ptr_mapping : Note <
  "expected lower bound on mapping of 'this' array section expression to be '0' or not specified">;
def note_omp_invalid_subscript_on_this_ptr_map : Note <
  "expected 'this' subscript expression on map clause to be 'this[0]'">;
def err_omp_invalid_map_this_expr : Error <
  "invalid 'this' expression on 'map' clause">;
def err_omp_implied_type_not_found : Error<
  "'%0' type not found; include <omp.h>">;
def err_omp_expected_omp_depend_t_lvalue : Error<
  "expected lvalue expression%select{ of 'omp_depend_t' type, not %1|}0">;
def err_omp_depobj_expected : Error<
  "expected depobj expression">;
def err_omp_depobj_single_clause_expected : Error<
  "exactly one of 'depend', 'destroy', or 'update' clauses is expected">;
def err_omp_scan_single_clause_expected : Error<
  "exactly one of 'inclusive' or 'exclusive' clauses is expected">;
def err_omp_inclusive_exclusive_not_reduction : Error<
  "the list item must appear in 'reduction' clause with the 'inscan' modifier "
  "of the parent directive">;
def err_omp_reduction_not_inclusive_exclusive : Error<
  "the inscan reduction list item must appear as a list item in an 'inclusive' or"
  " 'exclusive' clause on an inner 'omp scan' directive">;
def err_omp_wrong_inscan_reduction : Error<
  "'inscan' modifier can be used only in 'omp for', 'omp simd', 'omp for simd',"
  " 'omp parallel for', or 'omp parallel for simd' directive">;
def err_omp_inscan_reduction_expected : Error<
  "expected 'reduction' clause with the 'inscan' modifier">;
def note_omp_previous_inscan_reduction : Note<
  "'reduction' clause with 'inscan' modifier is used here">;
def err_omp_expected_predefined_allocator : Error<
  "expected one of the predefined allocators for the variables with the static "
  "storage: 'omp_default_mem_alloc', 'omp_large_cap_mem_alloc', "
  "'omp_const_mem_alloc', 'omp_high_bw_mem_alloc', 'omp_low_lat_mem_alloc', "
  "'omp_cgroup_mem_alloc', 'omp_pteam_mem_alloc' or 'omp_thread_mem_alloc'">;
def warn_omp_used_different_allocator : Warning<
  "allocate指令指定 %select{default|'%1'}0 分配器，而 "
  "以前使用%select{default|'%3'}2">,
  InGroup<OpenMPClauses>;
def note_omp_previous_allocator : Note<
  "previous allocator is specified here">;
def err_expected_allocator_clause : Error<"expected an 'allocator' clause "
  "inside of the target region; provide an 'allocator' clause or use 'requires'"
  " directive with the 'dynamic_allocators' clause">;
def err_expected_allocator_expression : Error<"expected an allocator expression "
  "inside of the target region; provide an allocator expression or use 'requires'"
  " directive with the 'dynamic_allocators' clause">;
def warn_omp_allocate_thread_on_task_target_directive : Warning<
  "具有'thread'特性访问权限的分配器在'%0'指令上具有未指定的行为">,
  InGroup<OpenMPClauses>;
def err_omp_expected_private_copy_for_allocate : Error<
  "the referenced item is not found in any private clause on the same directive">;
def err_omp_stmt_depends_on_loop_counter : Error<
  "the loop %select{initializer|condition}0 expression depends on the current loop control variable">;
def err_omp_invariant_or_linear_dependency : Error<
  "expected loop invariant expression or '<invariant1> * %0 + <invariant2>' kind of expression">;
def err_omp_wrong_dependency_iterator_type : Error<
  "expected an integer or a pointer type of the outer loop counter '%0' for non-rectangular nests">;
def err_device_unsupported_type : Error <
  "%0 requires %1 bit size %2 type support, but device '%3' does not support it">;
def err_omp_lambda_capture_in_declare_target_not_to : Error<
  "variable captured in declare target region must appear in a to clause">;
def err_omp_device_type_mismatch : Error<
  "'device_type(%0)' does not match previously specified 'device_type(%1)' for the same declaration">;
def err_omp_wrong_device_function_call : Error<
  "function with 'device_type(%0)' is not available on %select{device|host}1">;
def note_omp_marked_device_type_here : Note<"marked as 'device_type(%0)' here">;
def warn_omp_declare_target_after_first_use : Warning<
  "首次使用后标记为declare target的声明，可能会导致不正确的结果">,
  InGroup<OpenMPTarget>;
def err_omp_declare_variant_incompat_attributes : Error<
  "'#pragma omp declare variant' is not compatible with any target-specific attributes">;
def warn_omp_declare_variant_score_not_constant
    : Warning<"OpenMP上下文选择器中的分数表达式需要为常量； "
              "%0不是，将被忽略">,
      InGroup<SourceUsesOpenMP>;
def err_omp_declare_variant_user_condition_not_constant
    : Error<"the user condition in the OpenMP context selector needs to be "
            "constant; %0 is not">;
def warn_omp_declare_variant_after_used : Warning<
  "'#pragma omp declare variant' 首次使用后不能应用于函数； "
  "可以使用原始函数">, InGroup<SourceUsesOpenMP>;
def warn_omp_declare_variant_after_emitted : Warning<
  "'#pragma omp declare variant' 不能应用于已定义的函数；"
  " 可以使用原始函数">, InGroup<SourceUsesOpenMP>;
def err_omp_declare_variant_doesnt_support : Error<
  "'#pragma omp declare variant' does not "
  "support %select{function templates|virtual functions|"
  "deduced return types|constructors|destructors|deleted functions|"
  "defaulted functions|constexpr functions|consteval function}0">;
def err_omp_declare_variant_diff : Error<
  "function with '#pragma omp declare variant' has a different %select{calling convention"
  "|return type|constexpr specification|inline specification|storage class|"
  "linkage}0">;
def err_omp_declare_variant_incompat_types : Error<
  "variant in '#pragma omp declare variant' with type %0 is incompatible with type %1"
  >;
def warn_omp_declare_variant_marked_as_declare_variant : Warning<
  "variant函数在 '#pragma omp declare variant' 中本身被标记为'#pragma omp declare variant'"
  >, InGroup<SourceUsesOpenMP>;
def note_omp_marked_declare_variant_here : Note<"marked as 'declare variant' here">;
def err_omp_one_defaultmap_each_category: Error<
  "at most one defaultmap clause for each variable-category can appear on the directive">;
def err_omp_lastprivate_conditional_non_scalar : Error<
  "expected list item of scalar type in 'lastprivate' clause with 'conditional' modifier"
  >;
def err_omp_flush_order_clause_and_list : Error<
  "'flush' directive with memory order clause '%0' cannot have the list">;
def note_omp_flush_order_clause_here : Note<
  "memory order clause '%0' is specified here">;
def err_omp_non_lvalue_in_map_or_motion_clauses: Error<
  "expected addressable lvalue in '%0' clause">;
def err_omp_var_expected : Error<
  "expected variable of the '%0' type%select{|, not %2}1">;
def warn_unknown_declare_variant_isa_trait
    : Warning<"当前目标不知道isa特性'%0'； "
              "请验证拼写，或考虑进一步使用 "
              "'arch'选择器限制上下文选择器">,
      InGroup<SourceUsesOpenMP>;
def err_omp_non_pointer_type_array_shaping_base : Error<
  "expected expression with a pointer to a complete type as a base of an array "
  "shaping operation">;
def err_omp_reduction_task_not_parallel_or_worksharing : Error<
  "'reduction' clause with 'task' modifier allowed only on non-simd parallel or"
  " worksharing constructs">;
def err_omp_expected_array_alloctraits : Error<
  "expected constant sized array of 'omp_alloctrait_t' elements, not %0">;
def err_omp_predefined_allocator_with_traits : Error<
  "predefined allocator cannot have traits specified">;
def note_omp_predefined_allocator : Note<
  "predefined trait '%0' used here">;
def err_omp_nonpredefined_allocator_without_traits : Error<
  "non-predefined allocator must have traits specified">;
def err_omp_allocator_used_in_clauses : Error<
  "allocators used in 'uses_allocators' clause cannot appear in other "
  "data-sharing or data-mapping attribute clauses">;
def err_omp_allocator_not_in_uses_allocators : Error<
  "allocator must be specified in the 'uses_allocators' clause">;
def note_omp_protected_structured_block
    : Note<"jump bypasses OpenMP structured block">;
def note_omp_exits_structured_block
    : Note<"jump exits scope of OpenMP structured block">;
} // end of OpenMP category

let CategoryName = "Related Result Type Issue" in {
// Objective-C related result type compatibility
def warn_related_result_type_compatibility_class : Warning<
  "方法应返回其类类型的实例 "
  "%diff{$, but is declared to return $|"
  ", but is declared to return different type}0,1">;
def warn_related_result_type_compatibility_protocol : Warning<
  "协议方法应返回实现类的实例， "
  "但声明为返回%0">;
def note_related_result_type_family : Note<
  "%select{overridden|current}0 method is part of the '%select{|alloc|copy|init|"
  "mutableCopy|new|autorelease|dealloc|finalize|release|retain|retainCount|"
  "self}1' method family%select{| and is expected to return an instance of its "
  "class type}0">;
def note_related_result_type_overridden : Note<
  "overridden method returns an instance of its class type">;
def note_related_result_type_inferred : Note<
  "%select{class|instance}0 method %1 is assumed to return an instance of "
  "its receiver type (%2)">;
def note_related_result_type_explicit : Note<
  "%select{overridden|current}0 method is explicitly declared 'instancetype'"
  "%select{| and is expected to return an instance of its class type}0">;
def err_invalid_type_for_program_scope_var : Error<
  "the %0 type cannot be used to declare a program scope variable">;

}

let CategoryName = "Modules Issue" in {
def err_module_decl_in_module_map_module : Error<
  "'module' declaration found while building module from module map">;
def err_module_decl_in_header_module : Error<
  "'module' declaration found while building header unit">;
def err_module_interface_implementation_mismatch : Error<
  "missing 'export' specifier in module declaration while "
  "building module interface">;
def err_current_module_name_mismatch : Error<
  "module name '%0' specified on command line does not match name of module">;
def err_module_redefinition : Error<
  "redefinition of module '%0'">;
def note_prev_module_definition : Note<"previously defined here">;
def note_prev_module_definition_from_ast_file : Note<"module loaded from '%0'">;
def err_module_not_defined : Error<
  "definition of module '%0' is not available; use -fmodule-file= to specify "
  "path to precompiled module interface">;
def err_module_redeclaration : Error<
  "translation unit contains multiple module declarations">;
def note_prev_module_declaration : Note<"previous module declaration is here">;
def err_module_declaration_missing : Error<
  "missing 'export module' declaration in module interface unit">;
def err_module_declaration_missing_after_global_module_introducer : Error<
  "missing 'module' declaration at end of global module fragment "
  "introduced here">;
def err_module_private_specialization : Error<
  "%select{template|partial|member}0 specialization cannot be "
  "declared __module_private__">;
def err_module_private_local : Error<
  "%select{local variable|parameter|typedef}0 %1 cannot be declared "
  "__module_private__">;
def err_module_private_local_class : Error<
  "local %select{struct|interface|union|class|enum}0 cannot be declared "
  "__module_private__">;
def err_module_unimported_use : Error<
  "%select{declaration|definition|default argument|"
  "explicit specialization|partial specialization}0 of %1 must be imported "
  "from module '%2' before it is required">;
def err_module_unimported_use_header : Error<
  "%select{missing '#include'|missing '#include %3'}2; "
  "%select{||default argument of |explicit specialization of |"
  "partial specialization of }0%1 must be "
  "%select{declared|defined|defined|declared|declared}0 "
  "before it is used">;
def err_module_unimported_use_multiple : Error<
  "%select{declaration|definition|default argument|"
  "explicit specialization|partial specialization}0 of %1 must be imported "
  "from one of the following modules before it is required:%2">;
def note_unreachable_entity : Note<
  "%select{declaration|definition|default argument declared|"
  "explicit specialization declared|partial specialization declared}0 here "
  "is not %select{visible|reachable|reachable|reachable|reachable|reachable}0">;
def ext_module_import_in_extern_c : ExtWarn<
  "import of C++ module '%0' appears within extern \"C\" language linkage "
  "specification">, DefaultError,
  InGroup<DiagGroup<"module-import-in-extern-c">>;
def err_module_import_not_at_top_level_fatal : Error<
  "import of module '%0' appears within %1">, DefaultFatal;
def ext_module_import_not_at_top_level_noop : ExtWarn<
  "redundant #include of module '%0' appears within %1">, DefaultError,
  InGroup<DiagGroup<"modules-import-nested-redundant">>;
def note_module_import_not_at_top_level : Note<"%0 begins here">;
def err_module_self_import : Error<
  "import of module '%0' appears within same top-level module '%1'">;
def err_module_import_in_implementation : Error<
  "@import of module '%0' in implementation of '%1'; use #import">;

// C++ Modules
def err_module_decl_not_at_start : Error<
  "module declaration must occur at the start of the translation unit">;
def note_global_module_introducer_missing : Note<
  "add 'module;' to the start of the file to introduce a "
  "global module fragment">;
def err_export_within_anonymous_namespace : Error<
  "export declaration appears within anonymous namespace">;
def note_anonymous_namespace : Note<"anonymous namespace begins here">;
def ext_export_no_name_block : ExtWarn<
  "ISO C++20 does not permit %select{an empty|a static_assert}0 declaration "
  "to appear in an export block">, InGroup<ExportUnnamed>;
def ext_export_no_names : ExtWarn<
  "ISO C++20 does not permit a declaration that does not introduce any names "
  "to be exported">, InGroup<ExportUnnamed>;
def note_export : Note<"export block begins here">;
def err_export_no_name : Error<
  "%select{empty|static_assert|asm}0 declaration cannot be exported">;
def ext_export_using_directive : ExtWarn<
  "ISO C++20 does not permit using directive to be exported">,
  InGroup<DiagGroup<"export-using-directive">>;
def err_export_within_export : Error<
  "export declaration appears within another export declaration">;
def err_export_internal : Error<
  "declaration of %0 with internal linkage cannot be exported">;
def err_export_using_internal : Error<
  "using declaration referring to %0 with internal linkage cannot be exported">;
def err_export_not_in_module_interface : Error<
  "export declaration can only be used within a module interface unit"
  "%select{ after the module declaration|}0">;
def err_export_in_private_module_fragment : Error<
  "export declaration cannot be used in a private module fragment">;
def note_private_module_fragment : Note<
  "private module fragment begins here">;
def err_private_module_fragment_not_module : Error<
  "private module fragment declaration with no preceding module declaration">;
def err_private_module_fragment_redefined : Error<
  "private module fragment redefined">;
def err_private_module_fragment_not_module_interface : Error<
  "private module fragment in module implementation unit">;
def note_not_module_interface_add_export : Note<
  "add 'export' here if this is intended to be a module interface unit">;

def ext_equivalent_internal_linkage_decl_in_modules : ExtWarn<
  "ambiguous use of internal linkage declaration %0 defined in multiple modules">,
  InGroup<DiagGroup<"modules-ambiguous-internal-linkage">>;
def note_equivalent_internal_linkage_decl : Note<
  "declared here%select{ in module '%1'|}0">;

def note_redefinition_modules_same_file : Note<
  "'%0' included multiple times, additional include site in header from module '%1'">;
def note_redefinition_include_same_file : Note<
  "'%0' included multiple times, additional include site here">;
}

let CategoryName = "Coroutines Issue" in {
def err_return_in_coroutine : Error<
  "return statement not allowed in coroutine; did you mean 'co_return'?">;
def note_declared_coroutine_here : Note<
  "function is a coroutine due to use of '%0' here">;
def err_coroutine_objc_method : Error<
  "Objective-C methods as coroutines are not yet supported">;
def err_coroutine_unevaluated_context : Error<
  "'%0' cannot be used in an unevaluated context">;
def err_coroutine_within_handler : Error<
  "'%0' cannot be used in the handler of a try block">;
def err_coroutine_outside_function : Error<
  "'%0' cannot be used outside a function">;
def err_coroutine_invalid_func_context : Error<
  "'%1' cannot be used in %select{a constructor|a destructor"
  "|the 'main' function|a constexpr function"
  "|a function with a deduced return type|a varargs function"
  "|a consteval function}0">;
def err_implied_coroutine_type_not_found : Error<
  "%0 type was not found; include <experimental/coroutine> before defining "
  "a coroutine">;
def err_implicit_coroutine_std_nothrow_type_not_found : Error<
  "std::nothrow was not found; include <new> before defining a coroutine which "
  "uses get_return_object_on_allocation_failure()">;
def err_malformed_std_nothrow : Error<
  "std::nothrow must be a valid variable declaration">;
def err_malformed_std_coroutine_handle : Error<
  "std::experimental::coroutine_handle must be a class template">;
def err_coroutine_handle_missing_member : Error<
  "std::experimental::coroutine_handle missing a member named '%0'">;
def err_malformed_std_coroutine_traits : Error<
  "'std::experimental::coroutine_traits' must be a class template">;
def err_implied_std_coroutine_traits_promise_type_not_found : Error<
  "this function cannot be a coroutine: %q0 has no member named 'promise_type'">;
def err_implied_std_coroutine_traits_promise_type_not_class : Error<
  "this function cannot be a coroutine: %0 is not a class">;
def err_coroutine_promise_type_incomplete : Error<
  "this function cannot be a coroutine: %0 is an incomplete type">;
def err_coroutine_type_missing_specialization : Error<
  "this function cannot be a coroutine: missing definition of "
  "specialization %0">;
def err_coroutine_promise_incompatible_return_functions : Error<
  "the coroutine promise type %0 declares both 'return_value' and 'return_void'">;
def err_coroutine_promise_requires_return_function : Error<
  "the coroutine promise type %0 must declare either 'return_value' or 'return_void'">;
def note_coroutine_promise_implicit_await_transform_required_here : Note<
  "call to 'await_transform' implicitly required by 'co_await' here">;
def note_coroutine_promise_suspend_implicitly_required : Note<
  "call to '%select{initial_suspend|final_suspend}0' implicitly "
  "required by the %select{initial suspend point|final suspend point}0">;
def err_coroutine_promise_unhandled_exception_required : Error<
  "%0 is required to declare the member 'unhandled_exception()'">;
def warn_coroutine_promise_unhandled_exception_required_with_exceptions : Warning<
  "%0 被需要来声明成员 'unhandled_exception()' 当启用异常时">,
  InGroup<CoroutineMissingUnhandledException>;
def err_coroutine_promise_get_return_object_on_allocation_failure : Error<
  "%0: 'get_return_object_on_allocation_failure()' must be a static member function">;
def err_seh_in_a_coroutine_with_cxx_exceptions : Error<
  "cannot use SEH '__try' in a coroutine when C++ exceptions are enabled">;
def err_coroutine_promise_new_requires_nothrow : Error<
  "%0 is required to have a non-throwing noexcept specification when the promise "
   "type declares 'get_return_object_on_allocation_failure()'">;
def note_coroutine_promise_call_implicitly_required : Note<
  "call to %0 implicitly required by coroutine function here">;
def err_await_suspend_invalid_return_type : Error<
  "return type of 'await_suspend' is required to be 'void' or 'bool' (have %0)"
>;
def note_await_ready_no_bool_conversion : Note<
  "return type of 'await_ready' is required to be contextually convertible to 'bool'"
>;
def warn_coroutine_handle_address_invalid_return_type : Warning <
  "返回类型 'coroutine_handle<>::address 应该为 'void*' (have %0) 以便使用现有的异步C API获取功能。">,
  InGroup<Coroutine>;
def err_coroutine_promise_final_suspend_requires_nothrow : Error<
  "the expression 'co_await __promise.final_suspend()' is required to be non-throwing"
>;
def note_coroutine_function_declare_noexcept : Note<
  "must be declared with 'noexcept'"
>;
} // end of coroutines issue category

let CategoryName = "Documentation Issue" in {
def warn_not_a_doxygen_trailing_member_comment : Warning<
  "不是Doxygen尾随注释">, InGroup<Documentation>, DefaultIgnore;
} // end of documentation issue category

let CategoryName = "Nullability Issue" in {

def warn_mismatched_nullability_attr : Warning<
  "nullability说明符%0与现有说明符%1冲突">,
  InGroup<Nullability>;

def warn_nullability_declspec : Warning<
  "nullability说明符%0不能应用于非指针类型%1； "
  "您的意思是将说明符应用于 "
  "%select{pointer|block pointer|member pointer|function pointer|"
  "member function pointer}2?">,
  InGroup<NullabilityDeclSpec>,
  DefaultError;

def note_nullability_here : Note<"%0 specified here">;

def err_nullability_nonpointer : Error<
  "nullability specifier %0 cannot be applied to non-pointer type %1">;

def warn_nullability_lost : Warning<
  "从可为Null的指针%0隐式转换为不可为Null的 "
  "指针类型%1">,
  InGroup<NullableToNonNullConversion>, DefaultIgnore;
def warn_zero_as_null_pointer_constant : Warning<
  "零作为空指针常量">,
  InGroup<DiagGroup<"zero-as-null-pointer-constant">>, DefaultIgnore;

def err_nullability_cs_multilevel : Error<
  "nullability keyword %0 cannot be applied to multi-level pointer type %1">;
def note_nullability_type_specifier : Note<
  "use nullability type specifier %0 to affect the innermost "
  "pointer type of %1">;

def warn_null_resettable_setter : Warning<
  "合成 setter %0 属于null_resettable 属性 %1 不处理nil">,
  InGroup<Nullability>;

def warn_nullability_missing : Warning<
  "%select{pointer|block pointer|member pointer}0缺少nullability "
  "类型说明符 (_Nonnull, _Nullable, or _Null_unspecified)">,
  InGroup<NullabilityCompleteness>;
def warn_nullability_missing_array : Warning<
  "数组参数缺少nullability类型说明符 (_Nonnull, "
  "_Nullable, or _Null_unspecified)">,
  InGroup<NullabilityCompletenessOnArrays>;
def note_nullability_fix_it : Note<
  "insert '%select{_Nonnull|_Nullable|_Null_unspecified}0' if the "
  "%select{pointer|block pointer|member pointer|array parameter}1 "
  "%select{should never be null|may be null|should not declare nullability}0">;

def warn_nullability_inferred_on_nested_type : Warning<
  "推断 '_Nonnull' 为指针类型在 %select{array|reference}0内是 "
  "不推荐的">,
  InGroup<NullabilityInferredOnNestedType>;

def err_objc_type_arg_explicit_nullability : Error<
  "type argument %0 cannot explicitly specify nullability">;

def err_objc_type_param_bound_explicit_nullability : Error<
  "type parameter %0 bound %1 cannot explicitly specify nullability">;

}

let CategoryName = "Generics Issue" in {

def err_objc_type_param_bound_nonobject : Error<
  "type bound %0 for type parameter %1 is not an Objective-C pointer type">;

def err_objc_type_param_bound_missing_pointer : Error<
  "missing '*' in type bound %0 for type parameter %1">;
def err_objc_type_param_bound_qualified : Error<
  "type bound %1 for type parameter %0 cannot be qualified with '%2'">;

def err_objc_type_param_redecl : Error<
  "redeclaration of type parameter %0">;

def err_objc_type_param_arity_mismatch : Error<
  "%select{forward class declaration|class definition|category|extension}0 has "
  "too %select{few|many}1 type parameters (expected %2, have %3)">;

def err_objc_type_param_bound_conflict : Error<
  "type bound %0 for type parameter %1 conflicts with "
  "%select{implicit|previous}2 bound %3%select{for type parameter %5|}4">;

def err_objc_type_param_variance_conflict : Error<
  "%select{in|co|contra}0variant type parameter %1 conflicts with previous "
  "%select{in|co|contra}2variant type parameter %3">;

def note_objc_type_param_here : Note<"type parameter %0 declared here">;

def err_objc_type_param_bound_missing : Error<
  "missing type bound %0 for type parameter %1 in %select{@interface|@class}2">;

def err_objc_parameterized_category_nonclass : Error<
  "%select{extension|category}0 of non-parameterized class %1 cannot have type "
  "parameters">;

def err_objc_parameterized_forward_class : Error<
  "forward declaration of non-parameterized class %0 cannot have type "
  "parameters">;

def err_objc_parameterized_forward_class_first : Error<
  "class %0 previously declared with type parameters">;

def err_objc_type_arg_missing_star : Error<
  "type argument %0 must be a pointer (requires a '*')">;
def err_objc_type_arg_qualified : Error<
  "type argument %0 cannot be qualified with '%1'">;

def err_objc_type_arg_missing : Error<
  "no type or protocol named %0">;

def err_objc_type_args_and_protocols : Error<
  "angle brackets contain both a %select{type|protocol}0 (%1) and a "
  "%select{protocol|type}0 (%2)">;

def err_objc_type_args_non_class : Error<
  "type arguments cannot be applied to non-class type %0">;

def err_objc_type_args_non_parameterized_class : Error<
  "type arguments cannot be applied to non-parameterized class %0">;

def err_objc_type_args_specialized_class : Error<
  "type arguments cannot be applied to already-specialized class type %0">;

def err_objc_type_args_wrong_arity : Error<
  "too %select{many|few}0 type arguments for class %1 (have %2, expected %3)">;
}

def err_objc_type_arg_not_id_compatible : Error<
  "type argument %0 is neither an Objective-C object nor a block type">;

def err_objc_type_arg_does_not_match_bound : Error<
  "type argument %0 does not satisfy the bound (%1) of type parameter %2">;

def warn_objc_redundant_qualified_class_type : Warning<
  "参数化类%0已符合列出的协议；你忘了 "
  "一个'*'吗?">, InGroup<ObjCProtocolQualifiers>;

def warn_block_literal_attributes_on_omitted_return_type : Warning<
  "已忽略属性%0，因为它无法应用于省略的返回类型">,
  InGroup<IgnoredAttributes>;

def warn_block_literal_qualifiers_on_omitted_return_type : Warning<
  "'%0'限定符在省略的返回类型%1上无效">,
  InGroup<IgnoredQualifiers>;

def warn_shadow_field : Warning<
  "%select{parameter|non-static data member}3 %0 %select{|of %1 }3隐藏 "
  "成员继承自类型 %2">, InGroup<ShadowField>, DefaultIgnore;
def note_shadow_field : Note<"declared here">;

def err_multiversion_required_in_redecl : Error<
  "function declaration is missing %select{'target'|'cpu_specific' or "
  "'cpu_dispatch'}0 attribute in a multiversioned function">;
def note_multiversioning_caused_here : Note<
  "function multiversioning caused by this declaration">;
def err_multiversion_after_used : Error<
  "function declaration cannot become a multiversioned function after first "
  "usage">;
def err_bad_multiversion_option : Error<
  "function multiversioning doesn't support %select{feature|architecture}0 "
  "'%1'">;
def err_multiversion_duplicate : Error<
  "multiversioned function redeclarations require identical target attributes">;
def err_multiversion_noproto : Error<
  "multiversioned function must have a prototype">;
def err_multiversion_disallowed_other_attr : Error<
  "attribute '%select{target|cpu_specific|cpu_dispatch}0' multiversioning cannot be combined"
  " with attribute %1">;
def err_multiversion_mismatched_attrs
    : Error<"attributes on multiversioned functions must all match, attribute "
            "%0 %select{is missing|has different arguments}1">;
def err_multiversion_diff : Error<
  "multiversioned function declaration has a different %select{calling convention"
  "|return type|constexpr specification|inline specification|storage class|"
  "linkage}0">;
def err_multiversion_doesnt_support : Error<
  "attribute '%select{target|cpu_specific|cpu_dispatch}0' multiversioned functions do not "
  "yet support %select{function templates|virtual functions|"
  "deduced return types|constructors|destructors|deleted functions|"
  "defaulted functions|constexpr functions|consteval function}1">;
def err_multiversion_not_allowed_on_main : Error<
  "'main' cannot be a multiversioned function">;
def err_multiversion_not_supported : Error<
 "function multiversioning is not supported on the current target">;
def err_multiversion_types_mixed : Error<
  "multiversioning attributes cannot be combined">;
def err_cpu_dispatch_mismatch : Error<
 "'cpu_dispatch' function redeclared with different CPUs">;
def err_cpu_specific_multiple_defs : Error<
 "multiple 'cpu_specific' functions cannot specify the same CPU: %0">;
def warn_multiversion_duplicate_entries : Warning<
 "CPU列表包含重复条目；已忽略属性">,
  InGroup<FunctionMultiVersioning>;
def warn_dispatch_body_ignored : Warning<
  "cpu_dispatch函数的主体将被忽略">,
  InGroup<FunctionMultiVersioning>;

// three-way comparison operator diagnostics
def err_implied_comparison_category_type_not_found : Error<
  "cannot %select{use builtin operator '<=>'|default 'operator<=>'}1 "
  "because type '%0' was not found; include <compare>">;
def err_spaceship_argument_narrowing : Error<
  "argument to 'operator<=>' "
  "%select{cannot be narrowed from type %1 to %2|"
  "evaluates to %1, which cannot be narrowed to type %2}0">;
def err_std_compare_type_not_supported : Error<
  "standard library implementation of %0 is not supported; "
   "%select{member '%2' does not have expected form|"
   "member '%2' is missing|"
   "the type is not trivially copyable|"
   "the type does not have the expected form}1">;
def note_rewriting_operator_as_spaceship : Note<
  "while rewriting comparison as call to 'operator<=>' declared here">;
def err_three_way_vector_comparison : Error<
  "three-way comparison between vectors is not supported">;

// Memory Tagging Extensions (MTE) diagnostics
def err_memtag_arg_null_or_pointer : Error<
  "%0 argument of MTE builtin function must be a null or a pointer (%1 invalid)">;
def err_memtag_any2arg_pointer : Error<
  "at least one argument of MTE builtin function must be a pointer (%0, %1 invalid)">;
def err_memtag_arg_must_be_pointer : Error<
  "%0 argument of MTE builtin function must be a pointer (%1 invalid)">;
def err_memtag_arg_must_be_integer : Error<
  "%0 argument of MTE builtin function must be an integer type (%1 invalid)">;

def warn_dereference_of_noderef_type : Warning<
  "正在取消引用%0；已用'noderef'类型声明“">, InGroup<NoDeref>;
def warn_dereference_of_noderef_type_no_decl : Warning<
  "取消引用标记为'noderef'的表达式">, InGroup<NoDeref>;
def warn_noderef_on_non_pointer_or_array : Warning<
  "'noderef'只能用于数组或指针类型">, InGroup<IgnoredAttributes>;
def warn_noderef_to_dereferenceable_pointer : Warning<
  "转换为可取消引用的指针将删除'noderef'属性">, InGroup<NoDeref>;

def err_builtin_launder_invalid_arg : Error<
  "%select{non-pointer|function pointer|void pointer}0 argument to "
  "'__builtin_launder' is not allowed">;

def err_builtin_matrix_disabled: Error<
  "matrix types extension is disabled. Pass -fenable-matrix to enable it">;
def err_matrix_index_not_integer: Error<
  "matrix %select{row|column}0 index is not an integer">;
def err_matrix_index_outside_range: Error<
  "matrix %select{row|column}0 index is outside the allowed range [0, %1)">;
def err_matrix_incomplete_index: Error<
  "single subscript expressions are not allowed for matrix values">;
def err_matrix_separate_incomplete_index: Error<
  "matrix row and column subscripts cannot be separated by any expression">;
def err_matrix_subscript_comma: Error<
  "comma expressions are not allowed as indices in matrix subscript expressions">;
def err_builtin_matrix_arg: Error<"1st argument must be a matrix">;
def err_builtin_matrix_scalar_unsigned_arg: Error<
  "%0 argument must be a constant unsigned integer expression">;
def err_builtin_matrix_pointer_arg: Error<
  "%ordinal0 argument must be a pointer to a valid matrix element type">;
def err_builtin_matrix_pointer_arg_mismatch: Error<
  "the pointee of the 2nd argument must match the element type of the 1st argument (%0 != %1)">;
def err_builtin_matrix_store_to_const: Error<
  "cannot store matrix to read-only pointer">;
def err_builtin_matrix_stride_too_small: Error<
  "stride must be greater or equal to the number of rows">;
def err_builtin_matrix_invalid_dimension: Error<
  "%0 dimension is outside the allowed range [1, %1]">;

def warn_mismatched_import : Warning<
  "导入 %select{module|name}0 (%1) 与上一个声明的导入 %select{module|name}0 (%2)  "
  "不匹配">,
  InGroup<IgnoredAttributes>;
def warn_import_on_definition : Warning<
  "导入 %select{module|name}0 不能应用于具有定义的函数">,
  InGroup<IgnoredAttributes>;

def err_preserve_field_info_not_field : Error<
  "__builtin_preserve_field_info argument %0 not a field access">;
def err_preserve_field_info_not_const: Error<
  "__builtin_preserve_field_info argument %0 not a constant">;
def err_btf_type_id_not_const: Error<
  "__builtin_btf_type_id argument %0 not a constant">;
def err_preserve_type_info_invalid : Error<
  "__builtin_preserve_type_info argument %0 invalid">;
def err_preserve_type_info_not_const: Error<
  "__builtin_preserve_type_info argument %0 not a constant">;
def err_preserve_enum_value_invalid : Error<
  "__builtin_preserve_enum_value argument %0 invalid">;
def err_preserve_enum_value_not_const: Error<
  "__builtin_preserve_enum_value argument %0 not a constant">;

def err_bit_cast_non_trivially_copyable : Error<
  "__builtin_bit_cast %select{source|destination}0 type must be trivially copyable">;
def err_bit_cast_type_size_mismatch : Error<
  "__builtin_bit_cast source size does not equal destination size (%0 vs %1)">;

// SYCL-specific diagnostics
def warn_sycl_kernel_num_of_template_params : Warning<
  "'sycl_kernel' 属性仅适用于具有至少两个模板参数的"
  " 函数模板">, InGroup<IgnoredAttributes>;
def warn_sycl_kernel_invalid_template_param_type : Warning<
  "具有'sycl_kernel' 属性的函数模板的模板参数不能是"
  "非类型模板参数">, InGroup<IgnoredAttributes>;
def warn_sycl_kernel_num_of_function_params : Warning<
  "具有 'sycl_kernel' 属性的函数模板必须具有单个参数">,
  InGroup<IgnoredAttributes>;
def warn_sycl_kernel_return_type : Warning<
  "具有 'sycl_kernel' 属性的函数模板必须具有'void'返回类型">,
  InGroup<IgnoredAttributes>;

def err_ext_int_bad_size : Error<"%select{signed|unsigned}0 _ExtInt must "
                                 "have a bit size of at least %select{2|1}0">;
def err_ext_int_max_size : Error<"%select{signed|unsigned}0 _ExtInt of bit "
                                 "sizes greater than %1 not supported">;

// errors of expect.with.probability
def err_probability_not_constant_float : Error<
   "probability argument to __builtin_expect_with_probability must be constant "
   "floating-point expression">;
def err_probability_out_of_range : Error<
   "probability argument to __builtin_expect_with_probability is outside the "
   "range [0.0, 1.0]">;

// TCB warnings
def err_tcb_conflicting_attributes : Error<
  "attributes '%0(\"%2\")' and '%1(\"%2\")' are mutually exclusive">;
def warn_tcb_enforcement_violation : Warning<
  "调用%0违反了可信计算基础'%1'">,
  InGroup<DiagGroup<"tcb-enforcement">>;
} // end of sema component.
