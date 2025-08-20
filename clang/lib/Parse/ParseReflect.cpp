//===--- ParseReflect.cpp - C++2c Reflection Parsing (P2996) --------------===//
//
// Copyright 2024 Bloomberg Finance L.P.
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements parsing for reflection facilities.
//
//===----------------------------------------------------------------------===//

#include "clang/AST/LocInfoType.h"
#include "clang/Basic/DiagnosticParse.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/RAIIObjectsForParser.h"
#include "clang/Sema/EnterExpressionEvaluationContext.h"
using namespace clang;

ExprResult Parser::ParseRawTokenSequence()
{
  return ExprError();
}

ExprResult Parser::ParseCXXDelayedParsedExpression()  {
  //parse as: blueprintexpr []{}{ token-sequence }
  assert(Tok.is(tok::kw_blueprintexpr));
  SourceLocation BeginLoc = Tok.getLocation();
  ConsumeToken();
  //TODO(D0000): Parsing the lambda directly is wrong, as it parses the whole decleration. We have to use the ActOn logic directly
  // Parse lambda
  ExprResult EmptyLambdaOrErr = ParseLambdaExpression();
  if (EmptyLambdaOrErr.isInvalid())
    return ExprError();

  LambdaExpr* Lambda = EmptyLambdaOrErr.getAs<LambdaExpr>();


  assert(Tok.is(tok::l_brace));
  StealingTentativeParsingAction TokenCacher(*this);
  BalancedDelimiterTracker Braces(*this, tok::l_brace);
  Braces.consumeOpen();

  //TODO(D0000): Capture some syntax errors early, like {(}
  //TODO(D0000): Enforce an ending semicolon {token-seq;} so that the skipUntil wouldn't consume everything if there is a syntax error
  // Note this is ok thing to do if this is always a single decleration
  // A syntax error inside this is dangerous, because we enter this token stream later, which will have different errors everywhere
  // Catching these early is also a starting point for fragemnt parsing
  // Also note this is hard to do without tracking nesting of all delimiters at once. We need a new RAII type just for that
  bool SyntaxIsOk = SkipUntil(tok::r_brace, StopBeforeMatch);
  if (!SyntaxIsOk)
  {
    TokenCacher.Revert();
    return ExprError();
  }
  Token Last = Tok;
  // get cached tokens and do a trivial copy of them into a data structure to be iterated by Sema
  auto CachedTokens = TokenCacher.CommitAndSteal();
  Braces.consumeClose();

  //TODO: Pass brace location to the AST type
  auto Result = Actions.ActOnCXXDelayedParsedExpr(Lambda, CachedTokens, 
    Lambda->containsUnexpandedParameterPack());

  if (Result.isInvalid())
  {
    return Result;
  }

  Last.setKind(tok::annot_token_sequence);
  Last.setAnnotationValue(Result.getAsOpaquePointer());
  Last.setLocation(BeginLoc);
  Last.setAnnotationEndLoc(Braces.getCloseLocation());
  PP.AnnotateCachedTokens(Last);  
  return Result;
}

ExprResult Parser::ParseCXXReflectExpression(SourceLocation OpLoc) {
  SourceLocation OperandLoc = Tok.getLocation();

  Sema::ConstevalOnlyRecorder RecordConstevalOnly(Actions);
  EnterExpressionEvaluationContext EvalContext(
        Actions, Sema::ExpressionEvaluationContext::ReflectionContext);

  // Parse a leading nested-name-specifier, e.g.,
  //
  CXXScopeSpec SS;
  if (ParseOptionalCXXScopeSpecifier(SS, /*ObjectType=*/nullptr,
                                     /*ObjectHasErrors=*/false,
                                     /*EnteringContext=*/false)) {
    SkipUntil(tok::semi, StopAtSemi | StopBeforeMatch);
    return ExprError();
  }

  // Start the tentative parse: This will be reverted if the operand is found
  // to be a type (or rather: a type whose name is more complicated than a
  // single identifier).
  //
  TentativeParsingAction TentativeAction(*this);

  // Next, check for an unqualified-id.
  if (Tok.isOneOf(tok::identifier, tok::kw_operator, tok::kw_template,
                  tok::tilde, tok::annot_template_id)) {
    // Try parsing the operand name as an 'unqualified-id'.

    SourceLocation TemplateKWLoc;
    UnqualifiedId UnqualName;
    if (!ParseUnqualifiedId(SS, ParsedType{}, /*ObjectHadError=*/false,
                            /*EnteringContext=*/false,
                            /*AllowDestructorName=*/true,
                            /*AllowConstructorName=*/false,
                            /*AllowDeductionGuide=*/false,
                            SS.isSet() ? &TemplateKWLoc : nullptr,
                            UnqualName)) {
      bool AssumeType = false;
      if (UnqualName.getKind() == UnqualifiedIdKind::IK_TemplateId &&
          UnqualName.TemplateId->Kind == TNK_Type_template)
        AssumeType = true;
      else if (Tok.isOneOf(tok::l_square, tok::l_paren, tok::star, tok::amp,
                           tok::ampamp, tok::kw_const, tok::kw_volatile,
                           tok::kw_restrict))
        AssumeType = true;

      if (!AssumeType) {
        TentativeAction.Commit();
        return RecordConstevalOnly.RecordAndReturn(
                Actions.ActOnCXXReflectExpr(OpLoc, TemplateKWLoc, SS,
                                            UnqualName));
      }
    }
  } else if (SS.isValid() &&
             SS.getScopeRep()->getKind() == NestedNameSpecifier::Global) {
    // Check for '^::'.
    TentativeAction.Commit();

    Decl *TUDecl = Actions.getASTContext().getTranslationUnitDecl();
    return RecordConstevalOnly.RecordAndReturn(
            Actions.ActOnCXXReflectExpr(OpLoc, SourceLocation(), TUDecl));
  }
  TentativeAction.Revert();

  if (SS.isSet() &&
      TryAnnotateTypeOrScopeTokenAfterScopeSpec(SS, true,
                                                ImplicitTypenameContext::No)) {
    SkipUntil(tok::semi, StopAtSemi | StopBeforeMatch);
    return ExprError();
  }

  // Anything else must be a type-id (e.g., 'const int', 'Cls(*)(int)'.
  if (isCXXTypeId(TentativeCXXTypeIdContext::AsReflectionOperand)) {
    TypeResult TR = ParseTypeName(nullptr, DeclaratorContext::ReflectOperator);
    if (TR.isInvalid())
      return ExprError();

    std::string refKind;
    if (QualType QT = cast<LocInfoType>(TR.get().get())->getType();
        QT->isLValueReferenceType()) {
      refKind = "&";
    } else if (QT->isRValueReferenceType()) {
      refKind = "&&";
    } else if (auto *FPT = dyn_cast<FunctionProtoType>(QT)) {
      if (FPT->getRefQualifier() == RQ_LValue)
        refKind = "&";
      else if (FPT->getRefQualifier() == RQ_RValue)
        refKind = "&&";
    }

    if (!refKind.empty() &&
        !Tok.isOneOf(tok::r_paren, tok::greater, tok::greatergreater,
                     tok::comma, tok::r_brace, tok::r_square, tok::r_splice,
                     tok::semi, tok::ellipsis, tok::colon, tok::question)) {
      TypeLoc TL = cast<LocInfoType>(TR.get().get())
          ->getTypeSourceInfo()->getTypeLoc();

      Diag(OperandLoc, diag::warn_meant_parenthesize_reflection)
        << refKind << TL.getSourceRange();
    }

    return RecordConstevalOnly.RecordAndReturn(
            Actions.ActOnCXXReflectExpr(OpLoc, TR));
  }

  Diag(OperandLoc, diag::err_cannot_reflect_operand);
  return ExprError();
}

ExprResult Parser::ParseCXXMetafunctionExpression() {
  assert(Tok.is(tok::kw___metafunction) && "expected '___metafunction'");
  SourceLocation KwLoc = ConsumeToken();

  // Balance any number of arguments in parens.
  BalancedDelimiterTracker Parens(*this, tok::l_paren);
  if (Parens.expectAndConsume())
    return ExprError();

  SmallVector<Expr *, 2> Args;
  do {
    ExprResult Expr = ParseConstantExpression();
    if (Expr.isInvalid()) {
      Parens.skipToEnd();
      return ExprError();
    }
    Args.push_back(Expr.get());
  } while (TryConsumeToken(tok::comma));

  if (Parens.consumeClose())
    return ExprError();

  SourceLocation LPLoc = Parens.getOpenLocation();
  SourceLocation RPLoc = Parens.getCloseLocation();
  return Actions.ActOnCXXMetafunction(KwLoc, LPLoc, Args, RPLoc);
}

bool Parser::ParseSpliceSpecifier(bool TryParseSpecialization) {
  assert(Tok.is(tok::l_splice) && "expected '[:'");

  BalancedDelimiterTracker SpliceTokens(*this, tok::l_splice);
  if (SpliceTokens.expectAndConsume())
    return true;

  ExprResult ER = ParseConstantExpression();
  if (ER.isInvalid() || ER.get()->containsErrors()) {
    SpliceTokens.skipToEnd();
    return true;
  }
  Expr *Operand = ER.get();

  Token end = Tok;
  if (SpliceTokens.consumeClose())
    return true;

  SourceLocation LSplice = SpliceTokens.getOpenLocation();
  SourceLocation RSplice = SpliceTokens.getCloseLocation();

  SpliceResult SR;
  if (TryParseSpecialization && Tok.is(tok::less)) {
    ASTTemplateArgsPtr TArgsPtr;
    SourceLocation LAngleLoc, RAngleLoc;
    {
      TemplateArgList TArgs;
      if (ParseTemplateIdAfterTemplateName(/*ConsumeLastToken=*/false,
                                           LAngleLoc, TArgs, RAngleLoc,
                                           /*Template=*/nullptr))
        return true;

      TArgsPtr = ASTTemplateArgsPtr(TArgs.data(), TArgs.size());
      end = Tok;
      ConsumeToken();
    }
    SR = Actions.ActOnSpliceSpecifier(LSplice, Operand, RSplice, LAngleLoc,
                                      TArgsPtr, RAngleLoc);
  } else {
    SR = Actions.ActOnSpliceSpecifier(LSplice, Operand, RSplice);
  }
  if (SR.isInvalid())
    return true;
  SpliceSpecifier *Splice = SR.get();

  UnconsumeToken(end);
  Tok.setKind(tok::annot_splice);
  setSpliceAnnotation(Tok, Splice);
  Tok.setLocation(Splice->getBeginLoc());
  Tok.setAnnotationEndLoc(Splice->getEndLoc());
  PP.AnnotateCachedTokens(Tok);

  return false;
}

ExprResult Parser::ParseCXXSpliceAsExpr(SourceLocation TemplateKWLoc,
                                        bool AllowMemberReference) {
  assert(Tok.is(tok::annot_splice) && "expected a splice annotation");

  SpliceResult SR = getSpliceAnnotation(Tok);
  if (SR.isInvalid())
    return ExprError();
  SpliceSpecifier *Splice = SR.get();

  assert((!Splice->isSpecialization() || TemplateKWLoc.isValid()) &&
         "splice-specialization-specifier required leading 'template'");
  ConsumeAnnotationToken();

  return Actions.ActOnCXXSpliceExpression(TemplateKWLoc, Splice,
                                          AllowMemberReference);
}

TypeResult Parser::ParseCXXSpliceAsType(SourceLocation TypenameKWLoc,
                                        bool AllowDependent, bool Complain) {
  assert(Tok.is(tok::annot_splice) && "expected a splice annotation");

  SpliceResult SR = getSpliceAnnotation(Tok);
  if (SR.isInvalid())
    return TypeError();
  SpliceSpecifier *Splice = SR.get();

  TypeResult Result = Actions.ActOnCXXSpliceTypeSpecifier(TypenameKWLoc,
                                                          Splice, Complain);
  if (!Result.isInvalid())
    ConsumeAnnotationToken();

  return Result;
}

DeclResult Parser::ParseCXXSpliceAsNamespace() {
  assert(Tok.is(tok::annot_splice) && "expected annot_splice");

  SpliceResult SR = getSpliceAnnotation(Tok);
  if (SR.isInvalid())
    return DeclError();
  SpliceSpecifier *Splice = SR.get();

  assert(!Splice->isSpecialization() &&
         "splice-specialization-specifier cannot represent a namespace");
  ConsumeAnnotationToken();

  return Actions.ActOnCXXSpliceExpectingNamespace(Splice);
}
