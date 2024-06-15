#ifndef CSV
#define CSV

void write_int_array_CSV(const char *filename, int *array, int size);

void write_double_matrix_CSV(const char * filename, double ** array, int size_x, int size_y);

#endif