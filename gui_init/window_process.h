#pragma once
#include "included.h"
#include <sstream>
#include "../menu.h"
#include "create.h"
#include "finish.h"
#include "../im_manager/imgui_freetype.h"

struct {

    int init(ImVec2 pos, ImVec2 size) 
    {

        wnd.wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
        RegisterClassEx(&wnd.wc);

        menu::get().hDCScreen = GetDC(NULL);
        int Horres  = GetDeviceCaps(menu::get().hDCScreen, HORZRES);
        int Vertres = GetDeviceCaps(menu::get().hDCScreen, VERTRES);
        ReleaseDC(NULL, menu::get().hDCScreen);

        wnd.hwnd = CreateWindow(_T("ImGui Example"), _T("shuffle_app"), gui::get().WINDOWS_DATA_type_of_window, pos.x, pos.y, size.x, size.y, NULL, NULL, wnd.wc.hInstance, NULL);

        if ((wnd.pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        {
            UnregisterClass(_T("ImGui Example"), wnd.wc.hInstance);
            return 0;
        }
        ZeroMemory(&m_device.g_d3dpp, sizeof(m_device.g_d3dpp));
        m_device.g_d3dpp.Windowed = TRUE;
        m_device.g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        m_device.g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        m_device.g_d3dpp.EnableAutoDepthStencil = TRUE;
        m_device.g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        m_device.g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        if (wnd.pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd.hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_device.g_d3dpp, &m_device.g_pd3dDevice) < 0)
        {
            wnd.pD3D->Release();
            UnregisterClass(_T("ImGui Example"), wnd.wc.hInstance);
            return 0;
        }

        ImGui::CreateContext();

        ImFontConfig cfg;
        cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting;

        /* Font adding realization. I have vector, that i fill up with my class, that contains name of fonts, size and etc*/
        for (int i = 0; i < gui::get().fonts.size(); i++)
        {
            switch (gui::get().fonts[i].font_mode)
            {
                /* This case means that font is going to be added using his name */
            case 0:
            {
                std::stringstream ss;
                ss << "C:\\Windows\\Fonts\\" << gui::get().fonts[i].font_name << ".ttf";
                ImGuiIO& io = ImGui::GetIO(); (void)io;

                gui::get().fonts[i].font_addr = io.Fonts->AddFontFromFileTTF(ss.str().c_str(), gui::get().fonts[i].size_of_font, &cfg, io.Fonts->GetGlyphRangesCyrillic());

            }
            break;
            /* This case means that font is going to be added using memory */
            case 1:
            {

                ImGuiIO& io = ImGui::GetIO(); (void)io;
                gui::get().fonts[i].font_addr = io.Fonts->AddFontFromMemoryTTF(gui::get().fonts[i].font_data, gui::get().fonts[i].font_data_size, gui::get().fonts[i].size_of_font, &cfg, io.Fonts->GetGlyphRangesCyrillic());

            }
            break;
            /* This case means that nam vsem pizda. renderu pizda */
            case -1:
                MessageBox(0, L"Font not initializated", L"Ok", MB_OK);
                return 0;
                break;
            }
        }

    }

    void process_msg() {


        MSG msg;
        // Main loop
        ZeroMemory(&msg, sizeof(msg));
        ShowWindow(wnd.hwnd, SW_SHOWDEFAULT);
        UpdateWindow(wnd.hwnd);

        while (msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }

            m_directx.new_frame(); {
                menu::get().init();
            }
            m_directx.finish_frame();


            m_directx.create_state();
        }
    }

} window;