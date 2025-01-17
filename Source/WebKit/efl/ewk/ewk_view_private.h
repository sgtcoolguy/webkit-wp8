/*
    Copyright (C) 2009-2010 ProFUSION embedded systems
    Copyright (C) 2009-2012 Samsung Electronics
    Copyright (C) 2012 Intel Corporation

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef ewk_view_private_h
#define ewk_view_private_h

#include "Frame.h"
#include "NetworkStorageSession.h"
#include "Page.h"
#include "Widget.h"
#include "ewk_view.h"

namespace WebCore {
#if ENABLE(INPUT_TYPE_COLOR)
class Color;
class ColorChooserClient;
#endif

class Cursor;
class GraphicsContext3D;
class GraphicsLayer;
class HTMLPlugInElement;
class IntRect;
class IntSize;
class PopupMenuClient;
}

void ewk_view_cursor_set(Evas_Object* ewkView, const WebCore::Cursor& cursor);
void ewk_view_ready(Evas_Object* ewkView);
void ewk_view_input_method_state_set(Evas_Object* ewkView, bool active);
void ewk_view_title_set(Evas_Object* ewkView, const Ewk_Text_With_Direction* title);
void ewk_view_uri_changed(Evas_Object* ewkView);
void ewk_view_load_document_finished(Evas_Object* ewkView, Evas_Object* frame);
void ewk_view_load_started(Evas_Object* ewkView, Evas_Object* ewkFrame);
void ewk_view_load_provisional(Evas_Object* ewkView);
void ewk_view_load_provisional_failed(Evas_Object* ewkView, const Ewk_Frame_Load_Error* error);
void ewk_view_frame_main_load_started(Evas_Object* ewkView);
void ewk_view_frame_main_cleared(Evas_Object* ewkView);
void ewk_view_frame_main_icon_received(Evas_Object* ewkView);
void ewk_view_load_finished(Evas_Object* ewkView, const Ewk_Frame_Load_Error* error);
void ewk_view_load_error(Evas_Object* ewkView, const Ewk_Frame_Load_Error* error);
void ewk_view_load_progress_changed(Evas_Object* ewkView);
void ewk_view_load_show(Evas_Object* ewkView);
void ewk_view_onload_event(Evas_Object* ewkView, Evas_Object* frame);
void ewk_view_restore_state(Evas_Object* ewkView, Evas_Object* frame);
Evas_Object* ewk_view_window_create(Evas_Object* ewkView, bool javascript, const WebCore::WindowFeatures* coreFeatures);
void ewk_view_window_close(Evas_Object* ewkView);

void ewk_view_mouse_link_hover_in(Evas_Object* ewkView, void* data);
void ewk_view_mouse_link_hover_out(Evas_Object* ewkView);

void ewk_view_toolbars_visible_set(Evas_Object* ewkView, bool visible);
void ewk_view_toolbars_visible_get(Evas_Object* ewkView, bool* visible);

void ewk_view_statusbar_visible_set(Evas_Object* ewkView, bool visible);
void ewk_view_statusbar_visible_get(Evas_Object* ewkView, bool* visible);
void ewk_view_statusbar_text_set(Evas_Object* ewkView, const char* text);

void ewk_view_scrollbars_visible_set(Evas_Object* ewkView, bool visible);
void ewk_view_scrollbars_visible_get(Evas_Object* ewkView, bool* visible);

void ewk_view_menubar_visible_set(Evas_Object* ewkView, bool visible);
void ewk_view_menubar_visible_get(Evas_Object* ewkView, bool* visible);

void ewk_view_tooltip_text_set(Evas_Object* ewkView, const char* text);

void ewk_view_add_console_message(Evas_Object* ewkView, const char* message, unsigned int lineNumber, const char* sourceID);

void ewk_view_run_javascript_alert(Evas_Object* ewkView, Evas_Object* frame, const char* message);
bool ewk_view_run_javascript_confirm(Evas_Object* ewkView, Evas_Object* frame, const char* message);
bool ewk_view_run_before_unload_confirm(Evas_Object* ewkView, Evas_Object* frame, const char* message);
bool ewk_view_run_javascript_prompt(Evas_Object* ewkView, Evas_Object* frame, const char* message, const char* defaultValue, const char** value);
bool ewk_view_should_interrupt_javascript(Evas_Object* ewkView);
int64_t ewk_view_exceeded_application_cache_quota(Evas_Object* ewkView, Ewk_Security_Origin *origin, int64_t defaultOriginQuota, int64_t totalSpaceNeeded);
uint64_t ewk_view_exceeded_database_quota(Evas_Object* ewkView, Evas_Object* frame, const char* databaseName, uint64_t currentSize, uint64_t expectedSize);

bool ewk_view_run_open_panel(Evas_Object* ewkView, Evas_Object* frame, Ewk_File_Chooser* fileChooser, Eina_List** selectedFilenames);

WebCore::Page* ewk_view_core_page_get(const Evas_Object* ewkView);

void ewk_view_frame_rect_changed(Evas_Object* ewkView);

WTF::PassRefPtr<WebCore::Widget> ewk_view_plugin_create(Evas_Object* ewkView, Evas_Object* frame, const WebCore::IntSize& pluginSize, WebCore::HTMLPlugInElement* element, const WebCore::URL& url, const WTF::Vector<WTF::String>& paramNames, const WTF::Vector<WTF::String>& paramValues, const WTF::String& mimeType, bool loadManually);

#if ENABLE(INPUT_TYPE_COLOR)
void ewk_view_color_chooser_new(Evas_Object* ewkView, WebCore::ColorChooserClient* client, const WebCore::Color& initialColor);
void ewk_view_color_chooser_changed(Evas_Object* ewkView, const WebCore::Color& newColor);
#endif

void ewk_view_popup_new(Evas_Object* ewkView, WebCore::PopupMenuClient* client, int selected, const WebCore::IntRect& rect);
void ewk_view_viewport_attributes_set(Evas_Object* ewkView, const WebCore::ViewportArguments& arguments);

void ewk_view_download_request(Evas_Object* ewkView, Ewk_Download* download);

void ewk_view_editor_client_contents_changed(Evas_Object* ewkView);
void ewk_view_editor_client_selection_changed(Evas_Object* ewkView);

bool ewk_view_focus_can_cycle(Evas_Object* ewkView, Ewk_Focus_Direction direction);

#if ENABLE(NETSCAPE_PLUGIN_API)
void ewk_view_js_window_object_clear(Evas_Object* ewkView, Evas_Object* frame);
#endif

#if ENABLE(TOUCH_EVENTS)
void ewk_view_need_touch_events_set(Evas_Object*, bool needed);
bool ewk_view_need_touch_events_get(const Evas_Object*);
#endif

void ewk_view_layout_if_needed_recursive(Ewk_View_Private_Data* priv);

bool ewk_view_navigation_policy_decision(Evas_Object* ewkView, Ewk_Frame_Resource_Request* request, WebCore::NavigationType);

void ewk_view_contents_size_changed(Evas_Object* ewkView, Evas_Coord width, Evas_Coord height);

WebCore::FloatRect ewk_view_page_rect_get(const Evas_Object* ewkView);

void ewk_view_mixed_content_displayed_set(Evas_Object* ewkView, bool hasDisplayed);
void ewk_view_mixed_content_run_set(Evas_Object* ewkView, bool hasRun);

void ewk_view_root_graphics_layer_set(Evas_Object* ewkView, WebCore::GraphicsLayer* rootLayer);
void ewk_view_mark_for_sync(Evas_Object* ewkView);

void ewk_view_force_paint(Evas_Object* ewkView);

#if ENABLE(FULLSCREEN_API)
void ewk_view_fullscreen_enter(const Evas_Object* ewkView);
void ewk_view_fullscreen_exit(const Evas_Object* ewkView);
#endif

namespace EWKPrivate {
WebCore::Page *corePage(const Evas_Object *ewkView);
PlatformPageClient corePageClient(Evas_Object* ewkView);
WebCore::NetworkStorageSession* storageSession(const Evas_Object* ewkView);
} // namespace EWKPrivate

#endif // ewk_view_private_h
