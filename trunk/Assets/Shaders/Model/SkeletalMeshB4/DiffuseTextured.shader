<Shader>
  
  <RenderState>
    <Blending Enabled="true" BlendMode="Normal" />
    <Depth WriteEnabled="true" TestEnabled="true" TestFunction="Less" />
    <Culling CullMode="CW" />
  </RenderState>

  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_normal" Location="1" />
    <Attribute Name="a_texCoord" Location="3" />
    <Attribute Name="a_boneIndices" Location="5" />
    <Attribute Name="a_boneWeights" Location="6" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="u_World" BindToGlobal="World" />
    <Parameter Name="u_View" BindToGlobal="View" />
    <Parameter Name="u_Proj" BindToGlobal="Projection" />
    <Parameter Name="u_BoneMatrices" />

    <Parameter Name="u_MaterialEmissive" BindToGlobal="Material.Emissive" />
    <Parameter Name="u_MaterialDiffuse" BindToGlobal="Material.Diffuse" />

    <Parameter Name="u_AmbientLight" BindToGlobal="AmbientLight" />
    <Parameter Name="u_NumLights" BindToGlobal="NumActiveLights" />
    <Parameter Name="u_LightPositionsAndFalloffs" BindToGlobal="LightPositionsAndFalloffs" />
    <Parameter Name="u_LightColors" BindToGlobal="LightColors" />

    <Parameter Name="u_DiffuseTexture" Value="0" />
  </Parameters>
  
  <Techniques>

    <Technique Name="Simple">
      
      <VertexShader>
        <![CDATA[

          #define MAX_BONES 40
          
          // Attributes
          attribute vec4 a_position;
          attribute vec3 a_normal;
          attribute vec2 a_texCoord;
          attribute vec4 a_boneIndices;
          attribute vec4 a_boneWeights;

          // Uniforms - Transform
          uniform mat4 u_World;
          uniform mat4 u_View;
          uniform mat4 u_Proj;
          uniform vec4 u_BoneMatrices[MAX_BONES * 3];

          // Varying - Outputs
          varying vec4 v_position;
          varying vec3 v_normal;
          varying vec2 v_texCoord;
          
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
            int index = int(a_boneIndices[c_zero]) * c_three;
            SkinPosition(position, a_boneWeights[c_zero], index, skinnedPosition);
            SkinNormal(normal, a_boneWeights[c_zero], index, skinnedNormal);
            
            index = int(a_boneIndices[c_one]) * c_three;
            SkinPosition(position, a_boneWeights[c_one], index, skinnedPosition);
            SkinNormal(normal, a_boneWeights[c_one], index, skinnedNormal);
            
            index = int(a_boneIndices[c_two]) * c_three;
            SkinPosition(position, a_boneWeights[c_two], index, skinnedPosition);
            SkinNormal(normal, a_boneWeights[c_two], index, skinnedNormal);
            
            index = int(a_boneIndices[c_three]) * c_three;
            SkinPosition(position, a_boneWeights[c_three], index, skinnedPosition);
            SkinNormal(normal, a_boneWeights[c_three], index, skinnedNormal);
          }
          
          // -----------------------------------
          void main(void) 
          {
            mat4 viewProj = u_Proj * u_View;
                    
            // Skin the position & normal
            Do4BoneSkinning(a_position, a_normal, v_position, v_normal);

            // Transform the position from object to world space
            v_position = u_World * v_position;
            
            // Transform the normal from object to world space
            v_normal = normalize( u_World * vec4(v_normal, 0.0)).xyz;
            v_position.xyz += v_normal * 0.0001;
            
            // Transform the final position from world to projection space
            gl_Position = viewProj * v_position;    
            
            // Pass the tex coord through
            v_texCoord = a_texCoord;
            
          }
      
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          // Varying - Inputs
          varying vec2 v_texCoord;
          varying vec3 v_normal;    // world space
          varying vec4 v_position;  // world space
          
          // Uniforms - Material
          uniform vec4 u_MaterialEmissive;
          uniform vec4 u_MaterialDiffuse;
          
          // Uniforms - Lighting
          uniform vec3 u_AmbientLight;
          uniform int  u_NumLights;
          uniform vec4 u_LightPositionsAndFalloffs[8];      // w contains the falloff
          uniform vec3 u_LightColors[8];

          // Uniforms - Texturing
          uniform sampler2D u_DiffuseTexture;

          void main(void) 
          {
            // Start with ambient lighting + Emissive
            vec3 lightAccumulation = u_AmbientLight + u_MaterialEmissive.rgb;
            
            // Are there no active lights?
            if(u_NumLights == 0)
            {
              // Lighting disabled
              lightAccumulation = vec3(1,1,1);
            }
            else
            {
              // Loop through the active lights
              for(int light=0; light < u_NumLights; light++)
              {
                // Get the light position in world space
                vec3 lightPosition = u_LightPositionsAndFalloffs[light].xyz;
                
                // Calculate the direction vector to the light (in world space)
                vec3 lightDirection = normalize(lightPosition - v_position.xyz);

                // N dot L
                float lightWeight = max(dot(normalize(v_normal), lightDirection), 0.0);
                
                // Apply lighting falloff
                float falloff = u_LightPositionsAndFalloffs[light].w;
                lightWeight *= max(1.0 - pow(distance(v_position.xyz, lightPosition) / falloff, 2.0), 0.0);
                
                // Accumulate the light
                lightAccumulation += u_LightColors[light] * lightWeight;  
              }
            }
          
            // Sample the diffuse texture
            vec4 fragmentColor = texture2D(u_DiffuseTexture, v_texCoord);
            
            // Calculate the total final color
            gl_FragColor = vec4(fragmentColor.rgb * lightAccumulation, fragmentColor.a * u_MaterialDiffuse.a);
          }
        ]]>
      </FragmentShader>
    </Technique>

    
  </Techniques>

</Shader>