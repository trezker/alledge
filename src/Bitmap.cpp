#include "../alledge/Bitmap.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>

Bitmap::Bitmap()
:allegro_bitmap(NULL)
{
}

Bitmap::~Bitmap()
{
	al_destroy_bitmap(allegro_bitmap);
}

bool Bitmap::Load(const std::string& filename)
{
	if(allegro_bitmap != NULL)
	{
		al_destroy_bitmap(allegro_bitmap);
	}
	allegro_bitmap = al_load_bitmap(filename.c_str());
	return allegro_bitmap!=NULL;
}

bool Bitmap::Save(const std::string& filename)
{
	return al_save_bitmap(filename.c_str(), allegro_bitmap);
}

int Bitmap::Get_width() const
{
	return al_get_bitmap_width(allegro_bitmap);
}

int Bitmap::Get_height() const
{
	return al_get_bitmap_height(allegro_bitmap);
}

void Bitmap::Draw(float x, float y, int flags) const
{
	al_draw_bitmap(allegro_bitmap, x, y, flags);
}

void Bitmap::Draw_scaled(float x, float y, float w, float h, int flags) const
{
	int width = al_get_bitmap_width(allegro_bitmap);
	int height = al_get_bitmap_height(allegro_bitmap);
	al_draw_scaled_bitmap(allegro_bitmap, 0, 0, width, height, x, y, w, h, flags);
}

void Bitmap::Set_target()
{
	al_set_target_bitmap(allegro_bitmap);
}

int Bitmap::get_opengl_texture() const
{
	return al_get_opengl_texture(allegro_bitmap);
}
