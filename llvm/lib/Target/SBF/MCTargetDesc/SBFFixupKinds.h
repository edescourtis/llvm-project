//===-- SBFFixupKinds.h - SBF Specific Fixup Entries --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SBF_MCTARGETDESC_SBFFIXUPKINDS_H
#define LLVM_LIB_TARGET_SBF_MCTARGETDESC_SBFFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace SBF {
enum Fixups {
  /// fixup_sbf_hi32 - 32-bit fixup corresponding to foo\@hi
  fixup_sbf_hi32 = FirstTargetFixupKind,

  /// fixup_sbf_lo32 - 32-bit fixup corresponding to foo\@lo
  fixup_sbf_lo32,

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace SBF
} // namespace llvm

#endif
