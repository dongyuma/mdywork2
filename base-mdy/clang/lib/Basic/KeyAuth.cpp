/*!
 * FileName: KeyAuth.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-7-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: clang clang-tidy key正确性检测
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <stddef.h>

#include <set>
#include <regex>
#include <fstream>


#include "clang/Basic/KeyAuth.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Process.h"

namespace clang
{
	namespace keyauth
	{
// CBC mode, for memset
#ifndef CBC
#define CBC 1
#endif
#ifndef AES128
#define AES128 1
#endif
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
#if defined(AES256) && (AES256 == 1)
#define Nk 8
#define Nr 14
#elif defined(AES192) && (AES192 == 1)
#define Nk 6
#define Nr 12
#else
#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.
#endif
#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only
#if defined(AES256) && (AES256 == 1)
#define AES_KEYLEN 32
#define AES_keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
#define AES_KEYLEN 24
#define AES_keyExpSize 208
#else
#define AES_KEYLEN 16   // Key length in bytes
#define AES_keyExpSize 176
#endif
// jcallan@github points out that declaring Multiply as a function 
// reduces code size considerably with the Keil ARM compiler.
#ifndef MULTIPLY_AS_A_FUNCTION
#define MULTIPLY_AS_A_FUNCTION 0
#endif
/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
		typedef uint8_t state_t[4][4];
		struct AES_ctx
		{
			uint8_t RoundKey[AES_keyExpSize];
			uint8_t Iv[AES_BLOCKLEN];
		};

		// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
		// The numbers below can be computed dynamically trading ROM for RAM - 
		// This can be useful in (embedded) bootloader applications, where ROM is often limited.
		static const uint8_t sbox[256] = {
			//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
			0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
			0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
			0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
			0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
			0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
			0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
			0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
			0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
			0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
			0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
			0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
			0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
			0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
			0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
			0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
			0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
		static const uint8_t rsbox[256] = {
		  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
		// The round constant word array, Rcon[i], contains the values given by 
		// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
		static const uint8_t Rcon[11] = {
		  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
#define getSBoxValue(num) (sbox[(num)])
		// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
		static void KeyExpansion(uint8_t *RoundKey, const uint8_t *Key)
		{
			unsigned i, j, k;
			uint8_t tempa[4]; // Used for the column/row operations

			// The first round key is the key itself.
			for (i = 0; i < Nk; ++i)
			{
				RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
				RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
				RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
				RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
			}

			// All other round keys are found from the previous round keys.
			for (i = Nk; i < Nb * (Nr + 1); ++i)
			{
				{
					k = (i - 1) * 4;
					tempa[0] = RoundKey[k + 0];
					tempa[1] = RoundKey[k + 1];
					tempa[2] = RoundKey[k + 2];
					tempa[3] = RoundKey[k + 3];

				}

				if (i % Nk == 0)
				{
					// This function shifts the 4 bytes in a word to the left once.
					// [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

					// Function RotWord()
					{
						const uint8_t u8tmp = tempa[0];
						tempa[0] = tempa[1];
						tempa[1] = tempa[2];
						tempa[2] = tempa[3];
						tempa[3] = u8tmp;
					}

					// SubWord() is a function that takes a four-byte input word and 
					// applies the S-box to each of the four bytes to produce an output word.

					// Function Subword()
					{
						tempa[0] = getSBoxValue(tempa[0]);
						tempa[1] = getSBoxValue(tempa[1]);
						tempa[2] = getSBoxValue(tempa[2]);
						tempa[3] = getSBoxValue(tempa[3]);
					}

					tempa[0] = tempa[0] ^ Rcon[i / Nk];
				}
#if defined(AES256) && (AES256 == 1)
				if (i % Nk == 4)
				{
					// Function Subword()
					{
						tempa[0] = getSBoxValue(tempa[0]);
						tempa[1] = getSBoxValue(tempa[1]);
						tempa[2] = getSBoxValue(tempa[2]);
						tempa[3] = getSBoxValue(tempa[3]);
					}
				}
#endif
				j = i * 4; k = (i - Nk) * 4;
				RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
				RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
				RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
				RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
			}
		}
		static void AES_init_ctx_iv(struct AES_ctx *ctx, const uint8_t *key, const uint8_t *iv)
		{
			KeyExpansion(ctx->RoundKey, key);
			memcpy(ctx->Iv, iv, AES_BLOCKLEN);
		}
		// This function adds the round key to state.
		// The round key is added to the state by an XOR function.
		static void AddRoundKey(uint8_t round, state_t *state, const uint8_t *RoundKey)
		{
			uint8_t i, j;
			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
				{
					(*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
				}
			}
		}
		static uint8_t xtime(uint8_t x)
		{
			return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
		}
		// Multiply is used to multiply numbers in the field GF(2^8)
		// Note: The last call to xtime() is unneeded, but often ends up generating a smaller binary
		//       The compiler seems to be able to vectorize the operation better this way.
		//       See https://github.com/kokke/tiny-AES-c/pull/34
#if MULTIPLY_AS_A_FUNCTION
		static uint8_t Multiply(uint8_t x, uint8_t y)
		{
			return (((y & 1) * x) ^
				((y >> 1 & 1) * xtime(x)) ^
				((y >> 2 & 1) * xtime(xtime(x))) ^
				((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
				((y >> 4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
		}
#else
#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif
#define getSBoxInvert(num) (rsbox[(num)])
// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
		static void InvMixColumns(state_t *state)
		{
			int i;
			uint8_t a, b, c, d;
			for (i = 0; i < 4; ++i)
			{
				a = (*state)[i][0];
				b = (*state)[i][1];
				c = (*state)[i][2];
				d = (*state)[i][3];

				(*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
				(*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
				(*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
				(*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
			}
		}
		// The SubBytes Function Substitutes the values in the
		// state matrix with values in an S-box.
		static void InvSubBytes(state_t *state)
		{
			uint8_t i, j;
			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
				{
					(*state)[j][i] = getSBoxInvert((*state)[j][i]);
				}
			}
		}
		static void InvShiftRows(state_t *state)
		{
			uint8_t temp;

			// Rotate first row 1 columns to right  
			temp = (*state)[3][1];
			(*state)[3][1] = (*state)[2][1];
			(*state)[2][1] = (*state)[1][1];
			(*state)[1][1] = (*state)[0][1];
			(*state)[0][1] = temp;

			// Rotate second row 2 columns to right 
			temp = (*state)[0][2];
			(*state)[0][2] = (*state)[2][2];
			(*state)[2][2] = temp;

			temp = (*state)[1][2];
			(*state)[1][2] = (*state)[3][2];
			(*state)[3][2] = temp;

			// Rotate third row 3 columns to right
			temp = (*state)[0][3];
			(*state)[0][3] = (*state)[1][3];
			(*state)[1][3] = (*state)[2][3];
			(*state)[2][3] = (*state)[3][3];
			(*state)[3][3] = temp;
		}
		static void InvCipher(state_t *state, const uint8_t *RoundKey)
		{
			uint8_t round = 0;

			// Add the First round key to the state before starting the rounds.
			AddRoundKey(Nr, state, RoundKey);

			// There will be Nr rounds.
			// The first Nr-1 rounds are identical.
			// These Nr rounds are executed in the loop below.
			// Last one without InvMixColumn()
			for (round = (Nr - 1); ; --round)
			{
				InvShiftRows(state);
				InvSubBytes(state);
				AddRoundKey(round, state, RoundKey);
				if (round == 0) {
					break;
				}
				InvMixColumns(state);
			}

		}
		static void XorWithIv(uint8_t *buf, const uint8_t *Iv)
		{
			uint8_t i;
			for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
			{
				buf[i] ^= Iv[i];
			}
		}
		static uint8_t *AES_CBC_decrypt_buffer(struct AES_ctx *ctx, const uint8_t *data, size_t *data_length)
		{
			size_t i = 0, length = 0;
			uint8_t *buf = NULL, *buff = NULL, *res = NULL;
			uint8_t storeNextIv[AES_BLOCKLEN];
			uint64_t *plain_len_ptr;
			uint64_t plain_len = 0;
			if (data == NULL || data_length == NULL || *data_length < AES_BLOCKLEN)
			{
				return NULL;
			}
			if (*data_length % AES_BLOCKLEN != 0)
			{
				return NULL;
			}
			buff = (uint8_t *)calloc(*data_length, sizeof(uint8_t));
			if (buff == NULL)
			{
				return NULL;
			}
			buf = buff;
			length = *data_length;
			memcpy(buf, data, *data_length);
			for (i = 0; i < length; i += AES_BLOCKLEN)
			{
				memcpy(storeNextIv, buf, AES_BLOCKLEN);
				InvCipher((state_t *)buf, ctx->RoundKey);
				XorWithIv(buf, ctx->Iv);
				memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
				buf += AES_BLOCKLEN;
			}
			plain_len_ptr = (uint64_t *)buff;
			if (plain_len_ptr[1] != 0x19931022 || plain_len_ptr[0] == 0)
			{
				free(buff);
				return NULL;
			}
			plain_len = plain_len_ptr[0];
			if (plain_len >= *data_length)
			{
				free(buff);
				return NULL;
			}
			res = (uint8_t *)calloc(plain_len, sizeof(uint8_t));
			if (res == NULL)
			{
				free(buff);
				return NULL;
			}
			memcpy(res, buff + (2 * sizeof(uint64_t)), plain_len);
			free(buff);
			*data_length = plain_len;
			return res;
		}
		char *AES_cbc_128_decrypt_hex_str(const char *ciphers_data_hex_str, const uint8_t key[16])
		{
			int i = 0;
			char *res = NULL;
			char *plaintext = NULL;
			size_t ciphertext_len = 0, size = 0;
			uint8_t iv[] = { 0x93, 0x19, 0x09, 0x17, 0x15, 0x20, 0x22, 0x29, 0x10, 0x68, 0x69, 0x15, 0xec, 0xcd, 0xef, 0x78 };
			struct AES_ctx ctx;
			unsigned char *ciphers_data = NULL;
			if (ciphers_data_hex_str == NULL)
			{
				return NULL;
			}
			size = strlen(ciphers_data_hex_str);
			if (size == 0 || size % 2 != 0)
			{
				return NULL;
			}
			for (i = 0; i < (int)size; i++)
			{
				unsigned int ch = ciphers_data_hex_str[i];
				if (ch < '0' || ch > '9')
				{
					ch = toupper(ch);
					if (ch < 'A' || ch>'F')
					{
						return NULL;
					}
				}
			}
			AES_init_ctx_iv(&ctx, key, iv);
			ciphers_data = (unsigned char *)calloc(size / 2, sizeof(unsigned char));
			for (i = 0; i < (int)(size / 2); i++)
			{
				char buff[128];
				unsigned int ch;
				memset(buff, 0, sizeof(buff));
				memcpy(buff, ciphers_data_hex_str + (i * 2), 2);
				sscanf(buff, "%x", &ch);
				*(ciphers_data + i) = (unsigned char)ch;
			}
			ciphertext_len = size / 2;
			plaintext = (char *)AES_CBC_decrypt_buffer(&ctx, ciphers_data, &ciphertext_len);
			free(ciphers_data);
			if (plaintext == NULL)
			{
				return NULL;
			}
			res = (char *)calloc(ciphertext_len + 1, sizeof(char));
			memcpy(res, plaintext, ciphertext_len);
			free(plaintext);
			return res;
		}
		uint8_t __k__[] = { 0x10, 0x09, 0x57, 0x7f, 0x88, 0xae, 0xdc, 0x3f, 0xcf, 0xfe, 0xdf, 0x73, 0xe9, 0x85, 0x44, 0x3b };

		class StrCmpToRegex
		{
		public:
			StrCmpToRegex(const std::string &_str)
			{
				str = _str;
			}

			bool operator()(const std::string &regex_str)
			{
				bool res = false;
				std::regex r(regex_str);
				if (std::regex_match(str, r))
				{
					res = true;
				}
				return res;
			}
		private:
			std::string str;
		};
		//************************************
		// Method:    FindExistMatch
		// FullName:  FindExistMatch
		// Access:    public 
		// Returns:   std::vector<std::string>
		// Qualifier: 在sub中查找存在/不存在于regex_universal中的
		// Parameter: std::vector<std::string> regex_universal 正则表达式全集
		// Parameter: std::vector<std::string> sub 被查找元素集合
		// Parameter: bool exists  true-返回存在于regex_universal的   false-返回不存在于regex_universal的
		//************************************
		static std::vector<std::string> FindRegexMatch(std::vector<std::string> regex_universal, std::vector<std::string> sub, bool exists)
		{
			std::vector<std::string> res;
			for (std::string sub_i : sub)
			{
				if (std::find_if(regex_universal.begin(), regex_universal.end(), StrCmpToRegex(sub_i)) != regex_universal.end())
				{
					if (exists)
					{
						res.push_back(sub_i);
					}
				}
				else
				{
					if (!exists)
					{
						res.push_back(sub_i);
					}
				}
			}
			return res;
		}

		void CheckEnabledCheckerList(const std::vector<std::string> &enabled_list, const std::string &cip_path, const std::string &tool_name)
		{
#ifndef CLANG_BUILD_WITH_KEY_AUTH
			return;
#else
#if (CLANG_BUILD_WITH_KEY_AUTH != 1)
			return;
#endif	
#endif
			std::vector<std::string> check_list;
			std::string key_uuid;
			std::string env_uuid;

			if (cip_path.empty())
			{
				fprintf(stderr, "Key files are missing,please contact your software provider.\n");
				exit(1);
			}

			std::ifstream in;
			in.open(cip_path, std::ios::binary | std::ios::in);
			if (!in.is_open())
			{
				fprintf(stderr, "Key files are missing,please contact your software provider.\n");
				exit(1);
			}
			std::string file_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			in.close();

			std::string plain_str;

			char *plain_buff = AES_cbc_128_decrypt_hex_str(file_str.c_str(), __k__);
			if (plain_buff == NULL)
			{
				fprintf(stderr, "Key files corrupted,please contact your software provider.\n");
				exit(1);
			}
			plain_str = plain_buff;
			free(plain_buff);

			llvm::Expected<llvm::json::Value> E = llvm::json::parse(plain_str);
			if (!(E && E->kind() == llvm::json::Value::Object))
			{
				fprintf(stderr, "Key files corrupted,please contact your software provider.\n");
				exit(1);
			}
			llvm::json::Value j_value = E.get();
			llvm::json::Object *j_object = j_value.getAsObject();
			key_uuid = j_object->getString("release-uuid").getValue().str();
			if (key_uuid.empty())
			{
				fprintf(stderr, "Key files corrupted,please contact your software provider.\n");
				exit(1);
			}
			if (key_uuid == "CHECKS_INSIDE_PREVIEW_RELEASE")
			{
				return;
			}

			llvm::Optional<std::string> uuid_env = llvm::sys::Process::GetEnv("CHECKSCOPE_UUID");
			if (uuid_env)
			{
				env_uuid = uuid_env.getValue();
			}
			if (env_uuid != key_uuid)
			{
				fprintf(stderr, "Key files corrupted,please contact your software provider.\n");
				exit(1);
			}

			llvm::json::Array *j_array = j_object->getArray(tool_name);
			if (j_array == NULL)
			{
				fprintf(stderr, "Key files corrupted,please contact your software provider.\n");
				exit(1);
			}

			llvm::json::Array::iterator iter = j_array->begin();
			for (; iter != j_array->end(); iter++)
			{
				std::string check_regex = iter->getAsString().getValue().str();
				if (!check_regex.empty())
				{
					check_list.push_back(check_regex);
				}
			}

			std::vector<std::string> enable_regex_list, disable_regex_list;
			for (std::string regex_i : check_list)
			{
				if (regex_i.empty())
				{
					continue;
				}
				if (regex_i[0] == '-')
				{
					disable_regex_list.push_back(regex_i.substr(1));
				}
				else
				{
					if (regex_i[0] == '+')
					{
						enable_regex_list.push_back(regex_i.substr(1));
					}
					else
					{
						enable_regex_list.push_back(regex_i);
					}
				}
			}
			//查找不在enable列表中的已启用checker
			std::vector<std::string> not_in_enabled_regex = FindRegexMatch(enable_regex_list, enabled_list, false);

			//查找在disabled列表中的已启用checker
			std::vector<std::string> in_disabled_regex = FindRegexMatch(disable_regex_list, enabled_list, true);

			int illegal_count = not_in_enabled_regex.size() + in_disabled_regex.size();

			if (illegal_count > 0)
			{
				fprintf(stderr, "The following checks are not licensed, please contact your software provider.\n");
				std::set<std::string> illegal_set;
				for (std::string i : not_in_enabled_regex)
				{
					illegal_set.insert(i);
				}

				for (std::string i : in_disabled_regex)
				{
					illegal_set.insert(i);
				}

				int print_count = 0;
				for (std::string illegal_str : illegal_set)
				{
					if (print_count % 4 == 0)
					{
						fprintf(stderr, "%s%s", print_count == 0 ? "" : "\n", illegal_str.c_str());
					}
					else
					{
						fprintf(stderr, ",%s", illegal_str.c_str());
					}
					print_count++;
				}
				fprintf(stderr, "\n");
				exit(1);
			}
		}
	}//end keyauth ns
}//end clang ns