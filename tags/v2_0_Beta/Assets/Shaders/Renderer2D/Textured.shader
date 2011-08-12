<Shader>

  <Techniques>
    <Technique Name="Basic" Profile="Any">
      <VertexShader>
        <![CDATA[
		      attribute vec4 a_Position;
	        attribute vec2 a_TexCoord;
	        attribute vec4 a_Color;
          
	        varying vec2 v_TexCoord;
	        varying vec4 v_Color;
          
	        uniform mat4 u_Proj;
      	  
          void main()
	        {
	          gl_Position = u_Proj * a_Position;
	          v_TexCoord = a_TexCoord;
	          v_Color = a_Color;
	        }
	      ]]>
      </VertexShader>
      <FragmentShader>
        <![CDATA[
          precision mediump float;

          varying vec2 v_TexCoord;
          varying vec4 v_Color;
          uniform sampler2D s_Texture;

          void main()
          { 
            gl_FragColor = texture2D(s_Texture, v_TexCoord) * v_Color;
          }
        ]]>
      </FragmentShader>
    </Technique>
  </Techniques>
      
  <AttributeBindings>
    <Attribute Name="a_Position" Location="0" />
    <Attribute Name="a_TexCoord" Location="1" />
    <Attribute Name="a_Color" Location="2" />
  </AttributeBindings>

  <Parameters>
	  <Parameter Name="u_Proj" BindToGlobal="Projection" />
    <Parameter Name="s_Texture" Value="0" />
  </Parameters>
	
</Shader>