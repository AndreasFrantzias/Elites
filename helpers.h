int find_direction();
int index_of_bus(int bus_choice, int direction);
void clearScreen();
int* grab_bus_indices(int bus_index, int* num_stops);
int* find_bus_lines_for_stop(int stop_code, int *i);
int* grab_local_time();
int calculate_arrival_time(int* localtime, int* departuretime, int arrival_index, int bus_index);
int* list_of_arrival_times_two_hour_window(int* localtime, int arrival_index, int bus_index);
