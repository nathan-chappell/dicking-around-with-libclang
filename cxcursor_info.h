//cxcursor_info.h
#pragma once

#include "clang-c/Index.h"

#include <unordered_map>
#include <string>

std::string convert_cxstring(CXString);

std::string convert_cxstring(CXString cxstring);
std::string str_time(time_t t);
std::string string_ClangVersion(void);
std::string string_FileName(CXFile SFile);
std::string string_location(CXSourceLocation location);

std::string cursor_attribute_CustomId(CXCursor cursor);
std::string cursor_attribute_TypeSpelling(CXCursor cursor);
std::string cursor_attribute_TypeKindSpelling(CXCursor cursor);
std::string cursor_attribute_CursorUSR(CXCursor cursor);
std::string cursor_attribute_CursorSpelling(CXCursor cursor);
std::string cursor_attribute_CursorDisplayName(CXCursor cursor);
std::string cursor_attribute_CursorKindSpelling(CXCursor cursor);
std::string cursor_attribute_RawCommentText(CXCursor cursor);
std::string cursor_attribute_BriefCommentText(CXCursor cursor);
std::string cursor_attribute_Mangling(CXCursor cursor);
std::string cursor_attribute_location(CXCursor cursor);
std::string cursor_attribute_SemanticParent(CXCursor cursor);
std::string cursor_attribute_LexicalParent(CXCursor cursor);
std::string cursor_attribute_Referenced(CXCursor cursor);
std::string cursor_attribute_Definition(CXCursor cursor);
std::string cursor_attribute_CanonicalCursor(CXCursor cursor);
std::string cursor_attribute_SpecializedCursorTemplate(CXCursor cursor);

std::string string_attributes(CXCursor cursor, int indent = 0);
