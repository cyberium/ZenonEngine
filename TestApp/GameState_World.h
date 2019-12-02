#pragma once

class CGameState_World : public CGameState
{
    typedef CGameState base;
public:
    CGameState_World(const IApplication * _application, std::shared_ptr<IRenderWindow> RenderWindow);
    virtual ~CGameState_World();

    // IGameState
    bool Init();
    void Destroy();

    virtual void OnResize(ResizeEventArgs& e) override;

    virtual void OnPreRender(RenderEventArgs& e) override;
    virtual void OnRender(RenderEventArgs& e) override;
    virtual void OnPostRender(RenderEventArgs& e) override;
    virtual void OnRenderUI(RenderEventArgs& e) override;


private:
    void Load3D();
    void LoadUI();

private:
    std::shared_ptr<CUITextNode>              m_CameraPosText;
    std::shared_ptr<CUITextNode>              m_CameraRotText;

private:
    const uint16 c_WindowsWidth = 1280;
    const uint16 c_WindowsHeight = 1024;
    //const vec4 g_ClearColor = vec4(0.39f, 0.58f, 0.93f, 1.0f);
    const vec4 g_ClearColor = vec4(0.2f, 0.2f, 0.2f, 0.3f);
};