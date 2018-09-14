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
CXString clang_getClangVersion(void);
CXString clang_getFileName(CXFile SFile);
CXString clang_getTypeSpelling(CXType CT);
CXString clang_getTypeKindSpelling(enum CXTypeKind K);
CXString clang_getCursorKindSpelling(enum CXCursorKind Kind);

CXString clang_getCursorUSR(CXCursor);
CXString clang_getCursorSpelling(CXCursor);
CXString clang_getCursorDisplayName(CXCursor);
CXString clang_Cursor_getRawCommentText(CXCursor C);
CXString clang_Cursor_getBriefCommentText(CXCursor C);
CXString clang_Cursor_getMangling(CXCursor);
*/

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
    {"SpecializedCursorTemplate", cursor_attribute_SpecializedCursorTemplate}};

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
      cout << Options::help(argv[0]) << endl;
      //cout << options.dump() << "\n\n" << endl;
      return 1;
    }
  } catch (...) {
    //cout << options.dump() << "\n\n" << endl;
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
