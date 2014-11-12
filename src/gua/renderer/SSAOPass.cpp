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
#include <gua/renderer/SSAOPass.hpp>

#include <gua/renderer/GBuffer.hpp>
#include <gua/renderer/Pipeline.hpp>
#include <gua/databases/GeometryDatabase.hpp>
#include <gua/databases/Resources.hpp>
#include <gua/utils/Logger.hpp>

namespace gua {

////////////////////////////////////////////////////////////////////////////////

SSAOPassDescription::SSAOPassDescription()
  : PipelinePassDescription(), radius_(1.f), intensity_(1.f), falloff_(0.1f) {

  needs_color_buffer_as_input_ = false;
  writes_only_color_buffer_ = true;
  doClear_ = false;
  rendermode_ = RenderMode::Callback;

  depth_stencil_state_ = boost::make_optional(
      scm::gl::depth_stencil_state_desc(false, false));

  blend_state_ = boost::make_optional(
      scm::gl::blend_state_desc(scm::gl::blend_ops(
                                            true,
                                            scm::gl::FUNC_SRC_ALPHA,
                                            scm::gl::FUNC_ONE_MINUS_SRC_ALPHA,
                                            scm::gl::FUNC_SRC_ALPHA,
                                            scm::gl::FUNC_ONE_MINUS_SRC_ALPHA)));

}

////////////////////////////////////////////////////////////////////////////////

float SSAOPassDescription::radius() const { return radius_; }

////////////////////////////////////////////////////////////////////////////////

SSAOPassDescription& SSAOPassDescription::radius(float radius) {
  radius_ = radius;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

float SSAOPassDescription::intensity() const { return intensity_; }

////////////////////////////////////////////////////////////////////////////////

SSAOPassDescription& SSAOPassDescription::intensity(float intensity) {
  intensity_ = intensity;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

float SSAOPassDescription::falloff() const { return falloff_; }

////////////////////////////////////////////////////////////////////////////////

SSAOPassDescription& SSAOPassDescription::falloff(float falloff) {
  falloff_ = falloff;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

PipelinePassDescription* SSAOPassDescription::make_copy() const {
  return new SSAOPassDescription(*this);
}

////////////////////////////////////////////////////////////////////////////////

PipelinePass SSAOPassDescription::make_pass(RenderContext const& ctx) const {
  PipelinePass pass{};

  pass.shader_ = std::make_shared<ShaderProgram>();
  pass.shader_->create_from_sources(
      Resources::lookup_shader(Resources::shaders_common_fullscreen_quad_vert),
      Resources::lookup_shader(Resources::shaders_ssao_frag));

  pass.needs_color_buffer_as_input_ = needs_color_buffer_as_input_;
  pass.writes_only_color_buffer_    = writes_only_color_buffer_;
  pass.doClear_                     = doClear_;
  pass.rendermode_                  = rendermode_;

  if (depth_stencil_state_) {
    pass.depth_stencil_state_ =
        ctx.render_device->create_depth_stencil_state(*depth_stencil_state_);
  }
  if (blend_state_) {
    pass.blend_state_ = ctx.render_device->create_blend_state(*blend_state_);
  }
  if (rasterizer_state_) {
    pass.rasterizer_state_ =
      ctx.render_device->create_rasterizer_state(*rasterizer_state_);
  }

  auto noise_texture_ = std::make_shared<NoiseTexture>();

  // set uniforms and draw a full screen quad
  pass.process_ = [noise_texture_](
      PipelinePass & pass, PipelinePassDescription * desc, Pipeline & pipe) {
    auto const& ctx(pipe.get_context());
    SSAOPassDescription const* d(
        dynamic_cast<SSAOPassDescription const*>(desc));
    if (d) {
      pass.shader_->set_uniform(ctx, d->radius(), "gua_ssao_radius");
      pass.shader_->set_uniform(ctx, d->intensity(), "gua_ssao_intensity");
      pass.shader_->set_uniform(ctx, d->falloff(), "gua_ssao_falloff");
    }
    pass.shader_
        ->set_uniform(ctx, noise_texture_->get_handle(ctx), "gua_noise_tex");

    pipe.bind_gbuffer_input(pass.shader_);
    pipe.draw_fullscreen_quad();
  };

  return pass;
}

////////////////////////////////////////////////////////////////////////////////

}
