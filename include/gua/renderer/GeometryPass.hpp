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

#ifndef GUA_GEOMETRY_PASS_HPP
#define GUA_GEOMETRY_PASS_HPP

#include <gua/renderer/PipelinePass.hpp>

#include <typeindex>
#include <memory>
#include <unordered_map>

namespace gua {

class Pipeline;
class RessourceRenderer;
class GeometryPass;

class GeometryPassDescription : public PipelinePassDescription {
 public:
  virtual PipelinePassDescription* make_copy() const;
  friend class Pipeline;
  
 protected:
  virtual PipelinePass* make_pass() const;
};

class GeometryPass : public PipelinePass {
 public:

  virtual bool needs_color_buffer_as_input() const { return false; }
  virtual bool writes_only_color_buffer()    const { return false; }
  
  virtual void process(PipelinePassDescription* desc, Pipeline* pipe);

  friend class GeometryPassDescription;

 protected:
  GeometryPass() {}
  ~GeometryPass() {}

  std::shared_ptr<RessourceRenderer> get_renderer(std::type_index const& id);

 private:
  std::unordered_map<std::type_index, std::shared_ptr<RessourceRenderer>> renderers_;
};

}

#endif  // GUA_GEOMETRY_PASS_HPP
