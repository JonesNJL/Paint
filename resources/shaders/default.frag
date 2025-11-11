#version 430 core

out vec4 FragColor;

in vec3 bPos;
in vec2 bUV;
flat in int bTexture;

layout(std430, binding = 4) buffer testSSBO
{
	uint numberOfImages;
    uint ssboData[];
};

void main()
{
	//FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);

	if (-6 < -1)
	{
		FragColor = vec4(0.0f, 0.5f, -1 * bPos.z, 1.0f);
		//return;
	}


	uint imageStartIndex = 0;
	uint imageWidth = 0;
	uint imageHeight = 0;

	for (int i = 0; i < numberOfImages; i ++)
	{
		uint index = i * 4;

		if (ssboData[index] == bTexture)
		{
			imageStartIndex = ssboData[index + 1];
			imageWidth = ssboData[index + 2];
			imageHeight = ssboData[index + 3];
			break;
		}
	}

	if (imageStartIndex != 0)
	{
		FragColor = vec4(1.0, 1.0, 0.0, 1.0);

		

		uint pixelX = uint(floor(imageWidth * bUV.x));
		if (pixelX == imageWidth) { pixelX -= 1; }
		uint pixelY = uint(floor(imageHeight * bUV.y));
		if (pixelY == imageHeight) { pixelY -= 1; }

		uint pixelIndex = imageStartIndex + (imageWidth * pixelY) + pixelX;
		uint color = ssboData[pixelIndex];

		if (color != color)
		{
			FragColor = vec4(0.0, 1.0, 1.0, 1.0);
			return;
		}

		uint r = (color / 16777216);
		uint g = ((color - r * 16777216) / 65536);
		uint b = ((color - r * 16777216 - g * 65536) / 256);
		uint a = (color - r * 16777216 - g * 65536 - b * 256);

		FragColor = vec4(r, g, b, a) / 255.0;
	}
}