#include "Drawable.h"
#include "Graphics.h"
#include "TextureManager.h"

Drawable::Drawable()
{
	m_bInitialized = false;
	m_spriteData.width = 2;
	m_spriteData.height = 2;
	m_spriteData.x = 0.0;
	m_spriteData.y = 0.0;
	m_spriteData.scale = 1.0;
	m_spriteData.angle = 0.0;
	m_spriteData.rect.left = 0;
	m_spriteData.rect.top = 0;
	m_spriteData.rect.right = m_spriteData.width;
	m_spriteData.rect.bottom = m_spriteData.height;
	m_spriteData.texture = NULL;
	m_spriteData.flipHorizontal = false;
	m_spriteData.flipVertical = false;
	m_nCols = 1;
	m_pTextureManager = NULL;
	m_iFirstPage = 0;
	m_iLastPage = 0;
	m_iCurrentPage = 0;
	m_fFlipDelay = 1.0;
	m_fFlipTimer = 0.0;
	m_bVisible = true;
	m_bFlip = false;
	m_bOnLastPage = true;
	m_pGraphics = NULL;
	m_colorFilter = WHITE;
}


Drawable::~Drawable()
{
}

bool Drawable::Initialize(Graphics* pGraphics, int width, int height, int nCols,
	const char* textureName)
{
	return Initialize(pGraphics, width, height, nCols, 
		pGraphics->RetrieveTexture(textureName));
}

bool Drawable::Initialize(Graphics* pGraphics, int width, int height, int nCols,
	TextureManager* pTextureMan)
{
	bool bSuccess = true;

	try
	{
		m_pGraphics = pGraphics;
		m_pTextureManager = pTextureMan;
		m_spriteData.texture = m_pTextureManager->GetTexture();

		if (width == 0)
		{
			width = m_pTextureManager->GetWidth();
		}
        
		m_spriteData.width = width;

		if (height == 0)
		{
			height = m_pTextureManager->GetHeight();
		}

		m_spriteData.height = height;
		m_nCols = nCols;

		if (m_nCols == 0)
		{
			m_nCols = 1;
		}

		UpdateRect();
		m_bInitialized = true;
	}
	catch (...)
	{
		bSuccess = false;
	}

	return bSuccess;
}

void Drawable::Draw(COLOR_ARGB color)
{
	if (m_bVisible && m_pGraphics != NULL)
	{
		m_spriteData.texture = m_pTextureManager->GetTexture();
		m_pGraphics->DrawSprite(m_spriteData, color);
	}
}

void Drawable::SetCurrentPage(int page)
{
	if (page >= 0)
	{
		m_iCurrentPage = page;
		m_bOnLastPage = false;
		UpdateRect();
	}
}

void Drawable::UpdateRect()
{
	m_spriteData.rect.left = (m_iCurrentPage % m_nCols) * m_spriteData.width;
	m_spriteData.rect.right = m_spriteData.rect.left + m_spriteData.width;
	m_spriteData.rect.top = (m_iCurrentPage / m_nCols) * m_spriteData.height;
	m_spriteData.rect.bottom = m_spriteData.rect.top + m_spriteData.height;
}

void Drawable::UpdateAnimation(float deltaT)
{

	// If it is an animated drawable
	if (m_iLastPage - m_iFirstPage > 0)
	{
		m_fFlipTimer += deltaT;		// Time elapsed since last flip

		// If it's time to flip
		if (m_fFlipTimer > m_fFlipDelay)
		{
			m_fFlipTimer -= m_fFlipDelay;
			m_iCurrentPage++;

			// If the current page is outside the animation flipbook
			if (m_iCurrentPage < m_iFirstPage || m_iCurrentPage > m_iLastPage)
			{
				// If animating, reset to first page
				// Otherwise, set to last page and start flipping
				if (m_bFlip)
				{
					m_iCurrentPage = m_iFirstPage;
				}
				else
				{
					m_iCurrentPage = m_iLastPage;
					m_bFlip = true;
				}
			}

			UpdateRect();
		}
	}
}