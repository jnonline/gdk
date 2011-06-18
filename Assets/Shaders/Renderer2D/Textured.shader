<Shader>

  <Techniques>
    <Technique Name="Basic">
      <VertexShader>
        <![CDATA[
		      attribute vec4 a_position;
	        attribute vec2 a_texCoord;
	        attribute vec4 a_color;
          
	        varying vec2 v_texCoord;
	        varying vec4 v_color;
          
	        uniform mat4 projection;
      	  
          void main()
	        {
	          gl_Position = projection * a_position;
	          v_texCoord = a_texCoord;
	          v_color = a_color;
	        }
	      ]]>
      </VertexShader>
      <FragmentShader>
        <![CDATA[
        
          
          precision mediump float;

          varying vec2 v_texCoord;
          varying vec4 v_color;
          uniform sampler2D s_texture;

          void main()
          { 
            gl_FragColor = texture2D(s_texture, v_texCoord) * v_color;
          }
        ]]>
      </FragmentShader>
    </Technique>
  </Techniques>
      
  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_texCoord" Location="1" />
    <Attribute Name="a_color" Location="2" />
  </AttributeBindings>

  <Parameters>
	  <Parameter Name="projection" BindToGlobal="Projection" />
    <Parameter Name="s_texture" Value="0" />
  </Parameters>
	
</Shader>