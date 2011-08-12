<Shader>
  
  <RenderState>
    <Blending Enabled="true" BlendMode="Normal" />
    <Depth WriteEnabled="true" TestEnabled="true" TestFunction="Less" />
    <Culling CullMode="CW" />
  </RenderState>

  <AttributeBindings>
    <Attribute Name="a_Position" Location="0" />
    <Attribute Name="a_Normal" Location="1" />
    <Attribute Name="a_TexCoord" Location="3" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="u_World" BindToGlobal="World" />
    <Parameter Name="u_View" BindToGlobal="View" />
    <Parameter Name="u_Proj" BindToGlobal="Projection" />

    <Parameter Name="u_MaterialEmissive" BindToGlobal="Material.Emissive" />
    <Parameter Name="u_MaterialDiffuse" BindToGlobal="Material.Diffuse" />

    <Parameter Name="u_AmbientLight" BindToGlobal="AmbientLight" />
    <Parameter Name="u_NumLights" BindToGlobal="NumActiveLights" />
    <Parameter Name="u_LightPositionsAndFalloffs" BindToGlobal="LightPositionsAndFalloffs" />
    <Parameter Name="u_LightColors" BindToGlobal="LightColors" />

    <Parameter Name="u_DiffuseTexture" Value="0" />
  </Parameters>
  
  <Techniques>

    <Technique Name="FragmentLighting" Profile="GLSL" >
      <VertexShader>
        <![CDATA[

          // Attributes
          attribute vec3 a_Position;  // aVertexPosition;
          attribute vec3 a_Normal;    // aVertexNormal;
          attribute vec2 a_TexCoord;  // aTextureCoord;

          // Uniforms - Transform
          uniform mat4 u_World;
          uniform mat4 u_View;
          uniform mat4 u_Proj;

          // Varying - Outputs
          varying vec2 v_TexCoord;    // vTextureCoord;
          varying vec3 v_Normal;      // vTransformedNormal;
          varying vec4 v_Position;    // vPosition

          void main(void) 
          {
            // Transform the position from object to world space
            v_Position = u_World * vec4(a_Position, 1.0);
            
            // Transform the normal from object to world space
            v_Normal = normalize( u_World * vec4(a_Normal, 0.0)).xyz;
            v_Position.xyz += v_Normal * 0.0001;
            
            // Transform the final position from world to projection space
            mat4 viewProj = u_Proj * u_View;
            gl_Position = viewProj * v_Position;    
            
            // Pass the tex coord through
            v_TexCoord = a_TexCoord;
            
          }
      
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          // Varying - Inputs
          varying vec2 v_TexCoord;
          varying vec3 v_Normal;    // world space
          varying vec4 v_Position;  // world space
          
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
                vec3 lightDirection = normalize(lightPosition - v_Position.xyz);

                // N dot L
                float lightWeight = max(dot(normalize(v_Normal), lightDirection), 0.0);
                
                // Apply lighting falloff
                float falloff = u_LightPositionsAndFalloffs[light].w;
                lightWeight *= max(1.0 - pow(distance(v_Position.xyz, lightPosition) / falloff, 2.0), 0.0);
                
                // Accumulate the light
                lightAccumulation += u_LightColors[light] * lightWeight;  
              }
            }
          
            // Sample the diffuse texture
            vec4 fragmentColor = texture2D(u_DiffuseTexture, v_TexCoord);
            
            // Calculate the total final color
            gl_FragColor = vec4(fragmentColor.rgb * lightAccumulation, fragmentColor.a * u_MaterialDiffuse.a);
          }
        ]]>
      </FragmentShader>
    </Technique>

    <Technique Name="VertexLighting" Profile="GLES">
      <VertexShader>
        <![CDATA[

          // Attributes
          attribute vec3 a_Position;  // aVertexPosition;
          attribute vec3 a_Normal;    // aVertexNormal;
          attribute vec2 a_TexCoord;  // aTextureCoord;

          // Uniforms - Transform
          uniform mat4 u_World;
          uniform mat4 u_View;
          uniform mat4 u_Proj;
          
          // Uniforms - Material
          uniform vec4 u_MaterialEmissive;
          
          // Uniforms - Lighting
          uniform vec3 u_AmbientLight;
          uniform int  u_NumLights;
          uniform vec4 u_LightPositionsAndFalloffs[8];      // w contains the falloff
          uniform vec3 u_LightColors[8];

          // Varying - Outputs
          varying vec2 v_TexCoord;
          varying vec3 v_Color;

          void main(void) 
          {
            mat4 viewProj = u_Proj * u_View;
          
            // Transform the position from object to world space
            vec4 position = u_World * vec4(a_Position, 1.0);
            
            // Transform the normal from object to world space
            vec3 normal = normalize( u_World * vec4(a_Normal, 0.0)).xyz;
            position.xyz += normal * 0.0001;
            
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
                vec3 lightDirection = normalize(lightPosition - position.xyz);

                // N dot L
                float lightWeight = max(dot(normalize(normal), lightDirection), 0.0);
                
                // Apply lighting falloff
                float falloff = u_LightPositionsAndFalloffs[light].w;
                lightWeight *= max(1.0 - pow(distance(position.xyz, lightPosition) / falloff, 2.0), 0.0);
                
                // Accumulate the light
                lightAccumulation += u_LightColors[light] * lightWeight;  
              }
            }
            
            // Pass the final lighting value as the color
            v_Color = lightAccumulation;
            
            // Transform the final position from world to projection space
            gl_Position = viewProj * position;    
            
            // Pass the tex coord through
            v_TexCoord = a_TexCoord;
            
          }
      
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          // Varying - Inputs
          varying vec2 v_TexCoord;
          varying vec3 v_Color;
          
          // Uniforms - Material
          uniform vec4 u_MaterialDiffuse;

          // Uniforms - Texturing
          uniform sampler2D u_DiffuseTexture;

          void main(void) 
          {
            // Sample the diffuse texture
            vec4 fragmentColor = texture2D(u_DiffuseTexture, v_TexCoord);
            
            // Calculate the total final color
            gl_FragColor = vec4(fragmentColor.rgb * v_Color, fragmentColor.a * u_MaterialDiffuse.a);
          }
        ]]>
      </FragmentShader>
    </Technique>
    
  </Techniques>

</Shader>