#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: Write this function
            if(faceCulling.enabled)
            {
            glEnable(GL_CULL_FACE);
            // From this point on, all the faces that are not front-faces are discarded 
            // (try flying inside the cube to see that all inner faces are indeed discarded).
            //  Currently we save over 50% of performance on rendering fragments if OpenGL decides to render the back faces first
            glCullFace(faceCulling.culledFace);
            // The glCullFace function has three possible options:
            // GL_BACK: Culls only the back faces.
            // GL_FRONT: Culls only the front faces.
            // GL_FRONT_AND_BACK: Culls both the front and back faces.
            glFrontFace(faceCulling.frontFace);
            // The default value is GL_CCW that stands for counter-clockwise ordering with the other option 
            // being GL_CW which (obviously) stands for clockwise ordering.
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
            
            ////////////////////////////////////////////////////////////////
            if(depthTesting.enabled)
            {
              glEnable(GL_DEPTH_TEST);
              //so that the object vertices in the near appear while the far one not appear like it is correct 3d model
              //draw the near then the far like (glless function)
              glDepthFunc(depthTesting.function);
            }
            else
            {
              glDisable(GL_DEPTH_TEST);
            }
            ////////////////////////////////////////////////////////////////

            if(blending.enabled)
            {
            glEnable(GL_BLEND);
            //if alpha =0 that means that the source is fully transparent,1 means fully opaque
            //at alpha =0 you only see the the destination,while at alpha =1 you only see the source
            //C=@s*Cs  +  @d*Cd
            //where @s = the source factor which can be like 0 ,1 ,gl_src_alpha
            //where @d = the distination factor which can also be like them
            //+ is the blending equation
            glBlendEquation(blending.equation);
            glBlendFunc(blending.sourceFactor,blending.destinationFactor);
            glBlendColor(blending.constantColor.x,blending.constantColor.y,blending.constantColor.z,blending.constantColor.w);
            }
            else
            {
               glDisable(GL_BLEND); 
            }
            ////////////////////////////////////////////////////////////////
            glColorMask(colorMask.x,colorMask.y,colorMask.z,colorMask.w);
            //if depth mask is false it means that i prevent it from being drawn on the depth buffer
            //like if it draw once it wont be drawn again
            // if i move with camera it keeps the last depth drawn an object drawn while moving camera is not drawn 
            //if its depth is nearer than the first object drawn
            //gl colormask will be more useful while rendering transpernt objects
            //z fighting occurs when 2 objects are drawn in same position
            glDepthMask(depthMask);
            ////////////////////////////////////////////////////////////////
        
        }
         
        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}