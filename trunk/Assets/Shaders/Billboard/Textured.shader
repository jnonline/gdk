<Shader>
  <!-- Billboard - Textured -->

  <Techniques>
    <Technique Name="PixelLighting">
      <VertexShader>
	      <![CDATA[
        
          // Attributes
		      attribute vec4 a_position;
	        attribute vec4 a_texCoord;
	        attribute vec4 a_color;
          attribute vec4 a_facing;
          
          // Uniforms - Billboard Orientation
          uniform vec3 u_WorldUp;
          uniform vec3 u_CameraPosition;
          
           // Uniforms - Transform
          uniform mat4 u_View;
          uniform mat4 u_Proj;
          
          // Uniforms - Lighting
          uniform vec3 u_AmbientLight;
          uniform int  u_NumLights;
          uniform vec4 u_LightPositionsAndFalloffs[8];      // w contains the falloff
          uniform vec3 u_LightColors[8];

          
	        // Varying - Outputs
          varying vec2 v_texCoord;
          varying vec4 v_color;

          void main(void) 
          {
            // Seperate the rotation from the position
            float rotation = a_position.w;
            vec3 worldPosition = a_position.xyz;
            
            // Billboard Orienting
            // --------------------------
          
            // Calculate the orthonormal basis for the billboard orientation
            vec3 vAt, vUp, vRight;
            if(a_facing.w < 0.5)
            {
              // Screen facing
              vAt = normalize(u_CameraPosition - worldPosition);
              vRight = normalize( cross(u_WorldUp, vAt) );
              vUp = cross(vAt, vRight);
            }
            else
            {
              // Custom facing
              vAt = normalize(a_facing.xyz);
              vRight = normalize( cross(u_WorldUp, vAt) );
              vUp = cross(vAt, vRight);
            }
            
            // Extent Extrusion
            // ----------------------------
            
            // TODO: Rotation
            
            // Extrude the extents of this vertex along the vRight & vUp vectors
            vec3 rightExtent = a_texCoord.zzz * vRight;
            vec3 upExtent = a_texCoord.www * vUp;
            
            // Calculate the world position of the vertex
            worldPosition += rightExtent + upExtent;

            // Calculate lighting
            // ----------------------------------
            
            // Start with ambient lighting
            vec3 lightAccumulation = u_AmbientLight;
            
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
                vec3 lightDirection = normalize(lightPosition - worldPosition);

                // Apply lighting falloff
                float falloff = u_LightPositionsAndFalloffs[light].w;
                float lightWeight = max(1.0 - pow(distance(worldPosition, lightPosition) / falloff, 2.0), 0.0);
                
                // Accumulate the light
                lightAccumulation += u_LightColors[light] * lightWeight;  
              }
            }
            
            // Calculate the total final color (after lighting
            v_color = vec4(a_color.rgb * lightAccumulation, a_color.a);

            // Do final projection transforms
            // ----------------------------------

            // Calculate the final projection space position
            mat4 viewProj = u_Proj * u_View;
            gl_Position = viewProj * vec4(worldPosition, 1.0);
           
            // Pass through the texture coords & color
            v_texCoord = a_texCoord.xy;      
          }
	      ]]>
      </VertexShader>

      <FragmentShader>
        <![CDATA[
          precision mediump float;

          varying vec2 v_texCoord;
          varying vec4 v_color;
          
          uniform sampler2D u_DiffuseTexture;

          void main()
          {        
            gl_FragColor = texture2D(u_DiffuseTexture, v_texCoord) * v_color;
          }
        ]]>
      </FragmentShader>

    </Technique>
  </Techniques>
  
  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_texCoord" Location="1" />
    <Attribute Name="a_color" Location="2" />
    <Attribute Name="a_facing" Location="3" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="u_WorldUp" BindToGlobal="WorldUp" />
    <Parameter Name="u_CameraPosition" BindToGlobal="CameraPosition" />

    <Parameter Name="u_View" BindToGlobal="View" />
    <Parameter Name="u_Proj" BindToGlobal="Projection" />

    <Parameter Name="u_AmbientLight" BindToGlobal="AmbientLight" />
    <Parameter Name="u_NumLights" BindToGlobal="NumActiveLights" />
    <Parameter Name="u_LightPositionsAndFalloffs" BindToGlobal="LightPositionsAndFalloffs" />
    <Parameter Name="u_LightColors" BindToGlobal="LightColors" />

    <Parameter Name="u_DiffuseTexture" Value="0" />
  </Parameters>


</Shader>