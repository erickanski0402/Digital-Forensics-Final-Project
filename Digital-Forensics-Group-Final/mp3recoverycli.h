typedef struct ID3_Tag_Header
{
  CHAR id[3]
  UINT1 version[2];
  UINT1 flags;
  UINT4 size;
} tID3header;

typedef struct MP3_Header
{
  char* mpeg_version;
  char* layer_desc;
  bool protection_bit;
  INT2 bitrate_index;
  INT2 sample_rate_frequency_index;
  bool padding_bit;
  bool private_bit;
  char* channel_mode;
  char* channel_mode_extension;
  bool copy_bit;
  bool original_bit;
  char* emphasis;
};
