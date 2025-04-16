#ifndef X11

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>

#include "doomgeneric.h"
#include "m_argv.h"

void create_or_empty_dir(const char *dir_path)
{
    struct stat st = {0};

    if (stat(dir_path, &st) == -1)
    {
        if (mkdir(dir_path, 0755) != 0)
        {
            perror("Failed to create directory");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        DIR *dir = opendir(dir_path);
        if (!dir)
        {
            perror("Failed to open directory");
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            // Skip the special entries "." and ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            char entry_path[1024];
            snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_path, entry->d_name);

            if (remove(entry_path) != 0)
            {
                perror("Failed to remove entry");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
        }

        closedir(dir);
    }
}

void save_bmp(const char *filename, uint32_t *image_data, int width, int height)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }

    // BMP file header (14 bytes)
    uint8_t bmp_file_header[14] = {
        'B', 'M',   // Magic number
        0, 0, 0, 0, // File size (to be filled later)
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0 // Offset to pixel data (54 bytes header)
    };

    // BMP info header (40 bytes)
    uint8_t bmp_info_header[40] = {
        40, 0, 0, 0,      // Header size
        0, 0, 0, 0,       // Width (to be filled later)
        0, 0, 0, 0,       // Height (to be filled later)
        1, 0,             // Planes (must be 1)
        32, 0,            // Bit depth (32 bits per pixel)
        0, 0, 0, 0,       // Compression (0 = no compression)
        0, 0, 0, 0,       // Image size (can be 0 for uncompressed images)
        0x13, 0x0B, 0, 0, // Horizontal resolution (pixels per meter, 2835)
        0x13, 0x0B, 0, 0, // Vertical resolution (pixels per meter, 2835)
        0, 0, 0, 0,       // Colors used (0 = all colors)
        0, 0, 0, 0        // Important colors (0 = all colors)
    };

    // Fill the width and height in the info header
    *(uint32_t *)(bmp_info_header + 4) = width;
    *(uint32_t *)(bmp_info_header + 8) = height; // Negative height for top-down BMP

    // Calculate the file size
    uint32_t file_size = 54 + width * height * 4;
    *(uint32_t *)(bmp_file_header + 2) = file_size;

    // Write the file header
    fwrite(bmp_file_header, 1, 14, file);

    // Write the info header
    fwrite(bmp_info_header, 1, 40, file);

    // Write the pixel data
    for (int y = 0; y < height; y++)
    {
        fwrite(image_data + (height - y - 1) * width, 4, width, file);
    }

    // Close the file
    fclose(file);
}

uint32_t frameID = 0;

// genericdoom overrides

pixel_t *DG_ScreenBuffer = NULL;
pixel_t *halfScreenBuffer = NULL;

void M_FindResponseFile(void);
void D_DoomMain(void);

void DG_Init()
{
    halfScreenBuffer = (pixel_t *)malloc((DOOMGENERIC_RESX / 2) * (DOOMGENERIC_RESY / 2) * sizeof(pixel_t));
    if (!halfScreenBuffer)
    {
        fprintf(stderr, "Failed to allocate screen buffer\n");
        exit(EXIT_FAILURE);
    }
}

void DG_DrawFrame()
{
    if (frameID == 0)
    {
        create_or_empty_dir("frames");
    }
    char frameTitle[256];
    snprintf(frameTitle, sizeof(frameTitle), "frames/%d.bmp", frameID);

    for (int x = 0; x < DOOMGENERIC_RESX; x++)
    {
        for (int y = 0; y < DOOMGENERIC_RESY; y++)
        {
            halfScreenBuffer[(y / 2) * (DOOMGENERIC_RESX / 2) + (x / 2)] = DG_ScreenBuffer[y * DOOMGENERIC_RESX + x];
        }
    }

    save_bmp(frameTitle, halfScreenBuffer, DOOMGENERIC_RESX / 2, DOOMGENERIC_RESY / 2);
    frameID++;
}

void DG_SleepMs(uint32_t ms)
{
    usleep(ms * 1000);
}

uint32_t DG_GetTicksMs()
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

int DG_GetKey(int *pressed, unsigned char *doomKey) { return 0; }

void DG_SetWindowTitle(const char *title) {}

void doomgeneric_Create(int argc, char **argv)
{
    // save arguments
    myargc = argc;
    myargv = argv;

    M_FindResponseFile();

    DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

    DG_Init();

    D_DoomMain();
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    while (1)
    {
        doomgeneric_Tick();
    }

    return 0;
}

#endif