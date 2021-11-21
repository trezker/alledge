class Perlin {
private:
  int seed;
public:
  void Seed(int s);
  float Simplex2(float x, float y);
  float Simplex3(float x, float y, float z);
  float Perlin2(float x, float y);
  float Perlin3(float x, float y, float z);
};