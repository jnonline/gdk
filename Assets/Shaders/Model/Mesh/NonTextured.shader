<Shader>

  <RenderState>
    <Blending Enabled="true" BlendMode="Normal" />
    <Depth WriteEnabled="true" TestEnabled="true" TestFunction="Less" />
    <Culling CullMode="CW" />
  </RenderState>

  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_normal" Location="1" />
    <Attribute Name="a_color" Location="2" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="u_World" BindToGlobal="World" />
    <Parameter Name="u_View" BindToGlobal="View" />
    <Parameter Name="u_Proj" BindToGlobal="Projection" />

    <Parameter Name="u_MaterialEmissive" BindToGlobal="Material.Emissive" />

    <Parameter Name="u_AmbientLight" BindToGlobal="AmbientLight" />
    <Parameter Name="u_NumLights" BindToGlobal="NumActiveLights" />
    <Parameter Name="u_LightPositionsAndFalloffs" BindToGlobal="LightPositionsAndFalloffs" />
    <Parameter Name="u_LightColors" BindToGlobal="LightColors" />

  </Parameters>
  
  <Techniques>

    <Technique Name="FragmentLighting">
      <SupportedPlatform Name="Windows" />
      <SupportedPlatform Name="Mac" />
      <SupportedPlatform Name="Linux" />

      <VertexShader>
        <![CDATA[

          // Attributes
          attribute vec3 a_position;
          attribute vec3 a_normal;
          attribute vec4 a_color;

          // Uniforms - Transform
          uniform mat4 u_World;
          uniform mat4 u_View;
          uniform mat4 u_Proj;

          // Varying - Outputs
          varying vec3 v_normal;
          varying vec4 v_position;
          varying vec4 v_color;

          void main(void) 
          {
            // Transform the position from object to world space
            v_position = u_World * vec4(a_position, 1.0);
            
            // Transform the normal from object to world space
            v_normal = normalize( u_World * vec4(a_normal, 0.0)).xyz;
            v_position.xyz += v_normal * 0.0001;
            
            // Transform the final position from world to projection space
            mat4 viewProj = u_Proj * u_View;
            gl_Position = viewProj * v_position;    
            
            // Pass the color through
            v_color = a_color;
          }
      
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          // Varying - Inputs
          varying vec4 v_color;
          varying vec3 v_normal;    // world space
          varying vec4 v_position;  // world space
          
          // Uniforms - Material
          uniform vec4 u_MaterialEmissive;
          
          // Uniforms - Lighting
          uniform vec3 u_AmbientLight;
          uniform int  u_NumLights;
          uniform vec4 u_LightPositionsAndFalloffs[8];      // w contains the falloff
          uniform vec3 u_LightColors[8];

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
            
            // Calculate the total final color
            gl_FragColor = vec4(v_color.rgb * lightAccumulation, v_color.a);
          }
        ]]>
      </FragmentShader>
    </Technique>

    <Technique Name="VertexLighting">
      <VertexShader>
        <![CDATA[

          // Attributes
          attribute vec3 a_position;
          attribute vec3 a_normal;
          attribute vec4 a_color;

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
          varying vec4 v_color;

          void main(void) 
          {
            mat4 viewProj = u_Proj * u_View;
          
            // Transform the position from object to world space
            vec4 position = u_World * vec4(a_position, 1.0);
            
            // Transform the normal from object to world space
            vec3 normal = normalize( u_World * vec4(a_normal, 0.0)).xyz;
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
            v_color.rgb = lightAccumulation.rgb * a_color.rgb;
            v_color.a = a_color.a;
            
            // Transform the final position from world to projection space
            gl_Position = viewProj * position;    
          }
      
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          // Varying - Inputs
          varying vec4 v_color;

          void main(void) 
          {            
            // Calculate the total final color
            gl_FragColor = v_color;
          }
        ]]>
      </FragmentShader>
    </Technique>

  </Techniques>

</Shader>