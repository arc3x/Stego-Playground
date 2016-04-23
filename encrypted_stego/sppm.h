int getMaxPayloadSize(unsigned char* ppm); 
void embedStego(unsigned char* ppm, unsigned char* message, int message_len);
unsigned char* extractStego(unsigned char* ppm, unsigned char* plain);
