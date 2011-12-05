
// Required <Attribute> elements
//   <Attribute Name="a_BoneIndices" Location="5" />
//   <Attribute Name="a_BoneWeights" Location="6" />

// Required <Parameter> elements
//   <Parameter Name="u_BoneMatrices" />

#define MAX_BONES 40
          
// Attributes
attribute vec4 a_BoneIndices;
attribute vec4 a_BoneWeights;

// Uniforms - Transform
uniform vec4 u_BoneMatrices[MAX_BONES * 3];

// Constants
const int c_zero = 0;
const int c_one = 1;
const int c_two = 2;
const int c_three = 3;

// -----------------------------------
void SkinPosition(in vec4 position, float weight, int index, inout vec4 skinnedPosition)
{
	vec4 temp;
	temp.x = dot(position, u_BoneMatrices[index]);
	temp.y = dot(position, u_BoneMatrices[index + c_one]);
	temp.z = dot(position, u_BoneMatrices[index + c_two]);
	temp.w = position.w;
	skinnedPosition += weight * temp;
}

// -----------------------------------
void SkinNormal(in vec3 normal, float weight, int index, inout vec3 skinnedNormal)
{
	vec3 temp;
	temp.x = dot(normal, u_BoneMatrices[index].xyz);
	temp.y = dot(normal, u_BoneMatrices[index + c_one].xyz);
	temp.z = dot(normal, u_BoneMatrices[index + c_two].xyz);
	skinnedNormal += weight * temp;
}

// -----------------------------------
void Do4BoneSkinning(in vec4 position, in vec3 normal, out vec4 skinnedPosition, out vec3 skinnedNormal)
{
	// Initialize the position & normal vectors to zero
	skinnedPosition = vec4(float(c_zero));
	skinnedNormal = vec3(float(c_zero));

	// Apply all 4 bones to get the skinned position
	int index = int(a_BoneIndices[c_zero]) * c_three;
	SkinPosition(position, a_BoneWeights[c_zero], index, skinnedPosition);
	SkinNormal(normal, a_BoneWeights[c_zero], index, skinnedNormal);

	index = int(a_BoneIndices[c_one]) * c_three;
	SkinPosition(position, a_BoneWeights[c_one], index, skinnedPosition);
	SkinNormal(normal, a_BoneWeights[c_one], index, skinnedNormal);

	index = int(a_BoneIndices[c_two]) * c_three;
	SkinPosition(position, a_BoneWeights[c_two], index, skinnedPosition);
	SkinNormal(normal, a_BoneWeights[c_two], index, skinnedNormal);

	index = int(a_BoneIndices[c_three]) * c_three;
	SkinPosition(position, a_BoneWeights[c_three], index, skinnedPosition);
	SkinNormal(normal, a_BoneWeights[c_three], index, skinnedNormal);
}
      
// USAGE:   Put these calls in your main()
// ----------------------------------------------

// Skin the position & normal
//Do4BoneSkinning(a_Position, a_Normal, v_Position, v_Normal);

// Transform the position & normal from object to world space
//v_Position = u_World * v_Position;
//v_Normal = normalize( u_World * vec4(v_Normal, 0.0)).xyz;
            