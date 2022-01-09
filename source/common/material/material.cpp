#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: Write this function

        // 1) set the pipeline state and set the shader to be used
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: Write this function

        // 1) call material setup to set the pipeline state and set the shader to be used
        Material::setup();

        // 2) set the Uniform tint in tinted shader with the data member tint
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // TODO: Write this function

        // 1) call material setup to set the pipeline state and set the shader to be used
        Material::setup();

        // 2) set the Uniform alphaThreshold in textured shader with the data member alphathreshold
        shader->set("alphaThreshold", alphaThreshold);

        // 3) set the active texture unit to 0 then bind the texture to it
        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        // 4) bind the sampler to the same unit number 0
        sampler->bind(0);

        // 5) we send 0(the index of the texture unit we used above)to the "tex" uniform in textured shader
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup() const
    {
        // TODO: Write this function
        // 1) call material setup to set the pipeline state and set the shader to be used
        // std::cout << "I'm Hereeee" << std::endl;
        Material::setup();

        // 2) set the Uniform alphaThreshold in textured shader with the data member alphathreshold
        shader->set("alphaThreshold", alphaThreshold);
        glActiveTexture(GL_TEXTURE6);
        texture->bind();

        // 4) bind the sampler to the same unit number 0
        sampler->bind(6);

        // 5) we send 0(the index of the texture unit we used above)to the "tex" uniform in textured shader
        shader->set("tex", 6);
        if (textures[0])
        {
            glActiveTexture(GL_TEXTURE0);
            textures[0]->bind();
            // 4) bind the sampler to the same unit number 0
            sampler->bind(0);
            // 5) we send 0(the index of the texture unit we used above)to the "tex" uniform in textured shader
            shader->set("material.albedo", 0);
        }
        if (textures[1])
        {
            glActiveTexture(GL_TEXTURE1);
            textures[1]->bind();
            sampler->bind(1);
            shader->set("material.specular", 1);
        }
        if (textures[2])
        {
            glActiveTexture(GL_TEXTURE2);
            textures[2]->bind();
            sampler->bind(2);
            shader->set("material.roughness", 2);
        }
        if (textures[3])
        {
            glActiveTexture(GL_TEXTURE3);
            textures[3]->bind();
            sampler->bind(3);
            shader->set("material.ambient_occlusion", 3);
        }
        if (textures[4])
        {
            glActiveTexture(GL_TEXTURE4);
            textures[4]->bind();
            sampler->bind(4);
            shader->set("material.emission", 4);
        }
    }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json &data)
    {
        TexturedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        textures[0] = AssetLoader<Texture2D>::get(data.value("albedo-texture", ""));
        textures[1] = AssetLoader<Texture2D>::get(data.value("specular-texture", ""));
        textures[2] = AssetLoader<Texture2D>::get(data.value("roughness-texture", ""));
        textures[3] = AssetLoader<Texture2D>::get(data.value("ambient_occlusion-texture", ""));
        textures[4] = AssetLoader<Texture2D>::get(data.value("emission-texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
}