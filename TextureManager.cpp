#include "TextureManager.h"
#include "Graphics.h"

TextureManager::TextureManager(Graphics* pGraphics,
	const char* filename, const char* textureName)
{
	m_pGraphics = pGraphics;
	m_filename = filename;
	m_textureName = textureName;
	m_init = false;
	m_width = 0;
	m_height = 0;
	m_filename = "";
	m_pTexture = NULL;

}


TextureManager::~TextureManager()
{
}

bool TextureManager::Initialize(Graphics* pGraphics, 
	const char* filename,
	const char* textureName)
{
	TextureManager* pMan = new TextureManager(pGraphics, filename, textureName);

	HRESULT hr = pMan->m_pGraphics->LoadTexture(filename, TRANSPARENT, 
		pMan->m_width, pMan->m_height, pMan->m_pTexture);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pMan->m_pTexture);
	}
	else
	{
		pMan->m_init = true;
		pMan->m_pGraphics->SaveTexture(textureName, pMan);
	}

	return pMan->m_init;
}

void TextureManager::OnLostDevice()
{
	if (m_init)
	{
		SAFE_RELEASE(m_pTexture);
	}
}

void TextureManager::OnResetDevice()
{
	if (m_init)
	{
		m_pGraphics->LoadTexture(m_filename, TRANSPARENT, m_width, m_height, m_pTexture);
	}
}