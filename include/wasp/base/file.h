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

#ifndef WASP_BASE_FILE_H_
#define WASP_BASE_FILE_H_

#include <vector>

#include "wasp/base/buffer.h"
#include "wasp/base/optional.h"
#include "wasp/base/string_view.h"
#include "wasp/base/types.h"

namespace wasp {

optional<Buffer> ReadFile(string_view filename);

}  // namespace wasp

#endif  // WASP_BASE_FILE_H_
