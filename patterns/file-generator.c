#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *blinker, *toad, *beacon, *pulsar, *pentadecathlon;
    int arr_bl[] = {11, 12, 13};
    int arr_to[] = {14, 15, 16, 19, 20, 21};
    int arr_be[] = {7, 8, 13, 22, 27, 28};
    int arr_pu[] = {38, 39, 40, 44, 45, 46, 70, 75, 77, 82, 87, 92, 94, 99, 104, 109, 111, 116, 123, 124, 125, 129, 130, 131, 157, 158, 159, 163, 164, 165, 172, 177, 179, 184,
                    189, 194, 196, 201, 206, 211, 213, 218, 242, 243, 244, 248, 249, 250};
    int arr_pe[] = {49, 60, 70, 72, 82, 93, 104, 115, 125, 127, 137, 148};

    blinker = fopen("BLINKER.bin", "wb");
    toad = fopen("TOAD.bin", "wb");
    beacon = fopen("BEACON.bin", "wb");
    pulsar = fopen("PULSAR.bin", "wb");
    pentadecathlon = fopen("PENTADECATHLON.bin", "wb");

    if ((blinker == NULL) || (toad == NULL) || (beacon == NULL) || (pulsar == NULL) || (pentadecathlon == NULL))
    {
        exit(-1);
    }

    fwrite(arr_bl, sizeof(int), 3, blinker);
    fwrite(arr_to, sizeof(int), 6, toad);
    fwrite(arr_be, sizeof(int), 6, beacon);
    fwrite(arr_pu, sizeof(int), 48, pulsar);
    fwrite(arr_pe, sizeof(int), 12, pentadecathlon);

    fclose(blinker);
    fclose(toad);
    fclose(beacon);
    fclose(pulsar);
    fclose(pentadecathlon);
}