#include "Prop.h"
#include "mediaManager.h"

double Prop::getPosX()
{
	return m_posX;
}

double Prop::getPosY()
{
	return m_posY;
}

void Prop::setPosX(double x)
{
	m_posX = x;
}

void Prop::setPosY(double y)
{
	m_posY = y;
}


TextureID Prop::getTextureID()
{
	return m_txID;
}

ContentID& Prop::getContentID()
{
	return m_contentID;
}

Prop::Prop(TextureID txID, ContentID contentID, double x, double y) : m_contentID(contentID)
{
	m_txID = txID;
	m_posX = x;
	m_posY = y;
}
