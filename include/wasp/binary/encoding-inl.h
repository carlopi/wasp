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

namespace wasp {
namespace binary {
namespace encoding {

// static
inline optional<::wasp::binary::ValueType> ValueType::Decode(u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::ValueType::Name;
#include "wasp/binary/value_type.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::BlockType> BlockType::Decode(u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::BlockType::Name;
#include "wasp/binary/block_type.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::ElementType> ElementType::Decode(u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::ElementType::Name;
#include "wasp/binary/element_type.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::ExternalKind> ExternalKind::Decode(u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::ExternalKind::Name;
#include "wasp/binary/external_kind.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::Mutability> Mutability::Decode(u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::Mutability::Name;
#include "wasp/binary/mutability.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::SectionId> Section::Decode(u32 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::SectionId::Name;
#include "wasp/binary/section_id.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::Opcode> Opcode::Decode(u8 val) {
  switch (val) {
#define WASP_V(prefix, val, Name, str) \
  case Name:                           \
    return ::wasp::binary::Opcode::Name;
#include "wasp/binary/opcode.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

// static
inline optional<::wasp::binary::NameSubsectionId> NameSubsectionId::Decode(
    u8 val) {
  switch (val) {
#define WASP_V(val, Name, str) \
  case Name:                   \
    return ::wasp::binary::NameSubsectionId::Name;
#include "wasp/binary/name_subsection_id.def"
#undef WASP_V
    default:
      return nullopt;
  }
}

}  // namespace encoding
}  // namespace binary
}  // namespace wasp
