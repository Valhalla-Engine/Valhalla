struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
	PSInput input;
	input.position = position;
	input.color = color;

	return input;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}