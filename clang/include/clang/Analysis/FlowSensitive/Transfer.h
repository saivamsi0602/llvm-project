//===-- Transfer.h ----------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines a transfer function that evaluates a program statement and
//  updates an environment accordingly.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_ANALYSIS_FLOWSENSITIVE_TRANSFER_H
#define LLVM_CLANG_ANALYSIS_FLOWSENSITIVE_TRANSFER_H

#include "clang/AST/Stmt.h"
#include "clang/Analysis/FlowSensitive/DataflowEnvironment.h"
#include "llvm/ADT/Optional.h"

namespace clang {
namespace dataflow {

struct ContextSensitiveOptions {};

struct TransferOptions {
  /// Options for analyzing function bodies when present in the translation
  /// unit, or empty to disable context-sensitive analysis. Note that this is
  /// fundamentally limited: some constructs, such as recursion, are explicitly
  /// unsupported.
  llvm::Optional<ContextSensitiveOptions> ContextSensitiveOpts;
};

/// Maps statements to the environments of basic blocks that contain them.
class StmtToEnvMap {
public:
  virtual ~StmtToEnvMap() = default;

  /// Returns the environment of the basic block that contains `S` or nullptr if
  /// there isn't one.
  /// FIXME: Ensure that the result can't be null and return a const reference.
  virtual const Environment *getEnvironment(const Stmt &S) const = 0;
};

/// Evaluates `S` and updates `Env` accordingly.
///
/// Requirements:
///
///  `S` must not be `ParenExpr` or `ExprWithCleanups`.
void transfer(const StmtToEnvMap &StmtToEnv, const Stmt &S, Environment &Env,
              TransferOptions Options);

} // namespace dataflow
} // namespace clang

#endif // LLVM_CLANG_ANALYSIS_FLOWSENSITIVE_TRANSFER_H
