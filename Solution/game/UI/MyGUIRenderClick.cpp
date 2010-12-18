#include "game_pch.h"
#include "MyGUIRenderClick.h"
#include "MyGUI_DirectXPlatform.h"
#include <efd/Win32/Win32PlatformService.h>

MyGUIRenderClick::MyGUIRenderClick(InputServicePtr pInputService)
	:m_fRenderTime(0.0f)
	, m_pInputService(pInputService)
	, m_nCursorX(0)
	, m_nCursorY(0)
	, mGUI(nullptr)
	, mPlatform(nullptr)
	, hWnd(0)
	, mD3d(nullptr)
	, mDevice(nullptr)
	, mExit(false)
	, mResourceFileName("core.xml")
	, mIsDeviceLost(false)
{
}

MyGUIRenderClick::~MyGUIRenderClick(void)
{
}

void  MyGUIRenderClick::InitUI(IDirect3DDevice9 * pDevice)
{	

	//m_pDevice = pDevice;
	NiRenderer* pkRenderer = NiRenderer::GetRenderer();
	NIASSERT(pkRenderer);
	if(!pkRenderer) return;

// 	NiTexture::FormatPrefs kPrefs;
// 	const NiRenderTargetGroup* pkRTGroup = pkRenderer->GetDefaultRenderTargetGroup();
// 	const NiPixelFormat* pkPixelFormat = pkRTGroup->GetPixelFormat(0);
// 	if (pkPixelFormat->GetBitsPerPixel() == 16)
// 		kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::HIGH_COLOR_16;
// 	else
// 		kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
// 
// 	m_spRenderedTexture = NiRenderedTexture::Create(1<<1024, 
// 		1<<1024, pkRenderer, kPrefs);
// 
// 	if(!m_spRenderedTexture)
// 	{
// 		NIASSERT(FALSE && "Cannot create offscreen framebuffer\n");
// 		return ;
// 	}
// 
// 	m_spRenderTargetGroup = NiRenderTargetGroup::Create(
// 		m_spRenderedTexture->GetBuffer(), pkRenderer, true,
// 		true);
// 	if(!m_spRenderTargetGroup)
// 	{
// 		NIASSERT(FALSE && "Cannot create offscreen framebuffer\n");
// 		return ;
// 	}

	//SetRenderTargetGroup(m_spRenderTargetGroup);
	
	efd::Win32PlatformServicePtr spWin32 = m_pInputService->GetServiceManager()->GetSystemServiceAs<efd::Win32PlatformService>();
	EE_ASSERT(spWin32);
	hWnd = spWin32->GetWindowRef();

	create(pDevice);
 //	addResourceLocation(mRootMedia + "/Demos/Demo_Gui");
 //	addResourceLocation(mRootMedia + "/Common/Scene");
 //	addResourceLocation(mRootMedia + "/Common/Wallpapers");
	//addResourceLocation(mRootMedia + "/Demos/Demo_Colour");
	//addResourceLocation(mRootMedia + "/Demos/Demo_ScrollView");   
   
	//getGUI()->load("test.layout");
	MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("test.layout");
	MyGUI::WidgetPtr pWidget = root.at(0)->findWidget("_Main");
	m_pMainWindow = pWidget->castType<MyGUI::Window>(false);
	pWidget = m_pMainWindow->findWidget("Edit");
	m_pEdit = pWidget->castType<MyGUI::Edit>(false);
}

MyGUI::Char translateWin32Text(MyGUI::KeyCode kc)
{
	static WCHAR deadKey = 0;

	BYTE keyState[256];
	HKL  layout = GetKeyboardLayout(0);
	if ( GetKeyboardState(keyState) == 0 )
		return 0;

	int code = *((int*)&kc);
	unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
	if ( vk == 0 )
		return 0;

	WCHAR buff[3] = { 0, 0, 0 };
	int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
	if (ascii == 1 && deadKey != '\0' )
	{
		// A dead key is stored and we have just converted a character key
		// Combine the two into a single character
		WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
		WCHAR out[3];

		deadKey = '\0';
		if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
			return out[0];
	}
	else if (ascii == 1)
	{
		// We have a single character
		deadKey = '\0';
		return buff[0];
	}
	else if(ascii == 2)
	{
		// Convert a non-combining diacritical mark into a combining diacritical mark
		// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
		// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
		switch(buff[0])	{
			case 0x5E: // Circumflex accent: ?
				deadKey = 0x302; break;
			case 0x60: // Grave accent: ?
				deadKey = 0x300; break;
			case 0xA8: // Diaeresis: ?
				deadKey = 0x308; break;
			case 0xB4: // Acute accent: ?
				deadKey = 0x301; break;
			case 0xB8: // Cedilla: ?
				deadKey = 0x327; break;
			default:
				deadKey = buff[0]; break;
		}
	}

	return 0;
}


void MyGUIRenderClick::PerformRendering(unsigned int uiFrameID)
{
 	NiRenderer* pkRenderer = NiRenderer::GetRenderer();
 	NIASSERT(pkRenderer);
 	if(!pkRenderer) return;
// 	pkRenderer->SetScreenSpaceCameraData();
//   	pkRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup,
//   		NiRenderer::CLEAR_ALL);

	// 处理输入
	NiInputMousePtr pMouse;
	NiInputKeyboardPtr pKeyboard;
	if(m_pInputService->GetInputSystem())
	{
		pMouse = m_pInputService->GetInputSystem()->GetMouse();
		pKeyboard = m_pInputService->GetInputSystem()->GetKeyboard();

		if (pMouse)
		{
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(hWnd, &point);
			m_nCursorX = point.x;
			m_nCursorY = point.y;
			
			if (m_nCursorX < 0)
				m_nCursorX = 0;
			else if (m_nCursorX >= 1280)
				m_nCursorX = 1280 - 1;

			if (m_nCursorY < 0)
				m_nCursorY = 0;
			else if (m_nCursorY >= 720)
				m_nCursorY = 720 - 1;

			injectMouseMove(m_nCursorX, m_nCursorY, 0);

			unsigned int ui;
			for (ui = NiInputMouse::NIM_LEFT; ui < NiInputMouse::NIM_NUM_BUTTONS; ui++)
			{
				if (pMouse->ButtonWasPressed((NiInputMouse::Button)ui))
				{
					injectMousePress(m_nCursorX, m_nCursorY, MyGUI::MouseButton(MyGUI::MouseButton::Enum(ui)));
				}
				else if (pMouse->ButtonWasReleased((NiInputMouse::Button)ui))
				{
					injectMouseRelease(m_nCursorX, m_nCursorY, MyGUI::MouseButton(MyGUI::MouseButton::Enum(ui)));
				}
			}
		}

		if (pKeyboard)
		{
			unsigned int ui;
			for (ui = NiInputKeyboard::KEY_ESCAPE; ui < NiInputKeyboard::KEY_TOTAL_COUNT; ui++)
			{
				if (pKeyboard->KeyWasPressed((NiInputKeyboard::KeyCode)ui))
				{
					MyGUI::KeyCode key = MyGUI::KeyCode(MyGUI::KeyCode::Enum(ui));
					MyGUI::Char text;
					int scan_code = ui;

					if (scan_code > 70 && scan_code < 84)
					{
						static MyGUI::Char nums[13] = { 55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46 };
						text = nums[scan_code-71];
					}
					else if (key == MyGUI::KeyCode::Divide)
					{
						text = '/';
					}
					else
					{
						text = translateWin32Text(key);
					}

					injectKeyPress(key, text);
				}
				else if (pKeyboard->KeyWasReleased((NiInputKeyboard::KeyCode)ui))
				{
					injectKeyRelease(MyGUI::KeyCode(MyGUI::KeyCode::Enum(ui)));
				}
			}
		}
	}
	

	run();

// 	m_pDevice->SetTexture(0, ppTexture);
// 	m_pDevice->SetStreamSource(0, mpBuffer, 0, sizeof(MyGUI::Vertex))
// 	// count in vertexes, triangle_list = vertexes / 3
// 	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _count / 3);

//	pkRenderer->EndUsingRenderTargetGroup();
}

unsigned int MyGUIRenderClick::GetNumObjectsDrawn() const
{
	return 0;
}

float MyGUIRenderClick::GetCullTime() const
{
	return 0.0f;
}

float MyGUIRenderClick::GetRenderTime() const
{
	return m_fRenderTime;
}

void MyGUIRenderClick::_windowResized()
	{
// 		RECT rect = { 0, 0, 0, 0 };
// 		GetClientRect(hWnd, &rect);
// 		int width = rect.right - rect.left;
// 		int height = rect.bottom - rect.top;

		//resizeRender(width, height);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->setViewSize(1280, 720);

		//setInputViewSize(width, height);
	}

	bool MyGUIRenderClick::create(IDirect3DDevice9 * pDevice)
	{
		createRender(pDevice);

		createGui();

		_windowResized();		

		return true;
	}

	void MyGUIRenderClick::run()
	{		
		//captureInput();
		updateFPS();
		drawOneFrame();			
	}

	void MyGUIRenderClick::destroy()
	{
		destroyGui();
	}

	void MyGUIRenderClick::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open(std::string("resources.xml")))
			doc.getLastError();

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				bool root = false;
				if (node->findAttribute("root") != "")
				{
					root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root) mRootMedia = node->getContent();
				}
				addResourceLocation(node->getContent(), false);
			}
		}
	}

	void MyGUIRenderClick::createGui()
	{
		mPlatform = new MyGUI::DirectXPlatform();
		mPlatform->initialise(mDevice);

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise(mResourceFileName);

	}

	void MyGUIRenderClick::destroyGui()
	{
		if (mGUI)
		{
			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}
	

	void MyGUIRenderClick::addResourceLocation(const std::string & _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}	

	void MyGUIRenderClick::updateFPS()
	{
		// calc FPS
		/*static MyGUI::Timer timer;
		const unsigned long interval = 1000; 
		static int count_frames = 0;
		int accumulate = timer.getMilliseconds();
		if (accumulate > interval)
		{
			mInfo->change("FPS", (int)((unsigned long)count_frames * 1000 / accumulate));
			mInfo->update();

			count_frames = 0;
			timer.reset();
		}
		count_frames ++;*/
	}

	void MyGUIRenderClick::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseMove(_absx, _absy, _absz);
	}

	void MyGUIRenderClick::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMousePress(_absx, _absy, _id);
	}

	void MyGUIRenderClick::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseRelease(_absx, _absy, _id);
	}

	void MyGUIRenderClick::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (!mGUI)
			return;

		/*if (_key == MyGUI::KeyCode::Escape)
		{
			mExit = true;
			return;
		}	*/	

		HKL hKL = ::GetKeyboardLayout(0); // 获取当前线程所使用的键盘布局

		if( hKL && ::ImmIsIME(hKL) ) 
		{// 显示输入的汉字
			HIMC hIMC = ::ImmGetContext( hWnd );

			// Get the size of the result string.
			DWORD dwSize = ::ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);

			if (dwSize > 0)
			{
				wchar_t* lwszStr = new wchar_t[dwSize + 1];
				memset(lwszStr, 0, (dwSize + 1));

				// Get the result strings that is generated by IME into lpstr.
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lwszStr, dwSize);				

				m_pMainWindow->setCaption(lwszStr);
				delete[] lwszStr;
			}		


			::ImmReleaseContext( hWnd, hIMC );
		}
		if (hKL && ::ImmIsIME(hKL))
		{// 显示可选择的汉字
			HIMC hIMC;
			hIMC = ImmGetContext(hWnd); 
			DWORD dwCount = 0, dwSize = 0;
			dwSize = ImmGetCandidateListCount(hIMC, &dwCount);
			if (dwCount > 0)
			{
				std::wstring wstrCaption;
				char* pBuf   =   new   char[dwSize + 1];   
				LPCANDIDATELIST pList   =   (LPCANDIDATELIST)pBuf;   

				for   (int i=0;   i<dwCount;   i++)   
				{   
					::ImmGetCandidateList(hIMC,   i,   pList,dwSize);   
					for   (int j=0;   j<pList->dwPageSize;   j++)   
					{
						char* pStr   =   (char*)(pBuf+pList->dwOffset[pList->dwPageStart+j]);
						wchar_t wszStr[MAX_PATH + 1] = {0,};
						MultiByteToWideChar(CP_ACP,0,pStr,-1, wszStr , MAX_PATH);
						wstrCaption = wstrCaption + wszStr + L" ";
					}
				}

				delete[]   pBuf;   

				m_pEdit->setCaption(wstrCaption);
			}
			
			::ImmReleaseContext(hWnd,hIMC);     

			

			return ;  
		}
		else
		{
			mGUI->injectKeyPress(_key, _text);
		}
	}

	void MyGUIRenderClick::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		mGUI->injectKeyRelease(_key);
	}	

	bool MyGUIRenderClick::createRender(IDirect3DDevice9 * pDevice)
	{
		mDevice = pDevice;
		return true;
	}

	void MyGUIRenderClick::drawOneFrame()
	{	
		mPlatform->getRenderManagerPtr()->drawOneFrame();			
	}
