// cursor_attribute.cc

#include "clang-c/Index.h"

#include <iostream>
#include <map>
#include <string>

using cursor_attributes = std::map<std::string, std::string>;
using cx_string_getter = CXString (*)(CXCursor);
using cx_type_getter = CXType (*)(CXCursor);
using cx_uint_getter = unsigned (*)(CXCursor);
using cx_std_string = std::string (*)(CXCursor);

enum class CXReturnType {
  uinteger = 0,
  cxstring = 1,
  cxtype = 2,
  cx_std_string = 3
};

struct CXAttributeGetter {
  CXReturnType cxReturnType;
  union {
    cx_string_getter get_cx_string;
    cx_uint_getter get_uint;
    cx_type_getter get_type;
    cx_std_string get_std_string;
  };

  CXAttributeGetter(CXReturnType, cx_string_getter);
  CXAttributeGetter(CXReturnType, cx_uint_getter);
  CXAttributeGetter(CXReturnType, cx_type_getter);
  CXAttributeGetter(CXReturnType, cx_std_string);
  ~CXAttributeGetter() {}
};

CXAttributeGetter::CXAttributeGetter(CXReturnType r, cx_string_getter g)
    : cxReturnType(r), get_cx_string(g) {}

CXAttributeGetter::CXAttributeGetter(CXReturnType r, cx_uint_getter g)
    : cxReturnType(r), get_uint(g) {}

CXAttributeGetter::CXAttributeGetter(CXReturnType r, cx_type_getter g)
    : cxReturnType(r), get_type(g) {}

CXAttributeGetter::CXAttributeGetter(CXReturnType r, cx_std_string g)
    : cxReturnType(r), get_std_string(g) {}

std::string get_attribute(const CXAttributeGetter &getter, CXCursor cursor);

using namespace std;

std::string get_kind_spelling(CXCursor cursor) {
  switch (cursor.kind) {
  case CXCursor_UnexposedDecl: {
    return "CXCursor_UnexposedDecl";
  } break;
  case CXCursor_StructDecl: {
    return "CXCursor_StructDecl";
  } break;
  case CXCursor_UnionDecl: {
    return "CXCursor_UnionDecl";
  } break;
  case CXCursor_ClassDecl: {
    return "CXCursor_ClassDecl";
  } break;
  case CXCursor_EnumDecl: {
    return "CXCursor_EnumDecl";
  } break;
  case CXCursor_FieldDecl: {
    return "CXCursor_FieldDecl";
  } break;
  case CXCursor_EnumConstantDecl: {
    return "CXCursor_EnumConstantDecl";
  } break;
  case CXCursor_FunctionDecl: {
    return "CXCursor_FunctionDecl";
  } break;
  case CXCursor_VarDecl: {
    return "CXCursor_VarDecl";
  } break;
  case CXCursor_ParmDecl: {
    return "CXCursor_ParmDecl";
  } break;
  case CXCursor_ObjCInterfaceDecl: {
    return "CXCursor_ObjCInterfaceDecl";
  } break;
  case CXCursor_ObjCCategoryDecl: {
    return "CXCursor_ObjCCategoryDecl";
  } break;
  case CXCursor_ObjCProtocolDecl: {
    return "CXCursor_ObjCProtocolDecl";
  } break;
  case CXCursor_ObjCPropertyDecl: {
    return "CXCursor_ObjCPropertyDecl";
  } break;
  case CXCursor_ObjCIvarDecl: {
    return "CXCursor_ObjCIvarDecl";
  } break;
  case CXCursor_ObjCInstanceMethodDecl: {
    return "CXCursor_ObjCInstanceMethodDecl";
  } break;
  case CXCursor_ObjCClassMethodDecl: {
    return "CXCursor_ObjCClassMethodDecl";
  } break;
  case CXCursor_ObjCImplementationDecl: {
    return "CXCursor_ObjCImplementationDecl";
  } break;
  case CXCursor_ObjCCategoryImplDecl: {
    return "CXCursor_ObjCCategoryImplDecl";
  } break;
  case CXCursor_TypedefDecl: {
    return "CXCursor_TypedefDecl";
  } break;
  case CXCursor_CXXMethod: {
    return "CXCursor_CXXMethod";
  } break;
  case CXCursor_Namespace: {
    return "CXCursor_Namespace";
  } break;
  case CXCursor_LinkageSpec: {
    return "CXCursor_LinkageSpec";
  } break;
  case CXCursor_Constructor: {
    return "CXCursor_Constructor";
  } break;
  case CXCursor_Destructor: {
    return "CXCursor_Destructor";
  } break;
  case CXCursor_ConversionFunction: {
    return "CXCursor_ConversionFunction";
  } break;
  case CXCursor_TemplateTypeParameter: {
    return "CXCursor_TemplateTypeParameter";
  } break;
  case CXCursor_NonTypeTemplateParameter: {
    return "CXCursor_NonTypeTemplateParameter";
  } break;
  case CXCursor_TemplateTemplateParameter: {
    return "CXCursor_TemplateTemplateParameter";
  } break;
  case CXCursor_FunctionTemplate: {
    return "CXCursor_FunctionTemplate";
  } break;
  case CXCursor_ClassTemplate: {
    return "CXCursor_ClassTemplate";
  } break;
  case CXCursor_ClassTemplatePartialSpecialization: {
    return "CXCursor_ClassTemplatePartialSpecialization";
  } break;
  case CXCursor_NamespaceAlias: {
    return "CXCursor_NamespaceAlias";
  } break;
  case CXCursor_UsingDirective: {
    return "CXCursor_UsingDirective";
  } break;
  case CXCursor_UsingDeclaration: {
    return "CXCursor_UsingDeclaration";
  } break;
  case CXCursor_TypeAliasDecl: {
    return "CXCursor_TypeAliasDecl";
  } break;
  case CXCursor_ObjCSynthesizeDecl: {
    return "CXCursor_ObjCSynthesizeDecl";
  } break;
  case CXCursor_ObjCDynamicDecl: {
    return "CXCursor_ObjCDynamicDecl";
  } break;
  case CXCursor_CXXAccessSpecifier: {
    return "CXCursor_CXXAccessSpecifier";
  } break;
  case CXCursor_ObjCSuperClassRef: {
    return "CXCursor_ObjCSuperClassRef";
  } break;
  case CXCursor_ObjCProtocolRef: {
    return "CXCursor_ObjCProtocolRef";
  } break;
  case CXCursor_ObjCClassRef: {
    return "CXCursor_ObjCClassRef";
  } break;
  case CXCursor_TypeRef: {
    return "CXCursor_TypeRef";
  } break;
  case CXCursor_CXXBaseSpecifier: {
    return "CXCursor_CXXBaseSpecifier";
  } break;
  case CXCursor_TemplateRef: {
    return "CXCursor_TemplateRef";
  } break;
  case CXCursor_NamespaceRef: {
    return "CXCursor_NamespaceRef";
  } break;
  case CXCursor_MemberRef: {
    return "CXCursor_MemberRef";
  } break;
  case CXCursor_LabelRef: {
    return "CXCursor_LabelRef";
  } break;
  case CXCursor_OverloadedDeclRef: {
    return "CXCursor_OverloadedDeclRef";
  } break;
  case CXCursor_VariableRef: {
    return "CXCursor_VariableRef";
  } break;
  case CXCursor_FirstInvalid: {
    return "CXCursor_FirstInvalid";
  } break;
  case CXCursor_NoDeclFound: {
    return "CXCursor_NoDeclFound";
  } break;
  case CXCursor_NotImplemented: {
    return "CXCursor_NotImplemented";
  } break;
  case CXCursor_InvalidCode: {
    return "CXCursor_InvalidCode";
  } break;
  case CXCursor_FirstExpr: {
    return "CXCursor_FirstExpr";
  } break;
  case CXCursor_DeclRefExpr: {
    return "CXCursor_DeclRefExpr";
  } break;
  case CXCursor_MemberRefExpr: {
    return "CXCursor_MemberRefExpr";
  } break;
  case CXCursor_CallExpr: {
    return "CXCursor_CallExpr";
  } break;
  case CXCursor_ObjCMessageExpr: {
    return "CXCursor_ObjCMessageExpr";
  } break;
  case CXCursor_BlockExpr: {
    return "CXCursor_BlockExpr";
  } break;
  case CXCursor_IntegerLiteral: {
    return "CXCursor_IntegerLiteral";
  } break;
  case CXCursor_FloatingLiteral: {
    return "CXCursor_FloatingLiteral";
  } break;
  case CXCursor_ImaginaryLiteral: {
    return "CXCursor_ImaginaryLiteral";
  } break;
  case CXCursor_StringLiteral: {
    return "CXCursor_StringLiteral";
  } break;
  case CXCursor_CharacterLiteral: {
    return "CXCursor_CharacterLiteral";
  } break;
  case CXCursor_ParenExpr: {
    return "CXCursor_ParenExpr";
  } break;
  case CXCursor_UnaryOperator: {
    return "CXCursor_UnaryOperator";
  } break;
  case CXCursor_ArraySubscriptExpr: {
    return "CXCursor_ArraySubscriptExpr";
  } break;
  case CXCursor_BinaryOperator: {
    return "CXCursor_BinaryOperator";
  } break;
  case CXCursor_CompoundAssignOperator: {
    return "CXCursor_CompoundAssignOperator";
  } break;
  case CXCursor_ConditionalOperator: {
    return "CXCursor_ConditionalOperator";
  } break;
  case CXCursor_CStyleCastExpr: {
    return "CXCursor_CStyleCastExpr";
  } break;
  case CXCursor_CompoundLiteralExpr: {
    return "CXCursor_CompoundLiteralExpr";
  } break;
  case CXCursor_InitListExpr: {
    return "CXCursor_InitListExpr";
  } break;
  case CXCursor_AddrLabelExpr: {
    return "CXCursor_AddrLabelExpr";
  } break;
  case CXCursor_StmtExpr: {
    return "CXCursor_StmtExpr";
  } break;
  case CXCursor_GenericSelectionExpr: {
    return "CXCursor_GenericSelectionExpr";
  } break;
  case CXCursor_GNUNullExpr: {
    return "CXCursor_GNUNullExpr";
  } break;
  case CXCursor_CXXStaticCastExpr: {
    return "CXCursor_CXXStaticCastExpr";
  } break;
  case CXCursor_CXXDynamicCastExpr: {
    return "CXCursor_CXXDynamicCastExpr";
  } break;
  case CXCursor_CXXReinterpretCastExpr: {
    return "CXCursor_CXXReinterpretCastExpr";
  } break;
  case CXCursor_CXXConstCastExpr: {
    return "CXCursor_CXXConstCastExpr";
  } break;
  case CXCursor_CXXFunctionalCastExpr: {
    return "CXCursor_CXXFunctionalCastExpr";
  } break;
  case CXCursor_CXXTypeidExpr: {
    return "CXCursor_CXXTypeidExpr";
  } break;
  case CXCursor_CXXBoolLiteralExpr: {
    return "CXCursor_CXXBoolLiteralExpr";
  } break;
  case CXCursor_CXXNullPtrLiteralExpr: {
    return "CXCursor_CXXNullPtrLiteralExpr";
  } break;
  case CXCursor_CXXThisExpr: {
    return "CXCursor_CXXThisExpr";
  } break;
  case CXCursor_CXXThrowExpr: {
    return "CXCursor_CXXThrowExpr";
  } break;
  case CXCursor_CXXNewExpr: {
    return "CXCursor_CXXNewExpr";
  } break;
  case CXCursor_CXXDeleteExpr: {
    return "CXCursor_CXXDeleteExpr";
  } break;
  case CXCursor_UnaryExpr: {
    return "CXCursor_UnaryExpr";
  } break;
  case CXCursor_ObjCStringLiteral: {
    return "CXCursor_ObjCStringLiteral";
  } break;
  case CXCursor_ObjCEncodeExpr: {
    return "CXCursor_ObjCEncodeExpr";
  } break;
  case CXCursor_ObjCSelectorExpr: {
    return "CXCursor_ObjCSelectorExpr";
  } break;
  case CXCursor_ObjCProtocolExpr: {
    return "CXCursor_ObjCProtocolExpr";
  } break;
  case CXCursor_ObjCBridgedCastExpr: {
    return "CXCursor_ObjCBridgedCastExpr";
  } break;
  case CXCursor_PackExpansionExpr: {
    return "CXCursor_PackExpansionExpr";
  } break;
  case CXCursor_SizeOfPackExpr: {
    return "CXCursor_SizeOfPackExpr";
  } break;
  case CXCursor_LambdaExpr: {
    return "CXCursor_LambdaExpr";
  } break;
  case CXCursor_ObjCSelfExpr: {
    return "CXCursor_ObjCSelfExpr";
  } break;
  case CXCursor_OMPArraySectionExpr: {
    return "CXCursor_OMPArraySectionExpr";
  } break;
  case CXCursor_ObjCAvailabilityCheckExpr: {
    return "CXCursor_ObjCAvailabilityCheckExpr";
  } break;
  case CXCursor_FirstStmt: {
    return "CXCursor_FirstStmt";
  } break;
  case CXCursor_CaseStmt: {
    return "CXCursor_CaseStmt";
  } break;
  case CXCursor_WhileStmt: {
    return "CXCursor_WhileStmt";
  } break;
  case CXCursor_DoStmt: {
    return "CXCursor_DoStmt";
  } break;
  case CXCursor_ForStmt: {
    return "CXCursor_ForStmt";
  } break;
  case CXCursor_GotoStmt: {
    return "CXCursor_GotoStmt";
  } break;
  case CXCursor_IndirectGotoStmt: {
    return "CXCursor_IndirectGotoStmt";
  } break;
  case CXCursor_ContinueStmt: {
    return "CXCursor_ContinueStmt";
  } break;
  case CXCursor_BreakStmt: {
    return "CXCursor_BreakStmt";
  } break;
  case CXCursor_ReturnStmt: {
    return "CXCursor_ReturnStmt";
  } break;
  case CXCursor_GCCAsmStmt: {
    return "CXCursor_GCCAsmStmt";
  } break;
  case CXCursor_ObjCAtTryStmt: {
    return "CXCursor_ObjCAtTryStmt";
  } break;
  case CXCursor_ObjCAtCatchStmt: {
    return "CXCursor_ObjCAtCatchStmt";
  } break;
  case CXCursor_ObjCAtFinallyStmt: {
    return "CXCursor_ObjCAtFinallyStmt";
  } break;
  case CXCursor_ObjCAtThrowStmt: {
    return "CXCursor_ObjCAtThrowStmt";
  } break;
  case CXCursor_ObjCAtSynchronizedStmt: {
    return "CXCursor_ObjCAtSynchronizedStmt";
  } break;
  case CXCursor_ObjCAutoreleasePoolStmt: {
    return "CXCursor_ObjCAutoreleasePoolStmt";
  } break;
  case CXCursor_ObjCForCollectionStmt: {
    return "CXCursor_ObjCForCollectionStmt";
  } break;
  case CXCursor_CXXCatchStmt: {
    return "CXCursor_CXXCatchStmt";
  } break;
  case CXCursor_CXXTryStmt: {
    return "CXCursor_CXXTryStmt";
  } break;
  case CXCursor_CXXForRangeStmt: {
    return "CXCursor_CXXForRangeStmt";
  } break;
  case CXCursor_SEHTryStmt: {
    return "CXCursor_SEHTryStmt";
  } break;
  case CXCursor_SEHExceptStmt: {
    return "CXCursor_SEHExceptStmt";
  } break;
  case CXCursor_SEHFinallyStmt: {
    return "CXCursor_SEHFinallyStmt";
  } break;
  case CXCursor_MSAsmStmt: {
    return "CXCursor_MSAsmStmt";
  } break;
  case CXCursor_NullStmt: {
    return "CXCursor_NullStmt";
  } break;
  case CXCursor_DeclStmt: {
    return "CXCursor_DeclStmt";
  } break;
  case CXCursor_OMPParallelDirective: {
    return "CXCursor_OMPParallelDirective";
  } break;
  case CXCursor_OMPSimdDirective: {
    return "CXCursor_OMPSimdDirective";
  } break;
  case CXCursor_OMPForDirective: {
    return "CXCursor_OMPForDirective";
  } break;
  case CXCursor_OMPSectionsDirective: {
    return "CXCursor_OMPSectionsDirective";
  } break;
  case CXCursor_OMPSectionDirective: {
    return "CXCursor_OMPSectionDirective";
  } break;
  case CXCursor_OMPSingleDirective: {
    return "CXCursor_OMPSingleDirective";
  } break;
  case CXCursor_OMPParallelForDirective: {
    return "CXCursor_OMPParallelForDirective";
  } break;
  case CXCursor_OMPParallelSectionsDirective: {
    return "CXCursor_OMPParallelSectionsDirective";
  } break;
  case CXCursor_OMPTaskDirective: {
    return "CXCursor_OMPTaskDirective";
  } break;
  case CXCursor_OMPMasterDirective: {
    return "CXCursor_OMPMasterDirective";
  } break;
  case CXCursor_OMPCriticalDirective: {
    return "CXCursor_OMPCriticalDirective";
  } break;
  case CXCursor_OMPTaskyieldDirective: {
    return "CXCursor_OMPTaskyieldDirective";
  } break;
  case CXCursor_OMPBarrierDirective: {
    return "CXCursor_OMPBarrierDirective";
  } break;
  case CXCursor_OMPTaskwaitDirective: {
    return "CXCursor_OMPTaskwaitDirective";
  } break;
  case CXCursor_OMPFlushDirective: {
    return "CXCursor_OMPFlushDirective";
  } break;
  case CXCursor_SEHLeaveStmt: {
    return "CXCursor_SEHLeaveStmt";
  } break;
  case CXCursor_OMPOrderedDirective: {
    return "CXCursor_OMPOrderedDirective";
  } break;
  case CXCursor_OMPAtomicDirective: {
    return "CXCursor_OMPAtomicDirective";
  } break;
  case CXCursor_OMPForSimdDirective: {
    return "CXCursor_OMPForSimdDirective";
  } break;
  case CXCursor_OMPParallelForSimdDirective: {
    return "CXCursor_OMPParallelForSimdDirective";
  } break;
  case CXCursor_OMPTargetDirective: {
    return "CXCursor_OMPTargetDirective";
  } break;
  case CXCursor_OMPTeamsDirective: {
    return "CXCursor_OMPTeamsDirective";
  } break;
  case CXCursor_OMPTaskgroupDirective: {
    return "CXCursor_OMPTaskgroupDirective";
  } break;
  case CXCursor_OMPCancellationPointDirective: {
    return "CXCursor_OMPCancellationPointDirective";
  } break;
  case CXCursor_OMPCancelDirective: {
    return "CXCursor_OMPCancelDirective";
  } break;
  case CXCursor_OMPTargetDataDirective: {
    return "CXCursor_OMPTargetDataDirective";
  } break;
  case CXCursor_OMPTaskLoopDirective: {
    return "CXCursor_OMPTaskLoopDirective";
  } break;
  case CXCursor_OMPTaskLoopSimdDirective: {
    return "CXCursor_OMPTaskLoopSimdDirective";
  } break;
  case CXCursor_OMPDistributeDirective: {
    return "CXCursor_OMPDistributeDirective";
  } break;
  case CXCursor_OMPTargetEnterDataDirective: {
    return "CXCursor_OMPTargetEnterDataDirective";
  } break;
  case CXCursor_OMPTargetExitDataDirective: {
    return "CXCursor_OMPTargetExitDataDirective";
  } break;
  case CXCursor_OMPTargetParallelDirective: {
    return "CXCursor_OMPTargetParallelDirective";
  } break;
  case CXCursor_OMPTargetParallelForDirective: {
    return "CXCursor_OMPTargetParallelForDirective";
  } break;
  case CXCursor_OMPTargetUpdateDirective: {
    return "CXCursor_OMPTargetUpdateDirective";
  } break;
  case CXCursor_OMPDistributeParallelForDirective: {
    return "CXCursor_OMPDistributeParallelForDirective";
  } break;
  case CXCursor_OMPDistributeParallelForSimdDirective: {
    return "CXCursor_OMPDistributeParallelForSimdDirective";
  } break;
  case CXCursor_OMPDistributeSimdDirective: {
    return "CXCursor_OMPDistributeSimdDirective";
  } break;
  case CXCursor_OMPTargetParallelForSimdDirective: {
    return "CXCursor_OMPTargetParallelForSimdDirective";
  } break;
  case CXCursor_OMPTargetSimdDirective: {
    return "CXCursor_OMPTargetSimdDirective";
  } break;
  case CXCursor_OMPTeamsDistributeDirective: {
    return "CXCursor_OMPTeamsDistributeDirective";
  } break;
  case CXCursor_OMPTeamsDistributeSimdDirective: {
    return "CXCursor_OMPTeamsDistributeSimdDirective";
  } break;
  case CXCursor_OMPTeamsDistributeParallelForSimdDirective: {
    return "CXCursor_OMPTeamsDistributeParallelForSimdDirective";
  } break;
  case CXCursor_OMPTeamsDistributeParallelForDirective: {
    return "CXCursor_OMPTeamsDistributeParallelForDirective";
  } break;
  case CXCursor_OMPTargetTeamsDirective: {
    return "CXCursor_OMPTargetTeamsDirective";
  } break;
  case CXCursor_OMPTargetTeamsDistributeDirective: {
    return "CXCursor_OMPTargetTeamsDistributeDirective";
  } break;
  case CXCursor_OMPTargetTeamsDistributeParallelForDirective: {
    return "CXCursor_OMPTargetTeamsDistributeParallelForDirective";
  } break;
  case CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective: {
    return "CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective";
  } break;
  case CXCursor_OMPTargetTeamsDistributeSimdDirective: {
    return "CXCursor_OMPTargetTeamsDistributeSimdDirective";
  } break;
  case CXCursor_TranslationUnit: {
    return "CXCursor_TranslationUnit";
  } break;
  case CXCursor_FirstAttr: {
    return "CXCursor_FirstAttr";
  } break;
  case CXCursor_IBActionAttr: {
    return "CXCursor_IBActionAttr";
  } break;
  case CXCursor_IBOutletAttr: {
    return "CXCursor_IBOutletAttr";
  } break;
  case CXCursor_IBOutletCollectionAttr: {
    return "CXCursor_IBOutletCollectionAttr";
  } break;
  case CXCursor_CXXFinalAttr: {
    return "CXCursor_CXXFinalAttr";
  } break;
  case CXCursor_CXXOverrideAttr: {
    return "CXCursor_CXXOverrideAttr";
  } break;
  case CXCursor_AnnotateAttr: {
    return "CXCursor_AnnotateAttr";
  } break;
  case CXCursor_AsmLabelAttr: {
    return "CXCursor_AsmLabelAttr";
  } break;
  case CXCursor_PackedAttr: {
    return "CXCursor_PackedAttr";
  } break;
  case CXCursor_PureAttr: {
    return "CXCursor_PureAttr";
  } break;
  case CXCursor_ConstAttr: {
    return "CXCursor_ConstAttr";
  } break;
  case CXCursor_NoDuplicateAttr: {
    return "CXCursor_NoDuplicateAttr";
  } break;
  case CXCursor_CUDAConstantAttr: {
    return "CXCursor_CUDAConstantAttr";
  } break;
  case CXCursor_CUDADeviceAttr: {
    return "CXCursor_CUDADeviceAttr";
  } break;
  case CXCursor_CUDAGlobalAttr: {
    return "CXCursor_CUDAGlobalAttr";
  } break;
  case CXCursor_CUDAHostAttr: {
    return "CXCursor_CUDAHostAttr";
  } break;
  case CXCursor_CUDASharedAttr: {
    return "CXCursor_CUDASharedAttr";
  } break;
  case CXCursor_VisibilityAttr: {
    return "CXCursor_VisibilityAttr";
  } break;
  case CXCursor_DLLExport: {
    return "CXCursor_DLLExport";
  } break;
  case CXCursor_DLLImport: {
    return "CXCursor_DLLImport";
  } break;
  case CXCursor_PreprocessingDirective: {
    return "CXCursor_PreprocessingDirective";
  } break;
  case CXCursor_MacroDefinition: {
    return "CXCursor_MacroDefinition";
  } break;
  case CXCursor_MacroExpansion: {
    return "CXCursor_MacroExpansion";
  } break;
  case CXCursor_InclusionDirective: {
    return "CXCursor_InclusionDirective";
  } break;
  case CXCursor_ModuleImportDecl: {
    return "CXCursor_ModuleImportDecl";
  } break;
  case CXCursor_TypeAliasTemplateDecl: {
    return "CXCursor_TypeAliasTemplateDecl";
  } break;
  case CXCursor_StaticAssert: {
    return "CXCursor_StaticAssert";
  } break;
  case CXCursor_FriendDecl: {
    return "CXCursor_FriendDecl";
  } break;
  case CXCursor_OverloadCandidate: {
    return "CXCursor_OverloadCandidate";
  } break;
  case CXCursor_ObjCBoolLiteralExpr: {
    return "CXCursor_ObjCBoolLiteralExpr";
  } break;
  case CXCursor_LabelStmt: {
    return "CXCursor_LabelStmt";
  } break;
  case CXCursor_CompoundStmt: {
    return "CXCursor_CompoundStmt";
  } break;
  case CXCursor_DefaultStmt: {
    return "CXCursor_DefaultStmt";
  } break;
  case CXCursor_IfStmt: {
    return "CXCursor_IfStmt";
  } break;
  case CXCursor_SwitchStmt: {
    return "CXCursor_SwitchStmt";
  } break;
  default: { return "Couldn't determine kind"; }
  }
}

std::string convert_cxstring(CXCursor cursor,
                             CXString (*cx_string_getter)(CXCursor)) {
  CXString cx_str = cx_string_getter(cursor);
  string result = clang_getCString(cx_str);
  clang_disposeString(cx_str);
  return result;
}

std::string get_attribute(const CXAttributeGetter &getter, CXCursor cursor) {
  string result;
  switch (getter.cxReturnType) {
  case CXReturnType::uinteger: {
    result = ::to_string(getter.get_uint(cursor));
  }; break;
  case CXReturnType::cxstring: {
    result = convert_cxstring(cursor, getter.get_cx_string);
  }; break;
  case CXReturnType::cxtype: {
    CXType cxtype = clang_getCursorType(cursor);
    CXString cx_str = clang_getTypeSpelling(cxtype);
    result = clang_getCString(cx_str);
    clang_disposeString(cx_str);
  }; break;
  case CXReturnType::cx_std_string: {
    result = getter.get_std_string(cursor);
  } break;
  }
  return result;
}

std::map<cursor_attributes::key_type, CXAttributeGetter> attribute_getter_map =
    {{"USR", {CXReturnType::cxstring, clang_getCursorUSR}},
     {"Spelling", {CXReturnType::cxstring, clang_getCursorSpelling}},
     {"DisplayName", {CXReturnType::cxstring, clang_getCursorDisplayName}},
     {"Kind", {CXReturnType::cx_std_string, get_kind_spelling}},
     {"Type", {CXReturnType::cxtype, clang_getCursorType}},
     {"is_definition", {CXReturnType::uinteger, clang_isCursorDefinition}}};

cursor_attributes get_cursor_attributes(CXCursor cursor) {
  cursor_attributes result;
  for (auto &&att_get : attribute_getter_map) {
    result[att_get.first] = get_attribute(att_get.second, cursor);
  }
  return result;
}

void dump_cursor_attributes(const cursor_attributes& attributes) {
  for (auto&& att_val : attributes) {
    cout << "\"" + att_val.first + "\": \"" + att_val.second + "\"," << endl;
  }
  cout << "----------------------------------------" << endl;
}

CXChildVisitResult visit_cursor_attributes(CXCursor cursor, CXCursor, CXClientData) {
  dump_cursor_attributes(get_cursor_attributes(cursor));
  return CXChildVisit_Recurse;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "enter file name to analyze" << endl;
    return 1;
  }
  CXIndex index = clang_createIndex(0,0);
  CXTranslationUnit TU = clang_createTranslationUnitFromSourceFile(
      index,
      argv[1],
      0,
      nullptr,
      0,
      nullptr);

  clang_visitChildren(clang_getTranslationUnitCursor(TU), visit_cursor_attributes, nullptr);
  clang_disposeTranslationUnit(TU);
  clang_disposeIndex(index);

}
