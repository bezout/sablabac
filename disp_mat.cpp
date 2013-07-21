#include <CImg.h>
#include <vector>
#include <iostream>

struct Quad
{ 
  int a,b,c,d;
  Quad(int a_, int b_, int c_, int d_):a(a_),b(b_),c(c_),d(d_){}
};

struct Hide
{
  cimg_library::CImg<unsigned char> image;
  cimg_library::CImgDisplay display;
  Hide(int h):image(h,h,1,1),display(640,640)
  {
  }
};

class Hide;
struct Disp
{
  Hide *hide;
  Disp(int size);
  void add(int i, int j);
  bool is_closed() const;
};

Disp::Disp(int size)
{
  hide = new Hide(size);
}

void Disp::add(int i, int j)
{
  hide->image(i,j) += 255/6;
  hide->display = hide->image;
}

bool Disp::is_closed() const
{
  return hide->display.is_closed();
}

int main()
{
  
  Disp disp(200);
  
  std::vector<Quad> v;
  
    for(size_t j = 0 ; j < 200 ; ++j)
      for(size_t k = j ; k < 200 ; ++k)
        if ((double)random()/(double)RAND_MAX < 0.8)
          v.push_back(Quad(k,j,0,0));

  size_t cpt = 0;
  while(!disp.is_closed())
  {
    disp.add(v[cpt].a,v[cpt].b);
    cpt ++;
  }
  
}
