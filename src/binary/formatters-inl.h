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

#include "src/binary/formatters.h"

#include "src/base/macros.h"
#include "src/base/formatters.h"
#include "src/binary/defs.h"

namespace fmt {

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ValueType>::format(
    const ::wasp::binary::ValueType& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)          \
  case ::wasp::binary::ValueType::Name: \
    result = str;                       \
    break;
    WASP_FOREACH_VALUE_TYPE(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::BlockType>::format(
    const ::wasp::binary::BlockType& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)          \
  case ::wasp::binary::BlockType::Name: \
    result = "[" str "]";               \
    break;
    WASP_FOREACH_BLOCK_TYPE(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ElementType>::format(
    const ::wasp::binary::ElementType& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)            \
  case ::wasp::binary::ElementType::Name: \
    result = str;                         \
    break;
    WASP_FOREACH_ELEMENT_TYPE(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::ExternalKind>::format(
    const ::wasp::binary::ExternalKind& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)             \
  case ::wasp::binary::ExternalKind::Name: \
    result = str;                          \
    break;
    WASP_FOREACH_EXTERNAL_KIND(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Mutability>::format(
    const ::wasp::binary::Mutability& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)           \
  case ::wasp::binary::Mutability::Name: \
    result = str;                        \
    break;
    WASP_FOREACH_MUTABILITY(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::SectionId>::format(
    const ::wasp::binary::SectionId& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)          \
  case ::wasp::binary::SectionId::Name: \
    result = str;                       \
    break;
    WASP_FOREACH_SECTION(WASP_V)
#undef WASP_V
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
typename Ctx::iterator formatter<::wasp::binary::NameSubsectionId>::format(
    const ::wasp::binary::NameSubsectionId& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(val, Name, str)                 \
  case ::wasp::binary::NameSubsectionId::Name: \
    result = str;                              \
    break;
    WASP_FOREACH_NAME_SUBSECTION_ID(WASP_V)
#undef WASP_V
    default:
      WASP_UNREACHABLE();
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::MemArg>::format(
    const ::wasp::binary::MemArg& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{{align {}, offset {}}}", self.align_log2, self.offset);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Limits>::format(
    const ::wasp::binary::Limits& self,
    Ctx& ctx) {
  memory_buffer buf;
  if (self.max) {
    format_to(buf, "{{min {}, max {}}}", self.min, *self.max);
  } else {
    format_to(buf, "{{min {}}}", self.min);
  }
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
typename Ctx::iterator formatter<::wasp::binary::MemoryType>::format(
    const ::wasp::binary::MemoryType& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{}", self.limits);
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
typename Ctx::iterator formatter<::wasp::binary::Import>::format(
    const ::wasp::binary::Import& self,
    Ctx& ctx) {
  using ::wasp::get;
  using ::wasp::holds_alternative;

  memory_buffer buf;
  format_to(buf, "{{module \"{}\", name \"{}\", desc {}", self.module,
            self.name, self.kind());

  if (holds_alternative<::wasp::Index>(self.desc)) {
    format_to(buf, " {}}}", get<::wasp::Index>(self.desc));
  } else if (holds_alternative<::wasp::binary::TableType>(self.desc)) {
    format_to(buf, " {}}}", get<::wasp::binary::TableType>(self.desc));
  } else if (holds_alternative<::wasp::binary::MemoryType>(self.desc)) {
    format_to(buf, " {}}}", get<::wasp::binary::MemoryType>(self.desc));
  } else if (holds_alternative<::wasp::binary::GlobalType>(self.desc)) {
    format_to(buf, " {}}}", get<::wasp::binary::GlobalType>(self.desc));
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
  format_to(buf, "{}", self.data);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::Opcode>::format(
    const ::wasp::binary::Opcode& self,
    Ctx& ctx) {
  string_view result;
  switch (self) {
#define WASP_V(prefix, val, Name, str) \
  case ::wasp::binary::Opcode::Name:   \
    result = str;                      \
    break;
    WASP_FOREACH_OPCODE(WASP_V)
#undef WASP_V
    default: {
      // Special case for sections with unknown ids.
      memory_buffer buf;
      format_to(buf, "<unknown:{}>", static_cast<::wasp::u32>(self));
      return formatter<string_view>::format(to_string_view(buf), ctx);
    }
  }
  return formatter<string_view>::format(result, ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::CallIndirectImmediate>::format(
    const ::wasp::binary::CallIndirectImmediate& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.index, self.reserved);
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
typename Ctx::iterator formatter<::wasp::binary::Instruction>::format(
    const ::wasp::binary::Instruction& self,
    Ctx& ctx) {
  using ::wasp::get;
  using ::wasp::holds_alternative;

  memory_buffer buf;
  format_to(buf, "{}", self.opcode);

  if (holds_alternative<::wasp::binary::EmptyImmediate>(self.immediate)) {
    // Nothing.
  } else if (holds_alternative<::wasp::binary::BlockType>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::binary::BlockType>(self.immediate));
  } else if (holds_alternative<::wasp::Index>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::Index>(self.immediate));
  } else if (holds_alternative<::wasp::binary::CallIndirectImmediate>(
                 self.immediate)) {
    format_to(buf, " {}",
              get<::wasp::binary::CallIndirectImmediate>(self.immediate));
  } else if (holds_alternative<::wasp::binary::BrTableImmediate>(
                 self.immediate)) {
    format_to(buf, " {}",
              get<::wasp::binary::BrTableImmediate>(self.immediate));
  } else if (holds_alternative<::wasp::u8>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::u8>(self.immediate));
  } else if (holds_alternative<::wasp::binary::MemArg>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::binary::MemArg>(self.immediate));
  } else if (holds_alternative<::wasp::s32>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::s32>(self.immediate));
  } else if (holds_alternative<::wasp::s64>(self.immediate)) {
    format_to(buf, " {}", get<::wasp::s64>(self.immediate));
  } else if (holds_alternative<::wasp::f32>(self.immediate)) {
    format_to(buf, " {:f}", get<::wasp::f32>(self.immediate));
  } else if (holds_alternative<::wasp::f64>(self.immediate)) {
    format_to(buf, " {:f}", get<::wasp::f64>(self.immediate));
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
  format_to(buf, "{{table {}, offset {}, init {}}}", self.table_index,
            self.offset, self.init);
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
  format_to(buf, "{{memory {}, offset {}, init {}}}", self.memory_index,
            self.offset, self.init);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::NameAssoc>::format(
    const ::wasp::binary::NameAssoc& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} \"{}\"", self.index, self.name);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::IndirectNameAssoc>::format(
    const ::wasp::binary::IndirectNameAssoc& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.index, self.name_map);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

template <typename Ctx>
typename Ctx::iterator formatter<::wasp::binary::NameSubsection>::format(
    const ::wasp::binary::NameSubsection& self,
    Ctx& ctx) {
  memory_buffer buf;
  format_to(buf, "{} {}", self.id, self.data);
  return formatter<string_view>::format(to_string_view(buf), ctx);
}

}  // namespace fmt
