#ifndef Bitmap_h
#define Bitmap_h

#include <string>

struct ALLEGRO_BITMAP;

class Bitmap
{
public:
	Bitmap();
	~Bitmap();
	bool Load(const std::string& filename);
	void Draw(float x, float y, int flags) const;
	int get_opengl_texture() const;
private:
	ALLEGRO_BITMAP* allegro_bitmap;
};

#endif
