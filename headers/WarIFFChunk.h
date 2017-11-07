#ifndef WAR_IFF_FILE_H
#define WAR_IFF_FILE_H

#include <File.h>
#include <OS.h>

class WarIFFChunk: public BFile
{
public:
	WarIFFChunk(BDirectory *dir,const char *path,int open_mode);
	WarIFFChunk();

	size_t ReadUint8(uint8 *val);
	size_t ReadUint16(uint16 *val);
	size_t ReadUint32(uint32 *val);
	size_t ReadFloat(float *val);
	
	status_t GetSize(off_t *size);
	status_t SetSize(off_t size);

	status_t FindChunk(uint32 name,WarIFFChunk *war_chunk);
	
	uint32 Format();
private:
	void SetTo(WarIFFChunk *file,off_t start_position);

	off_t start_position;
	uint32 chunk; // the name
	uint32 size; // chunk size
	
	uint32 format; // if this chunk contains undetermined data, this chunk name tells the format
};

inline uint32 WarIFFChunk::Format()
{
	return format;
}

#endif // WAR_FILE_H