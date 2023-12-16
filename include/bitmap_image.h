/* 2252201 胡世成 计科二班 */

#pragma once

/* 需要用到的头文件 */
#include <cstring>
#include <fstream>

/* 根据需要加入自己的定义 */
#pragma pack(push, 1) // 确保结构体按照1字节对齐

// BMP文件头结构体
struct BMPFileHeader {
    uint16_t fileType;       // 文件类型，必须为"BM"（0x424D）
    uint32_t fileSize;       // 文件大小，单位为字节
    uint16_t reserved1;      // 保留字段，值为0
    uint16_t reserved2;      // 保留字段，值为0
    uint32_t dataOffset;     // 位图数据的起始位置，相对于文件头的偏移量
};

// BMP信息头结构体
struct BMPInfoHeader {
    uint32_t headerSize;        // 信息头大小，一般为40字节
    int32_t  imageWidth;        // 图像宽度，单位为像素
    int32_t  imageHeight;       // 图像高度，单位为像素
    uint16_t colorPlanes;       // 颜色平面数，必须为1
    uint16_t bitsPerPixel;      // 每个像素的位数，常见值为24（表示RGB颜色模式）
    uint32_t compressionMethod; // 图像压缩方式，常见值为0（表示不压缩）
    uint32_t imageSize;         // 图像数据的大小，单位为字节
    int32_t  horizontalRes;     // 水平分辨率，单位为像素每米
    int32_t  verticalRes;       // 垂直分辨率，单位为像素每米
    uint32_t colorsInPalette;   // 调色板中的颜色数，若为0，则表示使用所有可能的颜色
    uint32_t importantColors;   // 重要的颜色数，若为0，则表示所有颜色都重要
};

struct RGBQuad { // 调色板中的颜色项
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
};

#pragma pack(pop) // 恢复默认对齐方式

class bitmap_image
{
protected:
    /* 根据需要加入自己的定义 */
    std::string filename;
    std::ifstream bmpfile;
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    RGBQuad palette[256]; // 调色板最多有256个颜色项
    //内置函数
    void read_bitmap();
    unsigned int get_pixel_Monochrome(int row, int col); //单色色深的像素读取函数
    unsigned int get_pixel_16Color(int row, int col); //16色色深的像素读取函数
    unsigned int get_pixel_256Color(int row, int col); //256色色深的像素读取函数
    unsigned int get_pixel_24BitColor(int row, int col); //24位色深的像素读取函数
public:
    /* bmp文件初始化 */
    bitmap_image(const std::string& fname);
    ~bitmap_image();
    //实现功能
    int width() const;	//返回图片的宽度
    int height() const; //返回图片的高度
    unsigned int get_pixel(int row, int col); //返回指定点的RGB颜色
    void show_image(const int point_x = 0, const int point_y = 0, const int size = 1);//显示图片
};

#ifndef HANZIFONT_H
#define HANZIFONT_H

//16*16点阵字库读取
class HanziFont{
private:
    const char * filename;
    std::ifstream inFont;
    int fontByteSize;
    char fontData[32];//32位,暂时无用

    //内置功能函数
    void readCharacterDataByArea(int areaCode, int posCode, char* buffer);
    int readCharacterDataByInternalCode(unsigned char highByte, unsigned char lowByte, char* buffer);

public:
    /* 字库初始化 */
    HanziFont(const char* fontFilename, int byteSize = 32);
    ~HanziFont();
    //实现功能
    size_t getFileSize();
    int getCharacter(const char* character, char* buffer = NULL);
    bool onCharacter(const int row, const int col);
    void displayCharacterBitmap();
};

#endif /* HANZIFONT_H */

//在bmp文件中加字幕的衍生类
class bitmap_subtitles : public bitmap_image
{
protected:
    std::ofstream outfile;
    std::string subtitles;
    int posx;           //字幕横向位置，默认0
    int posy;           //字幕纵向位置，默认0
    int color;          //字幕前景颜色，默认0x0FFFFFF，即白色（仅对24bit色有效）
    int font;           //设置字体大小，默认16点阵
    int distance;       //设置字间距，默认3
    bool traditional;   //是否用繁体，默认false
    bool vertical;      //字幕方向，默认false
    HanziFont jianCh;   //简体字库
    HanziFont fanCh;    //繁体字库
    //内置函数
    void copy_bitmap_head();    //复制bmp文件头到outfile
    bool pixel_on_subtitles(const int row, const int col);  //判断row行col列的像素是否在字幕上
    int add_sub_Monochrome();   //单色色深的bmp文件字幕添加函数
    int add_sub_16Color();      //16色色深的bmp文件字幕添加函数
    int add_sub_256Color();     //256色色深的bmp文件字幕添加函数
    int add_sub_24BitColor();   //24位色深的bmp文件字幕添加函数
public:
    /* bmp文件初始化 */
    bitmap_subtitles(const std::string& bmpfname, const std::string& outfname, const std::string& subtit);
    ~bitmap_subtitles();
    //设置变量
    int set_pos(const int pos_x, const int pos_y);  //设置字幕位置
    int set_color(const std::string& colr);         //设置字幕前景颜色
    void set_traditional(const bool trad = true);   //设置是否用繁体
    void set_vertical(const bool vert = true);       //设置字幕是否垂直
    //进行转换
    int add_subtitles();   //添加字幕总函数
};