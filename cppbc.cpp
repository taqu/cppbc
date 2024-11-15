#include "cppbc.h"

namespace cppbc
{
namespace
{
    // clang-format off
    const u8 bc7_partitions2[64][16] =
    {
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
        {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
        {0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
        {0,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1},
        {0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1},
        {0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1},
        {0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1},

	    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1},
        {0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1},
        {0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},

        {0,0,0,0,1,0,0,0,1,1,1,0,1,1,1,1},
        {0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0},
        {0,1,1,1,0,0,1,1,0,0,0,1,0,0,0,0},
        {0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0},
        {0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1},
        
        {0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0},
        {0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0},
        {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
        {0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0},
        {0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0},
        {0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0},

	    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
        {0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0},
        {0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0},
        {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
        {0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0},
        {0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1},
        {0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,1},

	    {0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0},
        {0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0},
        {0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,0},
        {0,0,1,1,1,0,1,1,1,1,0,1,1,1,0,0},
        {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0},
        {0,0,1,1,1,1,0,0,1,1,0,0,0,0,1,1},
        {0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1},
        {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},

	    {0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
        {0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0},
        {0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0},
        {0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1},
        {0,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1},
        {0,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0},
        {0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0},

	    {0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1},
        {0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1},
        {0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,1},
        {0,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1},
        {0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0},
        {0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0},
        {0,1,0,0,0,1,0,0,0,1,1,1,0,1,1,1},
    };

    const u8 bc7_partitions3[64][16] =
    {
	    {0,0,1,1,0,0,1,1,0,2,2,1,2,2,2,2},
        {0,0,0,1,0,0,1,1,2,2,1,1,2,2,2,1},
        {0,0,0,0,2,0,0,1,2,2,1,1,2,2,1,1},
        {0,2,2,2,0,0,2,2,0,0,1,1,0,1,1,1},
        {0,0,0,0,0,0,0,0,1,1,2,2,1,1,2,2},
        {0,0,1,1,0,0,1,1,0,0,2,2,0,0,2,2},
        {0,0,2,2,0,0,2,2,1,1,1,1,1,1,1,1},
        {0,0,1,1,0,0,1,1,2,2,1,1,2,2,1,1},

	    {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2},
        {0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2},
        {0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,2},
        {0,0,1,2,0,0,1,2,0,0,1,2,0,0,1,2},
        {0,1,1,2,0,1,1,2,0,1,1,2,0,1,1,2},
        {0,1,2,2,0,1,2,2,0,1,2,2,0,1,2,2},
        {0,0,1,1,0,1,1,2,1,1,2,2,1,2,2,2},
        {0,0,1,1,2,0,0,1,2,2,0,0,2,2,2,0},

	    {0,0,0,1,0,0,1,1,0,1,1,2,1,1,2,2},
        {0,1,1,1,0,0,1,1,2,0,0,1,2,2,0,0},
        {0,0,0,0,1,1,2,2,1,1,2,2,1,1,2,2},
        {0,0,2,2,0,0,2,2,0,0,2,2,1,1,1,1},
        {0,1,1,1,0,1,1,1,0,2,2,2,0,2,2,2},
        {0,0,0,1,0,0,0,1,2,2,2,1,2,2,2,1},
        {0,0,0,0,0,0,1,1,0,1,2,2,0,1,2,2},
        {0,0,0,0,1,1,0,0,2,2,1,0,2,2,1,0},

	    {0,1,2,2,0,1,2,2,0,0,1,1,0,0,0,0},
        {0,0,1,2,0,0,1,2,1,1,2,2,2,2,2,2},
        {0,1,1,0,1,2,2,1,1,2,2,1,0,1,1,0},
        {0,0,0,0,0,1,1,0,1,2,2,1,1,2,2,1},
        {0,0,2,2,1,1,0,2,1,1,0,2,0,0,2,2},
        {0,1,1,0,0,1,1,0,2,0,0,2,2,2,2,2},
        {0,0,1,1,0,1,2,2,0,1,2,2,0,0,1,1},
        {0,0,0,0,2,0,0,0,2,2,1,1,2,2,2,1},

	    {0,0,0,0,0,0,0,2,1,1,2,2,1,2,2,2},
        {0,2,2,2,0,0,2,2,0,0,1,2,0,0,1,1},
        {0,0,1,1,0,0,1,2,0,0,2,2,0,2,2,2},
        {0,1,2,0,0,1,2,0,0,1,2,0,0,1,2,0},
        {0,0,0,0,1,1,1,1,2,2,2,2,0,0,0,0},
        {0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0},
        {0,1,2,0,2,0,1,2,1,2,0,1,0,1,2,0},
        {0,0,1,1,2,2,0,0,1,1,2,2,0,0,1,1},

	    {0,0,1,1,1,1,2,2,2,2,0,0,0,0,1,1},
        {0,1,0,1,0,1,0,1,2,2,2,2,2,2,2,2},
        {0,0,0,0,0,0,0,0,2,1,2,1,2,1,2,1},
        {0,0,2,2,1,1,2,2,0,0,2,2,1,1,2,2},
        {0,0,2,2,0,0,1,1,0,0,2,2,0,0,1,1},
        {0,2,2,0,1,2,2,1,0,2,2,0,1,2,2,1},
        {0,1,0,1,2,2,2,2,2,2,2,2,0,1,0,1},
        {0,0,0,0,2,1,2,1,2,1,2,1,2,1,2,1},

	    {0,1,0,1,0,1,0,1,0,1,0,1,2,2,2,2},
        {0,2,2,2,0,1,1,1,0,2,2,2,0,1,1,1},
        {0,0,0,2,1,1,1,2,0,0,0,2,1,1,1,2},
        {0,0,0,0,2,1,1,2,2,1,1,2,2,1,1,2},
        {0,2,2,2,0,1,1,1,0,1,1,1,0,2,2,2},
        {0,0,0,2,1,1,1,2,1,1,1,2,0,0,0,2},
        {0,1,1,0,0,1,1,0,0,1,1,0,2,2,2,2},
        {0,0,0,0,0,0,0,0,2,1,1,2,2,1,1,2},

	    {0,1,1,0,0,1,1,0,2,2,2,2,2,2,2,2},
        {0,0,2,2,0,0,1,1,0,0,1,1,0,0,2,2},
        {0,0,2,2,1,1,2,2,1,1,2,2,0,0,2,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,2,1,1,2},
        {0,0,0,2,0,0,0,1,0,0,0,2,0,0,0,1},
        {0,2,2,2,1,2,2,2,0,2,2,2,1,2,2,2},
        {0,1,0,1,2,2,2,2,2,2,2,2,2,2,2,2},
        {0,1,1,1,2,0,1,1,2,2,0,1,2,2,2,0},
    };
    // clang-format on

	u8 extract_mode(u32 src)
    {
        u8 mode = 0;
        for(; mode<8; ++mode){
            if(0x01UL == ((src >> mode) & 0x01UL)){
                break;
            }
        }
        return mode;
    }

    u32 get_num_subsets(u8 mode)
    {
        switch(mode){
        case 0:
            return 3;
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 2;
        case 7:
            return 2;
        default:
            return 1;
        }
    }

    u32 extract_partition_set_id(u8 mode, const u32 src[4])
    {
        switch(mode) {
        case 0:
            return (src[0] >> 1) & 0x0FUL;
        case 1:
            return (src[0] >> 2) & 0x3FUL;
        case 2:
            return (src[0] >> 3) & 0x3FUL;
        case 3:
            return (src[0] >> 4) & 0x3FUL;
        case 7:
            return (src[0] >> 8) & 0x3FUL;
        default:
            return 0;
        }
    }

    u32 get_color_bitcount(u32 index_mode)
    {
        return (0==index_mode)? 2 : 3;
    }

    const u16 aWeights2[] = {0, 21, 43, 64};
    const u16 aWeights3[] = {0, 9, 18, 27, 37, 46, 55, 64};
    const u16 aWeights4[] = {0, 4, 9, 13, 17, 21, 26, 30, 34, 38, 43, 47, 51, 55, 60, 64};

    u8 interpolate2(u8 e0, u8 e1, u8 index)
    {
        return (u8)(((64 - aWeights2[index]) * u16(e0) + aWeights2[index] * u16(e1) + 32) >> 6);
    }

    u8 interpolate3(u8 e0, u8 e1, u8 index)
    {
        return (u8)(((64 - aWeights3[index]) * u16(e0) + aWeights3[index] * u16(e1) + 32) >> 6);
    }

    u8 interpolate4(u8 e0, u8 e1, u8 index)
    {
        return (u8)(((64 - aWeights4[index]) * u16(e0) + aWeights4[index] * u16(e1) + 32) >> 6);
    }

    bool bc7_decode_mode0(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 0;
        u32 partition_set_id = extract_partition_set_id(mode, src);
        u8 endpoint_r[6];
        u8 endpoint_g[6];
        u8 endpoint_b[6];
        endpoint_r[0] = (src[0]>>5)&0x0FUL;
        endpoint_r[1] = (src[0]>>9)&0x0FUL;
        endpoint_r[2] = (src[0]>>13)&0x0FUL;
        endpoint_r[3] = (src[0]>>17)&0x0FUL;
        endpoint_r[4] = (src[0]>>21)&0x0FUL;
        endpoint_r[5] = (src[0]>>25)&0x0FUL;

        endpoint_g[0] = ((src[0]>>29)&0x07UL) | ((src[1]&0x01UL)<<3);
        endpoint_g[1] = (src[1]>>1)&0x0FUL;
        endpoint_g[2] = (src[1]>>5)&0x0FUL;
        endpoint_g[3] = (src[1]>>9)&0x0FUL;
        endpoint_g[4] = (src[1]>>13)&0x0FUL;
        endpoint_g[5] = (src[1]>>17)&0x0FUL;

        endpoint_b[0] = (src[1]>>21)&0x0FUL;
        endpoint_b[1] = (src[1]>>25)&0x0FUL;
        endpoint_b[2] = ((src[1]>>29)&0x07UL) | ((src[2]&0x01UL)<<3);
        endpoint_b[3] = (src[2]>>1)&0x0FUL;
        endpoint_b[4] = (src[2]>>5)&0x0FUL;
        endpoint_b[5] = (src[2]>>9)&0x0FUL;

        u8 pbits[6];
        pbits[0] = (src[2]>>13) & 0x01UL;
        pbits[1] = (src[2]>>14) & 0x01UL;
        pbits[2] = (src[2]>>15) & 0x01UL;
        pbits[3] = (src[2]>>16) & 0x01UL;
        pbits[4] = (src[2]>>17) & 0x01UL;
        pbits[5] = (src[2]>>18) & 0x01UL;

        for(u32 i=0; i<6; ++i){
            endpoint_r[i] = (endpoint_r[i]<<1) | pbits[i];
            endpoint_g[i] = (endpoint_g[i]<<1) | pbits[i];
            endpoint_b[i] = (endpoint_b[i]<<1) | pbits[i];

            endpoint_r[i] <<= 3;
            endpoint_g[i] <<= 3;
            endpoint_b[i] <<= 3;
            endpoint_r[i] |= endpoint_r[i]>>5;
            endpoint_g[i] |= endpoint_g[i]>>5;
            endpoint_b[i] |= endpoint_b[i]>>5;
        }
        u8 color_indices[16];
        color_indices[0] = (src[2]>>19)&0x7UL;
        color_indices[1] = (src[2]>>22)&0x7UL;
        color_indices[2] = (src[2]>>25)&0x7UL;
        color_indices[3] = (src[2]>>28)&0x7UL;

        color_indices[4] = ((src[2]>>31)&0x1UL) | ((src[3]&0x3UL)<<1);
        color_indices[5] = (src[3]>>2)&0x7UL;
        color_indices[6] = (src[3]>>5)&0x7UL;
        color_indices[7] = (src[3]>>8)&0x7UL;

        color_indices[8] = (src[3]>>11)&0x7UL;
        color_indices[9] = (src[3]>>14)&0x7UL;
        color_indices[10] = (src[3]>>17)&0x7UL;
        color_indices[11] = (src[3]>>20)&0x7UL;

        color_indices[12] = (src[3]>>23)&0x7UL;
        color_indices[13] = (src[3]>>26)&0x3UL;
        color_indices[14] = (src[3]>>28)&0x3UL;
        color_indices[15] = (src[3]>>30)&0x3UL;

        const u8* partition = bc7_partitions3[partition_set_id];
        for(u32 i=0; i<16; ++i){
            u8 subset_index = partition[i];
            u8 endpoint_start = 2*subset_index;
            u8 endpoint_end = 2*subset_index + 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate3(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
            dst[index+1] = interpolate3(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
            dst[index+2] = interpolate3(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
            dst[index+3] = 0xFFUL;
        }
    }

    bool bc7_decode_mode1(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 1;
        u32 partition_set_id = extract_partition_set_id(mode, src);
        u8 endpoint_r[4];
        u8 endpoint_g[4];
        u8 endpoint_b[4];
        endpoint_r[0] = (src[0]>>8)&0x3FUL;
        endpoint_r[1] = (src[0]>>14)&0x3FUL;
        endpoint_r[2] = (src[0]>>20)&0x3FUL;
        endpoint_r[3] = (src[0]>>26)&0x3FUL;

        endpoint_g[0] = (src[1]>>0)&0x3FUL;
        endpoint_g[1] = (src[1]>>6)&0x3FUL;
        endpoint_g[2] = (src[1]>>12)&0x3FUL;
        endpoint_g[3] = (src[1]>>18)&0x3FUL;

        endpoint_b[0] = (src[1]>>24)&0x3FUL;
        endpoint_b[1] = ((src[1]>>30)&0x03UL) | ((src[2]&0x0FUL)<<4);
        endpoint_b[2] = (src[2]>>4)&0x3FUL;
        endpoint_b[3] = (src[2]>>10)&0x3FUL;

        u8 pbits[2];
        pbits[0] = (src[2]>>16) & 0x01UL;
        pbits[1] = (src[2]>>17) & 0x01UL;

        {
            endpoint_r[0] = (endpoint_r[0] << 1) | pbits[0];
            endpoint_g[0] = (endpoint_g[0] << 1) | pbits[0];
            endpoint_b[0] = (endpoint_b[0] << 1) | pbits[0];

            endpoint_r[1] = (endpoint_r[1] << 1) | pbits[1];
            endpoint_g[1] = (endpoint_g[1] << 1) | pbits[1];
            endpoint_b[1] = (endpoint_b[1] << 1) | pbits[1];

            endpoint_r[2] = (endpoint_r[2] << 1) | pbits[0];
            endpoint_g[2] = (endpoint_g[2] << 1) | pbits[0];
            endpoint_b[2] = (endpoint_b[2] << 1) | pbits[0];

            endpoint_r[3] = (endpoint_r[3] << 1) | pbits[1];
            endpoint_g[3] = (endpoint_g[3] << 1) | pbits[1];
            endpoint_b[3] = (endpoint_b[3] << 1) | pbits[1];
        }

        for(u32 i=0; i<4; ++i){
            endpoint_r[i] <<= 1;
            endpoint_g[i] <<= 1;
            endpoint_b[i] <<= 1;
            endpoint_r[i] |= endpoint_r[i]>>7;
            endpoint_g[i] |= endpoint_g[i]>>7;
            endpoint_b[i] |= endpoint_b[i]>>7;
        }
        u8 color_indices[16];
        color_indices[0] = (src[2]>>18)&0x7UL;
        color_indices[1] = (src[2]>>21)&0x7UL;
        color_indices[2] = (src[2]>>24)&0x7UL;
        color_indices[3] = (src[2]>>27)&0x7UL;

        color_indices[4] = ((src[2]>>30)&0x3UL) | ((src[3]&0x1UL)<<1);
        color_indices[5] = (src[3]>> 1)&0x7UL;
        color_indices[6] = (src[3]>> 4)&0x7UL;
        color_indices[7] = (src[3]>> 7)&0x7UL;

        color_indices[8] = (src[3]>>10)&0x7UL;
        color_indices[9] = (src[3]>>13)&0x7UL;
        color_indices[10] = (src[3]>>16)&0x7UL;
        color_indices[11] = (src[3]>>19)&0x7UL;

        color_indices[12] = (src[3]>>22)&0x7UL;
        color_indices[13] = (src[3]>>25)&0x7UL;
        color_indices[14] = (src[3]>>27)&0x7UL;
        color_indices[15] = (src[3]>>30)&0x3UL;

        const u8* partition = bc7_partitions2[partition_set_id];
        for(u32 i=0; i<16; ++i){
            u8 subset_index = partition[i];
            u8 endpoint_start = 2*subset_index;
            u8 endpoint_end = 2*subset_index + 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate3(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
            dst[index+1] = interpolate3(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
            dst[index+2] = interpolate3(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
            dst[index+3] = 0xFFUL;
        }
    }

    bool bc7_decode_mode2(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 2;
        u32 partition_set_id = extract_partition_set_id(mode, src);
        u8 endpoint_r[6];
        u8 endpoint_g[6];
        u8 endpoint_b[6];
        endpoint_r[0] = (src[0]>>9)&0x1FUL;
        endpoint_r[1] = (src[0]>>14)&0x1FUL;
        endpoint_r[2] = (src[0]>>19)&0x1FUL;
        endpoint_r[3] = (src[0]>>24)&0x1FUL;
        endpoint_r[4] = ((src[0]>>29)&0x07UL) | ((src[1]&0x03UL)<<3);
        endpoint_r[5] = (src[1]>>2)&0x1FUL;

        endpoint_g[0] = (src[1]>>7)&0x1FUL;
        endpoint_g[1] = (src[1]>>12)&0x1FUL;
        endpoint_g[2] = (src[1]>>17)&0x1FUL;
        endpoint_g[3] = (src[1]>>22)&0x1FUL;
        endpoint_g[4] = (src[1]>>27)&0x1FUL;
        endpoint_g[5] = (src[2]>> 0)&0x1FUL;

        endpoint_b[0] = (src[2]>>5)&0x1FUL;
        endpoint_b[1] = (src[2]>>10)&0x1FUL;
        endpoint_b[2] = (src[2]>>15)&0x1FUL;
        endpoint_b[3] = (src[2]>>20)&0x1FUL;
        endpoint_b[4] = (src[2]>>25)&0x1FUL;
        endpoint_b[5] = ((src[2]>>30)&0x03UL) | ((src[3]&0x07UL)<<2);

        for(u32 i=0; i<6; ++i){
            endpoint_r[i] <<= 3;
            endpoint_g[i] <<= 3;
            endpoint_b[i] <<= 3;
            endpoint_r[i] |= endpoint_r[i]>>5;
            endpoint_g[i] |= endpoint_g[i]>>5;
            endpoint_b[i] |= endpoint_b[i]>>5;
        }
        u8 color_indices[16];
        color_indices[0] = (src[3]>>3)&0x3UL;
        color_indices[1] = (src[3]>>5)&0x3UL;
        color_indices[2] = (src[3]>>7)&0x3UL;
        color_indices[3] = (src[3]>>9)&0x3UL;

        color_indices[4] = (src[3]>>11)&0x3UL;
        color_indices[5] = (src[3]>>13)&0x3UL;
        color_indices[6] = (src[3]>>15)&0x3UL;
        color_indices[7] = (src[3]>>17)&0x3UL;

        color_indices[8] = (src[3]>>19)&0x3UL;
        color_indices[9] = (src[3]>>21)&0x3UL;
        color_indices[10] = (src[3]>>23)&0x3UL;
        color_indices[11] = (src[3]>>25)&0x3UL;

        color_indices[12] = (src[3]>>27)&0x3UL;
        color_indices[13] = (src[3]>>29)&0x1UL;
        color_indices[14] = (src[3]>>30)&0x1UL;
        color_indices[15] = (src[3]>>31)&0x1UL;

        const u8* partition = bc7_partitions3[partition_set_id];
        for(u32 i=0; i<16; ++i){
            u8 subset_index = partition[i];
            u8 endpoint_start = 2*subset_index;
            u8 endpoint_end = 2*subset_index + 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate2(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
            dst[index+1] = interpolate2(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
            dst[index+2] = interpolate2(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
            dst[index+3] = 0xFFUL;
        }
    }

    bool bc7_decode_mode3(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 3;
        u32 partition_set_id = extract_partition_set_id(mode, src);
        u8 endpoint_r[4];
        u8 endpoint_g[4];
        u8 endpoint_b[4];
        endpoint_r[0] = (src[0]>>10)&0x7FUL;
        endpoint_r[1] = (src[0]>>17)&0x7FUL;
        endpoint_r[2] = (src[0]>>24)&0x7FUL;
        endpoint_r[3] = ((src[0]>>31)&0x01UL) | ((src[1]&0x3FUL)<<1);

        endpoint_g[0] = (src[1]>>6)&0x7FUL;
        endpoint_g[1] = (src[1]>>13)&0x7FUL;
        endpoint_g[2] = (src[1]>>20)&0x7FUL;
        endpoint_g[3] = ((src[1]>>27)&0x1FUL) | ((src[2]*0x03UL)<<5);

        endpoint_b[0] = (src[2]>>2)&0x7FUL;
        endpoint_b[1] = (src[2]>>9)&0x7FUL;
        endpoint_b[2] = (src[2]>>16)&0x7FUL;
        endpoint_b[3] = (src[2]>>23)&0x7FUL;

        u8 pbits[4];
        pbits[0] = (src[2]>>30) & 0x01UL;
        pbits[1] = (src[2]>>31) & 0x01UL;
        pbits[2] = (src[3]>> 0) & 0x01UL;
        pbits[3] = (src[3]>> 1) & 0x01UL;

        for(u32 i=0; i<4; ++i){
            endpoint_r[i] = (endpoint_r[i] << 1) | pbits[i];
            endpoint_g[i] = (endpoint_g[i] << 1) | pbits[i];
            endpoint_b[i] = (endpoint_b[i] << 1) | pbits[i];
        }

        //for(u32 i=0; i<4; ++i){
        //    endpoint_r[i] <<= 0;
        //    endpoint_g[i] <<= 0;
        //    endpoint_b[i] <<= 0;
        //    endpoint_r[i] |= endpoint_r[i]>>8;
        //    endpoint_g[i] |= endpoint_g[i]>>8;
        //    endpoint_b[i] |= endpoint_b[i]>>8;
        //}
        u8 color_indices[16];
        color_indices[0] = (src[3]>>2)&0x3UL;
        color_indices[1] = (src[3]>>4)&0x3UL;
        color_indices[2] = (src[3]>>6)&0x3UL;
        color_indices[3] = (src[3]>>8)&0x3UL;

        color_indices[4] = (src[3]>>10)&0x3UL;
        color_indices[5] = (src[3]>>12)&0x3UL;
        color_indices[6] = (src[3]>>14)&0x3UL;
        color_indices[7] = (src[3]>>16)&0x3UL;

        color_indices[8] = (src[3]>>18)&0x3UL;
        color_indices[9] = (src[3]>>20)&0x3UL;
        color_indices[10] = (src[3]>>22)&0x3UL;
        color_indices[11] = (src[3]>>24)&0x3UL;

        color_indices[12] = (src[3]>>26)&0x3UL;
        color_indices[13] = (src[3]>>28)&0x3UL;
        color_indices[14] = (src[3]>>30)&0x1UL;
        color_indices[15] = (src[3]>>31)&0x1UL;

        const u8* partition = bc7_partitions2[partition_set_id];
        for(u32 i=0; i<16; ++i){
            u8 subset_index = partition[i];
            u8 endpoint_start = 2*subset_index;
            u8 endpoint_end = 2*subset_index + 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate2(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
            dst[index+1] = interpolate2(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
            dst[index+2] = interpolate2(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
            dst[index+3] = 0xFFUL;
        }
    }

    bool bc7_decode_mode4(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 4;
        u32 rotation = (src[0]>>5)&0x03UL;
        u32 index_mode = (src[0]>>7)&0x01UL;
        u32 color_bitcount = get_color_bitcount(index_mode);

        u8 endpoint_r[2];
        u8 endpoint_g[2];
        u8 endpoint_b[2];
        u8 endpoint_a[2];
        endpoint_r[0] = (src[0]>>8)&0x1FUL;
        endpoint_r[1] = (src[0]>>13)&0x1FUL;

        endpoint_g[0] = (src[0]>>18)&0x1FUL;
        endpoint_g[1] = (src[0]>>23)&0x1FUL;

        endpoint_b[0] = ((src[0]>>28)&0x0FUL) | ((src[1]&0x01UL)<<4);
        endpoint_b[1] = (src[1]>>1)&0x1FUL;

        endpoint_a[0] = (src[1]>>6)&0x3FUL;
        endpoint_a[1] = (src[1]>>12)&0x3FUL;

        for(u32 i=0; i<2; ++i){
            endpoint_r[i] <<= 3;
            endpoint_g[i] <<= 3;
            endpoint_b[i] <<= 3;
            endpoint_a[i] <<= 2;

            endpoint_r[i] |= endpoint_r[i]>>5;
            endpoint_g[i] |= endpoint_g[i]>>5;
            endpoint_b[i] |= endpoint_b[i]>>5;
            endpoint_a[i] |= endpoint_a[i]>>6;
        }
        u8 color_indices[16];
        color_indices[0] = (src[1]>>18)&0x3UL;
        color_indices[1] = (src[1]>>20)&0x3UL;
        color_indices[2] = (src[1]>>22)&0x3UL;
        color_indices[3] = (src[1]>>24)&0x3UL;

        color_indices[4] = (src[1]>>26)&0x3UL;
        color_indices[5] = (src[1]>>28)&0x3UL;
        color_indices[6] = (src[1]>>30)&0x3UL;
        color_indices[7] = (src[2]>> 0)&0x3UL;

        color_indices[8] = (src[2]>> 2)&0x3UL;
        color_indices[9] = (src[2]>> 4)&0x3UL;
        color_indices[10] = (src[2]>> 6)&0x3UL;
        color_indices[11] = (src[2]>> 8)&0x3UL;

        color_indices[12] = (src[2]>>10)&0x3UL;
        color_indices[13] = (src[2]>>12)&0x3UL;
        color_indices[14] = (src[2]>>14)&0x3UL;
        color_indices[15] = (src[2]>>16)&0x1UL;

        u8 alpha_indices[16];
        alpha_indices[0] = (src[2]>>17)&0x7UL;
        alpha_indices[1] = (src[2]>>20)&0x7UL;
        alpha_indices[2] = (src[2]>>23)&0x7UL;
        alpha_indices[3] = (src[2]>>26)&0x7UL;

        alpha_indices[4] = (src[2]>>29)&0x7UL;
        alpha_indices[5] = (src[3]>> 0)&0x7UL;
        alpha_indices[6] = (src[3]>> 3)&0x7UL;
        alpha_indices[7] = (src[3]>> 6)&0x7UL;

        alpha_indices[8] = (src[3]>> 9)&0x7UL;
        alpha_indices[9] = (src[3]>>12)&0x7UL;
        alpha_indices[10] = (src[3]>>15)&0x7UL;
        alpha_indices[11] = (src[3]>>18)&0x7UL;

        alpha_indices[12] = (src[3]>>21)&0x7UL;
        alpha_indices[13] = (src[3]>>24)&0x7UL;
        alpha_indices[14] = (src[3]>>27)&0x7UL;
        alpha_indices[15] = (src[3]>>30)&0x3UL;

        for(u32 i=0; i<16; ++i){
            static constexpr u8 endpoint_start = 0;
            static constexpr u8 endpoint_end = 1;
            u8 color_index = color_indices[i];
            u8 alpha_index = alpha_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate2(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
            dst[index+1] = interpolate2(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
            dst[index+2] = interpolate2(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
            dst[index+3] = interpolate3(endpoint_a[endpoint_start], endpoint_a[endpoint_end], alpha_index);

            //{
            //    if(mode.type == 4 OR == 5) {
            //        rotation = extract_rot_bits(mode, block);
            //        output = swap_channels(output, rotation);
            //    }
            //}
        }
    }

}

bool bc7_decode_block(u8 dst[64], const u8 src[16])
{
    u8 mode = extract_mode(src[0]);
    return false;
}
} // namespace cppbc

