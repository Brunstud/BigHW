/* 2252201 胡世成 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "../include/bitmap_image.h"
#include "../include/cmd_hdc_tools.h"
using namespace std;

/****************************************************************
* bitmap_image类：BMP图片处理函数
*****************************************************************/
//构造函数
bitmap_image::bitmap_image(const string& fname) : filename(fname), bmpfile(fname, ios::in | ios::binary) {
    if (!bmpfile.is_open()) {
        cerr << "无法打开bmp文件: " << filename << endl;
        return;
    }
    // 在构造函数中读取BMP文件头信息
    read_bitmap();
}

//析构函数：关闭文件
bitmap_image::~bitmap_image() {
    if (bmpfile.is_open()) {
        bmpfile.close();
    }
}

//内置信息读取函数
void bitmap_image::read_bitmap() {
    if (bmpfile) {
        // 读取BMP文件头
        bmpfile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
        bmpfile.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

        // 判断是否有调色板
        if (infoHeader.bitsPerPixel <= 8) {
            // 计算调色板项的数量
            int colorsCount = infoHeader.colorsInPalette > 0 ? infoHeader.colorsInPalette : (1 << infoHeader.bitsPerPixel);

            // 读取调色板数据
            bmpfile.read(reinterpret_cast<char*>(&palette), colorsCount * sizeof(RGBQuad));

            // 输出调色板中的颜色信息
            //for (int i = 0; i < colorsCount; ++i) {
                //std::cout << "Color " << i << ": R=" << (int)palette[i].red << " G=" << (int)palette[i].green << " B=" << (int)palette[i].blue << std::endl;
            //}
        }

        // 可以根据需要检查读取的文件头信息是否正确

    }
    return;
}

//单色色深的像素读取函数
unsigned int bitmap_image::get_pixel_Monochrome(int row, int col) {
    // 计算每行像素所占字节数（每个像素仅占1位）
    uint32_t bytes_per_row = ((infoHeader.imageWidth + 31) / 32) * 4;

    // 计算像素在文件中的偏移位置
    uint32_t byte_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col / 8;

    // 计算像素在字节中的偏移位置
    uint8_t bit_offset = 7 - (col % 8);

    // 打开文件并定位到像素位置
    if (bmpfile) {
        bmpfile.seekg(byte_offset);

        // 读取像素数据
        uint8_t pixel_byte = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));

        // 提取指定像素的颜色信息
        unsigned int color = (pixel_byte >> bit_offset) & 0x01;

        // 将单色值映射为RGB值
        unsigned int rgb_value = RGB(palette[color].red, palette[color].green, palette[color].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // 返回像素颜色值
    }
    else {
        //cerr << "打开文件失败: " << filename << endl;
        return 0; // 返回默认颜色值
    }
}

//16色色深的像素读取函数
unsigned int bitmap_image::get_pixel_16Color(int row, int col) {
    // 计算每行像素所占字节数（每个像素占用4位）
    uint32_t bytes_per_row = ((infoHeader.imageWidth * 4 + 31) / 32) * 4;

    // 计算像素在文件中的偏移位置
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col / 2;

    // 打开文件并定位到像素位置
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // 读取像素数据
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // 提取指定像素的颜色信息
        unsigned int color = (col % 2 == 0) ? (pixel_data >> 4) : (pixel_data & 0x0F);

        // 将色值映射为RGB值
        unsigned int rgb_value = RGB(palette[color].red, palette[color].green, palette[color].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // 返回像素颜色值
    }
    else {
        cerr << "打开文件失败: " << filename << endl;
        return 0; // 返回默认颜色值
    }
}

//256色色深的像素读取函数
unsigned int bitmap_image::get_pixel_256Color(int row, int col) {
    // 计算每行像素所占字节数（每个像素占用8位）
    uint32_t bytes_per_row = infoHeader.imageWidth;

    // 计算像素在文件中的偏移位置
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col;

    // 打开文件并定位到像素位置
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // 读取像素数据
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // 将色值映射为RGB值
        unsigned int rgb_value = RGB(palette[pixel_data].red, palette[pixel_data].green, palette[pixel_data].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // 返回像素颜色值
    }
    else {
        cerr << "打开文件失败: " << filename << endl;
        return 0; // 返回默认颜色值
    }
}

//24位色深的像素读取函数
unsigned int bitmap_image::get_pixel_24BitColor(int row, int col) {
    // 计算每行像素所占字节数（每个像素占用3字节）
    uint32_t bytes_per_row = (infoHeader.imageWidth) * 3 + infoHeader.imageWidth % 4;

    // 计算像素在文件中的偏移位置
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col * 3;

    // 打开文件并定位到像素位置
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // 读取RGB值
        uint8_t pixel_data[3] = { 0 };
        bmpfile.read(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));

        // 将RGB值合并为unsigned int类型
        unsigned int red = pixel_data[2];
        unsigned int green = pixel_data[1];
        unsigned int blue = pixel_data[0];

        return RGB(red, green, blue); // 返回RGB颜色值
    }
    else {
        cerr << "打开文件失败: " << filename << endl;
        return 0; // 返回默认颜色值
    }
}

//返回图片的宽度
int bitmap_image::width() const {
    return infoHeader.imageWidth;
}

//返回图片的高度
int bitmap_image::height() const {
    return infoHeader.imageHeight;
}

//返回指定点的RGB颜色
unsigned int bitmap_image::get_pixel(int row, int col) {
    switch (infoHeader.bitsPerPixel) {
        case 1:
            return get_pixel_Monochrome(row, col);
        case 4:
            return get_pixel_16Color(row, col);
        case 8:
            return get_pixel_256Color(row, col);
        case 24:
            return get_pixel_24BitColor(row, col);
        default:
            cerr << "不支持该色深: " << infoHeader.bitsPerPixel << "b/像素" << endl;
            return 0; // 返回默认颜色值
    }
}

//表现图片
void bitmap_image::show_image(const int point_x, const int point_y, const int size)
{
    /* 从上到下、从左到右显示整个图像 */
    int hei = height(), wid = width();
    for (int i = 0; i < hei; i++)
    {
        for (int j = 0; j < wid; j++)
        {
            int RGB = get_pixel(i, j);
            hdc_point(point_x + size * j, point_y + size * i, size, get_pixel(i, j));
        }
    }
}

/****************************************************************
* bitmap_subtitles类：在bmp文件中加字幕的衍生类
*****************************************************************/
/* bmp文件初始化 */
bitmap_subtitles::bitmap_subtitles(const std::string& bmpfname, const std::string& outfname, const std::string& subtit) : bitmap_image(bmpfname), outfile(outfname, ios::out | ios::binary), subtitles(subtit), jianCh("HZK16"), fanCh("HZK16F") {
    if (!outfile.is_open()) {
        cerr << "无法打开输出bmp文件: " << outfname << endl;
    }
    posx = posy = 0;
    color = 0x0FFFFFF;
    font = 16;
    distance = 3;
    traditional = false;
    vertical = false;
}

//析构函数
bitmap_subtitles::~bitmap_subtitles() {
    if (outfile.is_open()) {
        outfile.close();
    }
}

//设置字幕位置，成功返回0，失败返回-1
int bitmap_subtitles::set_pos(const int pos_x, const int pos_y) {
    if (pos_x < 0 || pos_y < 0 || pos_x > width() || pos_y > height())
        return -1;
    posx = pos_x;
    posy = pos_y;
    return 0;
}

//设置字幕颜色，设置成功返回当前颜色，失败返回-1并置默认颜色
int bitmap_subtitles::set_color(const std::string& colr) {
    if (colr.length() != 6) {
        cerr << "颜色值#" << colr << "长度为" << colr.length() << endl;
        return -1;
    }
    int oriColor = color;
    istringstream iss(colr);
    iss >> hex >> color >> dec;
    if (!iss) {
        cerr << "颜色值#" << colr << "不是十六进制数." << endl;
        color = oriColor;
        return -1;
    }
    return color;
}

//设置简体false/繁体true
void bitmap_subtitles::set_traditional(const bool trad) {
    traditional = trad;
}

//设置style(horizontal横向/vertical纵向），设置成功返回0，失败返回-1
void bitmap_subtitles::set_vertical(const bool vert) {
    vertical = vert;
}

//复制bmp文件头到outfile
void bitmap_subtitles::copy_bitmap_head() {
    if (outfile.is_open()) {
        //存储文件头的副本
        char* buffer = new char[fileHeader.dataOffset];

        // 将文件指针定位到文件开始处
        bmpfile.seekg(0, ios::beg);
        // 读取BMP文件头到副本
        bmpfile.read(buffer, fileHeader.dataOffset);

        // 将文件指针定位到文件开始处
        outfile.seekp(0, ios::beg);
        // 将文件头信息写入输出文件
        outfile.write(buffer, fileHeader.dataOffset);

        //释放副本
        delete[] buffer;

        // 如果需要检查写入的文件头信息是否正确，可以在这里添加代码

        // 确保写入成功后，刷新输出文件流
        outfile.flush();
    }
    else
        cerr << "输出bmp文件无法打开" << endl;
}

//判断row行col列的像素是否在字幕上
bool bitmap_subtitles::pixel_on_subtitles(const int row, const int col) {
    int x = col - posx, y = row - posy;
    int subtitle_length = subtitles.length() / 2; // 获取字幕长度
    //找到对应汉字
    int no = (vertical ? y / (font + distance) : x / (font + distance));
    if (x < 0 || y < 0 ||
        !vertical && (y >= font || x >= subtitle_length * (font + distance) || (x %= (font + distance)) >= font) ||
        vertical && (x >= font || y >= subtitle_length * (font + distance) || (y %= (font + distance)) >= font))
        return false;
    (traditional ? fanCh : jianCh).getCharacter(&subtitles[2 * no]);
    //(traditional ? fanCh : jianCh).displayCharacterBitmap();
    //while (getchar() != '\n');
    return (traditional ? fanCh : jianCh).onCharacter(y, x);
}

//单色色深的bmp文件字幕添加函数
int bitmap_subtitles::add_sub_Monochrome() {
    if (!bmpfile || !outfile)
        return -1;
    // 计算每行像素所占字节数（每个像素仅占1位）
    const uint32_t bytes_per_row = ((infoHeader.imageWidth + 31) / 32) * 4;
    for (uint32_t byte_offset = fileHeader.dataOffset; byte_offset < fileHeader.fileSize; ++byte_offset) {
        bmpfile.seekg(byte_offset);

        // 读取像素数据
        uint8_t pixel_byte = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));

        //逐像素判断
        for (uint32_t bit_offset = 0; bit_offset < 8; ++bit_offset) {
            int row = infoHeader.imageHeight - ((byte_offset - fileHeader.dataOffset) / bytes_per_row + 1);
            int col_byte = (byte_offset - fileHeader.dataOffset) % bytes_per_row; // 计算偏移在该行的字节偏移量
            int col_bit = 7 - bit_offset; // 得到列中的位偏移量
            int col = col_byte * 8 + col_bit; // 计算总的列偏移量
            if (pixel_on_subtitles(row, col)) {
                pixel_byte &= ~(0x01 << bit_offset);
                //hdc_point(col * 8, row * 8, 8, 0x0FFFFFF);
            }
        }

        //写操作
        outfile.seekp(byte_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));
    }
    return 0;
}

//16色色深的bmp文件字幕添加函数
int bitmap_subtitles::add_sub_16Color() {
    if (!bmpfile || !outfile)
        return -1;

    // 计算每行像素所占字节数（每个像素占4位）
    const uint32_t bytes_per_row = ((infoHeader.imageWidth * 4 + 31) / 32) * 4;

    for (uint32_t byte_offset = fileHeader.dataOffset; byte_offset < fileHeader.fileSize; ++byte_offset) {
        bmpfile.seekg(byte_offset);

        // 读取像素数据
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // 逐像素判断
        for (uint32_t bit_offset = 0; bit_offset < 8; bit_offset += 4) {
            int row = infoHeader.imageHeight - ((byte_offset - fileHeader.dataOffset) / bytes_per_row + 1);
            int col_byte = (byte_offset - fileHeader.dataOffset) % bytes_per_row; // 计算偏移在该行的字节偏移量
            int col_bit = (bit_offset / 4); // 得到列中的位偏移量
            int col = col_byte * 2 + col_bit; // 计算总的列偏移量

            if (pixel_on_subtitles(row, col)) {
                if (bit_offset == 0)
                    pixel_data &= 0xF0; // 清除低4位
                else
                    pixel_data &= 0x0F; // 清除高4位
            }
        }

        //写操作
        outfile.seekp(byte_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//256色色深的bmp文件字幕添加函数
int bitmap_subtitles::add_sub_256Color() {
    if (!bmpfile || !outfile)
        return -1;

    // 计算每行像素所占字节数（每个像素占用8位）
    const uint32_t bytes_per_row = infoHeader.imageWidth;

    for (uint32_t pixel_offset = fileHeader.dataOffset; pixel_offset < fileHeader.fileSize; ++pixel_offset) {
        bmpfile.seekg(pixel_offset);

        // 读取像素数据
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        int row = infoHeader.imageHeight - ((pixel_offset - fileHeader.dataOffset) / bytes_per_row + 1);
        int col = (pixel_offset - fileHeader.dataOffset) % bytes_per_row;

        if (pixel_on_subtitles(row, col)) {
            // 修改像素值
            pixel_data = 0;//默认黑色
        }

        //写操作
        outfile.seekp(pixel_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//24位色深的bmp文件字幕添加函数
int bitmap_subtitles::add_sub_24BitColor() {
    if (!bmpfile || !outfile)
        return -1;

    // 计算每行像素所占字节数（每个像素占用3字节）
    const uint32_t bytes_per_row = (infoHeader.imageWidth * 3) + (infoHeader.imageWidth % 4);

    for (uint32_t pixel_offset = fileHeader.dataOffset; pixel_offset < fileHeader.fileSize; pixel_offset += 3) {
        bmpfile.seekg(pixel_offset);

        // 读取 RGB 值
        uint8_t pixel_data[3] = { 0 };
        bmpfile.read(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));

        int row = infoHeader.imageHeight - ((pixel_offset - fileHeader.dataOffset) / bytes_per_row + 1);
        int col = (pixel_offset - fileHeader.dataOffset) % bytes_per_row / 3; // 每像素3字节

        outfile.seekp(pixel_offset);
        if (pixel_on_subtitles(row, col)) {
            // 修改像素值
            outfile.write(reinterpret_cast<char*>(&color), sizeof(pixel_data));
        }
        else
            outfile.write(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//添加字幕总函数
int bitmap_subtitles::add_subtitles() {
    // 计算字幕在图像中的位置（假设字幕在左上角）
    int subtitle_length = subtitles.length(); // 获取字幕长度
    int image_width = width(); // 获取图像宽度
    int image_height = height(); // 获取图像高度
    // 检查是否超出图像范围，如果超出，返回错误
    if (!vertical && posx + subtitle_length * (font + distance) - distance > image_width ||
        vertical && posy + subtitle_length * (font + distance) - distance >= image_height) {
        return -1; // 表示字幕超出图像范围
    }
    //复制bmp文件头
    copy_bitmap_head();
    //添加字幕
    switch (infoHeader.bitsPerPixel) {
        case 1:
            return add_sub_Monochrome();
        case 4:
            return add_sub_16Color();
        case 8:
            return add_sub_256Color();
        case 24:
            return add_sub_24BitColor();
        default:
            cerr << "不支持该色深: " << infoHeader.bitsPerPixel << "b/像素的字幕添加" << endl;
            return -1; // 返回错误值
    }
}

/****************************************************************
* HanziFont类：16*16点阵字库读取
*****************************************************************/
HanziFont::HanziFont(const char* fontFile, int byteSize) : filename(fontFile), inFont(fontFile, ios::in | ios::binary), fontByteSize(byteSize) {
    if (!inFont.is_open())
        cerr << "无法打开字库文件: " << filename << endl;
    if (byteSize != 32)
        cerr << "暂不支持" << byteSize << "字节点阵读取：" << fontFile << endl;
}

HanziFont::~HanziFont() {
    if (inFont.is_open())
        inFont.close();
}

//返回字库大小
size_t HanziFont::getFileSize() {
    if (!inFont.is_open()) {
        cerr << "无法打开字库文件: " << filename << endl;
        return 0;
    }
    inFont.seekg(0, ios::end);
    size_t fileSize = (size_t)inFont.tellg();
    inFont.close();
    return fileSize;
}

//从文件中根据区位获取汉字点阵
void HanziFont::readCharacterDataByArea(int areaCode, int posCode, char* buffer) {
    if (!inFont.is_open()) {
        std::cerr << "字库文件未打开" << std::endl;
        return;
    }
    int index = ((areaCode - 1) * 94 + (posCode - 1)) * fontByteSize;
    inFont.seekg(index, ios::beg);
    inFont.read(fontData, fontByteSize);
    if (buffer)
        inFont.read(buffer, fontByteSize);
}

//获取汉字点阵，成功返回0，失败返回-1
int HanziFont::readCharacterDataByInternalCode(unsigned char highByte, unsigned char lowByte, char* buffer) {
    if (highByte < 0xA1 || highByte > 0xFE || lowByte < 0xA1 || lowByte > 0xFE)
        return -1;//错误编码，返回-1;
    int areaCode = highByte - 0xA0;
    int posCode = lowByte - 0xA0;
    readCharacterDataByArea(areaCode, posCode, buffer);
    return 0;
}

//获取汉字点阵，成功返回0，失败返回-1
int HanziFont::getCharacter(const char * character, char* buffer) {
    return readCharacterDataByInternalCode(character[0], character[1], buffer);
}

//判断坐标row,col是否在汉字上
bool HanziFont::onCharacter(const int row, const int col) {
    if (row < 0 || row > 16 || col < 0 || col > 16) {
        cerr << "汉字判断超限" << endl;
        return false;
    }
    int Byte = row * 2 + col / 8;
    int bit = col % 8;
    return (fontData[Byte] & (0x80 >> bit));
}

//显示当前汉字
void HanziFont::displayCharacterBitmap() {
    // 在屏幕上显示16*16点阵信息
    int index = 0;
    unsigned char decoder = 0x80;//解码器
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            cout << (fontData[index] & decoder ? "**" : "  "); // 假设用 '*' 表示点亮的像素，' ' 表示空白像素
            decoder >>= 1;
            if (!decoder) {
                decoder = 0x80;
                index++;
            }
        }
        cout << endl;
    }
}