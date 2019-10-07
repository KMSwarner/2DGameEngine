#pragma once

struct Animation
{
	int index;
	int frameCount;
	int playbackSpeed;

	Animation() {}
	Animation(int i, int f, int s)
	{
		index = i;
		frameCount = f;
		playbackSpeed = s;
	}
};