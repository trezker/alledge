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
	allegro_bitmap = al_load_bitmap(filename.c_str());
	return allegro_bitmap!=NULL;
}

void Bitmap::Draw(float x, float y, int flags) const
{
	al_draw_bitmap(allegro_bitmap, x, y, flags);
}

int Bitmap::get_opengl_texture() const
{
	return al_get_opengl_texture(allegro_bitmap);
}
