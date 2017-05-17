// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xfa/fxfa/fm2js/cxfa_fmparse.h"

#include <vector>

#include "testing/gtest/include/gtest/gtest.h"
#include "testing/test_support.h"
#include "third_party/base/ptr_util.h"

TEST(CXFA_FMParseTest, Empty) {
  CXFA_FMErrorInfo errorInfo;
  auto parser = pdfium::MakeUnique<CXFA_FMParse>(L"", &errorInfo);
  std::unique_ptr<CXFA_FMFunctionDefinition> ast = parser->Parse();
  ASSERT(ast != nullptr);
  EXPECT_TRUE(errorInfo.message.IsEmpty());

  CFX_WideTextBuf buf;
  EXPECT_TRUE(ast->ToJavaScript(buf));
  // TODO(dsinclair): This is a little weird .....
  EXPECT_EQ(L"// comments only", buf.AsStringC());
}

TEST(CXFA_FMParseTest, CommentOnlyIsError) {
  CXFA_FMErrorInfo errorInfo;
  auto parser = pdfium::MakeUnique<CXFA_FMParse>(L"; Just comment", &errorInfo);
  std::unique_ptr<CXFA_FMFunctionDefinition> ast = parser->Parse();
  ASSERT(ast != nullptr);
  // TODO(dsinclair): This isn't allowed per the spec.
  EXPECT_TRUE(errorInfo.message.IsEmpty());
  // EXPECT_FALSE(errorInfo.message.IsEmpty());

  CFX_WideTextBuf buf;
  EXPECT_TRUE(ast->ToJavaScript(buf));
  EXPECT_EQ(L"// comments only", buf.AsStringC());
}

TEST(CXFA_FMParseTest, CommentThenValue) {
  const wchar_t ret[] =
      L"(\nfunction ()\n{\n"
      L"var pfm_ret = null;\n"
      L"pfm_ret = 12;\n"
      L"return pfm_rt.get_val(pfm_ret);\n"
      L"}\n).call(this);\n";

  CXFA_FMErrorInfo errorInfo;
  auto parser =
      pdfium::MakeUnique<CXFA_FMParse>(L"; Just comment\n12", &errorInfo);
  std::unique_ptr<CXFA_FMFunctionDefinition> ast = parser->Parse();
  ASSERT(ast != nullptr);
  EXPECT_TRUE(errorInfo.message.IsEmpty());

  CFX_WideTextBuf buf;
  EXPECT_TRUE(ast->ToJavaScript(buf));
  EXPECT_EQ(ret, buf.AsStringC());
}

TEST(CXFA_FMParseTest, Parse) {
  const wchar_t input[] = L"$ = Avg (-3, 5, -6, 12, -13);\n"
      L"$ = Avg (Table2..Row[*].Cell1);\n"
      L"\n"
      L"if ($ ne -1)then\n"
      L"  border.fill.color.value = \"255,64,64\";\n"
      L"else\n"
      L"  border.fill.color.value = \"20,170,13\";\n"
      L"endif\n"
      L"\n"
      L"$";

  const wchar_t ret[] =
      L"(\nfunction ()\n{\n"
      L"var pfm_ret = null;\n"
      L"if (pfm_rt.is_obj(this))\n{\n"
        L"pfm_rt.asgn_val_op(this, pfm_rt.Avg(pfm_rt.neg_op(3), 5, "
          L"pfm_rt.neg_op(6), 12, pfm_rt.neg_op(13)));\n"
      L"}\n"
      L"if (pfm_rt.is_obj(this))\n{\n"
        L"pfm_rt.asgn_val_op(this, pfm_rt.Avg(pfm_rt.dot_acc(pfm_rt.dotdot_acc("
          L"Table2, \"Table2\", \"Row\", 1), \"\", \"Cell1\", 0, 0)));\n"
      L"}\n"
      L"if (pfm_rt.get_val(pfm_rt.neq_op(this, pfm_rt.neg_op(1))))\n{\n"
        L"if (pfm_rt.is_obj(pfm_rt.dot_acc(pfm_rt.dot_acc(pfm_rt.dot_acc("
          L"border, \"border\", \"fill\", 0, 0), \"\", \"color\", 0, 0), \"\", "
          L"\"value\", 0, 0)))\n{\n"
            L"pfm_rt.asgn_val_op(pfm_rt.dot_acc(pfm_rt.dot_acc("
              L"pfm_rt.dot_acc(border, \"border\", \"fill\", 0, 0), \"\", "
                L"\"color\", 0, 0), \"\", \"value\", 0, 0), \"255,64,64\");\n"
        L"}\n"
      L"}\nelse\n{\n"
        L"if (pfm_rt.is_obj(pfm_rt.dot_acc(pfm_rt.dot_acc(pfm_rt.dot_acc("
          L"border, \"border\", \"fill\", 0, 0), \"\", \"color\", 0, 0), \"\", "
          L"\"value\", 0, 0)))\n{\n"
            L"pfm_rt.asgn_val_op(pfm_rt.dot_acc(pfm_rt.dot_acc("
              L"pfm_rt.dot_acc(border, \"border\", \"fill\", 0, 0), \"\", "
                L"\"color\", 0, 0), \"\", \"value\", 0, 0), \"20,170,13\");\n"
        L"}\n"
      L"}\n"
      L"pfm_ret = this;\n"
      L"return pfm_rt.get_val(pfm_ret);\n"
      L"}\n).call(this);\n";

  CXFA_FMErrorInfo errorInfo;
  auto parser = pdfium::MakeUnique<CXFA_FMParse>(input, &errorInfo);
  std::unique_ptr<CXFA_FMFunctionDefinition> ast = parser->Parse();
  ASSERT(ast != nullptr);
  EXPECT_TRUE(errorInfo.message.IsEmpty());

  CFX_WideTextBuf buf;
  EXPECT_TRUE(ast->ToJavaScript(buf));
  EXPECT_EQ(ret, buf.AsStringC());
}
