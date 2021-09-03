#include "stdafx.h"
#include "GameStateBase.h"
#include "GSIntro.h"
#include "GSMainmenu.h"
#include "GSOptions.h"
#include "GSLevel1.h"
#include "GSLevel2.h"
#include "GSLevel3.h"
#include "GSLevel4.h"
#include "GSLoadLevel.h"
#include "GSResult.h"
#include "GSAboutUs.h"


std::shared_ptr<GameStateBase> GameStateBase::CreateState(StateTypes stt)
{
	std::shared_ptr<GameStateBase> gs = nullptr;
	switch (stt)
	{
	case GS_INVALID:
		break;
	case GS_INTRO:
		gs = std::make_shared<GSIntro>();
		break;
	case GS_MAINMENU:
		gs = std::make_shared<GSMainmenu>();
		break;
	case GS_LOADLEVEL:
		gs = std::make_shared<GSLoadLevel>();
		break;
	case GS_LEVEL1:
		gs = std::make_shared<GSLevel1>();
		break;
	case GS_LEVEL2:
		gs = std::make_shared<GSLevel2>();
		break;
	case GS_LEVEL3:
		gs = std::make_shared<GSLevel3>();
		break;
	case GS_LEVEL4:
		gs = std::make_shared<GSLevel4>();
		break;
	case GS_OPTIONS:
		gs = std::make_shared<GSOptions>();
		break;
	case GS_RESULT:
		gs = std::make_shared<GSResult>();
		break;
	case GS_ABOUTUS:
		gs = std::make_shared<GSAboutUs>();
		break;
	default:
		break;
	}
	return gs;
	return std::shared_ptr<GameStateBase>();
}

std::shared_ptr<GameStateBase> GameStateBase::CreateState(StateTypes stt, bool bIsVictory, int currentLevel)
{
	std::shared_ptr<GameStateBase> gs = nullptr;
	switch (stt)
	{
	case GS_RESULT:
		gs = std::make_shared<GSResult>(bIsVictory, currentLevel);
		break;
	default:
		break;
	}
	return gs;
	return std::shared_ptr<GameStateBase>();
}
