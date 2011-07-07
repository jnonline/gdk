<Shader>

  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_normal" Location="1" />
    <Attribute Name="a_color" Location="2" />
    <Attribute Name="a_texCoord" Location="3" />
  </AttributeBindings>
  
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
          uniform sampler2D u_DiffuseTexture;

          void main()
          {
            // gl_FragColor = vec4(1,1,0,1);
            // gl_FragColor = v_color;
            // gl_FragColor = texture2D(u_DiffuseTexture, v_texCoord);
            
            vec4 clr = v_color;
            //clr.r = 0.5 + sin(gl_FragCoord.x) * 0.5;
            //clr.g = 0.5 + cos(gl_FragCoord.y/2) * 0.5;
            
            gl_FragColor = texture2D(u_DiffuseTexture, v_texCoord) * clr;
          }
        ]]>
      </FragmentShader>
    </Technique>
  </Techniques>

  <Parameters>
    <Parameter Name="u_DiffuseTexture" Value="0" />
	  <Parameter Name="projection" BindToGlobal="Projection" />
  </Parameters>
	
</Shader>