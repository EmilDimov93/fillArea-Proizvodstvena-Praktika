#include <stdio.h>
#include <stdlib.h>

typedef struct pixel {
	unsigned char R;
	unsigned char G;
	unsigned char B;
}pixel;

pixel **fileToPixels(char *filename, int *sizeX, int *sizeY){

    FILE *file = fopen(filename, "r");

    if (file == NULL){printf("Error! No such file!");}

    (*sizeX) = fgetc(file) - 48;
    fgetc(file);
    (*sizeY) = fgetc(file) - 48;
    fgetc(file);

    char buffer[(*sizeX) * 20 * (*sizeY)];
    int c;

    for(int i = 0; (c = fgetc(file)) != EOF; i++){

        buffer[i] = c;
        buffer[i + 1] = 'x';

    }

    pixel **pixels = (pixel **)malloc((*sizeX) * (*sizeY) * sizeof(pixel *));

    for (int i = 0; i < (*sizeX) * (*sizeY); ++i){

        pixels[i] = (pixel *)malloc(sizeof(pixel));

    }

    int x = 0, y = 0, pos = 1, buffer2;

    for(int i = 0; buffer[i] != 'x'; i++){

        if(buffer[i] >= 48 && buffer[i] <= 57){

            buffer2 = buffer[i] - 48;

            if(buffer[i + 1] >= 48 && buffer[i + 1] <= 57){

                buffer2 = buffer2 * 10 + buffer[i + 1] - 48;
                i++;

                if(buffer[i + 1] >= 48 && buffer[i + 1] <= 57){

                    buffer2 = buffer2 * 10 + buffer[i + 1] - 48;
                    i++;

                }

            }

            if(pos == 1){pixels[x][y].R = buffer2;}
            if(pos == 2){pixels[x][y].G = buffer2;}
            if(pos == 3){pixels[x][y].B = buffer2;}

        }
        else if(buffer[i] == ':'){

            pos++;
            if(pos == 4){
                pos = 1;
            }

        }
        else if(buffer[i] == ' '){

            x++;
            pos = 1;

        }
        else if(buffer[i] == '\n'){

            y++;
            x = 0;
            pos = 1;

        }

    }

    fclose(file);

    return pixels;

}

pixel **fillAround(int x, int y, pixel **pixels, int base_i, int sizeX, int sizeY){

    pixels[x][y].R = 0;
    pixels[x][y].G = 0;
    pixels[x][y].B = 0;

    sizeX--;
    sizeY--;

    int buffer;

    if(x != 0){if((buffer = (pixels[x - 1][y].R + pixels[x - 1][y].G + pixels[x - 1][y].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x - 1, y, pixels, base_i, sizeX, sizeY);}}

    if(y != 0){if((buffer = (pixels[x][y - 1].R + pixels[x][y - 1].G + pixels[x][y - 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x, y - 1, pixels, base_i, sizeX, sizeY);}}

    if(x != sizeX){if((buffer = (pixels[x + 1][y].R + pixels[x + 1][y].G + pixels[x + 1][y].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x + 1, y, pixels, base_i, sizeX, sizeY);}}

    if(y != sizeY){if((buffer = (pixels[x][y + 1].R + pixels[x][y + 1].G + pixels[x][y + 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x, y + 1, pixels, base_i, sizeX, sizeY);}}

    if(x != 0 && y != 0){if((buffer = (pixels[x - 1][y - 1].R + pixels[x - 1][y - 1].G + pixels[x - 1][y - 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x - 1, y - 1, pixels, base_i, sizeX, sizeY);}}

    if(x != sizeX && y != sizeY){if((buffer = (pixels[x + 1][y + 1].R + pixels[x + 1][y + 1].G + pixels[x + 1][y + 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x + 1, y + 1, pixels, base_i, sizeX, sizeY);}}

    if(x != sizeX && y != 0){if((buffer = (pixels[x + 1][y - 1].R + pixels[x + 1][y - 1].G + pixels[x + 1][y - 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x + 1, y - 1, pixels, base_i, sizeX, sizeY);}}

    if(x != 0 && y != sizeY){if((buffer = (pixels[x - 1][y + 1].R + pixels[x - 1][y + 1].G + pixels[x - 1][y + 1].B) / 3) == base_i || buffer == base_i + 1 || buffer == base_i - 1){fillAround(x - 1, y + 1, pixels, base_i, sizeX, sizeY);}}

    return pixels;

}

pixel **fillArea(int x, int y, pixel **pixels, int sizeX, int sizeY){

    if(pixels[x][y].R == 0 && pixels[x][y].G == 0 && pixels[x][y].R == 0){return pixels;}

    int base_i = (pixels[x][y].R + pixels[x][y].G + pixels[x][y].B) / 3;

    pixels = fillAround(x, y, pixels, base_i, sizeX, sizeY);

    return pixels;

}

void returnToFile(pixel **pixels, char *filename, int sizeX, int sizeY){

    FILE *file = fopen(filename, "w");

    if (file == NULL){printf("Error! No such file!");}

    fprintf(file , "%d %d\n", sizeX, sizeY);

    for(int i = 0; i < sizeY; i++){

        for(int j = 0; j < sizeX; j++){

            fprintf(file ,"%d:%d:%d ", pixels[j][i].R, pixels[j][i].G, pixels[j][i].B);

        }

        fprintf(file , "\n");

    }

    fclose(file);

}

void run(){

    int sizeX, sizeY;

    char *filename = malloc(sizeof(char) * 16);

    printf("Enter file name: ");

    gets(filename);

    pixel **pixels = fileToPixels(filename, &sizeX, &sizeY);

    int x, y;

    printf("Enter x and y coordinates of fill location: ");

    scanf("%d %d", &y, &x);

    pixels = fillArea(x, y, pixels, sizeX, sizeY);

    returnToFile(pixels, filename, sizeX, sizeY);

    for (int i = 0; i < sizeX * sizeY; i++){

        free(pixels[i]);

    }

    free(pixels);

    printf("Done!");

}

int main()
{

    run();

}
