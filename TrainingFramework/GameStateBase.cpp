#include "GameStateBase.h"
#include "GSIntro.h"
#include "GSMainmenu.h"
#include "GSLevel1.h"
#include "GSLevel2.h"


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
	case GS_LEVEL1:
		gs = std::make_shared<GSLevel1>();
		break;
	case GS_LEVEL2:
		gs = std::make_shared<GSLevel2>();
		break;
	case GS_OPTION:
		//gs = std::make_shared<GSOptions>();
		break;
	case GS_CREDIT:
		//gs = std::make_shared<GSAboutMe>();
		break;
	default:
		break;
	}
	return gs;
	return std::shared_ptr<GameStateBase>();
}
