#ifndef SECURE_H_
#define SECURE_H_
#include<stdint.h>
#include <openssl/aes.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include <openssl/md5.h>
//#include "sm3.h"
//#include "sm4.h"


#define INVALID_NULL_VALUE_INPUT    0x1000
#define INVALID_INPUT_LENGTH        0x1001
#define CREATE_SM2_KEY_PAIR_FAIL    0x1002
#define COMPUTE_SM3_DIGEST_FAIL     0x1003
#define ALLOCATION_MEMORY_FAIL      0x1004
#define COMPUTE_SM2_SIGNATURE_FAIL  0x1005
#define INVALID_SM2_SIGNATURE       0x1006
#define VERIFY_SM2_SIGNATURE_FAIL   0x1007

typedef struct sm2_key_pair_structure {
	unsigned char pri_key[64];
	unsigned char pub_key[64];
}SM2_KEY_PAIR;

class CSecure {
public:
	CSecure();
	~CSecure();

    int AesEcbEncrypt(const unsigned char *key, int keylen, unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen=0);
    int AesEcbDecrypt(const unsigned char *key, int keylen, unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen=0);
	int AesCbcEncrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen);
	int AesCbcDecrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen);
	//rsa
	int RsaEncrypt();
	int RsaDecrypt();
	int RsaSign();
	int RsaVerifySign();

	int Sha256();

	//sm2
	int Sm2CreateKey(SM2_KEY_PAIR *sm2Key);
	int Sm2Encrypt();
	int Sm2Decrypt();
	int Sm2Sign();
	int Sm2VerifySign();

	//sm3
	int Sm3Hash(unsigned char *src, int srclen, unsigned char *hash);
	//sm4
	int Sm4EcbEncrypt(unsigned char *key, int keylen, const unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen);
	int Sm4EcbDecrypt(unsigned char *key, int keylen, const unsigned char *cipher, unsigned int cipherlen,unsigned char *plain, int plainlen);
	int Sm4CbcEncrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen);
	int Sm4CbcDecrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen);

    //MD5
    int GenMd5(uint8_t *src,uint32_t srclen,uint8_t *dst);
protected:

private:

};


#endif
