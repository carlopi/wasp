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

#ifndef WASP_BINARY_READ_LINKING_H_
#define WASP_BINARY_READ_LINKING_H_

#include "wasp/binary/read.h"
#include "wasp/binary/types_linking.h"

namespace wasp {

class Features;

namespace binary {

class Errors;

optional<Comdat> Read(SpanU8*, const Features&, Errors&, Tag<Comdat>);

optional<ComdatSymbol> Read(SpanU8*,
                            const Features&,
                            Errors&,
                            Tag<ComdatSymbol>);

optional<ComdatSymbolKind> Read(SpanU8*,
                                const Features&,
                                Errors&,
                                Tag<ComdatSymbolKind>);

optional<InitFunction> Read(SpanU8*,
                            const Features&,
                            Errors&,
                            Tag<InitFunction>);

optional<LinkingSubsection> Read(SpanU8*,
                                 const Features&,
                                 Errors&,
                                 Tag<LinkingSubsection>);

optional<LinkingSubsectionId> Read(SpanU8*,
                                   const Features&,
                                   Errors&,
                                   Tag<LinkingSubsectionId>);

optional<RelocationEntry> Read(SpanU8*,
                               const Features&,
                               Errors&,
                               Tag<RelocationEntry>);

optional<RelocationType> Read(SpanU8*,
                              const Features&,
                              Errors&,
                              Tag<RelocationType>);

optional<SegmentInfo> Read(SpanU8*, const Features&, Errors&, Tag<SegmentInfo>);

optional<SymbolInfo> Read(SpanU8*, const Features&, Errors&, Tag<SymbolInfo>);

optional<SymbolInfoKind> Read(SpanU8*,
                              const Features&,
                              Errors&,
                              Tag<SymbolInfoKind>);


}  // namespace binary
}  // namespace wasp

#endif  // WASP_BINARY_READ_LINKING_H_
