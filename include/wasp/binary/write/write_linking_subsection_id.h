//
// Copyright 2019 WebAssembly Community Group participants
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

#ifndef WASP_BINARY_WRITE_WRITE_LINKING_SUBSECTION_ID_H_
#define WASP_BINARY_WRITE_WRITE_LINKING_SUBSECTION_ID_H_

#include "wasp/binary/encoding/linking_subsection_id_encoding.h"
#include "wasp/binary/linking_subsection_id.h"
#include "wasp/binary/write/write_u8.h"

namespace wasp {
namespace binary {

template <typename Iterator>
Iterator Write(LinkingSubsectionId value, Iterator out) {
  return Write(encoding::LinkingSubsectionId::Encode(value), out);
}

}  // namespace binary
}  // namespace wasp

#endif  // WASP_BINARY_WRITE_WRITE_LINKING_SUBSECTION_ID_H_