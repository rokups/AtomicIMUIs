//
// Copyright (c) 2016 Rokas Kupstys
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Atomic/Engine/Application.h>
#include <Atomic/Engine/EngineDefs.h>
#include <Atomic/Input/Input.h>
#include <Atomic/Graphics/Renderer.h>
#include <Atomic/Graphics/Graphics.h>
#include <Atomic/Graphics/Zone.h>
#include <Atomic/Graphics/Octree.h>
#include <Atomic/Graphics/Camera.h>
#include <Atomic/Scene/Scene.h>

#include <IconsFontAwesome.h>
#include <AtomicImGUI.h>


using namespace Atomic;

void ImGui::ShowTestWindow(bool*);
void ImGui::ShowUserGuide();
void ImGui::ShowStyleEditor(ImGuiStyle*);

class ImGuiDemo
    : public Application
{
    ATOMIC_OBJECT(ImGuiDemo, Application);
public:
    SharedPtr<Scene> _scene;
    SharedPtr<Viewport> _viewport;

    ImGuiDemo(Context* context)
        : Application(context)
    {
    }

    virtual void Setup()
    {
        engineParameters_[EP_WINDOW_TITLE]   = GetTypeName();
        engineParameters_[EP_WINDOW_WIDTH]   = 1440;
        engineParameters_[EP_WINDOW_HEIGHT]  = 960;
        engineParameters_[EP_FULL_SCREEN]    = false;
        engineParameters_[EP_HEADLESS]       = false;
        engineParameters_[EP_SOUND]          = false;
        engineParameters_[EP_RESOURCE_PATHS] = "CoreData;SampleData";
    }

    virtual void Start()
    {
        context_->RegisterFactory<ImGUI>();
        context_->RegisterSubsystem(context_->CreateObject<ImGUI>());

        auto imgui = GetSubsystem<ImGUI>();
        GetSubsystem<Input>()->SetMouseVisible(true);
        imgui->AddFont("fontawesome-webfont.ttf", 0, {ICON_MIN_FA, ICON_MAX_FA, 0}, true);
        SubscribeToEvent(E_IMGUIFRAME, std::bind(&ImGuiDemo::RenderUI, this));

        // For window background
        _scene = new Scene(context_);
        _scene->CreateComponent<Octree>();
        auto zone = _scene->CreateComponent<Zone>();
        zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
        zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
        zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
        zone->SetFogStart(100.0f);
        zone->SetFogEnd(300.0f);
        _viewport = new Viewport(context_, _scene, _scene->CreateChild("Camera")->CreateComponent<Camera>());
        GetSubsystem<Renderer>()->SetViewport(0, _viewport);
    }

    void RenderUI()
    {
        ImGui::Begin("Icon font demo");
        {
            ImGui::Text("We can mix icons %s with %s text.", ICON_FA_STAR, ICON_FA_CHECK);
            auto imgui = GetSubsystem<ImGUI>();
            float scale = imgui->GetScale();
            if (ImGui::SliderFloat("Scale UI", &scale, 0.8f, 1.2f))
                imgui->SetScale(scale);
        }
        ImGui::End();

        ImGui::ShowTestWindow();
        ImGui::ShowUserGuide();
        ImGui::ShowStyleEditor(0);
    }
};

ATOMIC_DEFINE_APPLICATION_MAIN(ImGuiDemo);
