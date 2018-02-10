struct DirectionalLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
};

float4 getColor(float3 normal, DirectionalLight light) {
	// calc normalized direction to light
	float3 normDirToLight = normalize(-light.direction);

	// calc the amount with NDOT
	float lightamount = saturate(dot(normal, normDirToLight));

	// Add directional light to ambient color
	return light.diffuseColor * lightamount + light.ambientColor;
}
// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// normalize normal vector
	input.normal = normalize(input.normal);
	float4 dir1 = getColor(input.normal, directionalLight);
	float4 dir2 = getColor(input.normal, directionalLight2);

	return dir1 + dir2;	
}
