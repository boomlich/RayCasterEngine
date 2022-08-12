#pragma once
#include "mediaManager.h"


class Prop
{
private:
	TextureID m_txID;
	ContentID &m_contentID;

	double m_posX;
	double m_posY;
public:
	double getPosX();
	double getPosY();
	void setPosX(double x);
	void setPosY(double y);
	TextureID getTextureID();
	ContentID& getContentID();

	Prop(TextureID txID, ContentID contentID, double x, double y);
};

class P_Box_01 : public Prop
{
public:
	P_Box_01(double x, double y) : Prop(TX_PR_BOX_01, C_ID_PR_BOX_01, x, y){}
};