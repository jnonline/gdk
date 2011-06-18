

#include "BasePCH.h"
#include "../VertexFormats.h"
#include "ModelInstance.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelMesh.h"

using namespace Gdk;

GLuint vb;
GLuint ib;

// ***********************************************************************
ModelInstance::ModelInstance()
{
	this->ParentModel = NULL;
}

// ***********************************************************************
ModelInstance::~ModelInstance()
{
	// Delete any material overrides
	for(vector<ModelMaterial*>::iterator iter = this->Materials.begin(); iter != this->Materials.end(); iter++)
	{
		ModelMaterial* material = *iter;
		if(material != NULL)
			GdkDelete(material);
	}
}
	
// ***********************************************************************
void ModelInstance::Draw()
{
	// NOTE: The caller is responsible for the Global Shared Uniforms
	// - W,V,P transforms
	// - Lighting
	
	// Render the Simple Meshes
	// -----------------------------

	GLuint meshStride = sizeof(VertexP3N3T2);

	// Meshes use 3 vertex attribute arrays
	Graphics::EnableVertexAttribArray(0, true);
	Graphics::EnableVertexAttribArray(1, true);
	Graphics::EnableVertexAttribArray(2, true);
	Graphics::EnableVertexAttribArray(3, false);

	// Loop through the meshes
	for(vector<ModelMesh*>::iterator meshIter = this->ParentModel->Meshes.begin(); meshIter != this->ParentModel->Meshes.end(); meshIter++)
	{
		// Get this mesh
		ModelMesh* mesh = *meshIter;
		
		// Bind the vertex & index buffers
		Graphics::BindVertexBuffer(mesh->VertexBuffer);
		Graphics::BindIndexBuffer(mesh->IndexBuffer);

		// Set the vertex data pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, meshStride, (void*) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, meshStride, (void*) sizeof(GLfloat[3]) );
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, meshStride, (void*) sizeof(GLfloat[6]) );

		// Loop through the mesh parts
		for(vector<ModelMeshPart*>::iterator meshPartIter = mesh->MeshParts.begin(); meshPartIter != mesh->MeshParts.end(); meshPartIter++)
		{
			// Get this mesh part
			ModelMeshPart* meshPart = *meshPartIter;

			// Get the material (Use the base model material, if this instance doesnt have one)
			ModelMaterial* material = this->Materials[meshPart->MaterialIndex];
			if(material == NULL)
				material = this->ParentModel->Materials[meshPart->MaterialIndex];

			// Setup the basic material properties
			Graphics::GlobalUniforms.Material.Emissive->SetFloat4(material->Emissive);
			Graphics::GlobalUniforms.Material.Ambient->SetFloat4(material->Ambient);
			Graphics::GlobalUniforms.Material.Diffuse->SetFloat4(material->Diffuse);
			Graphics::GlobalUniforms.Material.Specular->SetFloat4(material->Specular);
			Graphics::GlobalUniforms.Material.Shininess->SetFloat(material->Shininess);

			// Bind textures
			if((material->Flags & ModelMaterialFlags::DiffuseTextured) > 0 && material->DiffuseTexture != NULL)
				Graphics::BindTexture(material->DiffuseTexture->GLTextureId, TextureUnit::Unit_0);
			if((material->Flags & ModelMaterialFlags::BumpTextured) > 0 && material->BumpTexture != NULL)
				Graphics::BindTexture(material->BumpTexture->GLTextureId, TextureUnit::Unit_1);

			// Apply the shader
			material->MeshShader->Apply();

			// Render the mesh part
			glDrawElements(GL_TRIANGLES, meshPart->IndexCount, GL_UNSIGNED_SHORT, (void*) meshPart->IndexStart);

		} // foreach( MeshPart )

	} // foreach( Mesh )
}
