
#include "BasePCH.h"
#include "Material.h"

using namespace Gdk;

// ***********************************************************************
Material::Material(Shader* shader)
{
	// Store the shader
	this->ShaderRef = shader;

	// Create a set of shader parameters
	this->ShaderParameters = shader->CreateParameterSet();

	// Add map entries for all the textures in the shader parameters
	for(ShaderParameterNameMap::Iterator iter = this->ShaderParameters->ParametersByName.Begin(); iter != this->ShaderParameters->ParametersByName.End(); iter++)
	{
		// Is this a texture parameter?
		ShaderParameter* parameter = iter->second;
		if (parameter->GetType() == ShaderUniformType::Sampler2D ||
			parameter->GetType() == ShaderUniformType::SamplerCube )
		{
			this->TextureBindings[parameter] = 0;
		}
	}
}

// ***********************************************************************
Material::~Material()
{
}

// ***********************************************************************
void Material::Apply()
{
	// Loop through the textures
	int textureUnitIndex = 0;
	for(map<ShaderParameter*, GLuint>::iterator iter = this->TextureBindings.begin(); iter != this->TextureBindings.end(); iter++)
	{
		// Bind this shader uniform to the texture unit
		iter->first->SetSampler(textureUnitIndex);

		// Bind this texture to the texture unit
		Graphics::BindTexture(iter->second, TextureUnit::FromIndex(textureUnitIndex));
		
		// Next texture unit
		textureUnitIndex++;
	}

	// Set the shader
	this->ShaderRef->Apply(this->ShaderParameters);
}


// ***********************************************************************
Material* Material::Clone()
{
	// Create the duplicate
	Material* clone = new Material();

	// Copy the shader reference & the current shader parameters
	clone->ShaderRef = this->ShaderRef;
	clone->ShaderParameters = this->ShaderParameters->Clone();
	
	// Copy the texture bindings
	for(map<ShaderParameter*, GLuint>::iterator iter = this->TextureBindings.begin(); iter != this->TextureBindings.end(); iter++)
	{
		clone->TextureBindings[iter->first] = iter->second;
	}

	return clone;
}