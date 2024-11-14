#ifndef INC_CPPBC_H_
#define INC_CPPBC_H_
#include <cstdint>

namespace cppbc
{
	using s8 = int8_t;
	using s16 = int16_t;
	using s32 = int32_t;
	using s64 = int64_t;

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using f32 = float;
	using f64 = double;

struct bc7param
{
};

bool bc7_encode_block(u8 dst[16], u32 width, u32 height, const bc7param& param, const u8 src[64]);

} // namespace cppbc
#endif //INC_CPPBC_H_
