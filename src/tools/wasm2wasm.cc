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
#include "wasp/binary/read/read_ctx.h"
#include "wasp/binary/types.h"
#include "wasp/convert/to_text.h"
#include "wasp/text/formatters.h"
#include "wasp/text/types.h"
#include "wasp/text/write.h"
#include "wasp/valid/valid_ctx.h"
#include "wasp/valid/validate.h"

#include "src/tools/argparser.h"
#include "src/tools/text_errors.h"
#include "wasp/base/buffer.h"
#include "wasp/base/errors.h"
#include "wasp/base/features.h"
#include "wasp/base/file.h"
#include "wasp/base/formatters.h"
#include "wasp/base/span.h"
#include "wasp/base/string_view.h"
#include "wasp/binary/encoding.h"
#include "wasp/binary/formatters.h"
#include "wasp/binary/types.h"
#include "wasp/binary/visitor.h"
#include "wasp/binary/write.h"
#include "wasp/convert/to_binary.h"
#include "wasp/text/desugar.h"
#include "wasp/text/read.h"
#include "wasp/text/read/read_ctx.h"
#include "wasp/text/read/tokenizer.h"
#include "wasp/text/resolve.h"
#include "wasp/text/types.h"
#include "wasp/valid/valid_ctx.h"
#include "wasp/valid/validate.h"

#include "wasp/text/formatters.h"
#include "wasp/text/write.h"

namespace fs = std::filesystem;

namespace wasp {
namespace tools {
namespace wasm2wasm {

using absl::PrintF;
using absl::Format;

struct Options {
  Features features;
  bool validate = true;
  std::string output_filename;
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

  ArgParser parser{"wasp wasm2wasm"};
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

  if (options.output_filename.empty()) {
    // Create an output filename from the input filename.
    options.output_filename =
        fs::path(filename).replace_extension(".wasm1").string();
  }

  SpanU8 data{*optbuf};
  Tool tool{filename, data, options};
  return tool.Run();
}

Tool::Tool(string_view filename, SpanU8 data, Options options)
    : filename{filename}, options{options}, data{data} {}

int Tool::Run() {
  BinaryErrors errors{data};
  binary::ReadCtx read_context{options.features, errors};
  auto binary_module = binary::ReadModule(data, read_context);
  if (errors.HasError()) {
    errors.PrintTo(std::cerr);
    return 1;
  }

  if (options.validate) {
    valid::ValidCtx validate_context{options.features, errors};
    valid::Validate(validate_context, *binary_module);

    if (errors.HasError()) {
      errors.PrintTo(std::cerr);
      return 1;
    }
  }

  //Add whatever in here


//Either find the index of the function taking I32 and returning nothing
  int indexTYPE = 0;
  bool foundOne = false;
  for (auto x : (*binary_module).types)
  {
	if (x.value().is_function_type())
	{
		const wasp::binary::FunctionType& FT = x.value().function_type();
		if (FT.result_types.size() == 0)
			if (FT.param_types.size() == 1)
				if (FT.param_types[0].value().numeric_type() == NumericType::I32)
				{
					foundOne = true;
					break;
				}
	}
	indexTYPE++;
  }

//Or insert the relevant type in the type list
 if (!foundOne)
  {
	  (*binary_module).types.push_back(wasp::binary::DefinedType(wasp::binary::FunctionType{wasp::binary::ValueTypeList(1, wasp::binary::ValueType(NumericType::I32) ), wasp::binary::ValueTypeList()}));
  }

 //Add plippoplappo with the right type and the right module name
  auto S = (*binary_module).imports[0].value().module;
  const int indexFUNC = (*binary_module).imports.size();
  (*binary_module).imports.push_back(wasp::binary::Import{(string_view)S, "__magic_pgo_function__"_sv, Index{indexTYPE}});


//Add 1 to all the direct call
  for (auto& x : (*binary_module).codes)
  {
	  for (auto& i : x.value().body.instructions)
	  {
		  if (i.value().opcode == Opcode::Call)
		  {
			  if (i.value().index_immediate() >= indexFUNC)
				  i.value().index_immediate() = (i.value().index_immediate() + 1);
		  }
	  }
  }

  //Add 1 to the element table
  for (auto& x : (*binary_module).element_segments)
  {
	  for (auto& i : x.value().indexes().list)
	  {
		  if (i.value() >= indexFUNC)
			  i.value() = i.value() + 1;

	  }
  }

  //Add 1 to exports
  for (auto& x : (*binary_module).exports)
  {
		  if (x.value().index >= indexFUNC)
			 x.value().index = x.value().index + 1;
  }

  const bool logIfBrIfProbability = true;
  const bool logFunctionCalls = false;


if (logIfBrIfProbability)
{
  int ID = 0;
  for (auto& x : (*binary_module).codes)
  {
	  wasp::binary::InstructionList NEXT;
	  wasp::binary::InstructionList& current = x.value().body.instructions;

	  for(auto i : current)
	  {
		  if (i.value().opcode == Opcode::BrIf || i.value().opcode == Opcode::If)
		  {
			  //Encode increasing id
			  NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{ID++}});
			  NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});

			  if (i.value().opcode == Opcode::BrIf)
			        NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{0}});
			  else
			        NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{1}});
			  //Encode whether a BrIf or If
			  NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});
		  }
		  NEXT.push_back(i);
		  if (i.value().opcode == Opcode::BrIf || i.value().opcode == Opcode::If)
		  {
			  //Encode whether in IF has been taken or a BrIf has been skipped
			  NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{-1}});
			  NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});
		  }
	  }
	  std::swap(NEXT, current);
  }
}
else if (logFunctionCalls)
{
  int ID = 0;
  for (auto& x : (*binary_module).codes)
  {
	  wasp::binary::InstructionList NEXT;
	  wasp::binary::InstructionList& current = x.value().body.instructions;

	  NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{ID++}});
	  NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});

//	  NEXT.push_back(wasp::binary::Instruction{Opcode::Block, ...}; //to catch br to out of the function

	  for(auto i : current)
	  {
		  if (i.value().opcode == Opcode::Return)	//Handle other kinds of returns
		  {
			NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{-1}});
			NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});
		  }
		  NEXT.push_back(i);
	  }

	  NEXT.pop_back();


//	  NEXT.push_back(wasp::binary::Instruction{Opcode::End};
	NEXT.push_back(wasp::binary::Instruction{Opcode::I32Const, s32{-1}});
	NEXT.push_back(wasp::binary::Instruction{Opcode::Call, Index{indexFUNC}});
//	the return is implicit

	NEXT.push_back(current.back());


	  std::swap(NEXT, current);
  }
}













//Done





  if (options.validate) {
    valid::ValidCtx validate_context{options.features, errors};
    valid::Validate(validate_context, *binary_module);

    if (errors.HasError()) {
      errors.PrintTo(std::cerr);
      return 1;
    }
  }

  Buffer buffer;
  Write(*binary_module, std::back_inserter(buffer));

  std::ofstream fstream(options.output_filename,
                        std::ios_base::out | std::ios_base::binary);
  if (!fstream) {
    Format(&std::cerr, "Unable to open file %s.\n", options.output_filename);
    return 1;
  }

  auto span = ToStringView(buffer);
  fstream.write(span.data(), span.size());
  return 0;
}

}  // namespace wasm2wasm
}  // namespace tools
}  // namespace wasp
