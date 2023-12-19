//===-- SBFMCExpr.cpp - SBF specific MC expression classes ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the assembly expression modifiers
// accepted by the SBF architecture (e.g. "%hi", "%lo", ...).
//
//===----------------------------------------------------------------------===//

#include "SBFMCExpr.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSymbolELF.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Casting.h"

using namespace llvm;

#define DEBUG_TYPE "sbfmcexpr"

const SBFMCExpr *SBFMCExpr::create(VariantKind Kind, const MCExpr *Expr,
                                 MCContext &Ctx) {
  return new (Ctx) SBFMCExpr(Kind, Expr);
}

void SBFMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {

  bool closeParen = printVariantKind(OS, Kind);

  const MCExpr *Expr = getSubExpr();
  Expr->print(OS, MAI);

  if (closeParen)
    OS << ')';
  printVariantKindSuffix(OS, Kind);
}

bool SBFMCExpr::printVariantKind(raw_ostream &OS, VariantKind Kind) {
  switch (Kind) {
    case VK_SBF_None:
    case VK_SBF_HI32:
    case VK_SBF_LO32:
    return false;
  }
  return true;
}

void SBFMCExpr::printVariantKindSuffix(raw_ostream &OS, VariantKind Kind) {
  switch (Kind) {
  case VK_SBF_None:
  case VK_SBF_HI32:
    OS << "@hi";
    break;
  case VK_SBF_LO32:
    OS << "@lo";
    break;
  }
}

SBFMCExpr::VariantKind SBFMCExpr::parseVariantKind(StringRef name) {
  return StringSwitch<SBFMCExpr::VariantKind>(name)
      .Case("hi", VK_SBF_HI32)
      .Case("lo", VK_SBF_LO32)
      .Default(VK_SBF_None);
}

SBF::Fixups SBFMCExpr::getFixupKind(SBFMCExpr::VariantKind Kind) {
  switch (Kind) {
  default:
    llvm_unreachable("Unhandled SBFMCExpr::VariantKind");
  case VK_SBF_HI32:
    return SBF::fixup_sbf_hi32;
  case VK_SBF_LO32:
    return SBF::fixup_sbf_lo32;
  }

}

bool SBFMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                         const MCAsmLayout *Layout,
                                         const MCFixup *Fixup) const {
  if (!getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup))
    return false;

  Res =
      MCValue::get(Res.getSymA(), Res.getSymB(), Res.getConstant(), getKind());

  return true;
}

static void fixELFSymbolsInTLSFixupsImpl(const MCExpr *Expr, MCAssembler &Asm) {
  switch (Expr->getKind()) {
  case MCExpr::Target:
    llvm_unreachable("Can't handle nested target expr!");
    break;

  case MCExpr::Constant:
    break;

  case MCExpr::Binary: {
//TODO: FIGURE OUT IF WE NEED THIS???
//    const MCBinaryExpr *BE = cast<MCBinaryExpr>(Expr);
//    fixELFSymbolsInTLSFixupsImpl(BE->getLHS(), Asm);
//    fixELFSymbolsInTLSFixupsImpl(BE->getRHS(), Asm);
    break;
  }

  case MCExpr::SymbolRef: {
    // We're known to be under a TLS fixup, so any symbol should be
    // modified. There should be only one.
//TODO: FIGURE OUT IF WE NEED THIS???
//    const MCSymbolRefExpr &SymRef = *cast<MCSymbolRefExpr>(Expr);
//    cast<MCSymbolELF>(SymRef.getSymbol()).setType(ELF::STT_TLS);
    break;
  }

  case MCExpr::Unary:
//TODO: FIGURE OUT IF WE NEED THIS???
    //fixELFSymbolsInTLSFixupsImpl(cast<MCUnaryExpr>(Expr)->getSubExpr(), Asm);
    break;
  }
}

void SBFMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}
