// Copyright 2017 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fxfa/parser/cxfa_wsdlconnection.h"

namespace {

const CXFA_Node::PropertyData kPropertyData[] = {
    {XFA_Element::Operation, 1, 0},
    {XFA_Element::WsdlAddress, 1, 0},
    {XFA_Element::SoapAddress, 1, 0},
    {XFA_Element::SoapAction, 1, 0},
    {XFA_Element::EffectiveOutputPolicy, 1, 0},
    {XFA_Element::EffectiveInputPolicy, 1, 0},
    {XFA_Element::Unknown, 0, 0}};
const CXFA_Node::AttributeData kAttributeData[] = {
    {XFA_Attribute::Name, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::DataDescription, XFA_AttributeType::CData, nullptr},
    {XFA_Attribute::Unknown, XFA_AttributeType::Integer, nullptr}};

constexpr wchar_t kName[] = L"wsdlConnection";

}  // namespace

CXFA_WsdlConnection::CXFA_WsdlConnection(CXFA_Document* doc,
                                         XFA_PacketType packet)
    : CXFA_Node(doc,
                packet,
                XFA_XDPPACKET_ConnectionSet,
                XFA_ObjectType::Node,
                XFA_Element::WsdlConnection,
                kPropertyData,
                kAttributeData,
                kName) {}

CXFA_WsdlConnection::~CXFA_WsdlConnection() {}