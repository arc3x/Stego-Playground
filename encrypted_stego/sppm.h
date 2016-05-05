//returns the maximum number of bytes that can be embedded in the cover
int getMaxPayloadSize(unsigned char* ppm); 

//embeds a message of length message_len in the cover work ppm
void embedStego(unsigned char* ppm, unsigned char* message, int message_len);

//extracts stego info from ppm and writes to plain
unsigned char* extractStego(unsigned char* ppm, unsigned char* plain);
