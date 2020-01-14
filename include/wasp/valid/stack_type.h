//
// Copyright 2020 WebAssembly Community Group participants
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

#ifndef WASP_VALID_STACK_TYPE_H_
#define WASP_VALID_STACK_TYPE_H_

#include "wasp/base/types.h"

namespace wasp {
namespace valid {

enum class StackType : s32 {
#define WASP_V(val, Name, str) Name,
#define WASP_FEATURE_V(val, Name, str, feature) WASP_V(val, Name, str)
#include "wasp/valid/stack_type.def"
#undef WASP_V
#undef WASP_FEATURE_V
};

}  // namespace valid
}  // namespace wasp

#endif // WASP_VALID_STACK_TYPE_H_