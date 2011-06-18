<Shader>

  <Techniques>
    <Technique Name="Basic">
      <VertexShader>
	      <![CDATA[
		      attribute vec4 a_position;
	        attribute vec4 a_color;
          
	        varying vec4 v_color;
          
	        uniform mat4 projection;
      	  
          void main()
	        {
	          gl_Position = projection * a_position;
	          v_color = a_color;
	        }
	      ]]>
      </VertexShader>
      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          varying vec4 v_color;

          void main()
          {
            gl_FragColor = v_color;
          }
        ]]>
      </FragmentShader>
    </Technique>
  </Techniques>
  
  <AttributeBindings>
    <Attribute Name="a_position" Location="0" />
    <Attribute Name="a_color" Location="1" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="projection" BindToGlobal="Projection" />
  </Parameters>


</Shader>