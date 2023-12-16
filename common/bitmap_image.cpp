/* 2252201 ������ */
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "../include/bitmap_image.h"
#include "../include/cmd_hdc_tools.h"
using namespace std;

/****************************************************************
* bitmap_image�ࣺBMPͼƬ������
*****************************************************************/
//���캯��
bitmap_image::bitmap_image(const string& fname) : filename(fname), bmpfile(fname, ios::in | ios::binary) {
    if (!bmpfile.is_open()) {
        cerr << "�޷���bmp�ļ�: " << filename << endl;
        return;
    }
    // �ڹ��캯���ж�ȡBMP�ļ�ͷ��Ϣ
    read_bitmap();
}

//�����������ر��ļ�
bitmap_image::~bitmap_image() {
    if (bmpfile.is_open()) {
        bmpfile.close();
    }
}

//������Ϣ��ȡ����
void bitmap_image::read_bitmap() {
    if (bmpfile) {
        // ��ȡBMP�ļ�ͷ
        bmpfile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
        bmpfile.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

        // �ж��Ƿ��е�ɫ��
        if (infoHeader.bitsPerPixel <= 8) {
            // �����ɫ���������
            int colorsCount = infoHeader.colorsInPalette > 0 ? infoHeader.colorsInPalette : (1 << infoHeader.bitsPerPixel);

            // ��ȡ��ɫ������
            bmpfile.read(reinterpret_cast<char*>(&palette), colorsCount * sizeof(RGBQuad));

            // �����ɫ���е���ɫ��Ϣ
            //for (int i = 0; i < colorsCount; ++i) {
                //std::cout << "Color " << i << ": R=" << (int)palette[i].red << " G=" << (int)palette[i].green << " B=" << (int)palette[i].blue << std::endl;
            //}
        }

        // ���Ը�����Ҫ����ȡ���ļ�ͷ��Ϣ�Ƿ���ȷ

    }
    return;
}

//��ɫɫ������ض�ȡ����
unsigned int bitmap_image::get_pixel_Monochrome(int row, int col) {
    // ����ÿ��������ռ�ֽ�����ÿ�����ؽ�ռ1λ��
    uint32_t bytes_per_row = ((infoHeader.imageWidth + 31) / 32) * 4;

    // �����������ļ��е�ƫ��λ��
    uint32_t byte_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col / 8;

    // �����������ֽ��е�ƫ��λ��
    uint8_t bit_offset = 7 - (col % 8);

    // ���ļ�����λ������λ��
    if (bmpfile) {
        bmpfile.seekg(byte_offset);

        // ��ȡ��������
        uint8_t pixel_byte = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));

        // ��ȡָ�����ص���ɫ��Ϣ
        unsigned int color = (pixel_byte >> bit_offset) & 0x01;

        // ����ɫֵӳ��ΪRGBֵ
        unsigned int rgb_value = RGB(palette[color].red, palette[color].green, palette[color].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // ����������ɫֵ
    }
    else {
        //cerr << "���ļ�ʧ��: " << filename << endl;
        return 0; // ����Ĭ����ɫֵ
    }
}

//16ɫɫ������ض�ȡ����
unsigned int bitmap_image::get_pixel_16Color(int row, int col) {
    // ����ÿ��������ռ�ֽ�����ÿ������ռ��4λ��
    uint32_t bytes_per_row = ((infoHeader.imageWidth * 4 + 31) / 32) * 4;

    // �����������ļ��е�ƫ��λ��
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col / 2;

    // ���ļ�����λ������λ��
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // ��ȡ��������
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // ��ȡָ�����ص���ɫ��Ϣ
        unsigned int color = (col % 2 == 0) ? (pixel_data >> 4) : (pixel_data & 0x0F);

        // ��ɫֵӳ��ΪRGBֵ
        unsigned int rgb_value = RGB(palette[color].red, palette[color].green, palette[color].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // ����������ɫֵ
    }
    else {
        cerr << "���ļ�ʧ��: " << filename << endl;
        return 0; // ����Ĭ����ɫֵ
    }
}

//256ɫɫ������ض�ȡ����
unsigned int bitmap_image::get_pixel_256Color(int row, int col) {
    // ����ÿ��������ռ�ֽ�����ÿ������ռ��8λ��
    uint32_t bytes_per_row = infoHeader.imageWidth;

    // �����������ļ��е�ƫ��λ��
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col;

    // ���ļ�����λ������λ��
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // ��ȡ��������
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // ��ɫֵӳ��ΪRGBֵ
        unsigned int rgb_value = RGB(palette[pixel_data].red, palette[pixel_data].green, palette[pixel_data].blue);// = *(unsigned int*)(palette + color)

        return rgb_value; // ����������ɫֵ
    }
    else {
        cerr << "���ļ�ʧ��: " << filename << endl;
        return 0; // ����Ĭ����ɫֵ
    }
}

//24λɫ������ض�ȡ����
unsigned int bitmap_image::get_pixel_24BitColor(int row, int col) {
    // ����ÿ��������ռ�ֽ�����ÿ������ռ��3�ֽڣ�
    uint32_t bytes_per_row = (infoHeader.imageWidth) * 3 + infoHeader.imageWidth % 4;

    // �����������ļ��е�ƫ��λ��
    uint32_t pixel_offset = fileHeader.dataOffset + (infoHeader.imageHeight - row - 1) * bytes_per_row + col * 3;

    // ���ļ�����λ������λ��
    if (bmpfile) {
        bmpfile.seekg(pixel_offset);

        // ��ȡRGBֵ
        uint8_t pixel_data[3] = { 0 };
        bmpfile.read(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));

        // ��RGBֵ�ϲ�Ϊunsigned int����
        unsigned int red = pixel_data[2];
        unsigned int green = pixel_data[1];
        unsigned int blue = pixel_data[0];

        return RGB(red, green, blue); // ����RGB��ɫֵ
    }
    else {
        cerr << "���ļ�ʧ��: " << filename << endl;
        return 0; // ����Ĭ����ɫֵ
    }
}

//����ͼƬ�Ŀ��
int bitmap_image::width() const {
    return infoHeader.imageWidth;
}

//����ͼƬ�ĸ߶�
int bitmap_image::height() const {
    return infoHeader.imageHeight;
}

//����ָ�����RGB��ɫ
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
            cerr << "��֧�ָ�ɫ��: " << infoHeader.bitsPerPixel << "b/����" << endl;
            return 0; // ����Ĭ����ɫֵ
    }
}

//����ͼƬ
void bitmap_image::show_image(const int point_x, const int point_y, const int size)
{
    /* ���ϵ��¡���������ʾ����ͼ�� */
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
* bitmap_subtitles�ࣺ��bmp�ļ��м���Ļ��������
*****************************************************************/
/* bmp�ļ���ʼ�� */
bitmap_subtitles::bitmap_subtitles(const std::string& bmpfname, const std::string& outfname, const std::string& subtit) : bitmap_image(bmpfname), outfile(outfname, ios::out | ios::binary), subtitles(subtit), jianCh("HZK16"), fanCh("HZK16F") {
    if (!outfile.is_open()) {
        cerr << "�޷������bmp�ļ�: " << outfname << endl;
    }
    posx = posy = 0;
    color = 0x0FFFFFF;
    font = 16;
    distance = 3;
    traditional = false;
    vertical = false;
}

//��������
bitmap_subtitles::~bitmap_subtitles() {
    if (outfile.is_open()) {
        outfile.close();
    }
}

//������Ļλ�ã��ɹ�����0��ʧ�ܷ���-1
int bitmap_subtitles::set_pos(const int pos_x, const int pos_y) {
    if (pos_x < 0 || pos_y < 0 || pos_x > width() || pos_y > height())
        return -1;
    posx = pos_x;
    posy = pos_y;
    return 0;
}

//������Ļ��ɫ�����óɹ����ص�ǰ��ɫ��ʧ�ܷ���-1����Ĭ����ɫ
int bitmap_subtitles::set_color(const std::string& colr) {
    if (colr.length() != 6) {
        cerr << "��ɫֵ#" << colr << "����Ϊ" << colr.length() << endl;
        return -1;
    }
    int oriColor = color;
    istringstream iss(colr);
    iss >> hex >> color >> dec;
    if (!iss) {
        cerr << "��ɫֵ#" << colr << "����ʮ��������." << endl;
        color = oriColor;
        return -1;
    }
    return color;
}

//���ü���false/����true
void bitmap_subtitles::set_traditional(const bool trad) {
    traditional = trad;
}

//����style(horizontal����/vertical���򣩣����óɹ�����0��ʧ�ܷ���-1
void bitmap_subtitles::set_vertical(const bool vert) {
    vertical = vert;
}

//����bmp�ļ�ͷ��outfile
void bitmap_subtitles::copy_bitmap_head() {
    if (outfile.is_open()) {
        //�洢�ļ�ͷ�ĸ���
        char* buffer = new char[fileHeader.dataOffset];

        // ���ļ�ָ�붨λ���ļ���ʼ��
        bmpfile.seekg(0, ios::beg);
        // ��ȡBMP�ļ�ͷ������
        bmpfile.read(buffer, fileHeader.dataOffset);

        // ���ļ�ָ�붨λ���ļ���ʼ��
        outfile.seekp(0, ios::beg);
        // ���ļ�ͷ��Ϣд������ļ�
        outfile.write(buffer, fileHeader.dataOffset);

        //�ͷŸ���
        delete[] buffer;

        // �����Ҫ���д����ļ�ͷ��Ϣ�Ƿ���ȷ��������������Ӵ���

        // ȷ��д��ɹ���ˢ������ļ���
        outfile.flush();
    }
    else
        cerr << "���bmp�ļ��޷���" << endl;
}

//�ж�row��col�е������Ƿ�����Ļ��
bool bitmap_subtitles::pixel_on_subtitles(const int row, const int col) {
    int x = col - posx, y = row - posy;
    int subtitle_length = subtitles.length() / 2; // ��ȡ��Ļ����
    //�ҵ���Ӧ����
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

//��ɫɫ���bmp�ļ���Ļ��Ӻ���
int bitmap_subtitles::add_sub_Monochrome() {
    if (!bmpfile || !outfile)
        return -1;
    // ����ÿ��������ռ�ֽ�����ÿ�����ؽ�ռ1λ��
    const uint32_t bytes_per_row = ((infoHeader.imageWidth + 31) / 32) * 4;
    for (uint32_t byte_offset = fileHeader.dataOffset; byte_offset < fileHeader.fileSize; ++byte_offset) {
        bmpfile.seekg(byte_offset);

        // ��ȡ��������
        uint8_t pixel_byte = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));

        //�������ж�
        for (uint32_t bit_offset = 0; bit_offset < 8; ++bit_offset) {
            int row = infoHeader.imageHeight - ((byte_offset - fileHeader.dataOffset) / bytes_per_row + 1);
            int col_byte = (byte_offset - fileHeader.dataOffset) % bytes_per_row; // ����ƫ���ڸ��е��ֽ�ƫ����
            int col_bit = 7 - bit_offset; // �õ����е�λƫ����
            int col = col_byte * 8 + col_bit; // �����ܵ���ƫ����
            if (pixel_on_subtitles(row, col)) {
                pixel_byte &= ~(0x01 << bit_offset);
                //hdc_point(col * 8, row * 8, 8, 0x0FFFFFF);
            }
        }

        //д����
        outfile.seekp(byte_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_byte), sizeof(pixel_byte));
    }
    return 0;
}

//16ɫɫ���bmp�ļ���Ļ��Ӻ���
int bitmap_subtitles::add_sub_16Color() {
    if (!bmpfile || !outfile)
        return -1;

    // ����ÿ��������ռ�ֽ�����ÿ������ռ4λ��
    const uint32_t bytes_per_row = ((infoHeader.imageWidth * 4 + 31) / 32) * 4;

    for (uint32_t byte_offset = fileHeader.dataOffset; byte_offset < fileHeader.fileSize; ++byte_offset) {
        bmpfile.seekg(byte_offset);

        // ��ȡ��������
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        // �������ж�
        for (uint32_t bit_offset = 0; bit_offset < 8; bit_offset += 4) {
            int row = infoHeader.imageHeight - ((byte_offset - fileHeader.dataOffset) / bytes_per_row + 1);
            int col_byte = (byte_offset - fileHeader.dataOffset) % bytes_per_row; // ����ƫ���ڸ��е��ֽ�ƫ����
            int col_bit = (bit_offset / 4); // �õ����е�λƫ����
            int col = col_byte * 2 + col_bit; // �����ܵ���ƫ����

            if (pixel_on_subtitles(row, col)) {
                if (bit_offset == 0)
                    pixel_data &= 0xF0; // �����4λ
                else
                    pixel_data &= 0x0F; // �����4λ
            }
        }

        //д����
        outfile.seekp(byte_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//256ɫɫ���bmp�ļ���Ļ��Ӻ���
int bitmap_subtitles::add_sub_256Color() {
    if (!bmpfile || !outfile)
        return -1;

    // ����ÿ��������ռ�ֽ�����ÿ������ռ��8λ��
    const uint32_t bytes_per_row = infoHeader.imageWidth;

    for (uint32_t pixel_offset = fileHeader.dataOffset; pixel_offset < fileHeader.fileSize; ++pixel_offset) {
        bmpfile.seekg(pixel_offset);

        // ��ȡ��������
        uint8_t pixel_data = 0;
        bmpfile.read(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));

        int row = infoHeader.imageHeight - ((pixel_offset - fileHeader.dataOffset) / bytes_per_row + 1);
        int col = (pixel_offset - fileHeader.dataOffset) % bytes_per_row;

        if (pixel_on_subtitles(row, col)) {
            // �޸�����ֵ
            pixel_data = 0;//Ĭ�Ϻ�ɫ
        }

        //д����
        outfile.seekp(pixel_offset);
        outfile.write(reinterpret_cast<char*>(&pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//24λɫ���bmp�ļ���Ļ��Ӻ���
int bitmap_subtitles::add_sub_24BitColor() {
    if (!bmpfile || !outfile)
        return -1;

    // ����ÿ��������ռ�ֽ�����ÿ������ռ��3�ֽڣ�
    const uint32_t bytes_per_row = (infoHeader.imageWidth * 3) + (infoHeader.imageWidth % 4);

    for (uint32_t pixel_offset = fileHeader.dataOffset; pixel_offset < fileHeader.fileSize; pixel_offset += 3) {
        bmpfile.seekg(pixel_offset);

        // ��ȡ RGB ֵ
        uint8_t pixel_data[3] = { 0 };
        bmpfile.read(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));

        int row = infoHeader.imageHeight - ((pixel_offset - fileHeader.dataOffset) / bytes_per_row + 1);
        int col = (pixel_offset - fileHeader.dataOffset) % bytes_per_row / 3; // ÿ����3�ֽ�

        outfile.seekp(pixel_offset);
        if (pixel_on_subtitles(row, col)) {
            // �޸�����ֵ
            outfile.write(reinterpret_cast<char*>(&color), sizeof(pixel_data));
        }
        else
            outfile.write(reinterpret_cast<char*>(pixel_data), sizeof(pixel_data));
    }
    return 0;
}

//�����Ļ�ܺ���
int bitmap_subtitles::add_subtitles() {
    // ������Ļ��ͼ���е�λ�ã�������Ļ�����Ͻǣ�
    int subtitle_length = subtitles.length(); // ��ȡ��Ļ����
    int image_width = width(); // ��ȡͼ����
    int image_height = height(); // ��ȡͼ��߶�
    // ����Ƿ񳬳�ͼ��Χ��������������ش���
    if (!vertical && posx + subtitle_length * (font + distance) - distance > image_width ||
        vertical && posy + subtitle_length * (font + distance) - distance >= image_height) {
        return -1; // ��ʾ��Ļ����ͼ��Χ
    }
    //����bmp�ļ�ͷ
    copy_bitmap_head();
    //�����Ļ
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
            cerr << "��֧�ָ�ɫ��: " << infoHeader.bitsPerPixel << "b/���ص���Ļ���" << endl;
            return -1; // ���ش���ֵ
    }
}

/****************************************************************
* HanziFont�ࣺ16*16�����ֿ��ȡ
*****************************************************************/
HanziFont::HanziFont(const char* fontFile, int byteSize) : filename(fontFile), inFont(fontFile, ios::in | ios::binary), fontByteSize(byteSize) {
    if (!inFont.is_open())
        cerr << "�޷����ֿ��ļ�: " << filename << endl;
    if (byteSize != 32)
        cerr << "�ݲ�֧��" << byteSize << "�ֽڵ����ȡ��" << fontFile << endl;
}

HanziFont::~HanziFont() {
    if (inFont.is_open())
        inFont.close();
}

//�����ֿ��С
size_t HanziFont::getFileSize() {
    if (!inFont.is_open()) {
        cerr << "�޷����ֿ��ļ�: " << filename << endl;
        return 0;
    }
    inFont.seekg(0, ios::end);
    size_t fileSize = (size_t)inFont.tellg();
    inFont.close();
    return fileSize;
}

//���ļ��и�����λ��ȡ���ֵ���
void HanziFont::readCharacterDataByArea(int areaCode, int posCode, char* buffer) {
    if (!inFont.is_open()) {
        std::cerr << "�ֿ��ļ�δ��" << std::endl;
        return;
    }
    int index = ((areaCode - 1) * 94 + (posCode - 1)) * fontByteSize;
    inFont.seekg(index, ios::beg);
    inFont.read(fontData, fontByteSize);
    if (buffer)
        inFont.read(buffer, fontByteSize);
}

//��ȡ���ֵ��󣬳ɹ�����0��ʧ�ܷ���-1
int HanziFont::readCharacterDataByInternalCode(unsigned char highByte, unsigned char lowByte, char* buffer) {
    if (highByte < 0xA1 || highByte > 0xFE || lowByte < 0xA1 || lowByte > 0xFE)
        return -1;//������룬����-1;
    int areaCode = highByte - 0xA0;
    int posCode = lowByte - 0xA0;
    readCharacterDataByArea(areaCode, posCode, buffer);
    return 0;
}

//��ȡ���ֵ��󣬳ɹ�����0��ʧ�ܷ���-1
int HanziFont::getCharacter(const char * character, char* buffer) {
    return readCharacterDataByInternalCode(character[0], character[1], buffer);
}

//�ж�����row,col�Ƿ��ں�����
bool HanziFont::onCharacter(const int row, const int col) {
    if (row < 0 || row > 16 || col < 0 || col > 16) {
        cerr << "�����жϳ���" << endl;
        return false;
    }
    int Byte = row * 2 + col / 8;
    int bit = col % 8;
    return (fontData[Byte] & (0x80 >> bit));
}

//��ʾ��ǰ����
void HanziFont::displayCharacterBitmap() {
    // ����Ļ����ʾ16*16������Ϣ
    int index = 0;
    unsigned char decoder = 0x80;//������
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            cout << (fontData[index] & decoder ? "**" : "  "); // ������ '*' ��ʾ���������أ�' ' ��ʾ�հ�����
            decoder >>= 1;
            if (!decoder) {
                decoder = 0x80;
                index++;
            }
        }
        cout << endl;
    }
}