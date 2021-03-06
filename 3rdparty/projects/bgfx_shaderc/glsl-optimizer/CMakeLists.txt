cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds


project(glsl-optimizer)
set(
source_list
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast_array_index.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast_expr.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast_function.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast_to_hir.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ast_type.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_functions.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_type_macros.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_types.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/builtin_variables.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp-lex.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp-parse.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp-parse.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/glcpp.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glcpp/pp.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_lexer.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_optimizer.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_optimizer.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_parser.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_parser.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_parser_extras.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_parser_extras.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_symbol_table.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_symbol_table.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_types.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/glsl_types.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/hir_field_selection.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_basic_block.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_basic_block.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_builder.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_builder.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_clone.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_constant_expression.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_equals.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_expression_flattening.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_expression_flattening.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_function.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_function_can_inline.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_function_detect_recursion.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_function_inlining.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_hierarchical_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_hierarchical_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_hv_accept.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_import_prototypes.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_optimization.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_glsl_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_glsl_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_metal_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_metal_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_print_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_rvalue_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_rvalue_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_stats.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_stats.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_uniform.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_unused_structs.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_unused_structs.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_validate.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_variable_refcount.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_variable_refcount.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/ir_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_atomics.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_functions.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_interface_blocks.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_uniform_block_active_visitor.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_uniform_block_active_visitor.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_uniform_blocks.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_uniform_initializers.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_uniforms.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_varyings.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/link_varyings.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/linker.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/linker.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/list.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/loop_analysis.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/loop_analysis.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/loop_controls.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/loop_unroll.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_clip_distance.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_discard.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_discard_flow.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_if_to_cond_assign.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_instructions.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_jumps.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_mat_op_to_vec.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_named_interface_blocks.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_noise.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_offset_array.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_output_reads.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_packed_varyings.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_packing_builtins.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_ubo_reference.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_variable_index_to_cond_assign.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_vec_index_to_cond_assign.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_vec_index_to_swizzle.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_vector.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_vector_insert.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/lower_vertex_id.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_algebraic.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_array_splitting.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_constant_folding.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_constant_propagation.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_constant_variable.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_copy_propagation.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_copy_propagation_elements.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_cse.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_dead_builtin_variables.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_dead_builtin_varyings.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_dead_code.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_dead_code_local.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_dead_functions.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_flatten_nested_if_blocks.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_flip_matrices.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_function_inlining.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_if_simplification.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_minmax.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_noop_swizzle.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_rebalance_tree.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_redundant_jumps.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_structure_splitting.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_swizzle_swizzle.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_tree_grafting.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/opt_vectorize.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/program.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/s_expression.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/s_expression.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/standalone_scaffolding.cpp
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/standalone_scaffolding.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/strtod.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/glsl/strtod.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/compiler.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/config.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/context.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/core.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/dd.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/errors.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/glheader.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/glminimal.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/imports.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/imports.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/macros.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/mtypes.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/main/simple_list.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/hash_table.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/prog_hash_table.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/prog_instruction.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/prog_parameter.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/prog_statevars.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/symbol_table.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/mesa/program/symbol_table.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/util/hash_table.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/util/hash_table.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/util/macros.h
  ../../../bgfx/3rdparty/glsl-optimizer/src/util/ralloc.c
  ../../../bgfx/3rdparty/glsl-optimizer/src/util/ralloc.h
)

include_directories(../../../bgfx/3rdparty/glsl-optimizer/src)
include_directories(../../../bgfx/3rdparty/glsl-optimizer/include)
include_directories(../../../bgfx/3rdparty/glsl-optimizer/src/mesa)
include_directories(../../../bgfx/3rdparty/glsl-optimizer/src/mapi)
include_directories(../../../bgfx/3rdparty/glsl-optimizer/src/glsl)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#  add_definitions(-D_DEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64")
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O2 -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O2 -m64")
#endif()


add_library(glsl-optimizer STATIC ${source_list})

