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
	bool Save(const std::string& filename);
	int Get_width() const;
	int Get_height() const;
	void Draw(float x, float y, int flags) const;
	void Draw_scaled(float x, float y, float w, float h, int flags) const;
	void Set_target();
	int get_opengl_texture() const;
private:
	ALLEGRO_BITMAP* allegro_bitmap;
};

#endif
