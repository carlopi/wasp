//
// Copyright 2018 WebAssembly Community Group participants
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "wasp/binary/formatters.h"

#include <cassert>

#include "wasp/base/formatters.h"
#include "wasp/base/macros.h"

namespace wasp {

WASP_DEFINE_VARIANT_NAME(binary::RefType, "ref_type")
WASP_DEFINE_VARIANT_NAME(binary::ReferenceType, "reference_type")
WASP_DEFINE_VARIANT_NAME(binary::ValueType, "value_type")

}  // namespace wasp

namespace fmt {

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::HeapType>::format(
    const ::wasp::binary::HeapType& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.is_heap_kind()) {
    format_to(buf, "{}", self.heap_kind());
  } else {
    assert(self.is_index());
    format_to(buf, "{}", self.index());
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::RefType>::format(
    const ::wasp::binary::RefType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "ref ");
  if (self.null == ::wasp::Null::Yes) {
    format_to(buf, "null ");
  }
  format_to(buf, "{}", self.heap_type);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ReferenceType>::format(
    const ::wasp::binary::ReferenceType& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.is_reference_kind()) {
    format_to(buf, "{}", self.reference_kind());
  } else {
    assert(self.is_ref());
    format_to(buf, "{}", self.ref());
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ValueType>::format(
    const ::wasp::binary::ValueType& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.is_numeric_type()) {
    format_to(buf, "{}", self.numeric_type());
  } else {
    assert(self.is_reference_type());
    format_to(buf, "{}", self.reference_type());
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::VoidType>::format(
    const ::wasp::binary::VoidType& self,
    Ctx& ctx) {
  return formatter<string_view>::format("void", ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::BlockType>::format(
    const ::wasp::binary::BlockType& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.is_value_type()) {
    format_to(buf, "[{}]", self.value_type());
  } else if (self.is_void()) {
    format_to(buf, "[]");
  } else {
    assert(self.is_index());
    format_to(buf, "type[{}]", self.index());
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}


template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::SectionId>::format(
    const ::wasp::binary::SectionId& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str, ...)     \
  case ::wasp::binary::SectionId::Name: \
    result = str;                       \
    break;
#define WASP_FEATURE_V(...) WASP_V(__VA_ARGS__)
#include "wasp/binary/def/section_id.def"
#undef WASP_V
#undef WASP_FEATURE_V
    default: {
      // Special case for sections with unknown ids.
      memory_buffer buf;
      format_to(buf, "{}", static_cast<::wasp::u32>(self));
      return formatter<string_view>::format(to_string_view(buf), ctx);
    }
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::LetImmediate>::format(
    const ::wasp::binary::LetImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}, locals {}}}", self.block_type, self.locals);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::MemArgImmediate>::format(
    const ::wasp::binary::MemArgImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{align {}, offset {}}}", self.align_log2, self.offset);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Locals>::format(
    const ::wasp::binary::Locals& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} ** {}", self.type, self.count);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Section>::format(
    const ::wasp::binary::Section& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.is_known()) {
    format_to(buf, "{}", self.known());
  } else if (self.is_custom()) {
    format_to(buf, "{}", self.custom());
  } else {
    WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::KnownSection>::format(
    const ::wasp::binary::KnownSection& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{id {}, contents {}}}", self.id, self.data);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::CustomSection>::format(
    const ::wasp::binary::CustomSection& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{name \"{}\", contents {}}}", self.name, self.data);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::TypeEntry>::format(
    const ::wasp::binary::TypeEntry& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{}", self.type);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::FunctionType>::format(
    const ::wasp::binary::FunctionType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} -> {}", self.param_types, self.result_types);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::TableType>::format(
    const ::wasp::binary::TableType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.limits, self.elemtype);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::GlobalType>::format(
    const ::wasp::binary::GlobalType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.mut, self.valtype);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::EventType>::format(
    const ::wasp::binary::EventType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.attribute, self.type_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Import>::format(
    const ::wasp::binary::Import& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{module \"{}\", name \"{}\", desc {}", self.module,
            self.name, self.kind());

  switch (self.kind()) {
    case ::wasp::ExternalKind::Function:
      format_to(buf, " {}}}", self.index());
      break;

    case ::wasp::ExternalKind::Table:
      format_to(buf, " {}}}", self.table_type());
      break;

    case ::wasp::ExternalKind::Memory:
      format_to(buf, " {}}}", self.memory_type());
      break;

    case ::wasp::ExternalKind::Global:
      format_to(buf, " {}}}", self.global_type());
      break;

    case ::wasp::ExternalKind::Event:
      format_to(buf, " {}}}", self.event_type());
      break;

    default:
      break;
  }

  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Export>::format(
    const ::wasp::binary::Export& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{name \"{}\", desc {} {}}}", self.name, self.kind,
            self.index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Expression>::format(
    const ::wasp::binary::Expression& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{}", self.data);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ConstantExpression>::format(
    const ::wasp::binary::ConstantExpression& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} end", self.instructions);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ElementExpression>::format(
    const ::wasp::binary::ElementExpression& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} end", self.instructions);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::CallIndirectImmediate>::format(
    const ::wasp::binary::CallIndirectImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.index, self.table_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::BrTableImmediate>::format(
    const ::wasp::binary::BrTableImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.targets, self.default_target);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::BrOnExnImmediate>::format(
    const ::wasp::binary::BrOnExnImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.target, self.event_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::InitImmediate>::format(
    const ::wasp::binary::InitImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.segment_index, self.dst_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::CopyImmediate>::format(
    const ::wasp::binary::CopyImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.dst_index, self.src_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Instruction>::format(
    const ::wasp::binary::Instruction& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{}", self.opcode);

  switch (self.immediate.index()) {
    case 0: /* Nothing. */ break;
    case 1: format_to(buf, " {}", self.s32_immediate()); break;
    case 2: format_to(buf, " {}", self.s64_immediate()); break;
    case 3: format_to(buf, " {:f}", self.f32_immediate()); break;
    case 4: format_to(buf, " {:f}", self.f64_immediate()); break;
    case 5: format_to(buf, " {}", self.v128_immediate()); break;
    case 6: format_to(buf, " {}", self.index_immediate()); break;
    case 7: format_to(buf, " {}", self.block_type_immediate()); break;
    case 8: format_to(buf, " {}", self.br_on_exn_immediate()); break;
    case 9: format_to(buf, " {}", self.br_table_immediate()); break;
    case 10: format_to(buf, " {}", self.call_indirect_immediate()); break;
    case 11: format_to(buf, " {}", self.copy_immediate()); break;
    case 12: format_to(buf, " {}", self.init_immediate()); break;
    case 13: format_to(buf, " {}", self.let_immediate()); break;
    case 14: format_to(buf, " {}", self.mem_arg_immediate()); break;
    case 15: format_to(buf, " {}", self.heap_type_immediate()); break;
    case 16: format_to(buf, " {}", self.select_immediate()); break;
    case 17: format_to(buf, " {}", self.shuffle_immediate()); break;
    case 18: format_to(buf, " {}", self.simd_lane_immediate()); break;
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::InstructionList>::format(
    const ::wasp::binary::InstructionList& self,
    Ctx& ctx) {
  memory_buffer buf;
  string_view space = "";
  for (const auto& x : self) {
    format_to(buf, "{}{}", space, x);
    space = " ";
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Function>::format(
    const ::wasp::binary::Function& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}}}", self.type_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Table>::format(
    const ::wasp::binary::Table& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}}}", self.table_type);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Memory>::format(
    const ::wasp::binary::Memory& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}}}", self.memory_type);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Global>::format(
    const ::wasp::binary::Global& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}, init {}}}", self.global_type, self.init);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Start>::format(
    const ::wasp::binary::Start& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{func {}}}", self.func_index);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ElementSegment>::format(
    const ::wasp::binary::ElementSegment& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.has_indexes()) {
    format_to(buf, "{{type {}, init {}, ", self.indexes().kind,
              self.indexes().list);
  } else if (self.has_expressions()) {
    format_to(buf, "{{type {}, init {}, ", self.expressions().elemtype,
              self.expressions().list);
  }

  switch (self.type) {
    case ::wasp::SegmentType::Active:
      format_to(buf, "mode active {{table {}, offset {}}}}}", *self.table_index,
                *self.offset);
      break;

    case ::wasp::SegmentType::Passive:
      format_to(buf, "mode passive}}");
      break;

    case ::wasp::SegmentType::Declared:
      format_to(buf, "mode declared}}");
      break;
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator
formatter<::wasp::binary::ElementListWithIndexes>::format(
    const ::wasp::binary::ElementListWithIndexes& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}, list {}}}", self.kind, self.list);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator
formatter<::wasp::binary::ElementListWithExpressions>::format(
    const ::wasp::binary::ElementListWithExpressions& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}, init {}}}", self.elemtype, self.list);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Code>::format(
    const ::wasp::binary::Code& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{locals {}, body {}}}", self.locals, self.body);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::DataSegment>::format(
    const ::wasp::binary::DataSegment& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{init {}, ", self.init);
  switch (self.type) {
    case ::wasp::SegmentType::Active:
      format_to(buf, "mode active {{memory {}, offset {}}}}}",
                *self.memory_index, *self.offset);
      break;

    case ::wasp::SegmentType::Passive:
      format_to(buf, "mode passive}}");
      break;

    case ::wasp::SegmentType::Declared:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::DataCount>::format(
    const ::wasp::binary::DataCount& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{count {}}}", self.count);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Event>::format(
    const ::wasp::binary::Event& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{type {}}}", self.event_type);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::UnpackedCode>::format(
    const ::wasp::binary::UnpackedCode& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{locals {}, body {}}}", self.locals, self.body);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::UnpackedExpression>::format(
    const ::wasp::binary::UnpackedExpression& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{}", self.instructions);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

}  // namespace fmt
