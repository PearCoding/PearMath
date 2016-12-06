#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
typedef unsigned __int32  uint32_t;
#else
#include <stdint.h>
#endif

#include <iostream>

#define PM_BIT(x)		(1<<(x))

#define PM_CPUID_CODE_VENDOR 		(0x0)
#define PM_CPUID_CODE_FEATURES		(0x1)
#define PM_CPUID_CODE_MORE_FEATURES	(0x7)

#define PM_FEATURE_D_SSE	(PM_BIT(25))
#define PM_FEATURE_D_SSE_2	(PM_BIT(26))

#define PM_FEATURE_C_SSE_3	(PM_BIT(0))
#define PM_FEATURE_C_SSSE_3	(PM_BIT(9))
#define PM_FEATURE_C_SSE_4_1	(PM_BIT(19))
#define PM_FEATURE_C_SSE_4_2	(PM_BIT(20))
#define PM_FEATURE_C_AES	(PM_BIT(25))
#define PM_FEATURE_C_AVX	(PM_BIT(28))

#define PM_FEATURE_B_AVX2	(PM_BIT(5))
#define PM_FEATURE_B_AVX512F	(PM_BIT(16))
#define PM_FEATURE_B_AVX512DQ	(PM_BIT(17))
#define PM_FEATURE_B_AVX512IFMA	(PM_BIT(21))
#define PM_FEATURE_B_AVX512PF	(PM_BIT(26))
#define PM_FEATURE_B_AVX512ER	(PM_BIT(27))
#define PM_FEATURE_B_AVX512CD	(PM_BIT(28))
#define PM_FEATURE_B_AVX512BW	(PM_BIT(30))
#define PM_FEATURE_B_AVX512VL	(PM_BIT(31))

struct CPUIDData
{
	uint32_t EAX;
	uint32_t EBX;
	uint32_t ECX;
	uint32_t EDX;
};

CPUIDData cpuid(uint32_t code, uint32_t sub = 0)
{
	CPUIDData data;
#ifdef _WIN32
	__cpuid((int *)&data, (int)code);//Subs?
#else
    	asm volatile("cpuid" 
	: "=a" (data.EAX), "=b" (data.EBX), "=c" (data.ECX), "=d" (data.EDX)
       	: "a" (code), "c" (sub));
#endif
	return data;
}

#define CHECK_A(feature) (((data.EAX & (feature)) != 0) ? "YES" : "NO")
#define CHECK_B(feature) (((data.EBX & (feature)) != 0) ? "YES" : "NO")
#define CHECK_C(feature) (((data.ECX & (feature)) != 0) ? "YES" : "NO")
#define CHECK_D(feature) (((data.EDX & (feature)) != 0) ? "YES" : "NO")

int main(int, char**)
{
	CPUIDData data = cpuid(PM_CPUID_CODE_VENDOR);
	std::string vendor;
  	vendor += std::string((const char *)&data.EBX, 4);
  	vendor += std::string((const char *)&data.EDX, 4);
  	vendor += std::string((const char *)&data.ECX, 4);
	std::cout << "Vendor: " << vendor << std::endl;

	data = cpuid(PM_CPUID_CODE_FEATURES);
	std::cout << "SSE=" << CHECK_D(PM_FEATURE_D_SSE) << std::endl;
	std::cout << "SSE2=" << CHECK_D(PM_FEATURE_D_SSE_2) << std::endl;

	std::cout << "SSE3=" << CHECK_C(PM_FEATURE_C_SSE_3) << std::endl;
	std::cout << "SSSE3=" << CHECK_C(PM_FEATURE_C_SSSE_3) << std::endl;
	std::cout << "SSE4_1=" << CHECK_C(PM_FEATURE_C_SSE_4_1) << std::endl;
	std::cout << "SSE4_2=" << CHECK_C(PM_FEATURE_C_SSE_4_2) << std::endl;
	std::cout << "AES=" << CHECK_C(PM_FEATURE_C_AES) << std::endl;
	std::cout << "AVX=" << CHECK_C(PM_FEATURE_C_AVX) << std::endl;

	data = cpuid(PM_CPUID_CODE_MORE_FEATURES);
	std::cout << "AVX2=" << CHECK_B(PM_FEATURE_B_AVX2) << std::endl;
	std::cout << "AVX512=" << CHECK_B(PM_FEATURE_B_AVX512F) << std::endl;
	std::cout << "AVX512DQ=" << CHECK_B(PM_FEATURE_B_AVX512DQ) << std::endl;
	std::cout << "AVX512IFMA=" << CHECK_B(PM_FEATURE_B_AVX512IFMA) << std::endl;
	std::cout << "AVX512PF=" << CHECK_B(PM_FEATURE_B_AVX512PF) << std::endl;
	std::cout << "AVX512ER=" << CHECK_B(PM_FEATURE_B_AVX512ER) << std::endl;
	std::cout << "AVX512CD=" << CHECK_B(PM_FEATURE_B_AVX512CD) << std::endl;
	std::cout << "AVX512BW=" << CHECK_B(PM_FEATURE_B_AVX512BW) << std::endl;
	std::cout << "AVX512VL=" << CHECK_B(PM_FEATURE_B_AVX512VL) << std::endl;

	return 0;
}
