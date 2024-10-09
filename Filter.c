#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get RGB values
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            // Calculate the average
            int avg = round((red + green + blue) / 3.0);

            // Set all colors to the average
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap the pixel with its horizontal mirror
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary copy of the image
    RGBTRIPLE temp[height][width];

    // Copy the original image to temp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redSum = 0, greenSum = 0, blueSum = 0;
            int counter = 0;

            // Loop over the neighboring pixels
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if neighboring pixel is valid
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        redSum += temp[ni][nj].rgbtRed;
                        greenSum += temp[ni][nj].rgbtGreen;
                        blueSum += temp[ni][nj].rgbtBlue;
                        counter++;
                    }
                }
            }

            // Set the current pixel to the average of the surrounding pixels
            image[i][j].rgbtRed = round(redSum / (float)counter);
            image[i][j].rgbtGreen = round(greenSum / (float)counter);
            image[i][j].rgbtBlue = round(blueSum / (float)counter);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary copy of the image
    RGBTRIPLE temp[height][width];

    // Sobel Gx and Gy kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Copy the original image to temp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx_red = 0, Gx_green = 0, Gx_blue = 0;
            int Gy_red = 0, Gy_green = 0, Gy_blue = 0;

            // Apply Sobel kernels
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if neighboring pixel is valid
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        RGBTRIPLE pixel = temp[ni][nj];

                        Gx_red += Gx[di + 1][dj + 1] * pixel.rgbtRed;
                        Gx_green += Gx[di + 1][dj + 1] * pixel.rgbtGreen;
                        Gx_blue += Gx[di + 1][dj + 1] * pixel.rgbtBlue;

                        Gy_red += Gy[di + 1][dj + 1] * pixel.rgbtRed;
                        Gy_green += Gy[di + 1][dj + 1] * pixel.rgbtGreen;
                        Gy_blue += Gy[di + 1][dj + 1] * pixel.rgbtBlue;
                    }
                }
            }

            // Compute final color values
            int red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));

            // Ensure the values don't exceed 255
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }
}
