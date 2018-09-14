// parse_options.cc

#include "parse_cxcursor_info_options.h"
#include <iostream>

std::string newlines_on_size(const std::string &str, size_t width) {
  std::string result;
  result.reserve(str.size() + str.size() / width + 1);
  size_t pos = 0;
  size_t sub = 0;
  while (pos < str.size()) {
    if (pos + width >= str.size()) {
      sub = width;
    } else {
      sub = str.find_last_of(" \t\n", pos + width);
    }
    if (sub - pos <= 0) {
      result += str.substr(pos, std::string::npos);
      return result;
    }
    result += str.substr(pos, sub - pos) + "\n";
    pos += sub;
  }
  return result;
}

std::string usage =
    "[options] [-L location] -f sourcefile\n\n" +
    newlines_on_size(
        "Options either specify which information to show or "
        "describe the descent behavior.  The location should be "
        "specified as [line_number column_number], indicating "
        "where to start examining CXCursors.  If no location is "
        "specified, then the CXCursor of origin will be the "
        "Tranlation Unit cursor.  If attributes are indicate in the "
        "arguments, then only those attributes will be listed.\n\n"
        "I know it's cheesy, but the -f is mandtory, because I'm lazy.");

std::list<OptionTriple> option_list = {
    {"-r", "--recurse",
     "when a cursor is found to examine, recurse into its subtree"},
    {"-v", "--verbose",
     "display failed predicates and empty attributes"},
    {"-o", "--omit", "only display the collect information about "
                     "attributes not indicated in the options"}};

struct SupportedAttributeTriple {
  std::string short_opt;
  std::string attribute_key;
};

std::list<SupportedAttributeTriple> supported_attributes = {
    {"-cid", "--CustomId"},
    {"-ts", "--TypeSpelling"},
    {"-tks", "--TypeKindSpelling"},
    {"-usr", "--CursorUSR"},
    {"-spl", "--CursorSpelling"},
    {"-dn", "--CursorDisplayName"},
    {"-cks", "--CursorKindSpelling"},
    {"-rct", "--RawCommentText"},
    {"-bct", "--BriefCommentText"},
    {"-loc", "--location"},
    {"-sp", "--SemanticParent"},
    {"-lp", "--LexicalParent"},
    {"-ref", "--Referenced"},
    {"-def", "--Definition"},
    {"-can", "--CanonicalCursor"},
    {"-sct", "--SpecializedCursorTemplate"},
    {"-ha", "--hasAttributes"},
    {"-pish", "--isInSystemHeader"},
    {"-pfmf", "--isFromMainFile"},
    {"-pdec", "--isDeclaration"},
    {"-pref", "--isReference"},
    {"-pexp", "--isExpression"},
    {"-pst", "--isStatement"},
    {"-patt", "--isAttribute"},
    {"-pinv", "--isInvalid"},
    {"-ptu", "--isTranslationUnit"},
    {"-ppre", "--isPreprocessing"},
    {"-punx", "--isUnexposed"},
    {"-pmfl", "--isMacroFunctionLike"},
    {"-pmb", "--isMacroBuiltin"},
    {"-pfni", "--isFunctionInlined"},
    {"-pbf", "--isBitField"},
    {"-pdyn", "--isDynamicCall"},
    {"-pvdc", "--isVariadic"},
    {"-pcnc", "--isConvertingConstructor"},
    {"-pcpc", "--isCopyConstructor"},
    {"-pdfc", "--isDefaultConstructor"},
    {"-pmvc", "--isMoveConstructor"},
    {"-pmut", "--isMutable"},
    {"-pdfl", "--isDefaulted"},
    {"-pcdf", "--isCursorDefinition"},
    {"-ppv", "--isPureVirtual"},
    {"-pstc", "--isStatic"},
    {"-pvtl", "--isVirtual"},
    {"-pvb", "--isVirtualBase"},
    {"-pcon", "--isConst"},
    {"-cls", "--ClassType"},
    {"-ntp", "--NamedType"},
    {"-pcqt", "--isConstQualifiedType"},
    {"-pvqt", "--isVolatileQualifiedType"},
    {"-prqt", "--isRestrictQualifiedType"},
    {"-pftv", "--isFunctionTypeVariadic"},
    {"-ppod", "--isPODType"},
    {"-paln", "--AlignOf"},
    {"-psiz", "--SizeOf"},
    {"-pnta", "--NumTemplateArguments"},
    {"-pna", "--NumArguments"},
    {"-prfq", "--CXXRefQualifier"},
    {"-sto", "--StorageClass"}
};

Options::Options() : recurse(false), verbose(false), line(0), col(0) {}

std::string Options::help(const std::string &name) {
  std::string result = "Usage" + name + usage + "\n\n";
  for (auto &&opt : option_list) {
    add_opt_line(result, opt);
    result += "\n";
  }
  result += "\n  Supported CXCursor attributes:\n\n";
  std::string line;
  line.reserve(100);
  for (auto &&sa : supported_attributes) {
    result += "  " + sa.short_opt + "\t  " + sa.attribute_key + "\n";
  }
  result += "\n\nExamples:\n\n";
  result +=
      "./cxcursor_info -r -ref -ts -tks -cid -sp -loc -L 12 1 -f test2.cc\n";
  return result;
}

OptionTriple::OptionTriple(const std::string &s, const std::string &l,
                           const std::string &m)
    : short_opt(s), long_opt(l), message(m) {}

static const std::string help_opt_line_indent(2, ' ');
static const std::string help_message_line_indent(32, ' ');
static const size_t max_line_width = 80;
static const size_t initial_message_align = 30;
static const size_t initial_long_opt_align = 30;

std::string get_initial_opt(const OptionTriple &triple) {
  return triple.short_opt + std::string((4 - triple.short_opt.size()), ' ') +
         triple.long_opt;
}

std::list<std::string> get_message_split(const OptionTriple &triple) {
  std::list<std::string> result;
  size_t pos = 0;
  std::string initial_opt = get_initial_opt(triple);
  std::string line_indent(initial_message_align - (initial_opt.size() +
                                                   help_opt_line_indent.size()),
                          ' ');

  if (initial_message_align + triple.message.size() <= max_line_width) {
    return std::list<std::string>{line_indent + triple.message};
  } else {
    size_t last = triple.message.find_last_of(
        " \t\n", max_line_width - (initial_message_align + initial_opt.size()));
    size_t sub_size;

    result.push_back(line_indent + triple.message.substr(pos, last));

    pos += last;
    const size_t max_sub_size =
        max_line_width - help_message_line_indent.size();
    while (pos < triple.message.size()) {
      last = triple.message.find_last_of(" \t\n", pos + max_sub_size);
      if (last <= pos || triple.message.size() - pos <= max_sub_size) {
        // something funny happened...
        result.push_back(help_message_line_indent +
                         triple.message.substr(pos, std::string::npos));
        return result;
      }
      sub_size = last - pos;
      result.push_back(help_message_line_indent +
                       triple.message.substr(pos, sub_size));
      pos += sub_size;
    }
  }
  return result;
}

std::string get_attribute_key_from_option(const std::string &option) {
  for (auto &&sa : supported_attributes) { 
    if (option == sa.short_opt || option == sa.attribute_key) {
      return sa.attribute_key;
    }
  }
  return "";
}

void add_opt_line(std::string &help, const OptionTriple &triple) {
  std::list<std::string> message_split = get_message_split(triple);
  help += help_opt_line_indent + get_initial_opt(triple);
  for (auto &&line : message_split) {
    help += line + "\n";
  }
}

bool parse_options(Options &options,
                   const std::list<std::string> &attribute_list, int argc,
                   char *argv[]) {
  bool have_source = false;
  bool custom_attributes = false;
  bool invert = false;
  std::string arg;
  if (argc < 2) {
    return false;
  }
  for (int i = 1; i < argc; ++i) {
    arg = argv[i];
    if (arg == "-r" || arg == "--recurse") {
      options.recurse = true;
    } else if (arg == "-o" || arg == "--omit") {
      invert = true;
    } else if (arg == "-v" || arg == "--verbose") {
      options.verbose = true;
    }else if (arg == "-L") {
      if (i + 2 >= argc) {
        return false;
      }
      options.line = (size_t)atol(argv[++i]);
      options.col = (size_t)atol(argv[++i]);
    } else if (arg == "-f") {
      if (++i >= argc) {
        return false;
      }
      options.source = argv[i];
      have_source = true;
    } else {
      std::string attribute = get_attribute_key_from_option(arg);
      if (attribute.empty()) {
        return false;
      }
      custom_attributes = true;
      options.chosen_attributes.push_back(attribute);
    }
  }
  if (!custom_attributes) {
    options.chosen_attributes = attribute_list;
  }
  if (invert) {
    std::list<std::string> inverted;
    for (auto &&attribute : attribute_list) {
      inverted.push_back(attribute);
    }
    for (auto &&attribute : options.chosen_attributes) {
      inverted.remove(attribute);
    }
    options.chosen_attributes = inverted;
  }
  options.chosen_attributes.sort();
  return have_source;
}

std::string Options::dump() const {
  std::string result =
      "{recurse: " + std::to_string(recurse) + ",\nchosen_attributes: {\n";
  for (auto &&attribute = chosen_attributes.begin();
       attribute != chosen_attributes.end(); ++attribute) {
    result += "\t" + *attribute;
    if (next(attribute) != chosen_attributes.end()) {
      result += ",";
    }
    result += "\n";
  }
  result += "},\nsource: " + source + ", line: " + std::to_string(line) +
            ", col: " + std::to_string(col) + "}";
  return result;
}
