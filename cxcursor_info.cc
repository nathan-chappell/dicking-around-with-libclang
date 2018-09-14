// cxcursor_info.cc

#include "cxcursor_info.h"
#include "parse_cxcursor_info_options.h"

#include <iostream>
#include <list>

using namespace std;
using AttributeMap = std::unordered_map<std::string, std::string (*)(CXCursor)>;

std::size_t get_offset(std::size_t size, std::size_t align = 30) {
  return align > size ? align - size : 0;
}

std::string convert_cxstring(CXString cxstring) {
  if (cxstring.data == nullptr) {
    return "null cxstring";
  }
  std::string result = clang_getCString(cxstring);
  clang_disposeString(cxstring);
  return result;
}

std::string str_time(time_t t) {
  const std::size_t buffer_size = 100;
  char buffer[buffer_size];
  std::size_t llimit =
      strftime(buffer, buffer_size, "%H:%M:%S, %d.%m.%y", localtime(&t));
  buffer[llimit] = 0;
  return buffer;
}

/*
 * string conversions
 */

std::string string_ClangVersion(void) {
  return convert_cxstring(clang_getClangVersion());
}

std::string string_FileName(CXFile SFile) {
  return convert_cxstring(clang_getFileName(SFile));
}

std::string string_location(CXSourceLocation location) {
  CXFile cxfile;
  unsigned line;
  unsigned col;

  clang_getSpellingLocation(location, &cxfile, &line, &col, nullptr);

  if (cxfile == nullptr) {
    return "no location";
  } else {
    return string_FileName(cxfile) + ":" + to_string(line) + ":" +
           to_string(col);
  }
}

/*
 * Maintain a table of unique ids for each cursor
 */
struct CursorHash {
  size_t operator()(CXCursor cursor) const { return clang_hashCursor(cursor); }
};
struct CursorEqual {
  size_t operator()(CXCursor lhs, CXCursor rhs) const {
    return clang_equalCursors(lhs, rhs);
  }
};
static std::size_t custom_uid = 0;
static std::unordered_map<CXCursor, std::string, CursorHash, CursorEqual>
    id_table;

/*
 * cursor attributes
 */
std::string cursor_attribute_CustomId(CXCursor cursor) {
  auto it = id_table.find(cursor);
  if (it != id_table.end()) {
    return it->second;
  } else {
    std::string new_id = to_string(++custom_uid);
    id_table[cursor] = new_id;
    return new_id;
  }
}
std::string cursor_attribute_TypeSpelling(CXCursor cursor) {
  return convert_cxstring(clang_getTypeSpelling(clang_getCursorType(cursor)));
}
std::string cursor_attribute_TypeKindSpelling(CXCursor cursor) {
  return convert_cxstring(
      clang_getTypeKindSpelling(clang_getCursorType(cursor).kind));
}
std::string cursor_attribute_CursorUSR(CXCursor cursor) {
  return convert_cxstring(clang_getCursorUSR(cursor));
}
std::string cursor_attribute_CursorSpelling(CXCursor cursor) {
  return convert_cxstring(clang_getCursorSpelling(cursor));
}
std::string cursor_attribute_CursorDisplayName(CXCursor cursor) {
  return convert_cxstring(clang_getCursorDisplayName(cursor));
}
std::string cursor_attribute_CursorKindSpelling(CXCursor cursor) {
  return convert_cxstring(clang_getCursorKindSpelling(cursor.kind));
}
std::string cursor_attribute_RawCommentText(CXCursor cursor) {
  return convert_cxstring(clang_Cursor_getRawCommentText(cursor));
}
std::string cursor_attribute_BriefCommentText(CXCursor cursor) {
  return convert_cxstring(clang_Cursor_getBriefCommentText(cursor));
}
std::string cursor_attribute_Mangling(CXCursor cursor) {
  return convert_cxstring(clang_Cursor_getMangling(cursor));
}
std::string cursor_attribute_location(CXCursor cursor) {
  return string_location(clang_getCursorLocation(cursor));
}
std::string cursor_attribute_SemanticParent(CXCursor cursor) {
  CXCursor next = clang_getCursorSemanticParent(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}
std::string cursor_attribute_LexicalParent(CXCursor cursor) {
  CXCursor next = clang_getCursorLexicalParent(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}
std::string cursor_attribute_Referenced(CXCursor cursor) {
  CXCursor next = clang_getCursorReferenced(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}
std::string cursor_attribute_Definition(CXCursor cursor) {
  CXCursor next = clang_getCursorDefinition(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}
std::string cursor_attribute_CanonicalCursor(CXCursor cursor) {
  CXCursor next = clang_getCanonicalCursor(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}
std::string cursor_attribute_SpecializedCursorTemplate(CXCursor cursor) {
  CXCursor next = clang_getSpecializedCursorTemplate(cursor);
  if (!clang_Cursor_isNull(next)) {
    return cursor_attribute_CustomId(next);
  } else {
    return "-1";
  }
}

/*
 * cursor predicates
 */
std::string cursor_predicate_hasAttributes(CXCursor cursor) {
  if (clang_Cursor_hasAttrs(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isInSystemHeader(CXCursor cursor) {
  CXSourceLocation location = clang_getCursorLocation(cursor);
  if (clang_Location_isInSystemHeader(location)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isFromMainFile(CXCursor cursor) {
  CXSourceLocation location = clang_getCursorLocation(cursor);
  if (clang_Location_isFromMainFile(location)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isDeclaration(CXCursor cursor) {
  if (clang_isDeclaration(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isReference(CXCursor cursor) {
  if (clang_isReference(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isExpression(CXCursor cursor) {
  if (clang_isExpression(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isStatement(CXCursor cursor) {
  if (clang_isStatement(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isAttribute(CXCursor cursor) {
  if (clang_isAttribute(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isInvalid(CXCursor cursor) {
  if (clang_isInvalid(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isTranslationUnit(CXCursor cursor) {
  if (clang_isTranslationUnit(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isPreprocessing(CXCursor cursor) {
  if (clang_isPreprocessing(cursor.kind)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isUnexposed(CXCursor cursor) {
  if (clang_isUnexposed(cursor.kind)) {
    return "T";
  }
  return "F";
}

//
std::string cursor_predicate_isMacroFunctionLike(CXCursor cursor) {
  if (clang_Cursor_isMacroFunctionLike(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isMacroBuiltin(CXCursor cursor) {
  if (clang_Cursor_isMacroBuiltin(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isFunctionInlined(CXCursor cursor) {
  if (clang_Cursor_isFunctionInlined(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isBitField(CXCursor cursor) {
  if (clang_Cursor_isBitField(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isDynamicCall(CXCursor cursor) {
  if (clang_Cursor_isDynamicCall(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isVariadic(CXCursor cursor) {
  if (clang_Cursor_isVariadic(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isConvertingConstructor(CXCursor cursor) {
  if (clang_CXXConstructor_isConvertingConstructor(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isCopyConstructor(CXCursor cursor) {
  if (clang_CXXConstructor_isCopyConstructor(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isDefaultConstructor(CXCursor cursor) {
  if (clang_CXXConstructor_isDefaultConstructor(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isMoveConstructor(CXCursor cursor) {
  if (clang_CXXConstructor_isMoveConstructor(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isMutable(CXCursor cursor) {
  if (clang_CXXField_isMutable(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isDefaulted(CXCursor cursor) {
  if (clang_CXXMethod_isDefaulted(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isCursorDefinition(CXCursor cursor) {
  if (clang_isCursorDefinition(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isPureVirtual(CXCursor cursor) {
  if (clang_CXXMethod_isPureVirtual(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isStatic(CXCursor cursor) {
  if (clang_CXXMethod_isStatic(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isVirtual(CXCursor cursor) {
  if (clang_CXXMethod_isVirtual(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isVirtualBase(CXCursor cursor) {
  if (clang_isVirtualBase(cursor)) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isConst(CXCursor cursor) {
  if (clang_CXXMethod_isConst(cursor)) {
    return "T";
  }
  return "F";
}

std::string cursor_attribute_getClassType(CXCursor cursor) {
  return convert_cxstring(clang_getTypeSpelling(
      clang_Type_getClassType(clang_getCursorType(cursor))));
}
std::string cursor_attribute_getNamedType(CXCursor cursor) {
  return convert_cxstring(clang_getTypeSpelling(
      clang_Type_getNamedType(clang_getCursorType(cursor))));
}

std::string cursor_predicate_isConstQualifiedType(CXCursor cursor) {
  if (clang_isConstQualifiedType(clang_getCursorType(cursor))) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isVolatileQualifiedType(CXCursor cursor) {
  if (clang_isVolatileQualifiedType(clang_getCursorType(cursor))) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isRestrictQualifiedType(CXCursor cursor) {
  if (clang_isRestrictQualifiedType(clang_getCursorType(cursor))) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isFunctionTypeVariadic(CXCursor cursor) {
  if (clang_isFunctionTypeVariadic(clang_getCursorType(cursor))) {
    return "T";
  }
  return "F";
}
std::string cursor_predicate_isPODType(CXCursor cursor) {
  if (clang_isPODType(clang_getCursorType(cursor))) {
    return "T";
  }
  return "F";
}

std::string cursor_attribute_getAlignOf(CXCursor cursor) {
  return std::to_string(clang_Type_getAlignOf(clang_getCursorType(cursor)));
}
std::string cursor_attribute_getSizeOf(CXCursor cursor) {
  return std::to_string(clang_Type_getSizeOf(clang_getCursorType(cursor)));
}

std::string cursor_attribute_getNumArguments(CXCursor cursor) {
  return std::to_string(clang_Cursor_getNumArguments(cursor));
}

std::string cursor_attribute_getNumTemplateArguments(CXCursor cursor) {
  return std::to_string(clang_Cursor_getNumTemplateArguments(cursor));
}

std::string cursor_attribute_getCXXRefQualifier(CXCursor cursor) {
  switch (clang_Type_getCXXRefQualifier(clang_getCursorType(cursor))) {
  case CXRefQualifier_None: {
    return "None";
  } break;
  case CXRefQualifier_LValue: {
    return "LValue";
  } break;
  case CXRefQualifier_RValue: {
    return "RValue";
  } break;
  default:
    return "fail";
  }
}

std::string cursor_attribute_getStorageClass(CXCursor cursor) {
  switch (clang_Cursor_getStorageClass(cursor)) {
  case CX_SC_Invalid: {
    return "Invalid,";
  } break;
  case CX_SC_None: {
    return "None,";
  } break;
  case CX_SC_Extern: {
    return "Extern,";
  } break;
  case CX_SC_Static: {
    return "Static,";
  } break;
  case CX_SC_PrivateExtern: {
    return "PrivateExtern,";
  } break;
  case CX_SC_OpenCLWorkGroupLocal: {
    return "OpenCLWorkGroupLocal,";
  } break;
  case CX_SC_Auto: {
    return "Auto,";
  } break;
  case CX_SC_Register: {
    return "Register";
  } break;
  default:
    return "fail";
  }
}

AttributeMap cursor_attribute_map = {
    // straight up attributes
    {"CustomId", cursor_attribute_CustomId},
    {"TypeSpelling", cursor_attribute_TypeSpelling},
    {"TypeKindSpelling", cursor_attribute_TypeKindSpelling},
    {"CursorUSR", cursor_attribute_CursorUSR},
    {"CursorSpelling", cursor_attribute_CursorSpelling},
    {"CursorDisplayName", cursor_attribute_CursorDisplayName},
    {"CursorKindSpelling", cursor_attribute_CursorKindSpelling},
    {"RawCommentText", cursor_attribute_RawCommentText},
    {"BriefCommentText", cursor_attribute_BriefCommentText},
    //{"Mangling", cursor_attribute_Mangling},
    {"location", cursor_attribute_location},
    // cursor references
    {"SemanticParent", cursor_attribute_SemanticParent},
    {"LexicalParent", cursor_attribute_LexicalParent},
    {"Referenced", cursor_attribute_Referenced},
    {"Definition", cursor_attribute_Definition},
    {"CanonicalCursor", cursor_attribute_CanonicalCursor},
    {"SpecializedCursorTemplate", cursor_attribute_SpecializedCursorTemplate},
    // TODO need to add to the help menu, first do other TODOs
    {"hasAttributes", cursor_predicate_hasAttributes},
    {"isInSystemHeader", cursor_predicate_isInSystemHeader},
    {"isFromMainFile", cursor_predicate_isFromMainFile},
    {"isDeclaration", cursor_predicate_isDeclaration},
    {"isReference", cursor_predicate_isReference},
    {"isExpression", cursor_predicate_isExpression},
    {"isStatement", cursor_predicate_isStatement},
    {"isAttribute", cursor_predicate_isAttribute},
    {"isInvalid", cursor_predicate_isInvalid},
    {"isTranslationUnit", cursor_predicate_isTranslationUnit},
    {"isPreprocessing", cursor_predicate_isPreprocessing},
    {"isUnexposed", cursor_predicate_isUnexposed},
    //
    {"isMacroFunctionLike", cursor_predicate_isMacroFunctionLike},
    {"isMacroBuiltin", cursor_predicate_isMacroBuiltin},
    {"isFunctionInlined", cursor_predicate_isFunctionInlined},
    {"isBitField", cursor_predicate_isBitField},
    {"isDynamicCall", cursor_predicate_isDynamicCall},
    {"isVariadic", cursor_predicate_isVariadic},
    {"isConvertingConstructor", cursor_predicate_isConvertingConstructor},
    {"isCopyConstructor", cursor_predicate_isCopyConstructor},
    {"isDefaultConstructor", cursor_predicate_isDefaultConstructor},
    {"isMoveConstructor", cursor_predicate_isMoveConstructor},
    {"isMutable", cursor_predicate_isMutable},
    {"isDefaulted", cursor_predicate_isDefaulted},
    {"isCursorDefinition", cursor_predicate_isCursorDefinition},
    {"isPureVirtual", cursor_predicate_isPureVirtual},
    {"isStatic", cursor_predicate_isStatic},
    {"isVirtual", cursor_predicate_isVirtual},
    {"isVirtualBase", cursor_predicate_isVirtualBase},
    {"isConst", cursor_predicate_isConst},
    {"ClassType", cursor_attribute_getClassType},
    {"NamedType", cursor_attribute_getNamedType},
    {"isConstQualifiedType", cursor_predicate_isConstQualifiedType},
    {"isVolatileQualifiedType", cursor_predicate_isVolatileQualifiedType},
    {"isRestrictQualifiedType", cursor_predicate_isRestrictQualifiedType},
    {"isFunctionTypeVariadic", cursor_predicate_isFunctionTypeVariadic},
    {"isPODType", cursor_predicate_isPODType},
    {"AlignOf", cursor_attribute_getAlignOf},
    {"SizeOf", cursor_attribute_getSizeOf},
    {"NumTemplateArguments", cursor_attribute_getNumTemplateArguments},
    {"NumArguments", cursor_attribute_getNumArguments},
    {"CXXRefQualifier", cursor_attribute_getCXXRefQualifier},
    {"StorageClass", cursor_attribute_getStorageClass}};

void add_data_from_map(const std::list<std::string> &chosen_attributes,
                       std::string &result, CXCursor cursor,
                       const std::string &string_indent) {
  for (auto &&attribute : chosen_attributes) {
    // cout << &attribute << endl;
    if (cursor_attribute_map.find(attribute) == cursor_attribute_map.end()) {
      std::cerr << "unknown attribute in the mix" << endl;
      continue;
    }
    std::string middle_space =
        std::string(get_offset(attribute.size() + 1), ' ');
    result += string_indent + "\"" + attribute + "\":" + middle_space + "\"" +
              cursor_attribute_map[attribute](cursor) + "\",\n";
  }
}

std::string string_attributes(CXCursor cursor,
                              std::pair<Options, int> *options_i) {
  size_t indent = options_i->second;
  std::string result(indent, '_');
  result += '\n';
  std::string string_indent(indent, ' ');
  add_data_from_map(options_i->first.chosen_attributes, result, cursor,
                    string_indent);
  return result;
}

using namespace std;

const string hline = "----------------------------------------";

CXChildVisitResult subtree_attribute(CXCursor cursor, CXCursor,
                                     CXClientData data) {
  pair<Options, int> *options_i = (pair<Options, int> *)data;
  options_i->second += 2;
  cout << string_attributes(cursor, options_i) << hline << endl;
  // cout << string_attributes(cursor) << hline << endl;
  clang_visitChildren(cursor, subtree_attribute, data);
  options_i->second -= 2;
  return CXChildVisit_Continue;
}

int main(int argc, char *argv[]) {

  std::list<std::string> attribute_list;
  for (auto &&attribute : cursor_attribute_map) {
    attribute_list.push_back(attribute.first);
  }

  Options options;
  try {
    if (!parse_options(options, attribute_list, argc, argv)) {
      std::string name = argv[0];
      // cout << "fuu: " + name << endl;
      cout << Options::help(name) << endl;
      // cout << options.dump() << "\n\n" << endl;
      return 1;
    }
  } catch (const std::length_error &e) {
    cout << e.what() << endl;
    // cout << Options::help(argv[0]) << endl;
    return 1;
  }
  cout << options.dump() << "\n\n" << endl;

  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit TU = clang_createTranslationUnitFromSourceFile(
      index, options.source.c_str(), 0, nullptr, 0, nullptr);
  CXCursor cursor;

  if (options.line == 0) {
    cout << "getting whole thing" << endl;
    cursor = clang_getTranslationUnitCursor(TU);
  } else {
    CXFile cxfile = clang_getFile(TU, options.source.c_str());
    CXSourceLocation location =
        clang_getLocation(TU, cxfile, options.line, options.col);
    cout << "cxlocation: " << string_location(location) << endl;
    cursor = clang_getCursor(TU, location);
  }

  int i = 2;
  pair<Options, int> data{options, i};
  cout << string_attributes(cursor, &data) << hline << endl;
  clang_visitChildren(cursor, subtree_attribute, &data);

  clang_disposeTranslationUnit(TU);
  clang_disposeIndex(index);
}
