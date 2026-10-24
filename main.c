#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DEFAULT_CHAR_DELAY 200
#define SET_DELAY '~'
#define SET_PAUSE '@'
#define TYPE_NEXT_SYMBOL '^'

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
        printf("Write a file with lyrics in arguments\n");

        return 0;
    }

    FILE *fptr = fopen(argv[1], "r");
    fseek(fptr, 0l, SEEK_END);
    const long fsize = ftell(fptr);
    rewind(fptr);
    char *text = malloc(fsize);
    fread(text, 1, fsize, fptr);
    fclose(fptr);

    if (fsize == 0l) {
        free(text);
        printf("File doesn't contain text");

        return 0;
    }

    bool loop = true;
    size_t current_char = 0;
    clock_t char_delay = DEFAULT_CHAR_DELAY, scan_char = DEFAULT_CHAR_DELAY, last_scan_char = 0, pause = 0, pause_time = 0, last_type_time = 0;

    while (loop) {
        const clock_t temp_clock = clock();
        scan_char = temp_clock - last_scan_char;

        if (text[current_char] == SET_DELAY) {
            bool scan_delay_size_loop = true;
            size_t delay_size = 0, current_delay_char = current_char + 1;

            while (scan_delay_size_loop) {
                if (text[current_delay_char] != SET_DELAY) {
                    delay_size++;
                    current_delay_char++;
                }
                else {
                    scan_delay_size_loop = false;
                }
            }

            char *delay_string = malloc(delay_size + 1);

            for (size_t i = 0; i < delay_size; i++) {
                delay_string[i] = text[current_char + i + 1];
            }

            delay_string[delay_size] = '\0';
            char_delay = atoi(delay_string);
            free(delay_string);
            current_char += delay_size + 2;
            continue;
        }
        else if (text[current_char] == SET_PAUSE) {
            bool scan_pause_size_loop = true;
            size_t pause_size = 0, current_pause_char = current_char + 1;

            while (scan_pause_size_loop) {
                if (text[current_pause_char] != SET_PAUSE) {
                    pause_size++;
                    current_pause_char++;
                }
                else {
                    scan_pause_size_loop = false;
                }
            }

            char *pause_string = malloc(pause_size + 1);

            for (size_t i = 0; i < pause_size; i++) {
                pause_string[i] = text[current_char + i + 1];
            }

            pause_string[pause_size] = '\0';
            pause = atoi(pause_string);
            free(pause_string);
            pause_time = temp_clock;
            current_char += pause_size + 2;
            continue;
        }

        if (pause > 0) {
            if (temp_clock - pause_time > pause) {
                pause = 0;
                scan_char = char_delay;
            }
        }
        else if (scan_char >= char_delay) {

            if (text[current_char] == TYPE_NEXT_SYMBOL) {
                putchar(text[current_char + 1]);
                current_char += 2;
            }
            else {
                putchar(text[current_char]);
                current_char++;
            }

            last_scan_char = temp_clock;
            scan_char -= char_delay;

            if (current_char == fsize - 1) {
                loop = false;
            }
        }
    }

    free(text);
    getchar();

    return 0;
}