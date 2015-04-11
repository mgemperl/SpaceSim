#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9helper.h>

class Graphics;

class TextureManager
{
public:

	~TextureManager();

	static bool Initialize(Graphics* pGraphics, const char* filename,
		const char* textureName);

	void OnLostDevice();

	void OnResetDevice();

	UINT GetWidth() const { return m_width; }

	UINT GetHeight() const { return m_height; }

	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }

private:

	UINT m_width;
	UINT m_height;
	LPDIRECT3DTEXTURE9 m_pTexture;
	const char* m_filename;
	const char* m_textureName;
	Graphics* m_pGraphics;
	bool m_init;

	TextureManager(Graphics* pGraphics, const char* filename, 
		const char* textureName);

};

