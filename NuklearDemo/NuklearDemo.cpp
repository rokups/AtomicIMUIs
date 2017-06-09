/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Rokas Kupstys
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <Atomic/Engine/Application.h>
#include <Atomic/Engine/EngineDefs.h>
#include <Atomic/Input/Input.h>
#include <Atomic/Graphics/Renderer.h>
#include <Atomic/Graphics/Graphics.h>
#include <Atomic/Graphics/Zone.h>
#include <Atomic/Graphics/Octree.h>
#include <Atomic/Graphics/Camera.h>
#include <Atomic/Scene/Scene.h>
#include <Atomic/Core/CoreEvents.h>

#include <IconsFontAwesome.h>
#include <AtomicNuklearUI.h>


using namespace Atomic;
extern "C"
{
#define LEN(a) (sizeof(a)/sizeof(a)[0])
#include <limits.h>
#include "../dep/AtomicNuklearUI/nuklear/demo/overview.c"
#include "../dep/AtomicNuklearUI/nuklear/demo/style.c"
#include "../dep/AtomicNuklearUI/nuklear/demo/calculator.c"
#include "../dep/AtomicNuklearUI/nuklear/demo/node_editor.c"
};

class NuklearDemo
        : public Application
{
ATOMIC_OBJECT(NuklearDemo, Application);
public:
    SharedPtr<Scene> _scene;
    SharedPtr<Viewport> _viewport;
    WeakPtr<NuklearUI> _nk;

    NuklearDemo(Context* context)
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
        GetSubsystem<Input>()->SetMouseVisible(true);

        context_->RegisterFactory<NuklearUI>();
        context_->RegisterSubsystem(context_->CreateObject<NuklearUI>());

        _nk = GetSubsystem<NuklearUI>();
        _nk->AddDefaultFont();
        _nk->AddFont("fontawesome-webfont.ttf", 0, {ICON_MIN_FA, ICON_MAX_FA, 0}, NKUI_FONT_MERGE | NKUI_FONT_SET_DEFAULT);
        SubscribeToEvent(E_NUKLEARFRAME, std::bind(&NuklearDemo::RenderUI, this));

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
        auto nk = _nk->GetNkContext();

        nk_begin(nk, "Icon font demo", nk_rect(450, 10, 300, 150), NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE);
        {
            nk_layout_row_dynamic(nk, 22, 1);
            nk_labelf(nk, NK_TEXT_ALIGN_LEFT, "We can mix icons %s with %s text.", ICON_FA_STAR, ICON_FA_CHECK);

            float scale, old_scale;
            scale = old_scale = _nk->GetScale();
            nk_property_float(nk, "UI Scale", 0.8f, &scale, 1.2f, 0.01f, 0.003f);
            if (scale != old_scale)
                _nk->SetScale(scale);

            nk_label(nk, "Theme:", NK_TEXT_ALIGN_LEFT);
            const char* themes[] = {"Black", "White", "Red", "Blue", "Dark"};
            static int theme_index = 0;
            nk_combobox(nk, themes, 5, &theme_index, 22, nk_vec2(nk_widget_width(nk), 150));
            set_style(nk, (theme)theme_index);
        }
        nk_end(nk);

        overview(nk);
        calculator(nk);
        node_editor(nk);
    }
};

ATOMIC_DEFINE_APPLICATION_MAIN(NuklearDemo);
