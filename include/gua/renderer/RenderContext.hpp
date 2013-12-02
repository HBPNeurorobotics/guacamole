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

#ifndef GUA_RENDERCONTEXT_HPP
#define GUA_RENDERCONTEXT_HPP

// external headers
#include <scm/gl_core/config.h>
#include <scm/gl_core/data_formats.h>
#include <scm/core.h>
#include <scm/gl_core.h>
#include <scm/gl_core/window_management/context.h>
#include <scm/gl_core/window_management/display.h>
#include <scm/gl_core/window_management/surface.h>
#include <scm/gl_core/window_management/window.h>

#if SCM_LARGE_DATA == 1
	#include <scm/large_data/virtual_texture/vtexture_system.h>
	#include <scm/large_data/virtual_texture/vtexture_2d_context.h>
	#include <scm/large_data/virtual_texture/vtexture_3d_context.h>
#endif

namespace gua {

/**
 * Information on a specific context.
 *
 * Stores all relevant information on a OpenGL context.
 */
struct RenderContext {

  /**
   * The schism context of this RenderContext.
   */
  scm::gl::wm::context_ptr context;

  /**
   * The display where this context was opened.
   */
  scm::gl::wm::display_ptr display;

  /**
   * The window associated with this context.
   */
  scm::gl::wm::window_ptr window;

  /**
   * The schism render constext associated with this context.
   */
  scm::gl::render_context_ptr render_context;

  /**
   * The schism render device associated with this context.
   */
  scm::gl::render_device_ptr render_device;
#if SCM_LARGE_DATA == 1
  /**
  * The schism render device associated with this context.
  */
  scm::gl::render_device_ptr			     _vtexture_device;
  scm::data::vtexture_2d_context_ptr         _vtexture_2d_context;
  scm::data::vtexture_3d_context_ptr         _vtexture_3d_context;
#endif
  /**
   * The width of the window.
   */
  int width;

  /**
   * The height of the window.
   */
  int height;

  /**
   * A unique ID for this context.
   */
  unsigned id;
};

}

#endif  // GUA_RENDERCONTEXT_HPP
