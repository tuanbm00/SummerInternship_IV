#pragma once
#include "Sprite2D.h"
class GameButton :
	public Sprite2D
{
public:
	GameButton(int ID);
	~GameButton();
	void	SetOnClick(void(*pBtClick)());
	void	OnMouseButtonUp(GLint x, GLint y);
	bool	IsHandle();
	void	SetIsHandle(bool isHandle);

private:
	void(*m_pBtClick)();
	bool	m_isHandle;

};
