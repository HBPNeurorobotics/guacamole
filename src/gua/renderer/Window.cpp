/******************************************************************************
 * guacamole - delicious VR                                                   *
 *                                                                            *
 * Copyright: (c) 2011-2013 Bauhaus-Universität Weimar                        *
 * Contact:   felix.lauer@uni-weimar.de / simon.schneegans@uni-weimar.de      *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify it    *
 * under the terms of the GNU General Public License as published by the Free *
 * Software Foundation, either version 3 of the License, or (at your option)  *
 * any later version.                                                         *
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.                                                          *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with this program. If not, see <http://www.gnu.org/licenses/>.             *
 *                                                                            *
 ******************************************************************************/

// class header
#include <gua/renderer/Window.hpp>

// guacamole headers
#include <gua/platform.hpp>
#include <gua/renderer/Pipeline.hpp>
#include <gua/databases.hpp>
#include <gua/utils.hpp>

// external headers
#include <scm/gl_core/window_management/window.h>

#include <sstream>
#include <iostream>

#if WIN32 
  #include <Windows.h>
#endif

namespace gua {

////////////////////////////////////////////////////////////////////////////////

Window::Window(Configuration const& configuration)
  : WindowBase(configuration) {}

////////////////////////////////////////////////////////////////////////////////

Window::~Window() {

  close();
}

////////////////////////////////////////////////////////////////////////////////

void Window::open() {

  if (window_) {
   window_->hide();
  }

  ctx_.context.reset();
  ctx_.display.reset();
  window_.reset();

  scm::gl::data_format color_format = scm::gl::FORMAT_RGBA_8;
  scm::gl::data_format depth_stencil_format = scm::gl::FORMAT_D24_S8;
  bool double_buffer = true;
  bool quad_buffer_stereo = false;

  if (config.get_stereo_mode() == StereoMode::QUAD_BUFFERED) {
    quad_buffer_stereo = true;
  }

  scm::gl::wm::surface::format_desc window_format(
      color_format, depth_stencil_format, double_buffer, quad_buffer_stereo);

  int version_major = 4;
  int version_minor = 4;
  bool compatibility_profile = false;
  bool debug = config.get_debug();
  bool forward_compatible = false;
  bool es_profile = false;
  scm::gl::wm::context::attribute_desc context_attribs(
      version_major,
      version_minor,
      compatibility_profile,
      config.get_debug(),
      forward_compatible,
      es_profile);

  ctx_.display.reset(new scm::gl::wm::display(config.get_display_name()));

  window_.reset(new scm::gl::wm::window(
      ctx_.display,
      config.get_title(),
      config.get_window_position(),
      math::vec2ui(config.get_size().x, config.get_size().y),
      window_format));

  ctx_.context.reset(new scm::gl::wm::context(window_, context_attribs));

  set_active(true);
  window_->show();
}

////////////////////////////////////////////////////////////////////////////////

bool Window::get_is_open() const {
  return window_ != nullptr;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::should_close() const {
  return false;
}

////////////////////////////////////////////////////////////////////////////////

void Window::close() {
  if (get_is_open()) {
    window_->hide();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::set_active(bool active) {
  ctx_.context->make_current(window_, active);
  if (!ctx_.render_device) {
    init_context();
  }
}

////////////////////////////////////////////////////////////////////////////////

void Window::swap_buffers_impl() {
  // glfwSwapInterval(config.get_enable_vsync()? 1 : 0);
  window_->swap_buffers(config.get_enable_vsync());
}

////////////////////////////////////////////////////////////////////////////////
void Window::process_events()
{
#if WIN32
  MSG message;
  if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
  {
    // handle events?

    // process and pass message forwards
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
#else
#endif
}

////////////////////////////////////////////////////////////////////////////////

}
