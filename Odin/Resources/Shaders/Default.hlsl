struct PSInput
{
	float4 position : SV_POSITION;
};

PSInput VSMain(float4 position : POSITION)
{
	PSInput input;
	input.position = position;

	return input;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}