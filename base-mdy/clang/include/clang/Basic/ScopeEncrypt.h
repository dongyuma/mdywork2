/*!
 * FileName: ScopeEncrypt.h
 *
 * Author:   ZhangChaoZe
 * Date:     2022-4-18
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: scope加密算法
 */

#include <stdint.h>
namespace clang
{
namespace scope
{
namespace encrypt
{

//Binary <-> HexString encrypt decrypt
const char *scope_k_encrypt_hex(const char *plain_data, size_t data_size, const uint8_t key[16]);
const char *scope_k_decrypt_hex(const char *ciphers_data_hex_str, size_t *out_size, const uint8_t key[16]);
const char *scope_encrypt_hex(const char *plain_data, size_t data_size);
const char *scope_decrypt_hex(const char *ciphers_data_hex_str, size_t *out_size);

//Binary <-> Binary encrypy decrypt
const char *scope_k_encrypt_binary(const char *plain_data, size_t in_size, size_t *out_size, const uint8_t key[16]);
const char *scope_k_decrypt_binary(const char *ciphers_data, size_t in_size, size_t *out_size, const uint8_t key[16]);
const char *scope_encrypt_binary(const char *plain_data, size_t in_size, size_t *out_size);
const char *scope_decrypt_binary(const char *ciphers_data, size_t in_size, size_t *out_size);

void scope_free(const void *ptr);

} //end encrypt ns
} //end encrypt scope
} //end encrypt clang