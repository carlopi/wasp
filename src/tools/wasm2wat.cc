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

#include <filesystem>
#include <fstream>
#include <iostream>

#include "absl/strings/str_format.h"

#include "src/tools/argparser.h"
#include "src/tools/binary_errors.h"
#include "wasp/base/buffer.h"
#include "wasp/base/errors.h"
#include "wasp/base/features.h"
#include "wasp/base/file.h"
#include "wasp/base/formatters.h"
#include "wasp/base/span.h"
#include "wasp/base/string_view.h"
#include "wasp/binary/encoding.h"
#include "wasp/binary/formatters.h"
#include "wasp/binary/read.h"
#include "wasp/binary/read/context.h"
#include "wasp/binary/types.h"
#include "wasp/convert/to_text.h"
#include "wasp/text/formatters.h"
#include "wasp/text/types.h"
#include "wasp/text/write.h"
#include "wasp/valid/context.h"
#include "wasp/valid/validate.h"

namespace fs = std::filesystem;

namespace wasp {
namespace tools {
namespace wasm2wat {

using absl::PrintF;
using absl::Format;

struct Options {
  Features features;
  bool validate = true;
  optional<std::string> output_filename;
};

struct Tool {
  explicit Tool(string_view filename, SpanU8 data, Options);

  enum class PrintChars { No, Yes };

  int Run();

  std::string filename;
  Options options;
  SpanU8 data;
};

int Main(span<const string_view> args) {
  string_view filename;
  Options options;

  ArgParser parser{"wasp wasm2wat"};
  parser
      .Add("--help", "print help and exit",
           [&]() { parser.PrintHelpAndExit(0); })
      .Add('o', "--output", "<filename>", "write DOT file output to <filename>",
           [&](string_view arg) { options.output_filename = arg; })
      .Add("--no-validate", "Don't validate before writing",
           [&]() { options.validate = false; })
      .AddFeatureFlags(options.features)
      .Add("<filename>", "input wasm file", [&](string_view arg) {
        if (filename.empty()) {
          filename = arg;
        } else {
          Format(&std::cerr, "Filename already given\n");
        }
      });
  parser.Parse(args);

  if (filename.empty()) {
    Format(&std::cerr, "No filenames given.\n");
    parser.PrintHelpAndExit(1);
  }

  auto optbuf = ReadFile(filename);
  if (!optbuf) {
    Format(&std::cerr, "Error reading file %s.\n", filename);
    return 1;
  }

  if (options.output_filename) {
    // Create an output filename from the input filename.
    options.output_filename =
        fs::path(filename).replace_extension(".wat").string();
  }

  SpanU8 data{*optbuf};
  Tool tool{filename, data, options};
  return tool.Run();
}

Tool::Tool(string_view filename, SpanU8 data, Options options)
    : filename{filename}, options{options}, data{data} {}

int Tool::Run() {
  BinaryErrors errors{data};
  binary::Context read_context{options.features, errors};
  auto binary_module = binary::ReadModule(data, read_context);
  if (errors.HasError()) {
    errors.PrintTo(std::cerr);
    return 1;
  }

  if (options.validate) {
    valid::Context validate_context{options.features, errors};
    valid::Validate(validate_context, *binary_module);

    if (errors.HasError()) {
      errors.PrintTo(std::cerr);
      return 1;
    }
  }

  convert::TextContext convert_context;
  auto text_module = convert::ToText(convert_context, *binary_module);

  text::WriteContext write_context;
  Buffer buffer;
  text::Write(write_context, text_module, std::back_inserter(buffer));

  auto span = ToStringView(buffer);
  if (options.output_filename) {
    std::ofstream fstream(*options.output_filename,
                          std::ios_base::out | std::ios_base::binary);
    if (!fstream) {
      Format(&std::cerr, "Unable to open file %s.\n", *options.output_filename);
      return 1;
    }

    fstream.write(span.data(), span.size());
  } else {
    std::cout.write(span.data(), span.size());
  }

  return 0;
}

}  // namespace wasm2wat
}  // namespace tools
}  // namespace wasp
