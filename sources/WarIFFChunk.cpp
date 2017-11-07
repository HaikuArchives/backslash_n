#include "WarIFFChunk.h"

#include <File.h>
#include <stdio.h>

WarIFFChunk::WarIFFChunk():BFile()
{
	start_position = 0;
	chunk = 0;
	size = 0;
}

WarIFFChunk::WarIFFChunk(BDirectory *dir,const char *path,int open_mode):
	BFile(dir,path,open_mode)
{
	Seek(0,SEEK_SET);
	ReadUint32(&chunk);
	ReadUint32((uint32*)&size);

	if (chunk == 'FORM' || chunk == 'LIST' || chunk == 'PROP')
		ReadUint32(&format);
	
	start_position = Position();
}

void WarIFFChunk::SetTo(WarIFFChunk *file,off_t start)
{
	operator=(*file);

	start_position = start;

	Seek(start_position,SEEK_SET);
	ReadUint32(&chunk);
	ReadUint32((uint32*)&size);

	if (chunk == 'FORM' || chunk == 'LIST' || chunk == 'PROP')
		ReadUint32(&format);
	
	start_position = Position();
}

status_t WarIFFChunk::GetSize(off_t *ret_size)
{
	*ret_size = size;
	return B_OK;
}

status_t WarIFFChunk::SetSize(off_t new_size)
{
	return B_ERROR;
}

status_t WarIFFChunk::FindChunk(uint32 name,WarIFFChunk *war_chunk)
{
	war_chunk->Unset();

	Seek(start_position,SEEK_SET);
	uint32 chunk_size;
	uint32 chunk_name;
	
	//---- sök efter namnet
	if ( ReadUint32(&chunk_name) != 4 ) return B_ERROR;
	if ( ReadUint32(&chunk_size) != 4 ) return B_ERROR;
	
	while ( chunk_name != name )
	{
		Seek(chunk_size,SEEK_CUR);
		if ( ReadUint32(&chunk_name) != 4 ) return B_ERROR;
		if ( ReadUint32(&chunk_size) != 4 ) return B_ERROR;
	}
	//---- namnet ev. funnet
	
	if (name == chunk_name)
	{
		war_chunk->SetTo(this,Position() - 8);
		return B_OK;
	}
	else return B_ERROR;
}

size_t WarIFFChunk::ReadFloat(float *val)
{
	return ReadUint32((uint32*)(val));
}

size_t WarIFFChunk::ReadUint8(uint8 *val)
{
	size_t stat;
	uint8 tmp;
	stat = Read(&tmp,1);
	
//	printf("read:%d - %c\n",tmp,tmp);
	*val = tmp;
	return stat;
}

size_t WarIFFChunk::ReadUint16(uint16 *val)
{
	size_t stat;
	uint8 low,high;
	
	stat = ReadUint8(&high);
	stat += ReadUint8(&low);
	*val = high << 8;
	*val |= low;
	return stat;
}

size_t WarIFFChunk::ReadUint32(uint32 *val)
{
	size_t stat;
	uint8 low,low_middle,high_middle,high;
	
	stat = ReadUint8(&high);
	stat += ReadUint8(&high_middle);
	stat += ReadUint8(&low_middle);
	stat += ReadUint8(&low);

	*val = high << 24;
	*val |= high_middle << 16;
	*val |= low_middle << 8;
	*val |= low;
	return stat;
}

