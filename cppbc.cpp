#include "cppbc.h"
#include <cassert>
#include <cstring>
#include <utility>
#include <tuple>
#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#include <fstream>
#endif

namespace cppbc
{
namespace
{
#if defined(_MSC_VER)
#    define BC7_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#    define BC7_RESTRICT __restrict__
#else
#    define BC7_RESTRICT
#endif

    static constexpr f32 Epsilon = 1.0e-6f;
    static constexpr f32 AlmostZero = 1.0e-20f;
    f32 square(f32 x)
    {
        return x*x;
    }

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

    const u16 aWeights2[] = {0, 21, 43, 64};
    const u16 aWeights3[] = {0, 9, 18, 27, 37, 46, 55, 64};
    const u16 aWeights4[] = {0, 4, 9, 13, 17, 21, 26, 30, 34, 38, 43, 47, 51, 55, 60, 64};
    // clang-format on

    u8 interpolate(u8 e0, u8 e1, u8 index, u8 index_bits);

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

    struct FRGBA
    {
        f32 r_;
        f32 g_;
        f32 b_;
        f32 a_;

        const f32& operator[](u32 c) const
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

        f32& operator[](u32 c)
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

        FRGBA& operator*=(f32 x)
        {
            r_ *= x;
            g_ *= x;
            b_ *= x;
            a_ *= x;
            return *this;
        }
    };

    FRGBA operator+(const FRGBA& x0, const FRGBA& x1)
    {
        return {x0.r_+x1.r_, x0.g_+x1.g_, x0.b_+x1.b_, x0.a_+x1.a_};
    }

    FRGBA operator-(const FRGBA& x0, const FRGBA& x1)
    {
        return {x0.r_-x1.r_, x0.g_-x1.g_, x0.b_-x1.b_, x0.a_-x1.a_};
    }

    FRGBA operator*(const FRGBA& x0, f32 x1)
    {
        return {x0.r_*x1, x0.g_*x1, x0.b_*x1, x0.a_*x1};
    }

    FRGBA operator*(f32 x0, const FRGBA& x1)
    {
        return {x0*x1.r_, x0*x1.g_, x0*x1.b_, x0*x1.a_};
    }

    f32 dot(const FRGBA& x0, const FRGBA& x1)
    {
        return x0.r_*x1.r_ + x0.g_*x1.g_ + x0.b_*x1.b_ + x0.a_*x1.a_;
    }

    FRGBA normalize(const FRGBA& x)
    {
        f32 l = x.r_*x.r_ + x.g_*x.g_ + x.b_*x.b_ + x.a_*x.a_;
        if(l < Epsilon) {
            return {0.0f, 0.0f, 0.0f, 0.0f};
        }
        f32 inv = 1.0f / l;
        return {x.r_ * inv, x.g_ * inv, x.b_ * inv, x.a_ * inv};
    }

    struct FRGB
    {
        f32 r_;
        f32 g_;
        f32 b_;

        const f32& operator[](u32 c) const
        {
            switch(c){
            case 0:
                return r_;
            case 1:
                return g_;
            default:
                return b_;
            }
        }

        f32& operator[](u32 c)
        {
            switch(c){
            case 0:
                return r_;
            case 1:
                return g_;
            default:
                return b_;
            }
        }

        FRGB& operator*=(f32 x)
        {
            r_ *= x;
            g_ *= x;
            b_ *= x;
            return *this;
        }
    };
    FRGB operator+(const FRGB& x0, const FRGB& x1)
    {
        return {x0.r_+x1.r_, x0.g_+x1.g_, x0.b_+x1.b_};
    }

    FRGB operator-(const FRGB& x0, const FRGB& x1)
    {
        return {x0.r_-x1.r_, x0.g_-x1.g_, x0.b_-x1.b_};
    }

    struct CompressionStatus
    {
        u32 mode_;
        u8 color_bits_;
        u8 index_bits_;
        bool has_alpha_;
        bool has_pbits_;
        bool share_pbits_;
        u32 num_pixels_;
        RGBA pixels_[16];
    };

    struct CompressionResult
    {
        u64 error_;
        RGBA low_endpoint_;
        RGBA high_endpoint_;
        u8 pbits_[2];
        u8 indices_[16];
        u8 alpha_indices_[16];
    };

    /**
     * @brief Kahan's summation
     */
    FRGBA kahan(u32 size, const FRGBA* points)
    {
        FRGBA sum = {};
        FRGBA error = {};
        for(u32 i = 0; i < size; ++i) {
            FRGBA y = points[i] - error;
            FRGBA t = sum + y;
            error = (t - sum) - y;
            sum = t;
        }
        return sum;
    }

    /**
     * @brief Kahan's summation
     */
    void kahan(f32* BC7_RESTRICT sum, f32* BC7_RESTRICT error, f32 x)
    {
        f32 y = x - *error;
        f32 t = *sum + y;
        *error = (t - *sum) - y;
        *sum = t;
    }

    /**
     * @brief Kahan's summation
     */
    void kahan_rgb(f32 covariance[9], u32 size, const FRGBA* points, const FRGBA& average)
    {
        f32 errors[6] = {};
        for(u32 i = 0; i < size; ++i) {
            f32 dr = points[i].r_ - average.r_;
            f32 dg = points[i].g_ - average.g_;
            f32 db = points[i].b_ - average.b_;
            kahan(&covariance[0], &errors[0], dr * dr);
            kahan(&covariance[1], &errors[1], dr * dg);
            kahan(&covariance[2], &errors[2], dr * db);

            kahan(&covariance[4], &errors[3], dg * dg);
            kahan(&covariance[5], &errors[4], dg * db);

            kahan(&covariance[8], &errors[5], db * db);
        }
    }

    /**
     * @brief Kahan's summation
     */
    void kahan_rgba(f32 covariance[16], u32 size, const FRGBA* points, const FRGBA& average)
    {
        f32 errors[10] = {};
        for(u32 i = 0; i < size; ++i) {
            f32 dr = points[i].r_ - average.r_;
            f32 dg = points[i].g_ - average.g_;
            f32 db = points[i].b_ - average.b_;
            f32 da = points[i].a_ - average.a_;
            kahan(&covariance[0], &errors[0], dr * dr);
            kahan(&covariance[1], &errors[1], dr * dg);
            kahan(&covariance[2], &errors[2], dr * db);
            kahan(&covariance[3], &errors[3], dr * da);

            kahan(&covariance[5], &errors[4], dg * dg);
            kahan(&covariance[6], &errors[5], dg * db);
            kahan(&covariance[7], &errors[6], dg * da);

            kahan(&covariance[10], &errors[7], db * db);
            kahan(&covariance[11], &errors[8], db * da);

            kahan(&covariance[15], &errors[9], da * da);
        }
    }

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

    void rotate(f32 m[9], u32 i, u32 j, u32 k, u32 l, f32 cs, f32 sn)
    {
        f32 t0 = m[i * 3 + j];
        f32 t1 = m[k * 3 + l];
        m[i * 3 + j] = t0 * cs - t1 * sn;
        m[k * 3 + l] = t0 * sn + t1 * cs;
    }

    bool jacobi(f32* M, f32* N, u32 size)
    {
        const u32 Size = size;
        static constexpr u32 MaxIteration = 100;
        ::memset(N, 0, sizeof(f32) * 9);
        f32 cs;
        f32 sn = 0.0f;
        f32 offdiag = 0.0f;
        for(u32 i = 0; i < Size; ++i) {
            N[i * Size + i] = 1.0f;
            u32 t = i * Size + i;
            sn += M[t] * M[t];
            for(u32 j = i + 1; j < Size; ++j) {
                t = i * Size + j;
                offdiag += M[t] * M[t];
            }
        } // for(u32 i
        f32 tolerance = Epsilon * Epsilon * (sn * 0.5f + offdiag);
        u32 iteration = 0;
        for(; iteration < MaxIteration; ++iteration) {
            offdiag = 0.0f;
            for(u32 i = 0; i < Size - 1; ++i) {
                for(u32 j = i + 1; j < Size; ++j) {
                    u32 t = i * Size + j;
                    offdiag += M[t] * M[t];
                }
            }
            if(offdiag < tolerance) {
                break;
            }
            for(u32 i = 0; i < Size - 1; ++i) {
                for(u32 j = i + 1; j < Size; ++j) {
                    if(::fabsf(M[i * Size + j]) < AlmostZero) {
                        continue;
                    }
                    f32 t = (M[j * Size + j] - M[i * Size + i]) / (2.0f * M[i * Size + j]);
                    if(0.0f <= t) {
                        t = 1.0f / (t + ::sqrtf(t * t + 1.0f));
                    } else {
                        t = 1.0f / (t - ::sqrtf(t * t + 1.0f));
                    }
                    cs = 1.0f / ::sqrtf(t * t + 1.0f);
                    sn = t * cs;
                    t *= M[i * Size + j];
                    M[i * Size + i] -= t;
                    M[j * Size + j] += t;
                    M[i * Size + j] = 0.0f;
                    for(u32 k = 0; k < i; ++k) {
                        rotate(M, k, i, k, j, cs, sn);
                    }
                    for(u32 k = i + 1; k < j; ++k) {
                        rotate(M, i, k, k, j, cs, sn);
                    }
                    for(u32 k = j + 1; k < Size; ++k) {
                        rotate(M, i, k, j, k, cs, sn);
                    }
                    for(u32 k = 0; k < Size; ++k) {
                        rotate(N, i, k, j, k, cs, sn);
                    }
                } // for(u32 j
            }     // for(u32 i
        }         // for(u32 iteration
        return (iteration < MaxIteration);
    }

        bool PCA_RGB(FRGBA& center, FRGBA& axis, FRGBA& minP, FRGBA& maxP, const CompressionStatus& status)
{
        FRGBA colors[16];
        for(u32 i=0; i<status.num_pixels_; ++i){
            colors[i].r_ = 1.0f/255.0f * status.pixels_[i].r_;
            colors[i].g_ = 1.0f/255.0f * status.pixels_[i].g_;
            colors[i].b_ = 1.0f/255.0f * status.pixels_[i].b_;
            colors[i].a_ = 1.0f;
        }

    FRGBA average = kahan(status.num_pixels_, colors);
    f32 invSize = 1.0f / status.num_pixels_;
    average *= invSize;

    f32 covariance[9] = {};
    kahan_rgb(covariance, status.num_pixels_, colors, average);
    covariance[0] *= invSize;
    covariance[1] *= invSize;
    covariance[2] *= invSize;
    covariance[4] *= invSize;
    covariance[5] *= invSize;
    covariance[8] *= invSize;

    covariance[3] = covariance[1];
    covariance[6] = covariance[2];
    covariance[7] = covariance[5];

    f32 N[9];
    if(!jacobi(covariance, N, 3)) {
        return false;
    }
    FRGBA axises[3];
    axises[0] = normalize({N[0], N[1], N[2], 1.0f});
    axises[1] = normalize({N[3], N[4], N[5], 1.0f});
    axises[2] = normalize({N[6], N[7], N[8],1.0f});
    f32 d0 = dot(axises[0], average);
    f32 d1 = dot(axises[1], average);
    f32 d2 = dot(axises[2], average);
    minP = {};
    maxP = {};
    for(u32 i = 0; i < status.num_pixels_; ++i) {
        f32 dr = dot(axises[0], colors[i]) - d0;
        f32 dg = dot(axises[1], colors[i]) - d1;
        f32 db = dot(axises[2], colors[i]) - d2;
        minP.r_ = (std::min)(dr, minP.r_);
        minP.g_ = (std::min)(dg, minP.g_);
        minP.b_ = (std::min)(db, minP.b_);

        maxP.r_ = (std::max)(dr, maxP.r_);
        maxP.g_ = (std::max)(dg, maxP.g_);
        maxP.b_ = (std::max)(db, maxP.b_);
    }
    f32 half[3];
    half[0] = (maxP.r_ - minP.r_) * 0.5f;
    half[1] = (maxP.g_ - minP.g_) * 0.5f;
    half[2] = (maxP.b_ - minP.b_) * 0.5f;
    u32 maxIndex = 0;
    for(u32 i=1; i<3; ++i){
        if(half[maxIndex]<half[i]){
            maxIndex = i;
        }
    }
    FRGBA d = (minP + maxP) * 0.5f;
    center = average + axises[0] * d.r_ + axises[1] * d.g_ + axises[2] * d.b_;
    axis = axises[maxIndex];
    minP = center - axis*half[maxIndex];
    maxP = center + axis*half[maxIndex];
    center.a_ = 1.0f;
    minP.a_ = 1.0f;
    maxP.a_ = 1.0f;
    return true;
}

    bool PCA_RGBA(FRGBA& center, FRGBA& axis, FRGBA& minP, FRGBA& maxP, const CompressionStatus& status)
{
        FRGBA colors[16];
        for(u32 i=0; i<status.num_pixels_; ++i){
            colors[i].r_ = 1.0f/255.0f * status.pixels_[i].r_;
            colors[i].g_ = 1.0f/255.0f * status.pixels_[i].g_;
            colors[i].b_ = 1.0f/255.0f * status.pixels_[i].b_;
            colors[i].a_ = 1.0f/255.0f * status.pixels_[i].a_;
        }
    FRGBA average = kahan(status.num_pixels_, colors);
    f32 invSize = 1.0f / status.num_pixels_;
    average *= invSize;

    f32 covariance[16] = {};
    kahan_rgba(covariance, status.num_pixels_, colors, average);
    covariance[0] *= invSize;
    covariance[1] *= invSize;
    covariance[2] *= invSize;
    covariance[3] *= invSize;
    covariance[5] *= invSize;
    covariance[6] *= invSize;
    covariance[7] *= invSize;
    covariance[10] *= invSize;
    covariance[11] *= invSize;
    covariance[15] *= invSize;

    covariance[4] = covariance[1];
    covariance[8] = covariance[2];
    covariance[9] = covariance[6];
    covariance[12] = covariance[3];
    covariance[13] = covariance[7];
    covariance[14] = covariance[11];

    f32 N[16];
    if(!jacobi(covariance, N, 4)) {
        return false;
    }
    FRGBA axises[4];
    axises[0] = normalize({N[0], N[1], N[2], N[3]});
    axises[1] = normalize({N[4], N[5], N[6], N[7]});
    axises[2] = normalize({N[8], N[9], N[10], N[11]});
    axises[3] = normalize({N[12], N[13], N[14], N[15]});
    f32 d0 = dot(axises[0], average);
    f32 d1 = dot(axises[1], average);
    f32 d2 = dot(axises[2], average);
    f32 d3 = dot(axises[3], average);
    minP = {};
    maxP = {};
    for(u32 i = 0; i < status.num_pixels_; ++i) {
        f32 dr = dot(axises[0], colors[i]) - d0;
        f32 dg = dot(axises[1], colors[i]) - d1;
        f32 db = dot(axises[2], colors[i]) - d2;
        f32 da = dot(axises[3], colors[i]) - d3;
        minP.r_ = (std::min)(dr, minP.r_);
        minP.g_ = (std::min)(dg, minP.g_);
        minP.b_ = (std::min)(db, minP.b_);
        minP.a_ = (std::min)(da, minP.a_);

        maxP.r_ = (std::max)(dr, maxP.r_);
        maxP.g_ = (std::max)(dg, maxP.g_);
        maxP.b_ = (std::max)(db, maxP.b_);
        maxP.a_ = (std::max)(da, maxP.a_);
    }
    f32 half[4];
    half[0] = (maxP.r_ - minP.r_) * 0.5f;
    half[1] = (maxP.g_ - minP.g_) * 0.5f;
    half[2] = (maxP.b_ - minP.b_) * 0.5f;
    half[3] = (maxP.a_ - minP.a_) * 0.5f;
    u32 maxIndex = 0;
    for(u32 i=1; i<4; ++i){
        if(half[maxIndex]<half[i]){
            maxIndex = i;
        }
    }
    FRGBA d = (minP + maxP) * 0.5f;
    center = average + axises[0] * d.r_ + axises[1] * d.g_ + axises[2] * d.b_ + axises[3] * d.a_;
    axis = axises[maxIndex];
    minP = center - axis*half[maxIndex];
    maxP = center + axis*half[maxIndex];
    return true;
}

    void findMinMax(FRGBA& axis, FRGBA& minP, FRGBA& maxP, const CompressionStatus& status)
{
        assert(0<status.num_pixels_);
        minP.r_ = 1.0f / 255.0f * status.pixels_[0].r_;
        minP.g_ = 1.0f / 255.0f * status.pixels_[0].g_;
        minP.b_ = 1.0f / 255.0f * status.pixels_[0].b_;
        minP.a_ = 1.0f / 255.0f * status.pixels_[0].a_;
        maxP.r_ = minP.r_;
        maxP.g_ = minP.g_;
        maxP.b_ = minP.b_;
        maxP.a_ = minP.a_;
        for(u32 i = 1; i < status.num_pixels_; ++i) {
            FRGBA color;
            color.r_ = 1.0f/255.0f * status.pixels_[i].r_;
            color.g_ = 1.0f/255.0f * status.pixels_[i].g_;
            color.b_ = 1.0f/255.0f * status.pixels_[i].b_;
            color.a_ = 1.0f/255.0f * status.pixels_[i].a_;
            minP.r_ = (std::min)(minP.r_, color.r_);
            minP.g_ = (std::min)(minP.g_, color.g_);
            minP.b_ = (std::min)(minP.b_, color.b_);
            minP.a_ = (std::min)(minP.a_, color.a_);
            maxP.r_ = (std::max)(maxP.r_, color.r_);
            maxP.g_ = (std::max)(maxP.g_, color.g_);
            maxP.b_ = (std::max)(maxP.b_, color.b_);
            maxP.a_ = (std::max)(maxP.a_, color.a_);
        }
        axis = maxP-minP;
        axis = normalize(axis);
    }

    RGBA scale_color(const RGBA& c, u32 color_bits, u32 pbits)
{
	RGBA result;

	const u32 n = color_bits + pbits;
	assert(4 <= n && n <= 8);

	for (u32 i = 0; i < 4; ++i)
	{
		u32 v = c[i] << (8 - n);
		v |= (v >> n);
		assert(v <= 255);
		result[i] = static_cast<u8>(v);
	}

	return result;
}

    RGBA toRGBA(f32 r, f32 g, f32 b, f32 a)
    {
        s32 ir = static_cast<s32>(r);
        s32 ig = static_cast<s32>(g);
        s32 ib = static_cast<s32>(b);
        s32 ia = static_cast<s32>(a);
        RGBA rgba;
        rgba.r_ = static_cast<u8>(std::clamp(ir, 0, 255));
        rgba.g_ = static_cast<u8>(std::clamp(ig, 0, 255));
        rgba.b_ = static_cast<u8>(std::clamp(ib, 0, 255));
        rgba.a_ = static_cast<u8>(std::clamp(ia, 0, 255));
        return rgba;
    }

    void toYUV(f32& y, f32& u, f32& v, const RGBA& x)
{
        y = 0.2126f*x.r_ + 0.7152f*x.g_ + 0.0722f*x.b_;
        u = -0.114572f*x.r_ - 0.385428f*x.g_ + 0.5f*x.b_;
        v = 0.5f*x.r_ - 0.454153f*x.g_ - 0.045847f*x.b_;
    }

    u64 distance(const RGBA& x0, const RGBA& x1)
{
        static const f32 weights[] = {8.0f, 4.0f, 1.0f, 2.0f};
        f32 y0,u0,v0;
        f32 y1,u1,v1;
        toYUV(y0, u0, v0, x0);
        toYUV(y1, u1, v1, x1);
        f32 dy = y0-y1;
        f32 du = u0-u1;
        f32 dv = v0-v1;
        f32 da = static_cast<f32>(x1.a_) - static_cast<f32>(x0.a_);
        f32 d = weights[0]*(dy*dy) + weights[1]*(du*du) + weights[2]*(dv*dv) + weights[3]*(da*da);
        return static_cast<u64>(d);
}

void update_error(
    CompressionResult& result,
    const CompressionStatus& status,
    const RGBA& low,
    const RGBA& high,
    const u8 pbits[2])
{
    RGBA endpoint_start;
    RGBA endpoint_end;

	if(status.has_pbits_){

        u32 min_pbit;
        u32 max_pbit;
        if(status.share_pbits_){
            min_pbit = max_pbit = pbits[0];
        }else{
            min_pbit = pbits[0];
            max_pbit = pbits[1];
        }

		endpoint_start[0] = static_cast<u8>((low[0] << 1) | min_pbit);
		endpoint_start[1] = static_cast<u8>((low[1] << 1) | min_pbit);
		endpoint_start[2] = static_cast<u8>((low[2] << 1) | min_pbit);
		endpoint_start[3] = static_cast<u8>((low[3] << 1) | min_pbit);

		endpoint_end[0] = static_cast<u8>((high[0] << 1) | max_pbit);
		endpoint_end[1] = static_cast<u8>((high[1] << 1) | max_pbit);
		endpoint_end[2] = static_cast<u8>((high[2] << 1) | max_pbit);
		endpoint_end[3] = static_cast<u8>((high[3] << 1) | max_pbit);
	}else{
        endpoint_start = low;
        endpoint_end = high;
    }

	RGBA minColor = scale_color(endpoint_start, status.color_bits_, status.has_pbits_?1:0);
	RGBA maxColor = scale_color(endpoint_end, status.color_bits_, status.has_pbits_?1:0);

    const u8 N = 1<<status.index_bits_;
	RGBA weightedColors[16];
	weightedColors[0] = minColor;
	weightedColors[N-1] = maxColor;

	const u32 num_components = status.has_alpha_? 4 : 3;
	for (u8 i = 1; i < (N - 1); ++i){
		for (u32 j = 0; j < num_components; ++j){
            weightedColors[i][j] = interpolate(minColor[j], maxColor[j], i, status.index_bits_);
        }
    }

    u8 indices[16] = {};
	u64 total_error = 0;
    for(u32 i=0; i<status.num_pixels_; ++i){
        u64 min_error = std::numeric_limits<u64>::max();
        u32 min_index = 0;
        for(u32 j=0; j<N; ++j){
            u64 error = distance(weightedColors[j], status.pixels_[i]);
            if(error<min_error){
                min_error = error;
                min_index = j;
            }
        }
        total_error += min_error;
        indices[i] = static_cast<u8>(min_index);
    }

    if(total_error<result.error_){
        result.error_ = total_error;
        result.low_endpoint_ = low;
        result.high_endpoint_ = high;
        result.pbits_[0] = pbits[0];
        result.pbits_[1] = pbits[1];
        ::memcpy(result.indices_, indices, status.num_pixels_);
    }
}

void fixDegenerateEndpoints(RGBA& minColor, RGBA& maxColor, const FRGBA& low, const FRGBA& high, u32 iscale)
{
    for(u32 i = 0; i < 3; ++i) {
        if(minColor[i] != maxColor[i]) {
            continue;
        }
        if(std::abs(high[i] - low[i]) <= 0.0f) {
            continue;
        }
        if(minColor[i] > (iscale >> 1)) {
            if(0<minColor[i]){
                minColor[i] -= 1;
            }else if(maxColor[i] < iscale){
                maxColor[i] += 1;
            }
        } else {
            if(maxColor[i] < iscale){
                maxColor[i] += 1;
            }else if(0<minColor[i]){
                minColor[i] -= 1;
            }
        }
    }
}

u64 search_optimal_with_pbits(
    CompressionResult& result,
    const CompressionStatus& status,
    const FRGBA& minColor,
    const FRGBA& maxColor)
{
    const u32 num_components = 4;
    const s32 iscalep = (1 << (status.color_bits_ + 1)) - 1;
    const f32 scalep = static_cast<f32>(iscalep);

    u8 best_pbits[2];
    RGBA bestMinColor, bestMaxColor;

    f32 best_err0 = std::numeric_limits<f32>::infinity();
    f32 best_err1 = std::numeric_limits<f32>::infinity();

    for(u8 p = 0; p < 2; ++p) {
        RGBA xMinColor, xMaxColor;
        for(u8 c = 0; c < 4; ++c) {
            xMinColor[c] = static_cast<u8>(std::clamp(static_cast<s32>((minColor[c] * scalep - p) * 0.5f + 0.5f) * 2 + p, (s32)p, iscalep - 1 + p));
            xMaxColor[c] = static_cast<u8>(std::clamp(static_cast<s32>((maxColor[c] * scalep - p) * 0.5f + 0.5f) * 2 + p, (s32)p, iscalep - 1 + p));
        }

        RGBA scaledLow = scale_color(xMinColor, status.color_bits_, 1);
        RGBA scaledHigh = scale_color(xMaxColor, status.color_bits_, 1);

        f32 err0 = 0.0f;
        f32 err1 = 0.0f;
        for(u32 i = 0; i < num_components; ++i) {
            err0 += square(scaledLow[i] - minColor[i] * 255.0f);
            err1 += square(scaledHigh[i] - maxColor[i] * 255.0f);
        }

        if(err0 < best_err0) {
            best_err0 = err0;
            best_pbits[0] = p;
            bestMinColor[0] = xMinColor[0] >> 1;
            bestMinColor[1] = xMinColor[1] >> 1;
            bestMinColor[2] = xMinColor[2] >> 1;
            bestMinColor[3] = xMinColor[3] >> 1;
        }

        if(err1 < best_err1) {
            best_err1 = err1;
            best_pbits[1] = p;
            bestMaxColor[0] = xMaxColor[0] >> 1;
            bestMaxColor[1] = xMaxColor[1] >> 1;
            bestMaxColor[2] = xMaxColor[2] >> 1;
            bestMaxColor[3] = xMaxColor[3] >> 1;
        }
    }

    if(1 == status.mode_) {
        fixDegenerateEndpoints(bestMinColor, bestMaxColor, minColor, maxColor, iscalep >> 1);
    }

    update_error(result, status, bestMinColor, bestMaxColor, best_pbits);

    return result.error_;
}

u64 search_optimal_with_share_pbits(
    CompressionResult& result,
    const CompressionStatus& status,
        const FRGBA& minColor,
        const FRGBA& maxColor)
{
		const s32 iscalep = (1 << (status.color_bits_ + 1)) - 1;
		const f32 scalep = static_cast<f32>(iscalep);

		const u32 num_components = status.has_alpha_ ? 4 : 3;

		u8 best_pbits[2];
		RGBA bestMinColor, bestMaxColor;

        f32 min_error = std::numeric_limits<f32>::infinity();
        for(u8 p = 0; p < 2; ++p) {
            RGBA xMinColor, xMaxColor;
            for(u8 c = 0; c < 4; ++c) {
                xMinColor[c] = static_cast<u8>(std::clamp(static_cast<s32>((minColor[c] * scalep - p) * 0.5f + 0.5f) * 2 + p, (s32)p, iscalep - 1 + p));
                xMaxColor[c] = static_cast<u8>(std::clamp(static_cast<s32>((maxColor[c] * scalep - p) * 0.5f + 0.5f) * 2 + p, (s32)p, iscalep - 1 + p));
            }
            RGBA scaledLow = scale_color(xMinColor, status.color_bits_, 1);
            RGBA scaledHigh = scale_color(xMaxColor, status.color_bits_, 1);

            f32 error = 0.0f;
            for(u32 i = 0; i < num_components; ++i) {
                error += square(scaledLow[i] / 255.0f - minColor[i]) + square(scaledHigh[i] / 255.0f - maxColor[i]);
            }
            if(error < min_error) {
                min_error = error;
                best_pbits[0] = p;
                best_pbits[1] = p;
                for(u32 j = 0; j < 4; ++j) {
                    bestMinColor[j] = xMinColor[j] >> 1;
                    bestMaxColor[j] = xMaxColor[j] >> 1;
                }
            }
        }

        if(1 == status.mode_) {
            fixDegenerateEndpoints(bestMinColor, bestMaxColor, minColor, maxColor, iscalep >> 1);
        }
        update_error(result, status, bestMinColor, bestMaxColor, best_pbits);
        return result.error_;
}

u64 search_optimal(
    CompressionResult& result,
    const CompressionStatus& status,
    const FRGBA& minColor,
    const FRGBA& maxColor)
{
    const s32 iscalep = (1 << (status.color_bits_ + 1)) - 1;
    const f32 scalep = static_cast<f32>(iscalep);

    RGBA xMinColor = toRGBA(minColor[0] * scalep + 0.5f, minColor[1] * scalep + 0.5f, minColor[2] * scalep + 0.5f, minColor[3] * scalep + 0.5f);
    RGBA xMaxColor = toRGBA(maxColor[0] * scalep + 0.5f, maxColor[1] * scalep + 0.5f, maxColor[2] * scalep + 0.5f, maxColor[3] * scalep + 0.5f);

    if(1 == status.mode_) {
        fixDegenerateEndpoints(xMinColor, xMaxColor, minColor, maxColor, iscalep);
    }
    update_error(result, status, xMinColor, xMaxColor, result.pbits_);
    return result.error_;
}

void least_squares_endpoints_rgba(FRGBA& low, FRGBA& high, const CompressionStatus& status, const u8* indices)
{
    //  w*w w(1-w)            lr = wcr
    //  (1-w) (1-w)(1-w)      hr   (1-w)cr
    const u16* weights;
    switch(status.index_bits_){
    case 2:
        weights = aWeights2;
        break;
    case 3:
        weights = aWeights3;
        break;
    case 4:
        weights = aWeights4;
        break;
    default:
        return;
    }
    f32 a[4] = {};
    f32 br[2] = {};
    f32 bg[2] = {};
    f32 bb[2] = {};
    f32 ba[2] = {};
    for(u32 i=0; i<status.num_pixels_; ++i){
        u32 n = indices[i];
        f32 w = 1.0f/64.0f * weights[n];
        f32 iw = 1.0f-w;
        a[0] += (w*w);
        a[1] += iw*w;
        a[3] += iw*iw;
        br[0] += w*status.pixels_[i].r_;
        br[1] += iw*status.pixels_[i].r_;
        bg[0] += w*status.pixels_[i].g_;
        bg[1] += iw*status.pixels_[i].g_;
        bb[0] += w*status.pixels_[i].b_;
        bb[1] += iw*status.pixels_[i].b_;
        ba[0] += w*status.pixels_[i].a_;
        ba[1] += iw*status.pixels_[i].a_;
    }
    a[2] = a[1];
    f32 determinant = a[0]*a[3] - a[1]*a[2];
    if(Epsilon<std::abs(determinant)){
        determinant = 1.0f/determinant;
    }
    f32 ia[4];
    ia[0] = a[3]*determinant;
    ia[1] = -a[1]*determinant;
    ia[2] = -a[2]*determinant;
    ia[3] = a[0]*determinant;

    low.r_ = (ia[0]*br[0] + ia[1]*br[1]);
    low.g_ = (ia[0]*bg[0] + ia[1]*bg[1]);
    low.b_ = (ia[0]*bb[0] + ia[1]*bb[1]);
    low.a_ = (ia[0]*ba[0] + ia[1]*ba[1]);

    high.r_ = (ia[2]*br[0] + ia[3]*br[1]);
    high.g_ = (ia[2]*bg[0] + ia[3]*bg[1]);
    high.b_ = (ia[2]*bb[0] + ia[3]*bb[1]);
    high.a_ = (ia[2]*ba[0] + ia[3]*ba[1]);
}

void least_squares_endpoints_rgb(FRGBA& low, FRGBA& high, const CompressionStatus& status, const u8* indices)
{
    //  w*w w(1-w)            lr = wcr
    //  (1-w)w (1-w)(1-w)      hr   (1-w)cr
    const u16* weights;
    switch(status.index_bits_){
    case 2:
        weights = aWeights2;
        break;
    case 3:
        weights = aWeights3;
        break;
    case 4:
        weights = aWeights4;
        break;
    default:
        return;
    }
    f32 a[4] = {};
    f32 br[2] = {};
    f32 bg[2] = {};
    f32 bb[2] = {};
    for(u32 i=0; i<status.num_pixels_; ++i){
        u32 n = indices[i];
        f32 w = 1.0f/64.0f * weights[n];
        f32 iw = 1.0f-w;
        a[0] += (w*w);
        a[1] += iw*w;
        a[3] += iw*iw;
        br[0] += w*status.pixels_[i].r_;
        br[1] += iw*status.pixels_[i].r_;
        bg[0] += w*status.pixels_[i].g_;
        bg[1] += iw*status.pixels_[i].g_;
        bb[0] += w*status.pixels_[i].b_;
        bb[1] += iw*status.pixels_[i].b_;
    }
    a[2] = a[1];
    f32 determinant = a[0]*a[3] - a[1]*a[2];
    if(Epsilon<std::abs(determinant)){
        determinant = 1.0f/determinant;
    }
    f32 ia[4];
    ia[0] = a[3]*determinant;
    ia[1] = -a[1]*determinant;
    ia[2] = -a[2]*determinant;
    ia[3] = a[0]*determinant;

    low.r_ = (ia[0]*br[0] + ia[1]*br[1]);
    low.g_ = (ia[0]*bg[0] + ia[1]*bg[1]);
    low.b_ = (ia[0]*bb[0] + ia[1]*bb[1]);
    low.a_ = 1.0f;

    high.r_ = (ia[2]*br[0] + ia[3]*br[1]);
    high.g_ = (ia[2]*bg[0] + ia[3]*bg[1]);
    high.b_ = (ia[2]*bb[0] + ia[3]*bb[1]);
    high.a_ = 1.0f;
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

    u8 interpolate(u8 e0, u8 e1, u8 index, u8 index_bits)
    {
        switch(index_bits) {
        case 2:
            return interpolate2(e0, e1, index);
        case 3:
            return interpolate3(e0, e1, index);
        case 4:
            return interpolate4(e0, e1, index);
        default:
            return 0;
        }
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
        const u32 weight_bits[2] = { index_mode ? 3UL : 2UL,  index_mode ? 2UL : 3UL };
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
        u8 min_partition = 0;
        u8 endpoint_r[4];
        u8 endpoint_g[4];
        u8 endpoint_b[4];
        u8 pbits[2];
        u8 color_indices[16] = {};

        CompressionStatus status;
        status.mode_ = 1;
        status.color_bits_ = 6;
        status.index_bits_ = 3;
        status.has_alpha_ = false;
        status.has_pbits_ = true;
        status.share_pbits_ = true;

        FRGBA center, axis, low, high;
        for(u32 sub = 0; sub < 2; ++sub) {
            //for(u32 part = 0; part < 64; ++part) {
            for(u32 part = 0; part < 1; ++part) {
                const u8* partition = bc7_partitions2[part];
                u32 count = 0;
                for(u32 i = 0; i < 16; ++i) {
                    u8 subset_index = partition[i];
                    if(sub != subset_index) {
                        continue;
                    }
                    u32 index = 4 * i;
                    status.pixels_[count].r_ = src[index + 0];
                    status.pixels_[count].g_ = src[index + 1];
                    status.pixels_[count].b_ = src[index + 2];
                    status.pixels_[count].a_ = src[index + 3];
                    ++count;
                }
                status.num_pixels_ = count;
                CompressionResult result = {};
                result.error_ = std::numeric_limits<u64>::max();
                if(!PCA_RGB(center, axis, low, high, status)) {
                    findMinMax(axis, low, high, status);
                }

                if(search_optimal_with_share_pbits(result, status, low, high)) {
                    least_squares_endpoints_rgb(low, high, status, result.indices_);
                    CompressionResult result2 = {};
                    result2.error_ = std::numeric_limits<u64>::max();
                    search_optimal_with_share_pbits(result2, status, low, high);
                    if(result2.error_<result.error_){
                        result = result2;
                    }
                }
                assert(result.pbits_[0] == result.pbits_[1]);
                u32 sub_index = 2*sub;
                endpoint_r[sub_index+0] = result.low_endpoint_.r_;
                endpoint_g[sub_index+0] = result.low_endpoint_.g_;
                endpoint_b[sub_index+0] = result.low_endpoint_.b_;
                endpoint_r[sub_index+1] = result.high_endpoint_.r_;
                endpoint_g[sub_index+1] = result.high_endpoint_.g_;
                endpoint_b[sub_index+1] = result.high_endpoint_.b_;
                pbits[sub] = result.pbits_[0];
                count = 0;
                for(u32 i = 0; i < 16; ++i) {
                    u8 subset_index = partition[i];
                    if(sub != subset_index) {
                        continue;
                    }
                    color_indices[i] = result.indices_[count];
                    ++count;
                }

            }
        }

        {
            u8* dst = encode.block_;
            ::memset(dst, 0, 16);
            dst[0] |= 0x1UL << 1;
            dst[0] |= (min_partition & 0x3FUL) << 2;
            dst[1] |= (endpoint_r[0] & 0x3FUL) << 0;
            dst[1] |= (endpoint_r[1] & 0x03UL) << 6;
            dst[2] |= (endpoint_r[1] >> 2) & 0x0FUL;
            dst[2] |= (endpoint_r[2] & 0x0FUL) << 4;
            dst[3] |= (endpoint_r[2] >> 4) & 0x03UL;
            dst[3] |= (endpoint_r[3] & 0x3FUL) << 2;
            dst[4] |= (endpoint_g[0] & 0x3FUL) << 0;
            dst[4] |= (endpoint_g[1] & 0x03UL) << 6;
            dst[5] |= (endpoint_g[1] >> 2) & 0x0FUL;
            dst[5] |= (endpoint_g[2] & 0x0FUL) << 4;
            dst[6] |= (endpoint_g[2] >> 4) & 0x03UL;
            dst[6] |= (endpoint_g[3] & 0x3FUL) << 2;
            dst[7] |= (endpoint_b[0] & 0x3FUL) << 0;
            dst[7] |= (endpoint_b[1] & 0x03UL) << 6;
            dst[8] |= (endpoint_b[1] >> 2) & 0x0FUL;
            dst[8] |= (endpoint_b[2] & 0x0FUL) << 4;
            dst[9] |= (endpoint_b[2] >> 4) & 0x03UL;
            dst[9] |= (endpoint_b[3] & 0x3FUL) << 2;

            dst[10] |= pbits[0] << 0;
            dst[10] |= pbits[1] << 1;

            u32 offset = 2 + 8 * 10;
            for(u32 i = 0; i < 16; ++i) {
                u32 bits = (!i) || (i == bc7_anchor_index_second_subset_two[min_partition]) ? 2 : 3;
                write_bits(offset, dst, color_indices[i], bits);
            }
        }
#if 0
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
    #endif
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

void bc7_initialize()
{
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

