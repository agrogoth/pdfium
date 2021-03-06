// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/parser/cxfa_uri.h"

#include "fxjs/xfa/cjx_uri.h"
#include "third_party/base/ptr_util.h"

namespace {

const CXFA_Node::AttributeData kUriAttributeData[] = {
    {XFA_Attribute::Id, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Name, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Use, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Usehref, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Desc, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Lock, XFA_AttributeType::Integer, (void*)0},
    {XFA_Attribute::Unknown, XFA_AttributeType::Integer, nullptr}};

constexpr wchar_t kUriName[] = L"uri";

}  // namespace

CXFA_Uri::CXFA_Uri(CXFA_Document* doc, XFA_PacketType packet)
    : CXFA_Node(doc,
                packet,
                (XFA_XDPPACKET_Config | XFA_XDPPACKET_ConnectionSet),
                XFA_ObjectType::TextNode,
                XFA_Element::Uri,
                nullptr,
                kUriAttributeData,
                kUriName,
                pdfium::MakeUnique<CJX_Uri>(this)) {}

CXFA_Uri::~CXFA_Uri() {}
