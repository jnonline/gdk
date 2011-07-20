<Shader>

  <Techniques>
    <Technique Name="Basic" Profile="Any">
      <VertexShader>
	      <![CDATA[
		      attribute vec4 a_Position;
	        attribute vec4 a_Color;
          
	        varying vec4 v_Color;
          
	        uniform mat4 u_Proj;
      	  
          void main()
	        {
	          gl_Position = u_Proj * a_Position;
	          v_Color = a_Color;
	        }
	      ]]>
      </VertexShader>
      <FragmentShader>
        <![CDATA[
          precision mediump float;
          
          varying vec4 v_Color;

          void main()
          {
            gl_FragColor = v_Color;
          }
        ]]>
      </FragmentShader>
    </Technique>
  </Techniques>
  
  <AttributeBindings>
    <Attribute Name="a_Position" Location="0" />
    <Attribute Name="a_Color" Location="1" />
  </AttributeBindings>

  <Parameters>
    <Parameter Name="u_Proj" BindToGlobal="Projection" />
  </Parameters>


</Shader>