// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not
// be copied or disclosed except in accordance with the terms of that
// agreement.
//
//      Copyright (c) 1996-2009 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Calabasas, CA 91302
// http://www.emergent.net

#include "game_pch.h"
#include "ScreenTextureClick.h"

#include <efd/File.h>
#include <efd/TinyXML.h>
#include <efd/AssetLocatorService.h>
#include <ecr/CoreRuntimeMessages.h>

#include <NiMesh2DRenderView.h>
#include <NiScreenFillingRenderView.h>
#include <NiMeshScreenElements.h>
#include <NiVertexColorProperty.h>
#if defined(EE_PLATFORM_PS3)
#include <NiPS3Renderer.h>
#endif

using namespace efd;



NiImplementRTTI(ScreenTextureClick, NiViewRenderClick);

const efd::Point2 ScreenTextureClick::UseTextureSize(ScreenTextureClick::szTexture,
                                                     ScreenTextureClick::szTexture);
const efd::Point2 ScreenTextureClick::UseScreenSize(ScreenTextureClick::szScreen,
                                                    ScreenTextureClick::szScreen);

//--------------------------------------------------------------------------------------------------
ScreenTextureClick::ScreenTextureClick()
: NiViewRenderClick()
, m_aspect(0.0)
, m_screenSize(0, 0)
, m_pRenderView(NULL)
{
    m_pRenderView = EE_NEW NiMesh2DRenderView();
    EE_ASSERT(m_pRenderView);
    AppendRenderView(m_pRenderView);

    m_aspect = GetAspectRatio();

    NiRenderer* pRenderer = NiRenderer::GetRenderer();
    EE_ASSERT(pRenderer);

    NiRenderTargetGroup* pTargetGroup = pRenderer->GetDefaultRenderTargetGroup();
    EE_ASSERT(pTargetGroup);
    m_screenSize.x = static_cast<efd::Float32>(pTargetGroup->GetWidth(0));
    m_screenSize.y = static_cast<efd::Float32>(pTargetGroup->GetHeight(0));

    //EE_ASSERT(m_screenSize.x / m_screenSize.y == m_aspect);

    // Get the "safe zone" upper left and lower right
    efd::UInt32 x, y;
    pRenderer->GetOnScreenCoord(0.0f, 0.0f, 0, 0, x, y, NiRenderer::CORNER_TOP_LEFT);
    m_ulOffset = efd::Point2(static_cast<efd::Float32>(x), static_cast<efd::Float32>(y));
    pRenderer->GetOnScreenCoord(0.0f, 0.0f, 0, 0, x, y, NiRenderer::CORNER_BOTTOM_RIGHT);
    m_lrOffset = efd::Point2(m_screenSize.x - x, m_screenSize.y - y);

    m_spFadeMaterial = NiNew NiMaterialProperty;
    EE_ASSERT(m_spFadeMaterial);
    m_spFadeMaterial->SetEmittance(NiColor(1.0f, 1.0f, 1.0f));

    SetActive(true);
    EnablePreRenderCallback(true);
}

//--------------------------------------------------------------------------------------------------
ScreenTextureClick::~ScreenTextureClick()
{
    EnablePreRenderCallback(false);

    for (ElementMap::iterator it = m_elementMap.begin(); it != m_elementMap.end(); ++it)
    {
        EE_EXTERNAL_DELETE it->second;
    }
}

//--------------------------------------------------------------------------------------------------
void ScreenTextureClick::AddTexture(Element* pElement, const efd::utf8string& name)
{
    EE_ASSERT(pElement);
    EE_ASSERT(m_pRenderView);

    EE_ASSERT(m_spFadeMaterial);
    pElement->m_pScreenElement->AttachProperty(m_spFadeMaterial);
    pElement->m_pScreenElement->UpdateProperties();

    m_pRenderView->AppendScreenElement(pElement->m_pScreenElement);

    efd::Float32 textureAspect = pElement->m_textureSize.x / pElement->m_textureSize.y;

    // at this point the element's m_size member has to be either an actual size or
    EE_ASSERT(pElement->m_size.x != szTexture);
    EE_ASSERT(pElement->m_size.y != szTexture);

    // if both dimensions of the size are "scale to screen" then keep the aspect ratio.
    efd::Bool scaleToScreen = pElement->m_size.x == szScreen && pElement->m_size.y == szScreen;

    if (pElement->m_size.x == szScreen)
        pElement->m_size.x = m_screenSize.x;

    if (pElement->m_size.y == szScreen)
        pElement->m_size.y = m_screenSize.y;

    if (scaleToScreen)
    {
        // if the screen is narrow then swap for the correct aspect ratio
        if (m_aspect > 1.0f)
        {
            pElement->m_size.y = pElement->m_size.x / textureAspect;
        }
        else
        {
            pElement->m_size.x = pElement->m_size.y * textureAspect;
        }
    }

    // Calculate our position.
    efd::Point2 pos = pElement->m_position;

    if (pElement->m_alignment & posLeft)
        pos.x += m_ulOffset.x;
    else if (pElement->m_alignment & posRight)
        pos.x += m_screenSize.x - pElement->m_size.x - m_lrOffset.x;

    if (pElement->m_alignment & posTop)
        pos.y += m_ulOffset.y;
    else if (pElement->m_alignment & posBottom)
        pos.y += m_screenSize.y - pElement->m_size.y - m_lrOffset.y;

    pElement->SetSize(1.0, m_screenSize);

    // if we had an element name then we need to save this data.
    if (!name.empty())
    {
        // ensure that the names are unique.
        EE_ASSERT(m_elementMap.find(name) == m_elementMap.end());
        m_elementMap[name] = pElement;
    }
    else
    {
        // if we aren't saving it for later we can just delete it now.
        EE_EXTERNAL_DELETE pElement;
    }
}

//--------------------------------------------------------------------------------------------------
efd::Bool ScreenTextureClick::PreRenderCallback()
{
//    EE_FAIL("If you enable PreRenderCallbacks then you must override this function.");
    return true;
}

//--------------------------------------------------------------------------------------------------
efd::Bool ScreenTextureClick::PreRenderDispatch(NiRenderClick* pkCurrentRenderClick,
                                                void* pvCallbackData)
{
    EE_UNUSED_ARG(pvCallbackData);
    EE_ASSERT(pkCurrentRenderClick);
    ScreenTextureClick* pClick = reinterpret_cast<ScreenTextureClick*>(pkCurrentRenderClick);
    EE_ASSERT(pClick);
    return pClick->PreRenderCallback();
}

//--------------------------------------------------------------------------------------------------
efd::Float32 ScreenTextureClick::GetAspectRatio() const
{
    // Determine aspect ratio.
#if defined(WIN32) || defined (_XENON)
    Ni2DBuffer* pkBackbuffer =
        NiRenderer::GetRenderer()->GetDefaultBackBuffer();
    return (float)pkBackbuffer->GetWidth() / (float)pkBackbuffer->GetHeight();
#elif defined(_PS3)
    NiPS3Renderer* pkPS3Renderer =
        NiDynamicCast(NiPS3Renderer, NiRenderer::GetRenderer());
    return pkPS3Renderer->GetDisplayAspectRatio();
#else
    #error "Unsupported platform"
    return 1.0f;
#endif
}

//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
ScreenTextureClick::Element::Element(const efd::utf8string& textureName,
                                     PositionType alignment,
                                     const efd::Point2& size,
                                     const efd::Point2& position)
: m_textureName(textureName)
, m_alignment(alignment)
, m_position(position)
, m_pScreenElement(NULL)
{
    // create the screen elements
    NiSourceTexture* pScreenTex = NiSourceTexture::Create(m_textureName.c_str());
    if (!pScreenTex)
        return;

    m_textureSize.x = static_cast<efd::Float32>(pScreenTex->GetWidth());
    m_textureSize.y = static_cast<efd::Float32>(pScreenTex->GetHeight());

    m_size.x = (size.x == szTexture ? m_textureSize.x : size.x);
    m_size.y = (size.y == szTexture ? m_textureSize.y : size.y);

    m_pScreenElement =
        NiMeshScreenElements::Create(pScreenTex, NiTexturingProperty::APPLY_MODULATE);
    EE_ASSERT(m_pScreenElement);

    // add the polygons we want to use to the screen element
    efd::SInt32 polyIndex = m_pScreenElement->Insert(4);
    // we assume one poly per screen element elsewhere
    EE_ASSERT(polyIndex == 0);
    m_pScreenElement->SetRectangle(polyIndex, 0.0f, 0.0f, 1.0f, 1.0f);
    m_pScreenElement->SetTextures(polyIndex, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    m_pScreenElement->SetColors(polyIndex, NiColorA::WHITE);

    NiTexturingProperty* pkTexProp = NiDynamicCast(NiTexturingProperty,
        m_pScreenElement->GetProperty(NiProperty::TEXTURING));
    EE_ASSERT(pkTexProp);
    pkTexProp->SetBaseFilterMode(NiTexturingProperty::FILTER_TRILERP);
    pkTexProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

    NiAlphaProperty* pkAlphaProp = NiDynamicCast(NiAlphaProperty,
        m_pScreenElement->GetProperty(NiProperty::ALPHA));
    EE_ASSERT(pkAlphaProp);
    pkAlphaProp->SetAlphaBlending(true);

    NiVertexColorProperty* pkVertColor = NiDynamicCast(NiVertexColorProperty,
        m_pScreenElement->GetProperty(NiProperty::VERTEX_COLOR));
    EE_ASSERT(pkVertColor);
    pkVertColor->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
}

//--------------------------------------------------------------------------------------------------
namespace
{
    const efd::UInt32 HorizMask = 0x0f;
    const efd::UInt32 VertMask  = 0xf0;
}
void ScreenTextureClick::Element::SetSize(efd::Float32 scale, const efd::Point2& screenSize)
{
    efd::Point2 size = m_size * scale;

    // Calculate the upper left corner
    efd::Point2 pos = m_position;

    // center if necessary
    if ((m_alignment & HorizMask) == 0) // neither left or right are set.
        pos.x = (screenSize.x - size.x) / 2.0f - pos.x;
    if ((m_alignment & VertMask) == 0) // neither top or bottom are set.
        pos.y = (screenSize.y - size.y) / 2.0f - pos.y;

    // Normalize the units
    pos.x /= screenSize.x;
    pos.y /= screenSize.y;
    size.x /= screenSize.x;
    size.y /= screenSize.y;

    m_pScreenElement->SetRectangle(0, pos.x, pos.y, size.x, size.y);
}

//--------------------------------------------------------------------------------------------------
ScreenTextureClick::Element::~Element()
{
}


