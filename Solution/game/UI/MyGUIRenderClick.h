#pragma once
#include <NiRenderClick.h>
#include <ecrInput/InputService.h>

#include <MyGUI.h>
#include <d3dx9.h>

namespace MyGUI { class DirectXPlatform; class Window; class Edit; }

using namespace ecrInput;

class MyGUIRenderClick :	public NiRenderClick
{
public:
	MyGUIRenderClick(InputServicePtr pInputService);
	virtual ~MyGUIRenderClick(void);
	
	virtual void PerformRendering(unsigned int uiFrameID);
	
	virtual unsigned int GetNumObjectsDrawn() const;
	virtual float GetCullTime() const;
	virtual float GetRenderTime() const;

	void InitUI(IDirect3DDevice9 * pDevice);
	bool create(IDirect3DDevice9 * pDevice); // создае?начальну?точк?каркас?приложен?
	void destroy(); // очищае?вс?параметр?каркас?приложен?
	void run();
	void quit() { mExit = true; }

	void createDefaultScene() { }

	MyGUI::Gui* getGUI() { return mGUI; }
	const std::string& getRootMedia() { return mRootMedia; }
	void setResourceFilename(const std::string& _flename) { mResourceFileName = _flename; }
	void addResourceLocation(const std::string& _name, bool _recursive = false);

	virtual void setupResources();

	void injectMouseMove(int _absx, int _absy, int _absz);
	void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
	void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
	void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
	void injectKeyRelease(MyGUI::KeyCode _key);

	void createGui();
	void destroyGui();

	void _windowResized();

	void updateFPS();

	void resizeRender(int _width, int _height);
	bool createRender(IDirect3DDevice9 * pDevice);
	void drawOneFrame();

private:
	MyGUI::Gui* mGUI;
	MyGUI::DirectXPlatform* mPlatform;
	HWND hWnd;
    IDirect3D9 *mD3d;
	IDirect3DDevice9 *mDevice;
    D3DPRESENT_PARAMETERS mD3dpp;
	HINSTANCE hInstance;

	bool mExit;

	std::string mRootMedia;
	std::string mResourceFileName;
	bool mIsDeviceLost;

	InputServicePtr		m_pInputService;
	int					m_nCursorX;
	int					m_nCursorY;

	MyGUI::Window*		m_pMainWindow;
	MyGUI::Edit*		m_pEdit;

protected:
	float m_fRenderTime;
};
