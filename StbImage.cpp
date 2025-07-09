#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>
#include "stb_image.h"
#include "StbImage.h"

StbImage::StbImage()
{
    width = 0;
    height = 0;
    bpp = 0;
}

StbImage::~StbImage()
{
    if (data != nullptr)
        stbi_image_free(data);
}

void StbImage::loadFromFile(const std::string& filepath)
{
    std::cout<<"Loading: "<<filepath.c_str()<<" \n";
    data = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);
    if (data == nullptr)
        std::cerr << "Failed to load image!\n" << filepath << std::endl;
}

int StbImage::getWidth() const { return width; }

int StbImage::getHeight() const { return height; }

int StbImage::getBpp() const { return bpp; }

unsigned char* StbImage::getData() const { return data; }