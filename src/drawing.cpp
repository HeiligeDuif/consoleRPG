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

sprite drawing::loadPNG(const std::string& filePath, int frameX, int frameY, int frameWidth, int frameHeight) {
    sprite sprite;
    int imgWidth = 0, imgHeight = 0, channels = 0;

    unsigned char* rawBytes = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &channels, 4);

    if (!rawBytes) {
        printw("Fout bij laden van PNG");
        refresh();
        return sprite;
    }

    // Als er geen specifieke frame-grootte is meegegeven, pakken we de hele afbeelding!
    int targetWidth = (frameWidth > 0) ? frameWidth : imgWidth;
    int targetHeight = (frameHeight > 0) ? frameHeight : imgHeight;

    sprite.width = targetWidth;
    sprite.height = targetHeight;

    int gridHeight = (targetHeight + 1) / 2;
    sprite.pixels.resize(gridHeight, std::vector<colorPixel>(targetWidth));

    // Bepaal de start-offset in pixels
    int startPixelX = frameX * targetWidth;
    int startPixelY = frameY * targetHeight;

    for (int y = 0; y < targetHeight; y += 2) {
        int cellY = y / 2;
        int currentY = startPixelY + y;

        for (int x = 0; x < targetWidth; ++x) {
            int currentX = startPixelX + x;

            // Safety check: blijf binnen het geheugen van de geladen afbeelding
            if (currentX >= imgWidth || currentY >= imgHeight) {
                continue;
            }

            // 1. Bovenste pixel
            int topIndex = (currentY * imgWidth + currentX) * 4;
            bool topAlpha = rawBytes[topIndex + 3] < 128;
            short topFg = topAlpha ? -1 : getClosestColor(rawBytes[topIndex], rawBytes[topIndex + 1], rawBytes[topIndex + 2]);

            // 2. Onderste pixel
            bool bottomAlpha = true;
            short bottomBg = -1;

            if (y + 1 < targetHeight && (currentY + 1) < imgHeight) {
                int bottomIndex = ((currentY + 1) * imgWidth + currentX) * 4;
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

    stbi_image_free(rawBytes);
    return sprite;
}

void drawing::drawSprite(int startX, int startY, const sprite& sprite) {
    for (size_t y = 0; y < sprite.pixels.size(); ++y) {
        for (size_t x = 0; x < sprite.pixels[y].size(); ++x) {
            const colorPixel& pixel = sprite.pixels[y][x];

            if (pixel.transparent) {
                continue;
            }

            // Gebruik -1 (transparant) als 0 (zwart) voor de indexering
            short fg = (pixel.fgColor < 0) ? 0 : pixel.fgColor;
            short bg = (pixel.bgColor < 0) ? 0 : pixel.bgColor;

            // Bereken welk ID dit is in jouw 16x16 matrix!
            short pair = (fg * 16) + bg + 1;

            attron(COLOR_PAIR(pair));
            mvprintw(startY + static_cast<int>(y), startX + static_cast<int>(x), "%s", UPPER_BLOCK);
            attroff(COLOR_PAIR(pair));
        }
    }
#ifdef _WIN32
    HWND hwnd = GetActiveWindow();
    if (hwnd != NULL) {
        SetFocus(hwnd);
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
#endif
}

void drawing::clearArea(int startX, int startY, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            mvaddch(startY + y, startX + x, ' '); // Overschrijf met een lege spatie
        }
    }
}