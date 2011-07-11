using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk;

using Gdk.Forms;
using Tao.OpenGl;

namespace Gdk.Content
{
    // ===============================================================
	public enum CullingMode
	{
		Disabled = 0,
		Clockwise = 1,				CW = 1,
		CounterClockwise = 2,		CCW = 2,
	}


	// ===============================================================
    public enum CompareFunction
	{
		Never			= 0x0200,
		Always			= 0x0207,
		Equal			= 0x0202,
		NotEqual		= 0x0205,
		Less			= 0x0201,
		LessOrEqual		= 0x0203,
		Greater			= 0x0204,
		GreaterOrEqual	= 0x0206,
	}

	// ===============================================================
    public enum BlendFactor
	{
		 Zero					= 0, 
		 One					= 1, 
		 SrcColor				= 0x0300,
		 OneMinusSrcColor		= 0x0301, 
		 DestColor				= 0x0306,
		 OneMinusDestColor		= 0x0307,
		 SrcAlpha				= 0x0302,
		 OneMinusSrcAlpha		= 0x0303, 
		 DestAlpha				= 0x0304, 
		 OneMinusDestAlpha		= 0x0305, 
		 ConstantColor			= 0x8001,
		 OneMinusConstantColor	= 0x8002, 
		 ConstantAlpha			= 0x8003, 
		 OneMinusConstantAlpha	= 0x8004, 
		 SrcAlphaSaturate		= 0x0308,					// Only valid as a Source blend factor
	}

	// ===============================================================
    public enum BlendEquation
	{
		Add						= 0x8006,
		Subtract				= 0x800A,
		ReverseSubtract			= 0x800B,
	}

	// ===============================================================
    public enum BlendMode
	{
		Normal			= 0,
		Add,
		Multiply,
		MultiplyAdd,
		Subtract,
		MultiplySubtract,
	}

    // ===============================================================
    public enum ShaderFlags
    {
        ShaderIsText            = 0x0001
    }

    // ===============================================================
    public enum ShaderParameterFlags
    {
        BindToGlobal = 0x0001,
        HasInitialValue = 0x0002
    }

    // ===============================================================
    public enum ShaderRenderStateFlags
    {
        ColorMaskApply          = 0x00000001,
        BlendingApply           = 0x00000002,
        DepthApply              = 0x00000004,
        CullingApply            = 0x00000008,
										
        ColorRedWriteEnabled    = 0x00000100,
        ColorGreenWriteEnabled  = 0x00000200,
        ColorBlueWriteEnabled   = 0x00000400,
        ColorAlphaWriteEnabled  = 0x00000800,
		
        BlendingEnabled         = 0x00001000,
        UseSimpleBlendMode      = 0x00002000,
        DepthTestEnabled        = 0x00004000,
        DepthWriteEnabled       = 0x00008000,

        DepthBiasEnabled        = 0x00010000
    }

    // ===============================================================
    public class ShaderParameter
    {
        public string Name;
        public string Value;
        public string Binding;

        public bool IsBindToGlobal;
        public bool HasInitialValue;

        public List<int> IntValues = null;
        public List<float> FloatValues = null;

        // Used only during the build
        public ShaderUniformType UniformType;
        public int ArraySize;


        // CTor
        public ShaderParameter(string name, string value, bool isBindToGlobal)
        {
            this.Name = name;
            this.Value = value;
            this.IsBindToGlobal = isBindToGlobal;
            this.HasInitialValue = (this.IsBindToGlobal == false && string.IsNullOrEmpty(value) == false);

            this.UniformType = ShaderUniformType.Float;
            this.ArraySize = 1;
        }
    }

    // ===============================================================
    public enum ShaderAttributeType
	{
		Float		= Gl.GL_FLOAT,
		Float2		= Gl.GL_FLOAT_VEC2,
		Float3		= Gl.GL_FLOAT_VEC3,
		Float4		= Gl.GL_FLOAT_VEC4,
		Matrix2		= Gl.GL_FLOAT_MAT2,
		Matrix3		= Gl.GL_FLOAT_MAT3,
		Matrix4		= Gl.GL_FLOAT_MAT4,
	}

    // ===============================================================
    public enum ShaderUniformType
    {
        Float       = Gl.GL_FLOAT,
        Float1      = Gl.GL_FLOAT,
        Float2      = Gl.GL_FLOAT_VEC2,
        Float3      = Gl.GL_FLOAT_VEC3,
        Float4      = Gl.GL_FLOAT_VEC4,
        Int         = Gl.GL_INT,
        Int1        = Gl.GL_INT,
        Int2        = Gl.GL_INT_VEC2,
        Int3        = Gl.GL_INT_VEC3,
        Int4        = Gl.GL_INT_VEC4,
        Bool        = Gl.GL_BOOL,
        Bool1       = Gl.GL_BOOL,
        Bool2       = Gl.GL_BOOL_VEC2,
        Bool3       = Gl.GL_BOOL_VEC3,
        Bool4       = Gl.GL_BOOL_VEC4,
        Matrix2     = Gl.GL_FLOAT_MAT2,
        Matrix3     = Gl.GL_FLOAT_MAT3,
        Matrix4     = Gl.GL_FLOAT_MAT4,
        Sampler2D   = Gl.GL_SAMPLER_2D,
        SamplerCube = Gl.GL_SAMPLER_CUBE,
    }

    // ===============================================================
    public class ShaderUniform
    {
        public string Name;
        public ShaderUniformType UniformType;
        public int ArraySize;

        // CTor
        public ShaderUniform(string name, ShaderUniformType type, int arraySize)
        {
            Name = name;
            UniformType = type;
            ArraySize = arraySize;
        }
    }

    // ===============================================================
    public class ShaderAttribute
    {
        public string Name;
        public ShaderAttributeType AttrType;
        public int ArraySize;
        public int Location;

        // CTor
        public ShaderAttribute(string name, ShaderAttributeType type, int arraySize)
        {
            Name = name;
            AttrType = type;
            ArraySize = arraySize;
        }
    }

    // ===============================================================
    public class ShaderTechnique
    {
        public string Name = "";

        // The Shader programs
        public string VertexShaderText = "";
        public string FragmentShaderText = "";

        // Platform/Device support/exclusion
        public List<PlatformType> SupportedPlatforms = new List<PlatformType>();
        public List<DeviceType> SupportedDevices = new List<DeviceType>();
        public List<PlatformType> ExcludedPlatforms = new List<PlatformType>();
        public List<DeviceType> ExcludedDevices = new List<DeviceType>();

        // Build time only
        public Dictionary<string, ShaderAttribute> Attributes = new Dictionary<string,ShaderAttribute>();
        public Dictionary<string, ShaderUniform> Uniforms = new Dictionary<string,ShaderUniform>();
    }

    // ===============================================================
    public class ShaderRenderState
    {
       
        // Enable / Apply 
        public bool ColorMaskApply = false;
        public bool ColorRedWriteEnabled = true;
        public bool ColorGreenWriteEnabled = true;
        public bool ColorBlueWriteEnabled = true;
        public bool ColorAlphaWriteEnabled = true;

        // Blending
        public bool BlendingApply = false;
        public bool BlendingEnabled = true;
        public Color BlendConstantColor = Color.TransparentBlack;
        public bool UseSimpleBlendMode = true;
        public BlendMode BlendMode = BlendMode.Normal;
        public BlendFactor SourceRGBFactor = BlendFactor.SrcAlpha;
        public BlendFactor DestRGBFactor = BlendFactor.OneMinusSrcAlpha;
        public BlendEquation RGBBlendEquation = BlendEquation.Add;
        public BlendFactor SourceAlphaFactor = BlendFactor.SrcAlpha;
        public BlendFactor DestAlphaFactor = BlendFactor.OneMinusSrcAlpha;
        public BlendEquation AlphaBlendEquation = BlendEquation.Add;

        // Depth
        public bool DepthApply = false;
        public bool DepthTestEnabled = true;
        public bool DepthWriteEnabled = true;
        public CompareFunction DepthTestFunction = CompareFunction.Less;
        public bool DepthBiasEnabled = true;
        public float DepthBiasFactor = 0.0f;
        public float DepthBiasUnits = 0.0f;

        // Culling
        public bool CullingApply = false;
        public CullingMode CullingMode = CullingMode.Disabled;
    }

    // ===============================================================
    public class ShaderContent
    {
        // Render state
        public ShaderRenderState RenderState = new ShaderRenderState();

        // Techniques
        public List<ShaderTechnique> Techniques = new List<ShaderTechnique>();
        
        // Attributes
        public Dictionary<string, ShaderAttribute> Attributes = new Dictionary<string, ShaderAttribute>();

        // Parameters
        public Dictionary<string, ShaderParameter> Parameters = new Dictionary<string, ShaderParameter>();
    }
}
