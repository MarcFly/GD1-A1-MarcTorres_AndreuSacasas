#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"

#define MAX_FRAMES 25

class Animation
{
public:
	int anim_state;
	bool loop = true;
	bool pingpong = false;
	float speed = 0.5f;
	int offset_x = 0;
	int offset_y = 0;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
	enum pingpong
	{
		forward,
		backward
	};

	int direction = forward;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	int Animation::GetAnimationFrame(float dt, int size)
	{
		current_frame += (speed * dt);

		if (current_frame >= size)
		{
			if (!loop)
				current_frame = size - 1;
			else
				current_frame = 0.0f;

			loops++;
		}
		int counter = 0;
		for (int i = 0; i < size; i++, counter++)
		{
			if (counter == (int)(current_frame))
			{
				return (i);
			}
		}

		return 0;
	}

	SDL_Rect& GetCurrentFrame()
	{
			switch (direction)
		{
			case pingpong::forward:
			{
				current_frame += speed;
				if (current_frame >= last_frame)
				{
					current_frame = (loop || pingpong) ? 0.0f : last_frame - 1;
					direction = pingpong ? pingpong::backward : pingpong::forward;
					loops++;
				}
			}
			break;
			case pingpong::backward:
			{
				current_frame -= speed;
				if (current_frame <= 0.0f)
				{
					current_frame = 0.0f;
					direction = pingpong::forward;
					loops++;
				}
			}
			break;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};

#endif __ANIMATION_H__