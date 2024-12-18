#include "cppbc.h"
#include <cassert>
#include <cstring>
#include <utility>
#include <tuple>
#ifdef _DEBUG
#include <iostream>
#endif

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

    const u8 bc7_anchor_index_second_subset_two[64] =
    {
        15,15,15,15,15,15,15,15,
        15,15,15,15,15,15,15,15,
        15, 2, 8, 2, 2, 8, 8,15,
         2, 8, 2, 2, 8, 8, 2, 2,
        15,15, 6, 8, 2, 8,15,15,
         2, 8, 2, 2, 2,15,15, 6,
         6, 2, 6, 8,15,15, 2, 2,
        15,15,15,15,15, 2, 2,15,
    };

    const u8 bc7_anchor_index_second_subset_three[64] =
    {
        3, 3,15,15, 8, 3,15,15,
        8, 8, 6, 6, 6, 5, 3, 3,
        3, 3, 8,15, 3, 3, 6,10,
        5, 8, 8, 6, 8, 5,15,15,
        8,15, 3, 5, 6,10, 8,15,
        15, 3,15, 5,15,15,15,15,
        3,15, 5, 5, 5, 8, 5,10,
        5,10, 8,13,15,12, 3, 3,
    };

    const u8 bc7_anchor_index_third_subset_three[64] =
    {
        15, 8, 8, 3,15,15, 3, 8,
        15,15,15,15,15,15,15, 8,
        15, 8,15, 3,15, 8,15, 8,
        3,15, 6,10,15,15,10, 8,
        15, 3,15,10,10, 8, 9,10,
        6,15, 8,15, 3, 6, 6, 8,
        15, 3,15,15,15,15,15,15,
        15,15,15,15, 3,15,15, 8,
    };
    // clang-format on

    struct RGBA
    {
        u8 r_;
        u8 g_;
        u8 b_;
        u8 a_;

        const u8& operator[](u32 c) const
        {
            switch(c){
            case 0:
                return r_;
            case 1:
                return g_;
            case 2:
                return b_;
            default:
                return a_;
            }
        }

        u8& operator[](u32 c)
        {
            switch(c){
            case 0:
                return r_;
            case 1:
                return g_;
            case 2:
                return b_;
            default:
                return a_;
            }
        }
    };

    struct EncodeStats
    {
        u32 max_error_;
        u8 block_[16];
    };

    u8 read_bits(u32& offset, const u32* src, u32 size)
    {
        assert(size<=8);
        u32 bits = 0;
        u32 total_bits = 0;

        while(total_bits < size) {
            u32 byte_bit_offset = offset & 31;
            u32 bits_to_read = (std::min)(size - total_bits, 32 - byte_bit_offset);
            u32 byte_bits = src[offset >> 5] >> byte_bit_offset;
            byte_bits &= ((1 << bits_to_read) - 1);

            bits |= (byte_bits << total_bits);

            total_bits += bits_to_read;
            offset += bits_to_read;
        }
        assert(bits<=255);
        assert(offset<=128);
        return static_cast<u8>(bits);
    }

    void write_bits(u32& offset, u8* dst, u8 x, u32 size)
    {
        assert(size<=8);
        u32 total_bits = 0;

        while(total_bits < size) {
            u32 byte_bit_offset = offset & 7;
            u32 bits_to_write = (std::min)(size - total_bits, 8 - byte_bit_offset);
            dst[offset>>3] |= (x&((1<<bits_to_write)-1))<<byte_bit_offset;

            x >>= bits_to_write;
            total_bits += bits_to_write;
            offset += bits_to_write;
        }
        assert(offset<=128);
    }

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

    inline u8 dequant(u8 val, u8 pbit, u8 bits)
    {
        assert(pbit < 2);
        assert(4<=bits && bits<=8);
        const u8 total_bits = bits + 1;
        val = (val << 1) | pbit;
        val <<= (8 - total_bits);
        val |= (val >> total_bits);
        return val;
    }

    inline u8 dequant(u8 val, u8 bits)
    {
        assert(bits >= 4 && bits <= 8);
        val <<= (8 - bits);
        val |= (val >> bits);
        return val;
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


    u8 error(u8 x, u8 c)
    {
        return x<c? c-x : x-c;
    }

    u8 deinterpolate2(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb)
    {
        u32 e = 0;
        e += error(interpolate2(r0, r1, 0), cr);
        e += error(interpolate2(g0, g1, 0), cg);
        e += error(interpolate2(b0, b1, 0), cb);
        u8 index = 0;
        for(u8 i=1; i<4; ++i){
            u32 er = error(interpolate2(r0, r1, i), cr);
            u32 eg = error(interpolate2(g0, g1, i), cg);
            u32 eb = error(interpolate2(b0, b1, i), cb);
            u32 te = er+eg+eb;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return index;
    }

    std::tuple<u8, u32> deinterpolate3(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb)
    {
        u32 e = 0;
        e += error(interpolate3(r0, r1, 0), cr);
        e += error(interpolate3(g0, g1, 0), cg);
        e += error(interpolate3(b0, b1, 0), cb);
        u8 index = 0;
        for(u8 i=1; i<8; ++i){
            u32 er = error(interpolate3(r0, r1, i), cr);
            u32 eg = error(interpolate3(g0, g1, i), cg);
            u32 eb = error(interpolate3(b0, b1, i), cb);
            u32 te = er+eg+eb;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return std::make_tuple(index, e);
    }

    std::tuple<u8, u32> deinterpolate3_2(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb)
    {
        u32 e = 0;
        e += error(interpolate3(r0, r1, 0), cr);
        e += error(interpolate3(g0, g1, 0), cg);
        e += error(interpolate3(b0, b1, 0), cb);
        u8 index = 0;
        for(u8 i=1; i<4; ++i){
            u32 er = error(interpolate3(r0, r1, i), cr);
            u32 eg = error(interpolate3(g0, g1, i), cg);
            u32 eb = error(interpolate3(b0, b1, i), cb);
            u32 te = er+eg+eb;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return std::make_tuple(index, e);
    }


    u8 deinterpolate4(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb)
    {
        u32 e = 0;
        e += error(interpolate4(r0, r1, 0), cr);
        e += error(interpolate4(g0, g1, 0), cg);
        e += error(interpolate4(b0, b1, 0), cb);
        u8 index = 0;
        for(u8 i=1; i<16; ++i){
            u32 er = error(interpolate4(r0, r1, i), cr);
            u32 eg = error(interpolate4(g0, g1, i), cg);
            u32 eb = error(interpolate4(b0, b1, i), cb);
            u32 te = er+eg+eb;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return index;
    }

    u8 deinterpolate2(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb,
        u8 a0, u8 a1, u8 ca)
    {
        u32 e = 0;
        e += error(interpolate2(r0, r1, 0), cr);
        e += error(interpolate2(g0, g1, 0), cg);
        e += error(interpolate2(b0, b1, 0), cb);
        e += error(interpolate2(a0, a1, 0), ca);
        u8 index = 0;
        for(u8 i=1; i<4; ++i){
            u32 er = error(interpolate2(r0, r1, i), cr);
            u32 eg = error(interpolate2(g0, g1, i), cg);
            u32 eb = error(interpolate2(b0, b1, i), cb);
            u32 ea = error(interpolate2(a0, a1, i), ca);
            u32 te = er+eg+eb+ea;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return index;
    }

    u8 deinterpolate3(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb,
        u8 a0, u8 a1, u8 ca)
    {
        u32 e = 0;
        e += error(interpolate3(r0, r1, 0), cr);
        e += error(interpolate3(g0, g1, 0), cg);
        e += error(interpolate3(b0, b1, 0), cb);
        e += error(interpolate3(a0, a1, 0), ca);
        u8 index = 0;
        for(u8 i=1; i<8; ++i){
            u32 er = error(interpolate3(r0, r1, i), cr);
            u32 eg = error(interpolate3(g0, g1, i), cg);
            u32 eb = error(interpolate3(b0, b1, i), cb);
            u32 ea = error(interpolate3(a0, a1, i), ca);
            u32 te = er+eg+eb+ea;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return index;
    }

    std::tuple<u8, u32> deinterpolate4(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb,
        u8 a0, u8 a1, u8 ca)
    {
        u32 e = 0;
        e += error(interpolate4(r0, r1, 0), cr);
        e += error(interpolate4(g0, g1, 0), cg);
        e += error(interpolate4(b0, b1, 0), cb);
        e += error(interpolate4(a0, a1, 0), ca);
        u8 index = 0;
        for(u8 i=1; i<16; ++i){
            u32 er = error(interpolate4(r0, r1, i), cr);
            u32 eg = error(interpolate4(g0, g1, i), cg);
            u32 eb = error(interpolate4(b0, b1, i), cb);
            u32 ea = error(interpolate4(a0, a1, i), ca);
            u32 te = er+eg+eb+ea;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return std::make_tuple(index, e);
    }

    std::tuple<u8, u32> deinterpolate4_3(
        u8 r0, u8 r1, u8 cr,
        u8 g0, u8 g1, u8 cg,
        u8 b0, u8 b1, u8 cb,
        u8 a0, u8 a1, u8 ca)
    {
        u32 e = 0;
        e += error(interpolate4(r0, r1, 0), cr);
        e += error(interpolate4(g0, g1, 0), cg);
        e += error(interpolate4(b0, b1, 0), cb);
        e += error(interpolate4(a0, a1, 0), ca);
        u8 index = 0;
        for(u8 i=1; i<8; ++i){
            u32 er = error(interpolate4(r0, r1, i), cr);
            u32 eg = error(interpolate4(g0, g1, i), cg);
            u32 eb = error(interpolate4(b0, b1, i), cb);
            u32 ea = error(interpolate4(a0, a1, i), ca);
            u32 te = er+eg+eb+ea;
            if(te<e){
                e = te;
                index = i;
            }
        }
        return std::make_tuple(index, e);
    }

    void bc7_decode_mode0(u8 dst[64], const u32 src[4])
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
            endpoint_r[i] = dequant(endpoint_r[i], pbits[i], 4);
            endpoint_g[i] = dequant(endpoint_g[i], pbits[i], 4);
            endpoint_b[i] = dequant(endpoint_b[i], pbits[i], 4);
        }
        u8 color_indices[16];
        u32 offset = 19 + 2*32;
        for(u32 i=0; i<16; ++i){
            u32 bits = (!i)
                || (i == bc7_anchor_index_second_subset_three[partition_set_id])
                || (i == bc7_anchor_index_third_subset_three[partition_set_id])? 2 : 3;
            color_indices[i] = read_bits(offset, src, bits);
        }

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

    void bc7_decode_mode1(u8 dst[64], const u32 src[4])
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
        endpoint_b[1] = ((src[1]>>30)&0x03UL) | ((src[2]&0x0FUL)<<2);
        endpoint_b[2] = (src[2]>>4)&0x3FUL;
        endpoint_b[3] = (src[2]>>10)&0x3FUL;

        u8 pbits[2];
        pbits[0] = (src[2]>>16) & 0x01UL;
        pbits[1] = (src[2]>>17) & 0x01UL;

        u8 decoded_r[4];
        u8 decoded_g[4];
        u8 decoded_b[4];

        for(u32 i=0; i<4; ++i){
            u32 p = i>>1;
            decoded_r[i] = dequant(endpoint_r[i], pbits[p], 6);
            decoded_g[i] = dequant(endpoint_g[i], pbits[p], 6);
            decoded_b[i] = dequant(endpoint_b[i], pbits[p], 6);
        }
        u8 color_indices[16];
        u32 offset = 18 + 2*32;
        for(u32 i=0; i<16; ++i){
            u32 bits = (!i) || (i == bc7_anchor_index_second_subset_two[partition_set_id])? 2 : 3;
            color_indices[i] = read_bits(offset, src, bits);
        }
        const u8* partition = bc7_partitions2[partition_set_id];
        for(u32 i=0; i<16; ++i){
            u8 subset_index = partition[i];
            u8 endpoint_start = 2*subset_index;
            u8 endpoint_end = 2*subset_index + 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate3(decoded_r[endpoint_start], decoded_r[endpoint_end], color_index);
            dst[index+1] = interpolate3(decoded_g[endpoint_start], decoded_g[endpoint_end], color_index);
            dst[index+2] = interpolate3(decoded_b[endpoint_start], decoded_b[endpoint_end], color_index);
            dst[index+3] = 0xFFUL;
        }
#if 0
        std::cout << "mode1:" << std::endl;
        std::cout << "  partition: " << partition_set_id << std::endl;
        std::cout << "  endpoints:" << std::endl;
        for(u32 i=0; i<4; ++i){
            std::cout << "    " << static_cast<u32>(endpoint_r[i]) << " " << static_cast<u32>(endpoint_g[i]) << " " << static_cast<u32>(endpoint_b[i]) << std::endl;
        }
        std::cout << "  pbits:" << std::endl << "    ";
        for(u32 i=0; i<2; ++i){
            std::cout << static_cast<u32>(pbits[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "  index:" << std::endl << "    ";
        for(u32 i=0; i<16; ++i){
            std::cout << static_cast<u32>(color_indices[i]) << " ";
        }
        std::cout << std::endl;
#endif

    }

    void bc7_decode_mode2(u8 dst[64], const u32 src[4])
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
            endpoint_r[i] = dequant(endpoint_r[i], 5);
            endpoint_g[i] = dequant(endpoint_g[i], 5);
            endpoint_b[i] = dequant(endpoint_b[i], 5);
        }

        u8 color_indices[16];
        u32 offset = 3 + 3*32;
        for(u32 i=0; i<16; ++i){
            u32 bits = (!i)
                || (i == bc7_anchor_index_second_subset_three[partition_set_id])
                || (i == bc7_anchor_index_third_subset_three[partition_set_id])? 1 : 2;
            color_indices[i] = read_bits(offset, src, bits);
        }

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

    void bc7_decode_mode3(u8 dst[64], const u32 src[4])
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
        endpoint_g[3] = ((src[1]>>27)&0x1FUL) | ((src[2]&0x03UL)<<5);

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
            endpoint_r[i] = dequant(endpoint_r[i], pbits[i], 7);
            endpoint_g[i] = dequant(endpoint_g[i], pbits[i], 7);
            endpoint_b[i] = dequant(endpoint_b[i], pbits[i], 7);
        }

        u8 color_indices[16];
        u32 offset = 2 + 3*32;
        for(u32 i=0; i<16; ++i){
            u32 bits = (!i) || (i == bc7_anchor_index_third_subset_three[partition_set_id])? 1 : 2;
            color_indices[i] = read_bits(offset, src, bits);
        }

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

    void bc7_decode_mode4(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 4;
        u32 rotation = (src[0]>>5)&0x03UL;
        u32 index_mode = (src[0]>>7)&0x01UL;

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
            endpoint_r[i] = dequant(endpoint_r[i], 5);
            endpoint_g[i] = dequant(endpoint_g[i], 5);
            endpoint_b[i] = dequant(endpoint_b[i], 5);
            endpoint_a[i] = dequant(endpoint_a[i], 6);
        }
        u8 color_indices[16];
        u8 alpha_indices[16];
        const u32 weight_bits[2] = { index_mode ? 3 : 2,  index_mode ? 2 : 3 };
        {
            u8* c = index_mode? color_indices : alpha_indices;
            u8* a = index_mode? alpha_indices : color_indices;

            u32 offset = 17 * 32*1;
            for(u32 i = 0; i < 16; ++i){
                c[i] = read_bits(offset, src, weight_bits[index_mode] - ((!i) ? 1 : 0));
                }

            for(u32 i = 0; i < 16; ++i){
                a[i] = read_bits(offset, src, weight_bits[1 - index_mode] - ((!i) ? 1 : 0));
            }

        }
        for(u32 i=0; i<16; ++i){
            static constexpr u8 endpoint_start = 0;
            static constexpr u8 endpoint_end = 1;
            u8 color_index = color_indices[i];
            u8 alpha_index = alpha_indices[i];
            u32 index = i*4;
            if(0==index_mode) {
                dst[index + 0] = interpolate2(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
                dst[index + 1] = interpolate2(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
                dst[index + 2] = interpolate2(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
                dst[index + 3] = interpolate3(endpoint_a[endpoint_start], endpoint_a[endpoint_end], alpha_index);
            }else{
                dst[index + 0] = interpolate3(endpoint_r[endpoint_start], endpoint_r[endpoint_end], color_index);
                dst[index + 1] = interpolate3(endpoint_g[endpoint_start], endpoint_g[endpoint_end], color_index);
                dst[index + 2] = interpolate3(endpoint_b[endpoint_start], endpoint_b[endpoint_end], color_index);
                dst[index + 3] = interpolate2(endpoint_a[endpoint_start], endpoint_a[endpoint_end], alpha_index);
            }

            if(0<rotation) {
                std::swap(dst[index+rotation-1], dst[index+3]);
            }
        }
    }

    void bc7_decode_mode5(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 5;
        u32 rotation = (src[0]>>6)&0x03UL;

        u8 endpoint_r[2];
        u8 endpoint_g[2];
        u8 endpoint_b[2];
        u8 endpoint_a[2];
        endpoint_r[0] = (src[0]>>8)&0x7FUL;
        endpoint_r[1] = (src[0]>>15)&0x7FUL;

        endpoint_g[0] = (src[0]>>22)&0x7FUL;
        endpoint_g[1] = ((src[0]>>29)&0x07UL) | ((src[1]&0x0FUL)<<3);

        endpoint_b[0] = (src[1]>> 4)&0x7FUL;
        endpoint_b[1] = (src[1]>>11)&0x7FUL;

        endpoint_a[0] = (src[1]>>18)&0xFFUL;
        endpoint_a[1] = ((src[1]>>26)&0x3FUL) | ((src[2]&0x03UL)<<6);

        for(u32 i=0; i<2; ++i){
            endpoint_r[i] = dequant(endpoint_r[i], 7);
            endpoint_g[i] = dequant(endpoint_g[i], 7);
            endpoint_b[i] = dequant(endpoint_b[i], 7);
            //endpoint_a[i] = dequant(endpoint_a[i], 8);
        }
        u8 color_indices[16];
        color_indices[0] = (src[2]>>2)&0x1UL;
        color_indices[1] = (src[2]>>3)&0x3UL;
        color_indices[2] = (src[2]>>5)&0x3UL;
        color_indices[3] = (src[2]>>7)&0x3UL;

        color_indices[4] = (src[2]>>9)&0x3UL;
        color_indices[5] = (src[2]>>11)&0x3UL;
        color_indices[6] = (src[2]>>13)&0x3UL;
        color_indices[7] = (src[2]>>15)&0x3UL;

        color_indices[8] = (src[2]>>17)&0x3UL;
        color_indices[9] = (src[2]>>19)&0x3UL;
        color_indices[10] = (src[2]>>21)&0x3UL;
        color_indices[11] = (src[2]>>23)&0x3UL;

        color_indices[12] = (src[2]>>25)&0x3UL;
        color_indices[13] = (src[2]>>27)&0x3UL;
        color_indices[14] = (src[2]>>29)&0x3UL;
        color_indices[15] = ((src[2]>>31)&0x1UL) | ((src[3]&0x1UL)<<1);

        u8 alpha_indices[16];
        alpha_indices[0] = (src[3]>> 1)&0x1UL;
        alpha_indices[1] = (src[3]>> 2)&0x3UL;
        alpha_indices[2] = (src[3]>> 4)&0x3UL;
        alpha_indices[3] = (src[3]>> 6)&0x3UL;

        alpha_indices[4] = (src[3]>> 8)&0x3UL;
        alpha_indices[5] = (src[3]>>10)&0x3UL;
        alpha_indices[6] = (src[3]>>12)&0x3UL;
        alpha_indices[7] = (src[3]>>14)&0x3UL;

        alpha_indices[8] = (src[3]>>16)&0x3UL;
        alpha_indices[9] = (src[3]>>18)&0x3UL;
        alpha_indices[10] = (src[3]>>20)&0x3UL;
        alpha_indices[11] = (src[3]>>22)&0x3UL;

        alpha_indices[12] = (src[3]>>24)&0x3UL;
        alpha_indices[13] = (src[3]>>26)&0x3UL;
        alpha_indices[14] = (src[3]>>28)&0x3UL;
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
            dst[index+3] = interpolate2(endpoint_a[endpoint_start], endpoint_a[endpoint_end], alpha_index);

            if(0<rotation) {
                std::swap(dst[index+rotation-1], dst[index+3]);
            }
        }
    }

    void bc7_decode_mode6(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 6;

        u8 endpoint_r[2];
        u8 endpoint_g[2];
        u8 endpoint_b[2];
        u8 endpoint_a[2];
        endpoint_r[0] = (src[0]>>7)&0x7FUL;
        endpoint_r[1] = (src[0]>>14)&0x7FUL;

        endpoint_g[0] = (src[0]>>21)&0x7FUL;
        endpoint_g[1] = ((src[0]>>28)&0x0FUL) | ((src[1]&0x07UL)<<4);

        endpoint_b[0] = (src[1]>> 3)&0x7FUL;
        endpoint_b[1] = (src[1]>>10)&0x7FUL;

        endpoint_a[0] = (src[1]>>17)&0x7FUL;
        endpoint_a[1] = (src[1]>>24)&0x7FUL;

        u8 pbits[2];
        pbits[0] = (src[1]>>31) & 0x01UL;
        pbits[1] = (src[2]>> 0) & 0x01UL;

        u8 color_indices[16];
        color_indices[0] = (src[2]>>1)&0x7UL;
        color_indices[1] = (src[2]>>4)&0xFUL;
        color_indices[2] = (src[2]>>8)&0xFUL;
        color_indices[3] = (src[2]>>12)&0xFUL;

        color_indices[4] = (src[2]>>16)&0xFUL;
        color_indices[5] = (src[2]>>20)&0xFUL;
        color_indices[6] = (src[2]>>24)&0xFUL;
        color_indices[7] = (src[2]>>28)&0xFUL;

        color_indices[8] = (src[3]>> 0)&0xFUL;
        color_indices[9] = (src[3]>> 4)&0xFUL;
        color_indices[10] = (src[3]>>8)&0xFUL;
        color_indices[11] = (src[3]>>12)&0xFUL;

        color_indices[12] = (src[3]>>16)&0xFUL;
        color_indices[13] = (src[3]>>20)&0xFUL;
        color_indices[14] = (src[3]>>24)&0xFUL;
        color_indices[15] = (src[3]>>28)&0xFUL;

        u8 decoded_r[2];
        u8 decoded_g[2];
        u8 decoded_b[2];
        u8 decoded_a[2];

        for(u32 i=0; i<2; ++i){
            decoded_r[i] = dequant(endpoint_r[i], pbits[i], 7);
            decoded_g[i] = dequant(endpoint_g[i], pbits[i], 7);
            decoded_b[i] = dequant(endpoint_b[i], pbits[i], 7);
            decoded_a[i] = dequant(endpoint_a[i], pbits[i], 7);
        }

#if 0
        std::cout << "mode6:" << std::endl;
        std::cout << "  endpoints:" << std::endl;
        for(u32 i=0; i<2; ++i){
            std::cout << "    " << static_cast<u32>(endpoint_r[i]) << " " << static_cast<u32>(endpoint_g[i]) << " " << static_cast<u32>(endpoint_b[i]) << " " << static_cast<u32>(endpoint_a[i]) << std::endl;
        }
        for(u32 i=0; i<2; ++i){
            std::cout << "    " << static_cast<u32>(decoded_r[i]) << " " << static_cast<u32>(decoded_g[i]) << " " << static_cast<u32>(decoded_b[i]) << " " << static_cast<u32>(decoded_a[i]) << std::endl;
        }
        std::cout << "  pbits:" << std::endl << "    ";
        for(u32 i=0; i<2; ++i){
            std::cout << static_cast<u32>(pbits[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "  index:" << std::endl << "    ";
        for(u32 i=0; i<16; ++i){
            std::cout << static_cast<u32>(color_indices[i]) << " ";
        }
        std::cout << std::endl;
#endif

        for(u32 i=0; i<16; ++i){
            static constexpr u8 endpoint_start = 0;
            static constexpr u8 endpoint_end = 1;
            u8 color_index = color_indices[i];
            u32 index = i*4;
            dst[index+0] = interpolate4(decoded_r[endpoint_start], decoded_r[endpoint_end], color_index);
            dst[index+1] = interpolate4(decoded_g[endpoint_start], decoded_g[endpoint_end], color_index);
            dst[index+2] = interpolate4(decoded_b[endpoint_start], decoded_b[endpoint_end], color_index);
            dst[index+3] = interpolate4(decoded_a[endpoint_start], decoded_a[endpoint_end], color_index);
        }
    }

    void bc7_decode_mode7(u8 dst[64], const u32 src[4])
    {
        static constexpr u32 mode = 7;
        u32 partition_set_id = extract_partition_set_id(mode, src);

        u8 endpoint_r[4];
        u8 endpoint_g[4];
        u8 endpoint_b[4];
        u8 endpoint_a[4];
        endpoint_r[0] = (src[0]>>14)&0x1FUL;
        endpoint_r[1] = (src[0]>>19)&0x1FUL;
        endpoint_r[2] = (src[0]>>24)&0x1FUL;
        endpoint_r[3] = ((src[0]>>29)&0x7UL) | ((src[1]&0x3UL)<<3);

        endpoint_g[0] = (src[1]>>2)&0x1FUL;
        endpoint_g[1] = (src[1]>>7)&0x1FUL;
        endpoint_g[2] = (src[1]>>12)&0x1FUL;
        endpoint_g[3] = (src[1]>>19)&0x1FUL;

        endpoint_b[0] = (src[1]>>24)&0x1FUL;
        endpoint_b[1] = ((src[1]>>29)&0x7UL) | ((src[2]&0x3UL)<<3);
        endpoint_b[2] = (src[2]>>2)&0x1FUL;
        endpoint_b[3] = (src[2]>>7)&0x1FUL;

        endpoint_a[0] = (src[2]>>12)&0x1FUL;
        endpoint_a[1] = (src[2]>>19)&0x1FUL;
        endpoint_a[2] = (src[2]>>24)&0x1FUL;
        endpoint_a[3] = ((src[2]>>29)&0x7UL) | ((src[3]&0x3UL)<<3);

        u8 pbits[4];
        pbits[0] = (src[3]>>2) & 0x01UL;
        pbits[1] = (src[3]>>3) & 0x01UL;
        pbits[2] = (src[3]>>4) & 0x01UL;
        pbits[3] = (src[3]>>5) & 0x01UL;

        for(u32 i=0; i<4; ++i){
            endpoint_r[i] = dequant(endpoint_r[i], pbits[i], 5);
            endpoint_g[i] = dequant(endpoint_g[i], pbits[i], 5);
            endpoint_b[i] = dequant(endpoint_b[i], pbits[i], 5);
            endpoint_a[i] = dequant(endpoint_a[i], pbits[i], 5);
        }
        u8 color_indices[16];
        u32 offset = 6 + 32*3;
        for(u32 i = 0; i < 16; ++i){
            u32 bits = (!i) || (i==bc7_anchor_index_second_subset_two[partition_set_id])? 1 : 2;
            color_indices[i] = read_bits(offset, src, bits);
        }

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
            dst[index+3] = interpolate2(endpoint_a[endpoint_start], endpoint_a[endpoint_end], color_index);
        }
    }

    void bc7_encode_mode1(EncodeStats& encode, const u8 src[64])
    {
        u8 endpoint_r[4];
        u8 endpoint_g[4];
        u8 endpoint_b[4];
        u8 pbits[2];
        u8 decoded_r[4];
        u8 decoded_g[4];
        u8 decoded_b[4];
        u8 color_indices[16];

        u8 r_endpoint_r[4] = {};
        u8 r_endpoint_g[4] = {};
        u8 r_endpoint_b[4] = {};
        u8 r_pbits[2] = {};
        u8 r_color_indices[16] = {};

        u32 min_partition = 0;
        u32 min_error = 0xFFFF'FFFFU;
        for(u32 part=0; part<64; ++part){
            const u8* partition = bc7_partitions2[part];

            endpoint_r[0] = endpoint_g[0] = endpoint_b[0] = 0xFFU;
            endpoint_r[1] = endpoint_g[1] = endpoint_b[1] = 0x00U;
            endpoint_r[2] = endpoint_g[2] = endpoint_b[2] = 0xFFU;
            endpoint_r[3] = endpoint_g[3] = endpoint_b[3] = 0x00U;
            for(u32 i = 0; i < 16; ++i) {
                u32 index = 4 * i;
                u8 subset_index = partition[i];
                u8 endpoint_start = 2 * subset_index;
                u8 endpoint_end = 2 * subset_index + 1;
                endpoint_r[endpoint_start] = (std::min)(endpoint_r[endpoint_start], src[index + 0]);
                endpoint_g[endpoint_start] = (std::min)(endpoint_g[endpoint_start], src[index + 1]);
                endpoint_b[endpoint_start] = (std::min)(endpoint_b[endpoint_start], src[index + 2]);

                endpoint_r[endpoint_end] = (std::max)(endpoint_r[endpoint_end], src[index + 0]);
                endpoint_g[endpoint_end] = (std::max)(endpoint_g[endpoint_end], src[index + 1]);
                endpoint_b[endpoint_end] = (std::max)(endpoint_b[endpoint_end], src[index + 2]);
            }
            u32 low_count = (endpoint_r[0] & 0x1UL) + (endpoint_g[0] & 0x1UL) + (endpoint_b[0] & 0x1UL)
                            + (endpoint_r[1] & 0x1UL) + (endpoint_g[1] & 0x1UL) + (endpoint_b[1] & 0x1UL);
            u32 high_count = (endpoint_r[2] & 0x1UL) + (endpoint_g[2] & 0x1UL) + (endpoint_b[2] & 0x1UL)
                             + (endpoint_r[3] & 0x1UL) + (endpoint_g[3] & 0x1UL) + (endpoint_b[3] & 0x1UL);
            pbits[0] = low_count <= 3 ? 0 : 1;
            pbits[1] = high_count <= 3 ? 0 : 1;

            for(u32 i = 0; i < 4; ++i) {
                endpoint_r[i] >>= 1;
                endpoint_g[i] >>= 1;
                endpoint_b[i] >>= 1;
                u32 p = i >> 1;
                decoded_r[i] = dequant(endpoint_r[i], pbits[p], 6);
                decoded_g[i] = dequant(endpoint_g[i], pbits[p], 6);
                decoded_b[i] = dequant(endpoint_b[i], pbits[p], 6);
            }

            u32 error = 0;
            for(u32 i = 0; i < 16; ++i) {
                u8 subset_index = partition[i];
                u8 endpoint_start = 2 * subset_index;
                u8 endpoint_end = 2 * subset_index + 1;
                u32 index = i * 4;
                u32 bits = (!i) || (i == bc7_anchor_index_second_subset_two[part]) ? 2 : 3;
                if(bits == 3) {
                    auto [cindex, e] = deinterpolate3(
                        decoded_r[endpoint_start], decoded_r[endpoint_end], src[index + 0],
                        decoded_g[endpoint_start], decoded_g[endpoint_end], src[index + 1],
                        decoded_b[endpoint_start], decoded_b[endpoint_end], src[index + 2]);
                    color_indices[i] = cindex;
                    error += e;
                } else {
                    auto [cindex, e] = deinterpolate3_2(
                        decoded_r[endpoint_start], decoded_r[endpoint_end], src[index + 0],
                        decoded_g[endpoint_start], decoded_g[endpoint_end], src[index + 1],
                        decoded_b[endpoint_start], decoded_b[endpoint_end], src[index + 2]);
                    color_indices[i] = cindex;
                    error += e;
                }
            }
            if(error<min_error){
                min_partition = part;
                min_error = error;
                ::memcpy(r_endpoint_r, endpoint_r, 4);
                ::memcpy(r_endpoint_g, endpoint_g, 4);
                ::memcpy(r_endpoint_b, endpoint_b, 4);
                ::memcpy(r_pbits, pbits, 2);
                ::memcpy(r_color_indices, color_indices, 16);
            }
        }//for(u32 part=0;

#if 0
        std::cout << "max error: " << error << std::endl;
        std::cout << "mode1:" << std::endl;
        std::cout << "  partition: " << min_partition << std::endl;
        std::cout << "  endpoints:" << std::endl;
        for(u32 i=0; i<4; ++i){
            std::cout << "    " << static_cast<u32>(r_endpoint_r[i]) << " " << static_cast<u32>(r_endpoint_g[i]) << " " << static_cast<u32>(r_endpoint_b[i]) << std::endl;
        }
        std::cout << "  pbits:" << std::endl << "    ";
        for(u32 i=0; i<2; ++i){
            std::cout << static_cast<u32>(r_pbits[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "  index:" << std::endl << "    ";
        for(u32 i=0; i<16; ++i){
            std::cout << static_cast<u32>(r_color_indices[i]) << " ";
        }
        std::cout << std::endl;
#endif

        encode.max_error_ = min_error;
        u8* dst = encode.block_;
        ::memset(dst, 0, 16);
        dst[0] |= 0x1UL<<1;
        dst[0] |= (min_partition&0x3FUL)<<2;
        dst[1] |= (r_endpoint_r[0]&0x3FUL)<<0;
        dst[1] |= (r_endpoint_r[1]&0x03UL)<<6;
        dst[2] |= (r_endpoint_r[1]>>2)&0x0FUL;
        dst[2] |= (r_endpoint_r[2]&0x0FUL)<<4;
        dst[3] |= (r_endpoint_r[2]>>4)&0x03UL;
        dst[3] |= (r_endpoint_r[3]&0x3FUL)<<2;
        dst[4] |= (r_endpoint_g[0]&0x3FUL)<<0;
        dst[4] |= (r_endpoint_g[1]&0x03UL)<<6;
        dst[5] |= (r_endpoint_g[1]>>2)&0x0FUL;
        dst[5] |= (r_endpoint_g[2]&0x0FUL)<<4;
        dst[6] |= (r_endpoint_g[2]>>4)&0x03UL;
        dst[6] |= (r_endpoint_g[3]&0x3FUL)<<2;
        dst[7] |= (r_endpoint_b[0]&0x3FUL)<<0;
        dst[7] |= (r_endpoint_b[1]&0x03UL)<<6;
        dst[8] |= (r_endpoint_b[1]>>2)&0x0FUL;
        dst[8] |= (r_endpoint_b[2]&0x0FUL)<<4;
        dst[9] |= (r_endpoint_b[2]>>4)&0x03UL;
        dst[9] |= (r_endpoint_b[3]&0x3FUL)<<2;


        dst[10] |= r_pbits[0]<<0;
        dst[10] |= r_pbits[1]<<1;

        u32 offset = 2 + 8*10;
        for(u32 i = 0; i < 16; ++i) {
            u32 bits = (!i) || (i == bc7_anchor_index_second_subset_two[min_partition]) ? 2 : 3;
            write_bits(offset, dst, r_color_indices[i], bits);
        }
    }

    void bc7_encode_mode6(EncodeStats& encode, const u8 src[64])
    {
        u8 endpoint_r[2];
        u8 endpoint_g[2];
        u8 endpoint_b[2];
        u8 endpoint_a[2];

        endpoint_r[0] = src[0];
        endpoint_g[0] = src[1];
        endpoint_b[0] = src[2];
        endpoint_a[0] = src[3];

        endpoint_r[1] = src[0];
        endpoint_g[1] = src[1];
        endpoint_b[1] = src[2];
        endpoint_a[1] = src[3];

        for(u32 i=1; i<16; ++i){
            u32 index = i*4;
            endpoint_r[0] = (std::min)(endpoint_r[0], src[index+0]);
            endpoint_g[0] = (std::min)(endpoint_g[0], src[index+1]);
            endpoint_b[0] = (std::min)(endpoint_b[0], src[index+2]);
            endpoint_a[0] = (std::min)(endpoint_a[0], src[index+3]);

            endpoint_r[1] = (std::max)(endpoint_r[1], src[index+0]);
            endpoint_g[1] = (std::max)(endpoint_g[1], src[index+1]);
            endpoint_b[1] = (std::max)(endpoint_b[1], src[index+2]);
            endpoint_a[1] = (std::max)(endpoint_a[1], src[index+3]);
        }
        u32 low_count = (endpoint_r[0] & 0x1UL) + (endpoint_g[0] & 0x1UL) + (endpoint_b[0] & 0x1UL) + (endpoint_a[0] & 0x1UL);
        u32 high_count = (endpoint_r[1] & 0x1UL) + (endpoint_g[1] & 0x1UL) + (endpoint_b[1] & 0x1UL) + (endpoint_a[1] & 0x1UL);

        u8 pbits[2];
        pbits[0] = low_count<=2? 0 : 1;
        pbits[1] = high_count<=2? 0 : 1;

        for(u32 i=0; i<2; ++i){
            endpoint_r[i] >>= 1;
            endpoint_g[i] >>= 1;
            endpoint_b[i] >>= 1;
            endpoint_a[i] >>= 1;
        }

        u8 decoded_r[2];
        u8 decoded_g[2];
        u8 decoded_b[2];
        u8 decoded_a[2];
        for(u32 i=0; i<2; ++i){
            decoded_r[i] = dequant(endpoint_r[i], pbits[i], 7);
            decoded_g[i] = dequant(endpoint_g[i], pbits[i], 7);
            decoded_b[i] = dequant(endpoint_b[i], pbits[i], 7);
            decoded_a[i] = dequant(endpoint_a[i], pbits[i], 7);
        }

        u8 color_indices[16];
        u32 error=0;
        for(u32 i=0; i<1; ++i){
            u32 index = i*4;
            auto [cindex, e] = deinterpolate4_3(
                decoded_r[0], decoded_r[1], src[index+0],
                decoded_g[0], decoded_g[1], src[index+1],
                decoded_b[0], decoded_b[1], src[index+2],
                decoded_a[0], decoded_a[1], src[index+3]);
            color_indices[i] = cindex;
            error += e;
            assert(color_indices[i]<8);
        }
        for(u32 i=1; i<16; ++i){
            u32 index = i*4;
            auto [cindex, e] = deinterpolate4(
                decoded_r[0], decoded_r[1], src[index+0],
                decoded_g[0], decoded_g[1], src[index+1],
                decoded_b[0], decoded_b[1], src[index+2],
                decoded_a[0], decoded_a[1], src[index+3]);
            color_indices[i] = cindex;
            error += e;
            assert(color_indices[i]<16);
        }

#if 0
        std::cout << "max error: " << error << std::endl;
        std::cout << "mode6:" << std::endl;
        std::cout << "  endpoints:" << std::endl;
        for(u32 i=0; i<2; ++i){
            std::cout << "    " << static_cast<u32>(endpoint_r[i]) << " " << static_cast<u32>(endpoint_g[i]) << " " << static_cast<u32>(endpoint_b[i]) << " " << static_cast<u32>(endpoint_a[i]) << std::endl;
        }
        for(u32 i=0; i<2; ++i){
            std::cout << "    " << static_cast<u32>(decoded_r[i]) << " " << static_cast<u32>(decoded_g[i]) << " " << static_cast<u32>(decoded_b[i]) << " " << static_cast<u32>(decoded_a[i]) << std::endl;
        }
        std::cout << "  pbits:" << std::endl << "    ";
        for(u32 i=0; i<2; ++i){
            std::cout << static_cast<u32>(pbits[i]) << " ";
        }
        std::cout << std::endl;

        std::cout << "  index:" << std::endl << "    ";
        for(u32 i=0; i<16; ++i){
            std::cout << static_cast<u32>(color_indices[i]) << " ";
        }
        std::cout << std::endl;
#endif
        encode.max_error_ = error;
        u8* dst = encode.block_;
        ::memset(dst, 0, 16);
        dst[0] |= 0x1UL<<6;
        dst[0] |= (endpoint_r[0]&0x01UL)<<7;
        dst[1] |= (endpoint_r[0]>>1)&0x3FUL;
        dst[1] |= (endpoint_r[1]&0x03UL)<<6;
        dst[2] |= (endpoint_r[1]>>2)&0x1FUL;
        dst[2] |= (endpoint_g[0]&0x07UL)<<5;
        dst[3] |= (endpoint_g[0]>>3)&0x0FUL;
        dst[3] |= (endpoint_g[1]&0x0FUL)<<4;
        dst[4] |= (endpoint_g[1]>>4)&0x07UL;
        dst[4] |= (endpoint_b[0]&0x1FUL)<<3;
        dst[5] |= (endpoint_b[0]>>5)&0x03UL;
        dst[5] |= (endpoint_b[1]&0x3FUL)<<2;
        dst[6] |= (endpoint_b[1]>>6)&0x01UL;
        dst[6] |= (endpoint_a[0]&0x7FUL)<<1;
        dst[7] |= (endpoint_a[1]>>0)&0x7FUL;

        dst[7] |= (pbits[0]<<7);
        dst[8] |= (pbits[1]<<0);

        dst[8] |= (color_indices[0]&0x7UL)<<1;
        dst[8] |= (color_indices[1]&0xFUL)<<4;
        dst[9] |= (color_indices[2]&0xFUL)<<0;
        dst[9] |= (color_indices[3]&0xFUL)<<4;
        dst[10] |= (color_indices[4]&0xFUL)<<0;
        dst[10] |= (color_indices[5]&0xFUL)<<4;
        dst[11] |= (color_indices[6]&0xFUL)<<0;
        dst[11] |= (color_indices[7]&0xFUL)<<4;

        dst[12] |= (color_indices[8]&0xFUL)<<0;
        dst[12] |= (color_indices[9]&0xFUL)<<4;
        dst[13] |= (color_indices[10]&0xFUL)<<0;
        dst[13] |= (color_indices[11]&0xFUL)<<4;
        dst[14] |= (color_indices[12]&0xFUL)<<0;
        dst[14] |= (color_indices[13]&0xFUL)<<4;
        dst[15] |= (color_indices[14]&0xFUL)<<0;
        dst[15] |= (color_indices[15]&0xFUL)<<4;
    }

    
}

void bc7_decode_block(u8 dst[64], const u8 src[16])
{
    u32 packed[4] = {};
    packed[0] = static_cast<u32>(src[0]) | (static_cast<u32>(src[1])<<8) | (static_cast<u32>(src[2])<<16) | (static_cast<u32>(src[3])<<24);
    packed[1] = static_cast<u32>(src[4]) | (static_cast<u32>(src[5])<<8) | (static_cast<u32>(src[6])<<16) | (static_cast<u32>(src[7])<<24);
    packed[2] = static_cast<u32>(src[8]) | (static_cast<u32>(src[9])<<8) | (static_cast<u32>(src[10])<<16) | (static_cast<u32>(src[11])<<24);
    packed[3] = static_cast<u32>(src[12]) | (static_cast<u32>(src[13])<<8) | (static_cast<u32>(src[14])<<16) | (static_cast<u32>(src[15])<<24);
    u8 mode = extract_mode(src[0]);
    switch(mode){
    case 0:
        bc7_decode_mode0(dst, packed);
        break;
    case 1:
        bc7_decode_mode1(dst, packed);
        break;
    case 2:
        bc7_decode_mode2(dst, packed);
        break;
    case 3:
        bc7_decode_mode3(dst, packed);
        break;
    case 4:
        bc7_decode_mode4(dst, packed);
        break;
    case 5:
        bc7_decode_mode5(dst, packed);
        break;
    case 6:
        bc7_decode_mode6(dst, packed);
        break;
    case 7:
        bc7_decode_mode7(dst, packed);
        break;
    }
}

void bc7_encode_block(u8 dst[16], const bc7param& param, const u8 src[64])
{
    bool hasAlpha = false;
    for(u32 i=3; i<64; i+=4){
        if(src[i]<param.max_alpha_){
            hasAlpha = true;
        }
    }
    if(hasAlpha){
        EncodeStats encode6;
        bc7_encode_mode6(encode6, src);
        ::memcpy(dst, encode6.block_, 16);
    }else{
        EncodeStats encode1;
        bc7_encode_mode1(encode1, src);
        ::memcpy(dst, encode1.block_, 16);

        //EncodeStats encode6;
        //bc7_encode_mode6(encode6, src);
        //if(encode1.max_error_<encode6.max_error_){
        //    ::memcpy(dst, encode1.block_, 16);
        //}else{
        //    ::memcpy(dst, encode6.block_, 16);
        //}
    }
}
} // namespace cppbc

