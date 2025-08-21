#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Basic/Token.h"

using namespace clang;

//===----------------------------------------------------------------------===//
// Token Class Implementation
//===----------------------------------------------------------------------===//

/// isObjCAtKeyword - Return true if we have an ObjC keyword identifier.
bool Token::isObjCAtKeyword(tok::ObjCKeywordKind objcKey) const {
  if (isAnnotation())
    return false;
  if (const IdentifierInfo *II = getIdentifierInfo())
    return II->getObjCKeywordID() == objcKey;
  return false;
}

/// getObjCKeywordID - Return the ObjC keyword kind.
tok::ObjCKeywordKind Token::getObjCKeywordID() const {
  if (isAnnotation())
    return tok::objc_not_keyword;
  const IdentifierInfo *specId = getIdentifierInfo();
  return specId ? specId->getObjCKeywordID() : tok::objc_not_keyword;
}


/// Determine whether the token kind starts a simple-type-specifier.
bool Token::isSimpleTypeSpecifier(const LangOptions &LangOpts) const {
  switch (getKind()) {
  case tok::annot_typename:
  case tok::annot_decltype:
  case tok::annot_pack_indexing_type:
    return true;

  case tok::kw_short:
  case tok::kw_long:
  case tok::kw___int64:
  case tok::kw___int128:
  case tok::kw_signed:
  case tok::kw_unsigned:
  case tok::kw_void:
  case tok::kw_char:
  case tok::kw_int:
  case tok::kw_half:
  case tok::kw_float:
  case tok::kw_double:
  case tok::kw___bf16:
  case tok::kw__Float16:
  case tok::kw___float128:
  case tok::kw___ibm128:
  case tok::kw_wchar_t:
  case tok::kw_bool:
  case tok::kw__Bool:
  case tok::kw__Accum:
  case tok::kw__Fract:
  case tok::kw__Sat:
#define TRANSFORM_TYPE_TRAIT_DEF(_, Trait) case tok::kw___##Trait:
#include "clang/Basic/TransformTypeTraits.def"
  case tok::kw___auto_type:
  case tok::kw_char16_t:
  case tok::kw_char32_t:
  case tok::kw_typeof:
  case tok::kw_decltype:
  case tok::kw_char8_t:
    return getIdentifierInfo()->isKeyword(LangOpts);

  default:
    return false;
  }
}
