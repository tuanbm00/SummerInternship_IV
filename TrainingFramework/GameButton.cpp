#include "GameButton.h"
#include <iostream>
#include "ComfirmBox.h"


GameButton::GameButton(int ID)
	: Sprite2D(ID)
{

	m_pBtClick = nullptr;
	m_isHandle = false;
	m_isNeedComfirm = false;
}

GameButton::~GameButton()
{

}

void GameButton::SetOnClick(void(*pBtClickFun)())
{
	m_pBtClick = pBtClickFun;
}

void GameButton::OnMouseButtonUp(GLint x, GLint y)
{
	if (!GetActive()) return;
	m_isHandle = false;
	if ((x > m_Vec2DPos.x - m_iWidth / 2) && (x < m_Vec2DPos.x + m_iWidth / 2) && (y > m_Vec2DPos.y - m_iHeight / 2) && (y < m_Vec2DPos.y + m_iHeight / 2))
	{
		if (m_isNeedComfirm) {
			Singleton<ComfirmBox>::GetInstance()->GetButtonYes()->SetOnClick(m_pBtClick);
			Singleton<ComfirmBox>::GetInstance()->SetActive(true);
		}
		else {
			m_pBtClick();
			m_isHandle = true;
		}
	}
}

bool GameButton::IsHandle()
{
	return m_isHandle;
}

void GameButton::SetIsHandle(bool isHandle)
{
	m_isHandle = isHandle;
}
