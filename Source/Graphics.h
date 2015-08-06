// Mostly implemented by Charles Kelly

#ifndef _GRAPHICS_H             // prevent multiple definitions if this 
#define _GRAPHICS_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN



#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "Data.h"
#include "Point2D.h"
#include "GameException.h"
#include <unordered_map>

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)| \
	(((g)&0xff)<<8)|((b)&0xff)))

// D3DFVF_XYZRHW - vertices are transformed
// D3DFVF_DIFFUSE - The verticies contain diffuse color data
#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// Colors
const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
const COLOR_ARGB BLACK = D3DCOLOR_ARGB(0, 0, 0, 0);
const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);
const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);

struct SpriteData;
struct VertexC;
class TextureManager;

class Graphics
{
private:
    // DirectX pointers and stuff
	LPDIRECT3DDEVICE9 m_Device3D;
	LPDIRECT3D9 m_Direct3D;
    D3DPRESENT_PARAMETERS m_d3dpp;
    D3DDISPLAYMODE pMode;
	LP_SPRITE m_pSprite;
	ID3DXLine* m_pPolygon;	// Not guaranteed to be on a single plane

    // other variables
    HRESULT     m_Result;         // standard Windows return codes
    HWND        m_Hwnd;
    bool        m_bFullscreen;
    int         m_Width;
    int         m_Height;
    COLOR_ARGB  m_BackColor;      // background color

	std::unordered_map<std::string, TextureManager*> m_textures;

    // (For internal engine use only. No user serviceable parts inside.)
    // Initialize D3D presentation parameters
    void InitD3Dpp();

	HRESULT CreateVertexBuffer(VertexC vertices[], UINT size,
		LPDIRECT3DVERTEXBUFFER9 vertexBuffer);

public:
    // Constructor
    Graphics();

    // Destructor
    virtual ~Graphics();

    // Releases direct3d and device3d.
    void ReleaseAll();

    // Initialize DirectX graphics
    // Throws GameException on error
    // Pre: hw = handle to window
    //      width = width in pixels
    //      height = height in pixels
    //      fullscreen = true for full screen, false for window
    void Initialize(HWND hw, int width, int height, bool fullscreen);

	/** Save a texture manager in a map with a string as a key **/
	void SaveTexture(const const char* textureName, TextureManager* pTexture);

	/** Retrieve a previously saved texture manager using its key **/
	TextureManager* RetrieveTexture(const char* texture);

	/** Free all saved texture managers **/
	void ReleaseSavedTextures();

	void OnLostDevice();

	void OnResetDevice();

	LP_SPRITE GetSprite() { return m_pSprite; }

	//=====================================================================
	// Load texture into D3D memory. For internal engine use only.
	// Use TextureManager class load game textures.
	// Pre: filename is the name of texture file
	// Post: width and height = size of texture
	//	     texture points to texture
	// Returns HRESULT
	//=====================================================================
	HRESULT LoadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture);

	//=====================================================================
	// Draw sprite described by the argued SpriteData struct.
	// Color is an optional filter, use WHITE to apply no filter.
	// Pre: sprite->Begin() has been called
	// Post: sprite->End() must be called
	//		data.rect defines the portion of spriteData.texture to draw
	//		data.rect.right must be right edge + 1
	//		data.rect.bottom must be bottom edge + 1
	//=====================================================================
	void DrawSprite(const SpriteData& data, COLOR_ARGB color);

	void DrawPolygon(const std::vector<Space::Point2D<float>>& vertices, 
		int width, COLOR_ARGB color);

	void FillPolygon(const std::vector<Space::Point2D<float>>& vertices,
		COLOR_ARGB color);

    // Display the offscreen backbuffer to the screen.
    HRESULT ShowBackbuffer();

    // Checks the adapter to see if it is compatible with the BackBuffer height,
    // width and refresh rate specified in d3dpp. Fills in the pMode structure with
    // the format of the compatible mode, if found.
    // Pre: d3dpp is initialized.
    // Post: Returns true if compatible mode found and pMode structure is filled.
    //       Returns false if no compatible mode found.
    bool IsAdapterCompatible();

    // Reset the graphics device.
    HRESULT Reset();

    // get functions
    // Return direct3d.
	LPDIRECT3D9 Get3D() { return m_Direct3D; }

    // Return device3d.
	LPDIRECT3DDEVICE9 get3Ddevice() { return m_Device3D; }

    // Return handle to device context (window).
    HDC     GetWindow()             { return GetDC(m_Hwnd); }

    // Test for lost device
    HRESULT GetDeviceState();

    // Set color used to clear screen
    void SetBackColor(COLOR_ARGB c) {m_BackColor = c;}

    //=============================================================================
    // Clear backbuffer and BeginScene()
    //=============================================================================
    HRESULT BeginScene() 
    {
        m_Result = E_FAIL;
		if (m_Device3D == NULL)
		{
            return m_Result;
		}

        // clear backbuffer to backColor
        m_Device3D->Clear(0, NULL, D3DCLEAR_TARGET, m_BackColor, 1.0F, 0);
        m_Result = m_Device3D->BeginScene();          // begin scene for drawing
        return m_Result;
    }

    //=============================================================================
    // EndScene()
    //=============================================================================
    HRESULT EndScene() 
    {
        m_Result = E_FAIL;

		if (m_Device3D)
		{
			m_Result = m_Device3D->EndScene();
		}

        return m_Result;
    }

	void SpriteBegin()
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	void SpriteEnd()
	{
		m_pSprite->End();
	}

	void PolygonBegin()
	{
		m_pPolygon->Begin();
	}

	void PolygonEnd()
	{
		m_pPolygon->End();
	}
};

// Data needed to draw a sprite to the screen
struct SpriteData
{
	int width;
	int height;
	float x;				// Screen location of sprite
	float y;
	float scale;
	float angle;			// Angle in radians
	RECT rect;				// used to select from larger texture
	LP_TEXTURE texture;
	bool flipHorizontal;
	bool flipVertical;
};

// Vertex struct for drawing Direct3D primitives 
struct VertexC
{
	float x, y, z;			// Vertex location
	float rhw;				// reciprocol homogeneous W (set to 1)
	unsigned long color;	// vertex color
};

#endif

