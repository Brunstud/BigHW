/* 2252201 ������ �ƿƶ��� */

#pragma once

/* ��Ҫ�õ���ͷ�ļ� */
#include <cstring>
#include <fstream>

/* ������Ҫ�����Լ��Ķ��� */
#pragma pack(push, 1) // ȷ���ṹ�尴��1�ֽڶ���

// BMP�ļ�ͷ�ṹ��
struct BMPFileHeader {
    uint16_t fileType;       // �ļ����ͣ�����Ϊ"BM"��0x424D��
    uint32_t fileSize;       // �ļ���С����λΪ�ֽ�
    uint16_t reserved1;      // �����ֶΣ�ֵΪ0
    uint16_t reserved2;      // �����ֶΣ�ֵΪ0
    uint32_t dataOffset;     // λͼ���ݵ���ʼλ�ã�������ļ�ͷ��ƫ����
};

// BMP��Ϣͷ�ṹ��
struct BMPInfoHeader {
    uint32_t headerSize;        // ��Ϣͷ��С��һ��Ϊ40�ֽ�
    int32_t  imageWidth;        // ͼ���ȣ���λΪ����
    int32_t  imageHeight;       // ͼ��߶ȣ���λΪ����
    uint16_t colorPlanes;       // ��ɫƽ����������Ϊ1
    uint16_t bitsPerPixel;      // ÿ�����ص�λ��������ֵΪ24����ʾRGB��ɫģʽ��
    uint32_t compressionMethod; // ͼ��ѹ����ʽ������ֵΪ0����ʾ��ѹ����
    uint32_t imageSize;         // ͼ�����ݵĴ�С����λΪ�ֽ�
    int32_t  horizontalRes;     // ˮƽ�ֱ��ʣ���λΪ����ÿ��
    int32_t  verticalRes;       // ��ֱ�ֱ��ʣ���λΪ����ÿ��
    uint32_t colorsInPalette;   // ��ɫ���е���ɫ������Ϊ0�����ʾʹ�����п��ܵ���ɫ
    uint32_t importantColors;   // ��Ҫ����ɫ������Ϊ0�����ʾ������ɫ����Ҫ
};

struct RGBQuad { // ��ɫ���е���ɫ��
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
};

#pragma pack(pop) // �ָ�Ĭ�϶��뷽ʽ

class bitmap_image
{
protected:
    /* ������Ҫ�����Լ��Ķ��� */
    std::string filename;
    std::ifstream bmpfile;
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    RGBQuad palette[256]; // ��ɫ�������256����ɫ��
    //���ú���
    void read_bitmap();
    unsigned int get_pixel_Monochrome(int row, int col); //��ɫɫ������ض�ȡ����
    unsigned int get_pixel_16Color(int row, int col); //16ɫɫ������ض�ȡ����
    unsigned int get_pixel_256Color(int row, int col); //256ɫɫ������ض�ȡ����
    unsigned int get_pixel_24BitColor(int row, int col); //24λɫ������ض�ȡ����
public:
    /* bmp�ļ���ʼ�� */
    bitmap_image(const std::string& fname);
    ~bitmap_image();
    //ʵ�ֹ���
    int width() const;	//����ͼƬ�Ŀ��
    int height() const; //����ͼƬ�ĸ߶�
    unsigned int get_pixel(int row, int col); //����ָ�����RGB��ɫ
    void show_image(const int point_x = 0, const int point_y = 0, const int size = 1);//��ʾͼƬ
};

#ifndef HANZIFONT_H
#define HANZIFONT_H

//16*16�����ֿ��ȡ
class HanziFont{
private:
    const char * filename;
    std::ifstream inFont;
    int fontByteSize;
    char fontData[32];//32λ,��ʱ����

    //���ù��ܺ���
    void readCharacterDataByArea(int areaCode, int posCode, char* buffer);
    int readCharacterDataByInternalCode(unsigned char highByte, unsigned char lowByte, char* buffer);

public:
    /* �ֿ��ʼ�� */
    HanziFont(const char* fontFilename, int byteSize = 32);
    ~HanziFont();
    //ʵ�ֹ���
    size_t getFileSize();
    int getCharacter(const char* character, char* buffer = NULL);
    bool onCharacter(const int row, const int col);
    void displayCharacterBitmap();
};

#endif /* HANZIFONT_H */

//��bmp�ļ��м���Ļ��������
class bitmap_subtitles : public bitmap_image
{
protected:
    std::ofstream outfile;
    std::string subtitles;
    int posx;           //��Ļ����λ�ã�Ĭ��0
    int posy;           //��Ļ����λ�ã�Ĭ��0
    int color;          //��Ļǰ����ɫ��Ĭ��0x0FFFFFF������ɫ������24bitɫ��Ч��
    int font;           //���������С��Ĭ��16����
    int distance;       //�����ּ�࣬Ĭ��3
    bool traditional;   //�Ƿ��÷��壬Ĭ��false
    bool vertical;      //��Ļ����Ĭ��false
    HanziFont jianCh;   //�����ֿ�
    HanziFont fanCh;    //�����ֿ�
    //���ú���
    void copy_bitmap_head();    //����bmp�ļ�ͷ��outfile
    bool pixel_on_subtitles(const int row, const int col);  //�ж�row��col�е������Ƿ�����Ļ��
    int add_sub_Monochrome();   //��ɫɫ���bmp�ļ���Ļ��Ӻ���
    int add_sub_16Color();      //16ɫɫ���bmp�ļ���Ļ��Ӻ���
    int add_sub_256Color();     //256ɫɫ���bmp�ļ���Ļ��Ӻ���
    int add_sub_24BitColor();   //24λɫ���bmp�ļ���Ļ��Ӻ���
public:
    /* bmp�ļ���ʼ�� */
    bitmap_subtitles(const std::string& bmpfname, const std::string& outfname, const std::string& subtit);
    ~bitmap_subtitles();
    //���ñ���
    int set_pos(const int pos_x, const int pos_y);  //������Ļλ��
    int set_color(const std::string& colr);         //������Ļǰ����ɫ
    void set_traditional(const bool trad = true);   //�����Ƿ��÷���
    void set_vertical(const bool vert = true);       //������Ļ�Ƿ�ֱ
    //����ת��
    int add_subtitles();   //�����Ļ�ܺ���
};