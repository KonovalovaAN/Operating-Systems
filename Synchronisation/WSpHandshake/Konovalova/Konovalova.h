// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the KONOVALOVA_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// KONOVALOVA_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef KONOVALOVA_EXPORTS
#define KONOVALOVA_API __declspec(dllexport)
#else
#define KONOVALOVA_API __declspec(dllimport)
#endif

// This class is exported from the dll
class KONOVALOVA_API CKonovalova {
public:
	CKonovalova(void);
	// TODO: add your methods here.
};

extern KONOVALOVA_API int nKonovalova;

KONOVALOVA_API int fnKonovalova(void);
