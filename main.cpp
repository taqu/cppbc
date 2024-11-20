#include <stdlib.h>
#include <random>
#include "cppbc.h"
#include <lodepng.h>
#include <bc7decomp.h>
#include <bc7enc.h>

namespace
{
void extract(cppbc::u8 block[16 * 4], cppbc::u32 x, cppbc::u32 y, cppbc::u32 w, cppbc::u32 /*h*/, const cppbc::u8* src)
{
	using namespace cppbc;
	src += (w*y + x)*4;
	for(u32 i=0; i<4; ++i){
		for(u32 j=0; j<4; ++j){
			u32 d = (i*4+j)*4;
			u32 s = (i*w+j)*4;
            block[d + 0] = src[s + 0];
            block[d + 1] = src[s + 1];
            block[d + 2] = src[s + 2];
            block[d + 3] = src[s + 3];
        }
	}
}

void inject(cppbc::u8* dst, cppbc::u32 x, cppbc::u32 y, cppbc::u32 w, cppbc::u32 h, const cppbc::u8 block[16 * 4])
{
	using namespace cppbc;
	dst += (w*y + x)*4;
	for(u32 i=0; i<4; ++i){
		for(u32 j=0; j<4; ++j){
			u32 d = (i*w+j)*4;
			u32 s = (i*4+j)*4;
            dst[d + 0] = block[s + 0];
            dst[d + 1] = block[s + 1];
            dst[d + 2] = block[s + 2];
            dst[d + 3] = block[s + 3];
        }
	}
}

void proc(const char* file, cppbc::u32 no, bool alpha = false)
{
	using namespace cppbc;
	cppbc::u8* out = nullptr;
	u32 width = 0;
	u32 height = 0;
	u32 size = lodepng_decode32_file(&out, &width, &height, file);

	if(alpha){
		std::mt19937 rand;
        {
			std::random_device device;
			rand.seed(device());
        }
		for(u32 i=0; i<height; ++i){
			for(u32 j=0; j<width; ++j){
				u32 r = rand();
				if(r&0x01UL){
					continue;
				}
				u32 index = (i*width+j)*4;
				out[index+3] = rand()&0xFFUL;
			}
		}
	}

	u32 wblocks = width / 4;
    u32 hblocks = height / 4;
    u8* blocks = (u8*)malloc(16 * wblocks * hblocks);
	u8 block[16*4];
    {
        bc7enc_compress_block_init();
        bc7enc_compress_block_params params;
        bc7enc_compress_block_params_init(&params);

		cppbc::bc7param params1;
        for(u32 i = 0; i < hblocks; ++i) {
            for(u32 j = 0; j < wblocks; ++j) {
				extract(block, j*4, i*4, width, height, out);
                u8* b = blocks + ((i * wblocks) + j) * 16;
                //bc7enc_compress_block(b, block, &params);
				cppbc::bc7_encode_block(b, params1, block);
            }
        }
        free(out);
    }

	u8* out2 = (u8*)malloc(width*height*4);
	u8* out3 = (u8*)malloc(width*height*4);
	{
		for(u32 i=0; i<hblocks; ++i){
			for(u32 j=0; j<wblocks; ++j){
				const u8* b = blocks + ((i*wblocks) + j)*16;
				bool r = bc7decomp::unpack_bc7(b, (bc7decomp::color_rgba*)block);
				assert(r);
				inject(out2, j*4, i*4, width, height, block);

				cppbc::bc7_decode_block(block, (const u8*)b);
				assert(r);
				inject(out3, j*4, i*4, width, height, block);
			}
		}
		free(blocks);
    }

	char buffer[64];
	if(alpha){
		sprintf_s(buffer, "out1_alpha_%02d.png", no);
    }else{
		sprintf_s(buffer, "out1_%02d.png", no);
	}
	lodepng_encode32_file(buffer, out2, width, height);
	if(alpha){
        sprintf_s(buffer, "out2_alpha_%02d.png", no);
	}else{
		sprintf_s(buffer, "out2_%02d.png", no);
	}
	lodepng_encode32_file(buffer, out3, width, height);
	free(out2);
	free(out3);
}
}

int main(void)
{
	proc("data/cat00.png", 0);
	proc("data/cat01.png", 1);
	proc("data/cat02.png", 2);

	proc("data/cat00.png", 0, true);
	proc("data/cat01.png", 1, true);
	proc("data/cat02.png", 2, true);

	return 0;
}
