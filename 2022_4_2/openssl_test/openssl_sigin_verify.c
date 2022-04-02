#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h> 
#include <openssl/crypto.h> 

/*
 * 参考https://blog.csdn.net/zjf535214685/article/details/82182241
 */ 

#define PUBLIC_KEY_PATH  ("./rsa_public_key.pem")
#define PRIVATE_KEY_PATH ("./rsa_private_key.pem")

#define isUseSha256    (1)

#if isUseSha256
#define SHA_WHICH        NID_sha256
#define WHICH_DIGEST_LENGTH    SHA256_DIGEST_LENGTH
#else
#define SHA_WHICH        NID_sha512
#define WHICH_DIGEST_LENGTH    SHA512_DIGEST_LENGTH
#endif


void printHex(unsigned char *md, int len)
{

    int i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x", md[i]);
    }

    printf("\n");
}

/*读取私钥*/
RSA* ReadPrivateKey(char* p_KeyPath)
{   
    FILE *fp = NULL; 
    RSA  *priRsa = NULL;

    printf("PrivateKeyPath[%s] \n", p_KeyPath);

    /*  打开密钥文件 */
    if(NULL == (fp = fopen(p_KeyPath, "r")))
    {
        printf( "fopen[%s] failed \n", p_KeyPath);
        return NULL;
    }
    /*  获取私钥 */
    priRsa = PEM_read_RSAPrivateKey(fp, NULL, NULL,NULL);
    if(NULL == priRsa)
    {
 //       ERR_print_errors_fp(stdout);
        printf( "PEM_read_RSAPrivateKey\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    return priRsa;
}

/*读取公匙*/
RSA* ReadPublicKey(char* p_KeyPath)
{   
    FILE *fp = NULL; 
    RSA *pubRsa = NULL;

    printf("PublicKeyPath[%s]\n", p_KeyPath);

    /*  打开密钥文件 */
    if(NULL == (fp = fopen(p_KeyPath, "r")))
    {
        printf( "fopen[%s] \n", p_KeyPath);
        return NULL;
    }
    /*  获取公钥 */
    if(NULL == (pubRsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL,NULL)))
    {
        printf( "PEM_read_RSAPrivateKey error\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    return pubRsa;
}


void RSA_encrypt_decrypt()
{
     unsigned  char *data = "china";
    unsigned char buf[128] = {0};
    RSA *pubKey = NULL;
    RSA *privKey = NULL;
    int nOutLen = sizeof(buf);
    int nRet = 0;

    //1. 对数据进行sha256算法摘要
  /*  unsigned char md[WHICH_DIGEST_LENGTH];
#if isUseSha256
    SHA256((unsigned char *)data, strlen(data), md);
#else
    SHA512((unsigned char *)data, strlen(data), md);
#endif*/
//    printHex(md, WHICH_DIGEST_LENGTH);

    // 2. 读取私钥
    privKey = ReadPrivateKey(PRIVATE_KEY_PATH);
    if (!privKey) 
    {  
   //     ERR_print_errors_fp (stderr);    
        return -1;  
    }

    // 3. 读取公匙
    pubKey = ReadPublicKey(PUBLIC_KEY_PATH);  
    if(!pubKey)
    {
        RSA_free(privKey);   
        printf("Error: can't load public key");
        return -1;
    }

    printf("before encrypt str is %s\n",data);
    nRet=RSA_public_encrypt(strlen(data),data,buf,pubKey,RSA_PKCS1_OAEP_PADDING);
    if(-1==nRet)
    {
      RSA_free(privKey);
    RSA_free(pubKey);

   printf("encrypt error\n");
        return;
    }
    printf("encrypt length is %d\n",nRet);

    printHex(buf, nOutLen);
    //printf("crypt text is %s\n",buf);
    char *out[128]={0};
    nRet=RSA_private_decrypt(128,buf,out,privKey,RSA_PKCS1_OAEP_PADDING);
     if(-1==nRet)
    {
     RSA_free(privKey);
    RSA_free(pubKey);

    printf("encrypt error\n");
        return;
    }
    printf("after decrypt str is %s\n",out);
    printf("decrypt succes");

    RSA_free(privKey);
    RSA_free(pubKey);

    return 0;

}

int test_RSA_sign_verify(void)
{
    char *data = "china";
    char buf[128] = {0};
    RSA *pubKey = NULL;
    RSA *privKey = NULL;
    int nOutLen = sizeof(buf);
    int nRet = 0;

    //1. 对数据进行sha256算法摘要
    unsigned char md[WHICH_DIGEST_LENGTH];
#if isUseSha256
    SHA256((unsigned char *)data, strlen(data), md);
#else
    SHA512((unsigned char *)data, strlen(data), md);
#endif
    printHex(md, WHICH_DIGEST_LENGTH);

    // 2. 读取私钥
    privKey = ReadPrivateKey(PRIVATE_KEY_PATH);
    if (!privKey) 
    {  
   //     ERR_print_errors_fp (stderr);    
        return -1;  
    }

    // 3. 读取公匙
    pubKey = ReadPublicKey(PUBLIC_KEY_PATH);  
    if(!pubKey)
    {
        RSA_free(privKey);   
        printf("Error: can't load public key");
        return -1;
    }


    // 4. 签名
    nRet = RSA_sign(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, &nOutLen, privKey);
    if(nRet != 1)
    {
        printf("RSA_sign err !!! \n");    
        goto quit;
    }
    printf("RSA_sign len = %d:", nOutLen);
    printHex(buf, nOutLen);

    // 5. 验签 
    nRet = RSA_verify(SHA_WHICH, md, WHICH_DIGEST_LENGTH, buf, nOutLen, pubKey);
    if(nRet != 1)
    {
        printf("RSA_verify err !!! \n");    
        goto quit;
    }
    printf("RSA_verify Success !!! \n");  

quit:
    RSA_free(privKey);
    RSA_free(pubKey);

    return 0;
}


int main(int argc, char *argv[])
{
    RSA_encrypt_decrypt();
    return 0;
}
