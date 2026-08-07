#include "consoleGame.hpp"
#include "stb_image.h"
const char* UPPER_BLOCK = "\xE2\x96\x80";

short drawing::getClosestColor(unsigned char r, unsigned char g, unsigned char b) {
    short bestColor = COLOR_BLACK;
    double minDistance = 999999.0;

    for (int i = 0; i < 16; ++i) {
        double dr = static_cast<double>(r) - ANSI_PALETTE[i].r;
        double dg = static_cast<double>(g) - ANSI_PALETTE[i].g;
        double db = static_cast<double>(b) - ANSI_PALETTE[i].b;

        double distance = std::sqrt(dr * dr + dg * dg + db * db);

        if (distance < minDistance) {
            minDistance = distance;
            bestColor = ANSI_PALETTE[i].ncursesId;
        }
    }
    return bestColor;
}

sprite drawing::loadPNG(const std::string& filePath) {
    sprite sprite;
    int imgWidth = 0;
    int imgHeight = 0;
    int channels = 0;

    // Laad PNG als RGBA (4 kanalen)
    unsigned char* rawBytes = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &channels, 4);

    if (!rawBytes) {
        printw("Fout bij laden van PNG");
        refresh();
        return sprite;
    }

    sprite.width = imgWidth;
    sprite.height = imgHeight;

    // We halveren de hoogte van onze vector omdat 2 verticale pixels samen 1 cel vormen
    int gridHeight = (imgHeight + 1) / 2;
    sprite.pixels.resize(gridHeight, std::vector<colorPixel>(imgWidth));

    for (int y = 0; y < imgHeight; y += 2) {
        int cellY = y / 2;

        for (int x = 0; x < imgWidth; ++x) {
            // 1. Bovenste pixel
            int topIndex = (y * imgWidth + x) * 4;
            bool topAlpha = rawBytes[topIndex + 3] < 128;
            short topFg = topAlpha ? -1 : getClosestColor(rawBytes[topIndex], rawBytes[topIndex + 1], rawBytes[topIndex + 2]);

            // 2. Onderste pixel
            bool bottomAlpha = true;
            short bottomBg = -1;

            if (y + 1 < imgHeight) {
                int bottomIndex = ((y + 1) * imgWidth + x) * 4;
                bottomAlpha = rawBytes[bottomIndex + 3] < 128;
                if (!bottomAlpha) {
                    bottomBg = getClosestColor(rawBytes[bottomIndex], rawBytes[bottomIndex + 1], rawBytes[bottomIndex + 2]);
                }
            }

            // Vul de ColorPixel struct
            colorPixel& pixel = sprite.pixels[cellY][x];
            pixel.fgColor = topFg;
            pixel.bgColor = bottomBg;
            pixel.transparent = (topAlpha && bottomAlpha);
        }
    }

    // Geheugen netjes vrijgeven!
    stbi_image_free(rawBytes);
    return sprite;
}

void drawing::drawSprite(int startX, int startY, const sprite& sprite)
{
    short pairIdCounter = 1;

    for (size_t y = 0; y < sprite.pixels.size(); ++y) {
        for (size_t x = 0; x < sprite.pixels[y].size(); ++x) {
            const colorPixel& pixel = sprite.pixels[y][x];

            if (pixel.transparent) {
                continue;
            }

            short pair = pairIdCounter++;
            init_pair(pair, pixel.fgColor, pixel.bgColor);

            attron(COLOR_PAIR(pair));
            mvprintw(startY + static_cast<int>(y), startX + static_cast<int>(x), "%s", UPPER_BLOCK);
            attroff(COLOR_PAIR(pair));
        }
    }
}