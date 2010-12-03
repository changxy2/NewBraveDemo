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
#include "LogoSplash.h"

#include <NiSourceTexture.h>
#include <NiMeshScreenElements.h>
#include <NiScreenFillingRenderView.h>
#include <NiMesh2DRenderView.h>
#include <NiBezFloatKey.h>
#include <efd/ServiceManager.h>

using namespace efd;


	NiImplementRTTI(LogoSplash, ScreenTextureClick);

	//--------------------------------------------------------------------------------------------------
	LogoSplash::LogoSplash(
		const utf8string& textureName,
		efd::Bool skippable,
		const efd::Point2& size,
		efd::Float32 duration)
		: m_skippable(skippable)
		, m_duration(duration)
		, m_spAlphaInterpolator(FadeInOutAlpha())
		, m_spSizeInterpolator(GrowSize())
		, m_pServiceManager(NULL)
		, m_startTime(0)
	{
		EE_ASSERT(m_spAlphaInterpolator);
		EE_ASSERT(m_spSizeInterpolator);

		m_spAlphaInterpolator->GuaranteeTimeRange(0.0f, m_duration);
		m_spSizeInterpolator->GuaranteeTimeRange(0.0f, m_duration);

		ScreenTextureClick::Element* pElement =
			EE_EXTERNAL_NEW ScreenTextureClick::Element(textureName, posCenter, size);
		AddTexture(pElement, "splash");
	}

	//--------------------------------------------------------------------------------------------------
	LogoSplash::~LogoSplash()
	{
		EnablePreRenderCallback(false);
	}

	//--------------------------------------------------------------------------------------------------
	void LogoSplash::OnInit(efd::ServiceManager* pServiceManager)
	{
		EE_ASSERT(!m_pServiceManager);
		m_pServiceManager = pServiceManager;
		EE_ASSERT(m_pServiceManager);

		EnablePreRenderCallback(true);
	}

	//--------------------------------------------------------------------------------------------------
	efd::Bool LogoSplash::IsDrawing() const
	{
		return (m_duration != 0 && m_startTime > 0.0);
	}

	//--------------------------------------------------------------------------------------------------
	efd::Bool LogoSplash::Abort()
	{
		if (m_skippable)
		{
			m_duration = 0.0;
		}
		return m_skippable || m_duration == 0.0;
	}

	//--------------------------------------------------------------------------------------------------
	efd::Float64 LogoSplash::ElapsedTime() const
	{
		EE_ASSERT(m_pServiceManager);

		efd::TimeType now = m_pServiceManager->GetServiceManagerTime();
		if (m_startTime == 0)
			m_startTime = now;

		return (now - m_startTime);
	}

	//--------------------------------------------------------------------------------------------------
	efd::Bool LogoSplash::IsDone() const
	{
		return (m_duration == 0.0);
	}

	//--------------------------------------------------------------------------------------------------
	NiFloatInterpolator* LogoSplash::GrowSize()
	{
		NiFloatInterpolator* interp = NiNew NiFloatInterpolator();
		NiBezFloatKey* pkSizeKeys = NiNew NiBezFloatKey[3];
		pkSizeKeys[0] = NiBezFloatKey(0.0f, 0.92f, 0.0f, 0.0f);
		pkSizeKeys[1] = NiBezFloatKey(1.0f, 0.95f, 0.15f, 0.0f);
		pkSizeKeys[2] = NiBezFloatKey(5.0f, 0.95f, 0.0f, 0.0f);
		interp->ReplaceKeys(pkSizeKeys, 3, NiBezFloatKey::BEZKEY);
		return interp;
	}

	//--------------------------------------------------------------------------------------------------
	NiFloatInterpolator* LogoSplash::FadeInOutAlpha()
	{
		NiFloatInterpolator* interp = NiNew NiFloatInterpolator();
		NiBezFloatKey* pkAlphaKeys = NiNew NiBezFloatKey[4];
		pkAlphaKeys[0] = NiBezFloatKey(0.0f, 0.0f, 0.0f,  0.0f);
		pkAlphaKeys[1] = NiBezFloatKey(1.0f, 1.0f, 3.0f,  0.0f);
		pkAlphaKeys[2] = NiBezFloatKey(4.0f, 1.0f, 0.0f, -3.0f);
		pkAlphaKeys[3] = NiBezFloatKey(5.0f, 0.0f, 0.0f,  0.0f);
		interp->ReplaceKeys(pkAlphaKeys, 4, NiBezFloatKey::BEZKEY);
		return interp;
	}

	//--------------------------------------------------------------------------------------------------
	NiFloatInterpolator* LogoSplash::ConstInterpolator(efd::Float32 value)
	{
		NiFloatInterpolator* interp = NiNew NiFloatInterpolator(value);
		return interp;
	}

	//--------------------------------------------------------------------------------------------------
	efd::Bool LogoSplash::PreRenderCallback()
	{
		if (m_elementMap.empty())
			m_duration = 0.0;

		if (m_duration == 0.0)
			return false;

		efd::Float64 elapsed = ElapsedTime();
		if (elapsed >= m_duration)
			elapsed = m_duration;

		float alpha, scale;
		// update our interpolators, if they fail treat it like we're done.
		if (m_spAlphaInterpolator->Update(static_cast<float>(elapsed), 0, alpha) == false ||
			m_spSizeInterpolator ->Update(static_cast<float>(elapsed), 0, scale)  == false)
		{
			m_duration = 0.0;
			return false;
		}

		EE_ASSERT(m_spFadeMaterial);
		m_spFadeMaterial->SetAlpha(alpha);

		EE_ASSERT(m_aspect != 0.0);
		// iterate over all elements and update them
		for (ElementMap::iterator it = m_elementMap.begin(); it != m_elementMap.end(); ++it)
		{
			Element* pElement = it->second;
			pElement->SetSize(scale, m_screenSize);
		}
		if (elapsed >= m_duration)
		{
			m_duration = 0.0; // signal that we're done.
		}
		return true;
	}

	//--------------------------------------------------------------------------------------------------
	void LogoSplash::SetSizeInterpolator(NiFloatInterpolator* pSize)
	{
		m_spSizeInterpolator = pSize;
	}

	//--------------------------------------------------------------------------------------------------
	void LogoSplash::SetAlphaInterpolator(NiFloatInterpolator* pAlpha)
	{
		m_spAlphaInterpolator = pAlpha;
	}

	//--------------------------------------------------------------------------------------------------

