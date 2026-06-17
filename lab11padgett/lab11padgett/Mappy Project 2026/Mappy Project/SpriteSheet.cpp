#include "SpriteSheet.h"

Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = 80;


	maxFrame = 8;
	facing = 1;
	curFrame = 0;
	frameCount = 0;
	isJumping = false;
	jumpFrameCount = 0;
	frameDelay = 18;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}
void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if (dir == 1) {
		animationDirection = 1;
		facing = 1;
		x += 5;
		if (!isJumping)
		{
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 1;
			}
		}
	}
	else if (dir == 0) {
		animationDirection = 0;
		facing = 0;
		x -= 5;
		if (!isJumping)
		{
			if (++frameCount > frameDelay)
			{
				frameCount = 0;
				if (++curFrame > maxFrame)
					curFrame = 1;
			}
		}
	}
	else {
		animationDirection = dir;
		if (!isJumping)
			curFrame = 0;
	}

	if (animationDirection == 0)
	{
		if (collided(x, y + frameHeight)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth, y + frameHeight)) {
			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (facing == 0)
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	else
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
}
int Sprite::jumping(int jump, const int JUMPIT)
{
	const int jumpFrameStart = 8;
	const int jumpFrameEnd = 11;

	if (jump == JUMPIT) {
		if (!collided(x + frameWidth / 2, y + frameHeight + 5)) {
			jump = 0;
			isJumping = true;  //fallong
		}
		else {
			isJumping = false; //standing on solid ground
		}
	}
	else
	{	
		isJumping = true;
		int oldy = y;
		y -= jump / 3;
		jump--;

		if (jumpValue(x + frameWidth / 2, y)) {
			y = oldy;
			jump = 0;
		}

		if (curFrame < jumpFrameStart || curFrame > jumpFrameEnd)
			curFrame = jumpFrameStart;

		if (++jumpFrameCount > frameDelay)
		{
			jumpFrameCount = 0;
			if (++curFrame > jumpFrameEnd)
				curFrame = jumpFrameStart;
		}
	}

	if (jump < 0)
	{
		if (collided(x + frameWidth / 2, y + frameHeight))
		{
			jump = JUMPIT;
			isJumping = false;
			curFrame = 0;
			while (collided(x + frameWidth / 2, y + frameHeight))
			{
				y -= 3;
			}
		}
	}

	return jump;
}