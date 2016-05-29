//returns the maximum number of bytes that can be embedded in the cover
int getMaxPayloadSize(unsigned char* ppm); 

//embeds a message of length message_len in the cover work ppm
void embedStego(unsigned char* ppm, unsigned char* message, int message_len);

//extracts stego info from ppm and writes to plain
unsigned char* extractStego(unsigned char* ppm, unsigned char* plain);

//advances ptr to the beginning of embedding area
//returns new ptr
unsigned char* advanceToEmbeddingArea(unsigned char* ptr);

//advances ptr to the next data chunk
//returns new ptr
unsigned char* advanceToNextChunk(unsigned char* ptr);

//returns the width in pixels of the image
int getWidthFix(unsigned char* ptr);

//returns the height in pixels of the image
int getHeightFix(unsigned char* ptr);