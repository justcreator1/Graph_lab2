
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#define RD "D:/Graph_lab1/image.ppm"
#define WD "D:/Graph_lab1/new_image2.ppm"
#define PI 3.14159265358979323846 

using namespace std;

struct RGB {
    unsigned char r, g, b;
};
struct Point {
    int x, y;
};
class Image1
{
public:
    int width, height;
    std::vector<RGB> pixels;

    Image1(const string& filepath)
    {
        ifstream stream(filepath, ios::binary);
        if (!stream) {
            cout << "невозможно открыть файл" << endl;
            return;
        }
        stream.ignore(100, '\n');
        stream >> width >> height;
        stream.ignore(100, '\n');
        stream.ignore(100, '\n');

        pixels.resize(width * height);
        stream.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(RGB));
    }
    void Replace_Pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
    {
        pixels[y * width + x] = { r, g, b };
    }
    bool Save_image(const string& filepath)
    {
        ofstream stream(filepath, ios::binary);
        if (!stream) {
            cout << "не удалось создать файл" << endl;
            return false;
        }
        stream << "P6\n" << width << " " << height << "\n255\n";
        stream.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(RGB));
        return true;
    }
};

class Canvas1 {
public:
    int width, height;
    std::vector<RGB> pixels;
    Canvas1(const int& w, const int& h) 
    {
        width = w;
        height = h;
        pixels.assign(width * height,{255,255,255});
    }
    void Replace_Pixel(int x, int y, RGB& color)
    {
        pixels[y * width + x] = color;
    }
    void DrawLine(Point p1, Point p2, RGB& color)
    {
        int stepx = abs(p2.x - p1.x);
        int stepy = abs(p2.y - p1.y);
        int d = (stepx > stepy) ? stepx : stepy;
        int sx = (p1.x < p2.x) ? 1 : -1;
        int sy = (p1.y < p2.y) ? 1 : -1;
        int bx = 0;
        int by = 0;
        while (true) {
            if (p1.x == p2.x && p1.y == p2.y) break;
            Replace_Pixel(p1.x, p1.y, color);
            bx += stepx;
            by += stepy;
            p1.x += sx*(bx / d);
            p1.y += sy * (by / d);
            bx %= d;
            by %= d;
        }
       
    }
    void CopyLine(Point p1, Point p2, Point i1, Point i2, Image1 image)
    {
        int stepx = abs(p2.x - p1.x);
        int stepy = abs(p2.y - p1.y);
        int d = (stepx > stepy) ? stepx : stepy;
        int sx = (p1.x < p2.x) ? 1 : -1;
        int sy = (p1.y < p2.y) ? 1 : -1;
        int bx = 0;
        int by = 0;

        int istepx = abs(i2.x - i1.x);
        int istepy = abs(i2.y - i1.y);
        int id = (istepx > istepy) ? istepx : istepy;
        int isx = (i1.x < i2.x) ? 1 : -1;
        int isy = (i1.y < i2.y) ? 1 : -1;
        int ibx = 0;
        int iby = 0;

        while (true) {
            if (p1.x == p2.x && p1.y == p2.y) break;
            Replace_Pixel(p1.x, p1.y, image.pixels[i1.y*image.width+i1.x]);
            bx += stepx;
            by += stepy;
            p1.x += sx * (bx / d);
            p1.y += sy * (by / d);
            bx %= d;
            by %= d;

            ibx += istepx;
            iby += istepy;
            i1.x += isx * (ibx / id);
            i1.y += isy * (iby / id);
            ibx %= id;
            iby %= id;
        }
    }
    void CopyPart(Image1& image, Point place, Point start, Point end, int h)
    {
        Point e{ place.x + end.x - start.x, place.y + end.y - start.y };
        for (int i = 0; i < h; i++)
        {
            CopyLine(place, e, start, end, image);
            start.y += 1;
            end.y += 1;
            place.y += 1;
            e.y += 1;
        }
    }
    void DrawFunk( int step, Point zero, int l, RGB color)
    {
        for (int i = 1; i <= l; i += 1) 
        {
            cout << zero.x + i << ' '<< sin(2 * PI * ((double)i / step)) << '\n';
            DrawLine({ zero.x + i-1, int(zero.y - (zero.y - 1) * sin(2 * PI * ((double)(i-1) / step))) }, { zero.x + i, int(zero.y - (zero.y - 1) * sin(2 * PI * ((double)i / step))) }, color);
        }
    }
    bool Save_Canvas(const string& filepath)
    {
        ofstream stream(filepath, ios::binary);
        if (!stream) {
            cout << "не удалось создать файл" << endl;
            return false;
        }
        stream << "P6\n" << width << " " << height << "\n255\n";
        stream.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(RGB));
        return true;
    }
};




int main()
{
    Canvas1 canvas(600, 300);
    Image1 img(RD);
    Point place{ 300,170 };
    Point p1{ 50,150 };
    Point p2{300,100 };
    RGB color = { 0,0,0 };
    RGB bl = { 0,0,255 };
    canvas.CopyPart(img, place, p1, p2, 70);
    canvas.DrawLine({ 0, canvas.height / 2 }, { canvas.width-1, canvas.height / 2 }, color);
    canvas.DrawLine({ canvas.width / 2, 0 }, { canvas.width / 2, (canvas.height-1)}, color);
    canvas.DrawLine({ canvas.width - 1, canvas.height / 2 }, { canvas.width - 1 - canvas.width / 20, canvas.height / 2 + canvas.height / 20 }, color);
    canvas.DrawLine({ canvas.width - 1, canvas.height / 2 }, { canvas.width - 1 - canvas.width / 20, canvas.height / 2 - canvas.height / 20 }, color);
    canvas.DrawLine({ canvas.width / 2 + canvas.width / 20 ,canvas.height / 20, }, { canvas.width / 2, 0 }, color);
    canvas.DrawLine({ canvas.width / 2 - canvas.width / 20 ,canvas.height / 20, }, { canvas.width / 2, 0 }, color);
    canvas.DrawFunk(canvas.width / 10, { canvas.width / 2, canvas.height / 2 }, canvas.width / 2 - 1,color);
    canvas.Save_Canvas(WD);
}

