#include"Secure.h"


CSecure::CSecure()
{

}

CSecure::~CSecure()
{

}

int CSecure::AesEcbEncrypt(const unsigned char *key, int keylen, unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen)
{
	if (NULL == key || NULL == plain || NULL == cipher) {
		return -1;
	}
	int bits = 128;
	int offset = 16;
	if (keylen == 16) {
		bits = 128;
		offset = 16;
	}
	if (keylen == 32) {
		bits = 256;
		offset = 32;
	}

	AES_KEY aes_enc_ctx;
	int ret = AES_set_encrypt_key(key, bits, &aes_enc_ctx);
    int n = (plainlen+15) / offset;
	for (int i = 0; i< n; ++i)
	{
		AES_ecb_encrypt(plain + i * offset, cipher + i * offset,  &aes_enc_ctx, AES_ENCRYPT);
	}

    return n*offset;
}

int CSecure::AesEcbDecrypt(const unsigned char *key, int keylen, unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen)
{
	if (NULL == key || NULL == plain || NULL == cipher) {
		return -1;
	}
	int bits = 128;
	int offset = 16;
	if (keylen == 16) {
		bits = 128;
		offset = 16;
	}
	if (keylen == 32) {
		bits = 256;
		offset = 32;
	}

	AES_KEY aes_enc_ctx;
	int ret = AES_set_decrypt_key(key, bits, &aes_enc_ctx);
    int n = (cipherlen+15) / offset;
	printf("ecb n=%d,offset=%d,cipherlen=%d\n",n,offset, cipherlen);

	for (int i = 0; i< n; ++i)
	{
		AES_ecb_encrypt(cipher + i * offset, plain + i * offset, &aes_enc_ctx, AES_DECRYPT);
	}
    return n*offset;
}

int CSecure::AesCbcEncrypt(const unsigned char *key,int keylen,unsigned char *iv,int ivLen,unsigned char *plain,int plainlen,unsigned char *cipher,unsigned int cipherlen)
{
	if (NULL == key || NULL == iv || NULL == plain || NULL == cipher) {
		return -1;
	}
	int bits = 128;
	int offset = 16;
	if (keylen == 16) {
		bits = 128;
		offset = 16;
	}
	if (keylen == 32) {
		bits = 256;
		offset = 32;
	}
	
	AES_KEY aes_enc_ctx;
	int ret = AES_set_encrypt_key(key, bits, &aes_enc_ctx);
	int n = plainlen /offset;
	for (int i = 0; i< n; ++i)
	{
		AES_cbc_encrypt(plain + i * offset, cipher + i * offset, offset, &aes_enc_ctx, iv, AES_ENCRYPT);
	}
	return 0;
}

int CSecure::AesCbcDecrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen,unsigned char *cipher, unsigned int cipherlen,unsigned char *plain, int plainlen)
{
	if (NULL == key || NULL == iv || NULL == plain || NULL == cipher) {
		return -1;
	}
	int bits = 128;
	int offset = 16;
	if (keylen == 16) {
		bits = 128;
		offset = 16;
	}
	if (keylen == 32) {
		bits = 256;
		offset = 32;
	}

	AES_KEY aes_enc_ctx;
	int ret = AES_set_decrypt_key(key, bits, &aes_enc_ctx);
	int n = cipherlen / offset;
	for (int i = 0; i< n; ++i)
	{
		AES_cbc_encrypt(plain + i * offset, cipher + i * offset, offset, &aes_enc_ctx, iv, AES_DECRYPT);
	}
	return 0;
}

//rsa
int CSecure::RsaEncrypt()
{
	return 0;
}
int CSecure::RsaDecrypt()
{
	return 0;
}

int CSecure::RsaSign()
{
	return 0;
}
int CSecure::RsaVerifySign()
{
	return 0;
}

int CSecure::Sha256()
{
	return 0;
}

//sm2
int CSecure::Sm2CreateKey(SM2_KEY_PAIR *sm2Key)
{
#if 0
	int error_code;
	BN_CTX* ctx = NULL;    //�洢�����������ʱ����
	BIGNUM* bn_d = NULL, *bn_x = NULL, *bn_y = NULL;//bn_d?? ,����x,y
	const BIGNUM* bn_order; //��
	EC_GROUP* group = NULL; //��Բ����
	EC_POINT* ec_pt = NULL;//��׼�����
	unsigned char pub_key_x[32], pub_key_y[32];

	error_code = ALLOCATION_MEMORY_FAIL;  //�����ڴ�ռ�ʧ��

	if (!(ctx = BN_CTX_secure_new())) //��ʼ��һ��BIGNUM�ṹ��
	{                         //BN_CTX_secure_new()��̬���䲢��ʼ��һ�� BIGNUM �ṹ�塣���ִ�гɹ���
		//����ֵΪָ�� BIGNUM �ṹ���ָ�룬��� BIGNUM ��ֵ����Ϊ 0�����ִ��ʧ���򷵻� NULL ��
		goto clear_up;
	}
	BN_CTX_start(ctx);        //���Ҫ��ȡ BN_CTX �ṹ���е���ʱ BIGNUM �������������£�
	bn_d = BN_CTX_get(ctx);   //�ȵ��� BN_CTX_start()���ٵ��� BN_CTX_get() ��ȡ��ʱ BIGNUM ������
	bn_x = BN_CTX_get(ctx);   //���Զ�ε��� BN_CTX_get()�������� BN_CTX_end()��
	bn_y = BN_CTX_get(ctx);    //���ù� BN_CTX_end() �� BN_CTX �ṹ���е� BIGNUM ����ֵ�������Ч��

	if (!(bn_y))
	{
		goto clear_up;
	}
	if (!(group = EC_GROUP_new_by_curve_name(NID_sm2))) //����һ����������
	{
		goto clear_up;
	}
	if (!(bn_order = EC_GROUP_get0_order(group)))  //��ȡ��
	{
		goto clear_up;
	}
	if (!(ec_pt = EC_POINT_new(group)))   //��Բ�����ϵõ�
	{
		goto clear_up;
	}
	error_code = CREATE_SM2_KEY_PAIR_FAIL;    //����SM2��Կʧ��
    
	do
	{
		if (!(BN_rand_range(bn_d, bn_order)))
		{
			goto clear_up;
		}

	} while (BN_is_zero(bn_d));
	if (!(EC_POINT_mul(group, ec_pt, bn_d, NULL, NULL, ctx)))
	{
		goto clear_up;
	}
	if (!(EC_POINT_get_affine_coordinates_GFp(
		group, ec_pt, bn_x, bn_y, ctx)
		))
	{
		goto clear_up;
	}
	if (BN_bn2binpad(bn_x, pub_key_x, sizeof(pub_key_x)) != sizeof(pub_key_x))
	{
		goto clear_up;
	}
	if (BN_bn2binpad(bn_y, pub_key_y, sizeof(pub_key_y)) != sizeof(pub_key_y))
	{
		goto clear_up;
	}
	sm2Key->pub_key[0] = 0x4;
	//memccpy((key_pair->pri_key+1),pub_key_x,sizeof(pub_key_x));
	memcpy((sm2Key->pub_key + 1), pub_key_x, sizeof(pub_key_x));
	memcpy((sm2Key->pub_key + 1 + sizeof(pub_key_x)), pub_key_y, sizeof(pub_key_y));
	error_code = 0;
clear_up:
	if (ctx)
	{
		BN_CTX_end(ctx);
		BN_CTX_free(ctx);
	}
	if (group)
	{
		EC_GROUP_free(group);
	}
	if (ec_pt)
	{
		EC_POINT_free(ec_pt);
	}
	return error_code;
#endif
	return 0;
}
int CSecure::Sm2Encrypt()
{
	return 0;
}
int CSecure::Sm2Decrypt()
{
	return 0;
}
int CSecure::Sm2Sign()
{
	return 0;
}
int CSecure::Sm2VerifySign()
{
	return 0;
}

//sm3
int CSecure::Sm3Hash(unsigned char *src,int srclen,unsigned char *hash)
{
#if 0
	if (NULL == src || srclen <= 0 || NULL == hash) {
		return -1;
	}
	sm3(src, srclen, hash);
#endif
	return 0;
}

//sm4
int CSecure::Sm4EcbEncrypt(unsigned char *key,int keylen, const unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen)
{
    #if 0
	if (NULL == key || NULL == plain || NULL == cipher) {
		return -1;
	}
	sm4_context ctx;
	sm4_setkey_enc(&ctx, key);
	sm4_crypt_ecb(&ctx, SM4_ENCRYPT, plainlen, (unsigned char *)plain, cipher);
   #endif
	return 0;
}

int CSecure::Sm4EcbDecrypt(unsigned char *key, int keylen, const unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen)
{
#if 0
	if (NULL == key || NULL == plain || NULL == cipher) {
		return -1;
	}
	sm4_context ctx;
	sm4_setkey_dec(&ctx, key);
	sm4_crypt_ecb(&ctx, SM4_DECRYPT, cipherlen, (unsigned char *)cipher, plain);
#endif
	return 0;
}

int CSecure::Sm4CbcEncrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *plain, int plainlen, unsigned char *cipher, unsigned int cipherlen)
{
#if 0
	if (NULL == key || NULL == iv || NULL == plain || NULL == cipher) {
		return -1;
	}

	sm4_context ctx;
	sm4_setkey_enc(&ctx, (unsigned char *)key);
	sm4_crypt_cbc(&ctx, SM4_ENCRYPT, plainlen, iv, plain, cipher);
#endif
	return 0;
}

int CSecure::Sm4CbcDecrypt(const unsigned char *key, int keylen, unsigned char *iv, int ivLen, unsigned char *cipher, unsigned int cipherlen, unsigned char *plain, int plainlen)
{
 #if 0
	if (NULL == key || NULL == iv || NULL == plain || NULL == cipher) {
		return -1;
	}

	sm4_context ctx;
	sm4_setkey_dec(&ctx, (unsigned char *)key);
	sm4_crypt_cbc(&ctx, SM4_DECRYPT, cipherlen, iv, cipher, plain);
#endif
	return 0;
}

int CSecure::GenMd5(uint8_t *src,uint32_t srclen,uint8_t *dst)
{
    int res = -1;
    MD5_CTX c;
    MD5_Init(&c);
    MD5_Update(&c, src, srclen);
    MD5_Final(dst,&c);
    return res;
}

