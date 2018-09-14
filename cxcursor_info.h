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

std::string cursor_predicate_hasAttributes(CXCursor cursor);
std::string cursor_predicate_isInSystemHeader(CXCursor cursor);
std::string cursor_predicate_isFromMainFile(CXCursor cursor);

std::string cursor_predicate_isDeclaration(CXCursor cursor);
std::string cursor_predicate_isReference(CXCursor cursor);
std::string cursor_predicate_isExpression(CXCursor cursor);
std::string cursor_predicate_isStatement(CXCursor cursor);
std::string cursor_predicate_isAttribute(CXCursor cursor);
std::string cursor_predicate_isInvalid(CXCursor cursor);
std::string cursor_predicate_isTranslationUnit(CXCursor cursor);
std::string cursor_predicate_isPreprocessing(CXCursor cursor);
std::string cursor_predicate_isUnexposed(CXCursor cursor);

std::string cursor_predicate_isMacroFunctionLike(CXCursor cursor);
std::string cursor_predicate_isMacroBuiltin(CXCursor cursor);
std::string cursor_predicate_isFunctionInlined(CXCursor cursor);
std::string cursor_predicate_isBitField(CXCursor cursor);
std::string cursor_predicate_isDynamicCall(CXCursor cursor);
std::string cursor_predicate_isVariadic(CXCursor cursor);
std::string cursor_predicate_isConvertingConstructor(CXCursor cursor);
std::string cursor_predicate_isCopyConstructor(CXCursor cursor);
std::string cursor_predicate_isDefaultConstructor(CXCursor cursor);
std::string cursor_predicate_isMoveConstructor(CXCursor cursor);
std::string cursor_predicate_isMutable(CXCursor cursor);
std::string cursor_predicate_isDefaulted(CXCursor cursor);
std::string cursor_predicate_isCursorDefinition(CXCursor cursor);
std::string cursor_predicate_isPureVirtual(CXCursor cursor);
std::string cursor_predicate_isStatic(CXCursor cursor);
std::string cursor_predicate_isVirtual(CXCursor cursor);
std::string cursor_predicate_isVirtualBase(CXCursor cursor);
std::string cursor_predicate_isConst(CXCursor cursor);

std::string cursor_attribute_getClassType(CXCursor cursor);
std::string cursor_attribute_getNamedType(CXCursor cursor);

std::string cursor_predicate_isConstQualifiedType(CXCursor cursor);
std::string cursor_predicate_isVolatileQualifiedType(CXCursor cursor);
std::string cursor_predicate_isRestrictQualifiedType(CXCursor cursor);
std::string cursor_predicate_isFunctionTypeVariadic(CXCursor cursor);
std::string cursor_predicate_isPODType(CXCursor cursor);

std::string cursor_attribute_getAlignOf(CXCursor cursor);
std::string cursor_attribute_getSizeOf(CXCursor cursor);

std::string cursor_attribute_getNumTemplateArguments(CXCursor cursor);

std::string cursor_attribute_getNumArguments(CXCursor cursor);
std::string cursor_attribute_getNumTemplateArguments(CXCursor cursor);

std::string cursor_attribute_getCXXRefQualifier(CXCursor cursor);
std::string cursor_attribute_getStorageClass(CXCursor cursor);


enum CX_StorageClass clang_Cursor_getStorageClass(CXCursor);

std::string string_attributes(CXCursor cursor, int indent = 0);
